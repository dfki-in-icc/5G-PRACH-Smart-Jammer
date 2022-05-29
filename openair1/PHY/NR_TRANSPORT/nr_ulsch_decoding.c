/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.0  (the "License"); you may not use this file
 * except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.openairinterface.org/?page_id=698
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */

/*! \file PHY/NR_TRANSPORT/nr_ulsch_decoding.c
* \brief Top-level routines for decoding  LDPC (ULSCH) transport channels from 38.212, V15.4.0 2018-12
* \author Ahmed Hussein
* \date 2019
* \version 0.1
* \company Fraunhofer IIS
* \email: ahmed.hussein@iis.fraunhofer.de
* \note
* \warning
*/

/*! \file PHY/NR_TRANSPORT/nr_ulsch_decoding.c
* \brief Time test of the number of code blocks
* \author Sendren Xu, SY Yeh(fdragon), Hongming, Terng-Yin Hsu
* \date 2022-05-29
* \version 4.0
* \email: summery19961210@gmail.com
*/


// [from gNB coding]
#include "PHY/defs_gNB.h"
#include "PHY/phy_extern.h"
#include "PHY/CODING/coding_extern.h"
#include "PHY/CODING/coding_defs.h"
#include "PHY/CODING/lte_interleaver_inline.h"
#include "PHY/CODING/nrLDPC_extern.h"
#include "PHY/NR_TRANSPORT/nr_transport_common_proto.h"
#include "PHY/NR_TRANSPORT/nr_ulsch.h"
#include "PHY/NR_TRANSPORT/nr_dlsch.h"
#include "SCHED_NR/sched_nr.h"
#include "defs.h"
#include "common/utils/LOG/vcd_signal_dumper.h"
#include "common/utils/LOG/log.h"
#include <syscall.h>
//#define DEBUG_ULSCH_DECODING
//#define gNB_DEBUG_TRACE

#define OAI_UL_LDPC_MAX_NUM_LLR 27000//26112 // NR_LDPC_NCOL_BG1*NR_LDPC_ZMAX = 68*384
//#define PRINT_CRC_CHECK
#define PRINT_CRC_CHECK 1
#define UL_LDPC_MODE 2
#include "nr_ldpc_decoding_pym.h"
//extern double cpuf;
#include <time.h>

void free_gNB_ulsch(NR_gNB_ULSCH_t **ulschptr,uint8_t N_RB_UL)
{

  int i,r;
  uint16_t a_segments = MAX_NUM_NR_ULSCH_SEGMENTS;  //number of segments to be allocated
  NR_gNB_ULSCH_t *ulsch = *ulschptr;

  if (ulsch) {
    if (N_RB_UL != 273) {
      a_segments = a_segments*N_RB_UL;
      a_segments = a_segments/273 +1;
    }  


    for (i=0; i<NR_MAX_ULSCH_HARQ_PROCESSES; i++) {

      if (ulsch->harq_processes[i]) {
        if (ulsch->harq_processes[i]->b) {
          free16(ulsch->harq_processes[i]->b,a_segments*1056);
          ulsch->harq_processes[i]->b = NULL;
        }
        for (r=0; r<a_segments; r++) {
          free16(ulsch->harq_processes[i]->c[r],(8448)*sizeof(uint8_t));
          ulsch->harq_processes[i]->c[r] = NULL;
        }
        for (r=0; r<a_segments; r++) {
          if (ulsch->harq_processes[i]->d[r]) {
            free16(ulsch->harq_processes[i]->d[r],(68*384)*sizeof(int16_t));
            ulsch->harq_processes[i]->d[r] = NULL;
          }
        }
        for (r=0; r<a_segments; r++) {
          if (ulsch->harq_processes[i]->w[r]) {
            free16(ulsch->harq_processes[i]->w[r],(3*(6144+64))*sizeof(int16_t));
            ulsch->harq_processes[i]->w[r] = NULL;
          }
        }
        for (r=0; r<a_segments; r++) {
          if (ulsch->harq_processes[i]->p_nrLDPC_procBuf[r]){
            nrLDPC_free_mem(ulsch->harq_processes[i]->p_nrLDPC_procBuf[r]);
            ulsch->harq_processes[i]->p_nrLDPC_procBuf[r] = NULL;
          }
        }
        free16(ulsch->harq_processes[i],sizeof(NR_UL_gNB_HARQ_t));
        ulsch->harq_processes[i] = NULL;
      }
    }
    free16(ulsch,sizeof(NR_gNB_ULSCH_t));
    *ulschptr = NULL;
  }
}


NR_gNB_ULSCH_t *new_gNB_ulsch(uint8_t max_ldpc_iterations,uint16_t N_RB_UL, uint8_t abstraction_flag)
{

  NR_gNB_ULSCH_t *ulsch;
  uint8_t exit_flag = 0,i,r;
  uint16_t a_segments = MAX_NUM_NR_ULSCH_SEGMENTS;  //number of segments to be allocated

  if (N_RB_UL != 273) {
    a_segments = a_segments*N_RB_UL;
    a_segments = a_segments/273 +1;
  }

  uint16_t ulsch_bytes = a_segments*1056;  // allocated bytes per segment
  ulsch = (NR_gNB_ULSCH_t *)malloc16(sizeof(NR_gNB_ULSCH_t));

  if (ulsch) {

    memset(ulsch,0,sizeof(NR_gNB_ULSCH_t));

    ulsch->max_ldpc_iterations = max_ldpc_iterations;
    ulsch->Mlimit = 4;

    for (i=0; i<NR_MAX_ULSCH_HARQ_PROCESSES; i++) {

      ulsch->harq_processes[i] = (NR_UL_gNB_HARQ_t *)malloc16(sizeof(NR_UL_gNB_HARQ_t));

      if (ulsch->harq_processes[i]) {

        memset(ulsch->harq_processes[i],0,sizeof(NR_UL_gNB_HARQ_t));

        ulsch->harq_processes[i]->b = (uint8_t*)malloc16(ulsch_bytes);

        if (ulsch->harq_processes[i]->b)
          memset(ulsch->harq_processes[i]->b,0,ulsch_bytes);
        else
          exit_flag=3;

        if (abstraction_flag == 0) {
          for (r=0; r<a_segments; r++) {

            ulsch->harq_processes[i]->p_nrLDPC_procBuf[r] = nrLDPC_init_mem();

            ulsch->harq_processes[i]->c[r] = (uint8_t*)malloc16(8448*sizeof(uint8_t));

            if (ulsch->harq_processes[i]->c[r])
              memset(ulsch->harq_processes[i]->c[r],0,8448*sizeof(uint8_t));
            else
              exit_flag=2;

            ulsch->harq_processes[i]->d[r] = (int16_t*)malloc16((68*384)*sizeof(int16_t));

            if (ulsch->harq_processes[i]->d[r])
              memset(ulsch->harq_processes[i]->d[r],0,(68*384)*sizeof(int16_t));
            else
              exit_flag=2;

            ulsch->harq_processes[i]->w[r] = (int16_t*)malloc16((3*(6144+64))*sizeof(int16_t));

            if (ulsch->harq_processes[i]->w[r])
              memset(ulsch->harq_processes[i]->w[r],0,(3*(6144+64))*sizeof(int16_t));
            else
              exit_flag=2;
          }
        }
      } else {
        exit_flag=1;
      }
    }

    if (exit_flag==0)
      return(ulsch);
  }
  printf("new_gNB_ulsch with size %zu: exit_flag = %hhu\n",sizeof(NR_UL_gNB_HARQ_t), exit_flag);
  free_gNB_ulsch(&ulsch,N_RB_UL);
  return(NULL);
}

void clean_gNB_ulsch(NR_gNB_ULSCH_t *ulsch)
{
  unsigned char i, j;

  if (ulsch) {
    ulsch->harq_mask = 0;
    ulsch->bundling = 0;
    ulsch->beta_offset_cqi_times8 = 0;
    ulsch->beta_offset_ri_times8 = 0;
    ulsch->beta_offset_harqack_times8 = 0;
    ulsch->Msg3_active = 0;
    ulsch->Msg3_flag = 0;
    ulsch->Msg3_subframe = 0;
    ulsch->Msg3_frame = 0;
    ulsch->rnti = 0;
    ulsch->rnti_type = 0;
    ulsch->cyclicShift = 0;
    ulsch->cooperation_flag = 0;
    ulsch->Mlimit = 0;
    ulsch->max_ldpc_iterations = 0;
    ulsch->last_iteration_cnt = 0;
    for (i=0;i<NR_MAX_SLOTS_PER_FRAME;i++) ulsch->harq_process_id[i] = 0;

    for (i=0; i<NR_MAX_ULSCH_HARQ_PROCESSES; i++) {
      if (ulsch->harq_processes[i]){
        /// Nfapi ULSCH PDU
        //nfapi_nr_ul_config_ulsch_pdu ulsch_pdu;
        ulsch->harq_processes[i]->frame=0;
        ulsch->harq_processes[i]->slot=0;
        ulsch->harq_processes[i]->round=0;
        ulsch->harq_processes[i]->TPC=0;
        ulsch->harq_processes[i]->mimo_mode=0;
        ulsch->harq_processes[i]->dci_alloc=0;
        ulsch->harq_processes[i]->rar_alloc=0;
        ulsch->harq_processes[i]->status=NR_SCH_IDLE;
        ulsch->harq_processes[i]->subframe_scheduling_flag=0;
        ulsch->harq_processes[i]->previous_first_rb=0;
        ulsch->harq_processes[i]->handled=0;
        ulsch->harq_processes[i]->delta_TF=0;

        ulsch->harq_processes[i]->TBS=0;
        /// Pointer to the payload (38.212 V15.4.0 section 5.1)
        //uint8_t *b;
        ulsch->harq_processes[i]->B=0;
        /// Pointers to code blocks after code block segmentation and CRC attachment (38.212 V15.4.0 section 5.2.2)
        //uint8_t *c[MAX_NUM_NR_ULSCH_SEGMENTS];
        ulsch->harq_processes[i]->K=0;
        ulsch->harq_processes[i]->F=0;
        ulsch->harq_processes[i]->C=0;
        /// Pointers to code blocks after LDPC coding (38.212 V15.4.0 section 5.3.2)
        //int16_t *d[MAX_NUM_NR_ULSCH_SEGMENTS];
        /// LDPC processing buffer
        //t_nrLDPC_procBuf* p_nrLDPC_procBuf[MAX_NUM_NR_ULSCH_SEGMENTS];
        ulsch->harq_processes[i]->Z=0;
        /// code blocks after bit selection in rate matching for LDPC code (38.212 V15.4.0 section 5.4.2.1)
        //int16_t e[MAX_NUM_NR_DLSCH_SEGMENTS][3*8448];
        ulsch->harq_processes[i]->E=0;


        ulsch->harq_processes[i]->n_DMRS=0;
        ulsch->harq_processes[i]->n_DMRS2=0;
        ulsch->harq_processes[i]->previous_n_DMRS=0;


        ulsch->harq_processes[i]->cqi_crc_status=0;
        for (j=0;j<MAX_CQI_BYTES;j++) ulsch->harq_processes[i]->o[j]=0;
        ulsch->harq_processes[i]->uci_format=0;
        ulsch->harq_processes[i]->Or1=0;
        ulsch->harq_processes[i]->Or2=0;
        ulsch->harq_processes[i]->o_RI[0]=0; ulsch->harq_processes[i]->o_RI[1]=0;
        ulsch->harq_processes[i]->O_RI=0;
        ulsch->harq_processes[i]->o_ACK[0]=0; ulsch->harq_processes[i]->o_ACK[1]=0;
        ulsch->harq_processes[i]->o_ACK[2]=0; ulsch->harq_processes[i]->o_ACK[3]=0;
        ulsch->harq_processes[i]->O_ACK=0;
        ulsch->harq_processes[i]->V_UL_DAI=0;
        /// "q" sequences for CQI/PMI (for definition see 36-212 V8.6 2009-03, p.27)
        //int8_t q[MAX_CQI_PAYLOAD];
        ulsch->harq_processes[i]->o_RCC=0;
        /// coded and interleaved CQI bits
        //int8_t o_w[(MAX_CQI_BITS+8)*3];
        /// coded CQI bits
        //int8_t o_d[96+((MAX_CQI_BITS+8)*3)];
        for (j=0;j<MAX_ACK_PAYLOAD;j++) ulsch->harq_processes[i]->q_ACK[j]=0;
        for (j=0;j<MAX_RI_PAYLOAD;j++) ulsch->harq_processes[i]->q_RI[j]=0;
        /// Temporary h sequence to flag PUSCH_x/PUSCH_y symbols which are not scrambled
        //uint8_t h[MAX_NUM_CHANNEL_BITS];
        /// soft bits for each received segment ("w"-sequence)(for definition see 36-212 V8.6 2009-03, p.15)
        //int16_t w[MAX_NUM_NR_ULSCH_SEGMENTS][3*(6144+64)];
      }
    }
  }
}

#ifdef PRINT_CRC_CHECK
  static uint32_t prnt_crc_cnt = 0;
#endif

#if 1

#define CB_TEST_NUM 3
#define CB_NUM CB_TEST_NUM+1
char multi_indata[26112*CB_NUM+16]; //68*384
char multi_outdata[1056*CB_NUM+16]; //22*384*1/8
int8_t buffer_in[26112+16]={0};
int8_t buffer_out[1056+16]={0};



void nr_processULSegment(void* arg) {
  cpu_set_t cpuset;
  int cpu = 1;
	int s;
  cpu = sysconf(_SC_NPROCESSORS_CONF);
  printf("cpus: %d\n", cpu);
	CPU_ZERO(&cpuset);       //clears the cpuset
	// CPU_SET(  0 , &cpuset); //set CPU 0~7 on cpuset
  // CPU_SET(  1 , &cpuset);
  CPU_SET(  2 , &cpuset);
  printf("pthread_self() = %d\n",pthread_self());
  
	s = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
  if (s != 0){	
		perror( "pthread_setaffinity_np");
		exit_fun("Error setting processor affinity");
	}


  ldpcDecode_t *rdata = (ldpcDecode_t*) arg;
  PHY_VARS_gNB *phy_vars_gNB = rdata->gNB;
  NR_UL_gNB_HARQ_t *ulsch_harq = rdata->ulsch_harq;
  t_nrLDPC_dec_params *p_decoderParms = &rdata->decoderParms;
  int length_dec;
  int no_iteration_ldpc;
  int Kr;
  int Kr_bytes;
  int K_bits_F;
  uint8_t crc_type;
  int i;
  int j;
  int r = rdata->segment_r;
  int A = rdata->A;
  int E = rdata->E;
  int Qm = rdata->Qm;
  int rv_index = rdata->rv_index;
  int r_offset = rdata->r_offset;
  uint8_t kc = rdata->Kc;
  uint32_t Tbslbrm = rdata->Tbslbrm;
  short* ulsch_llr = rdata->ulsch_llr;
  int max_ldpc_iterations = p_decoderParms->numMaxIter;
  int8_t llrProcBuf[OAI_UL_LDPC_MAX_NUM_LLR] __attribute__ ((aligned(32)));

  int16_t  z [68*384 + 16] __attribute__ ((aligned(16)));
  int8_t   l [68*384 + 16] __attribute__ ((aligned(16)));

  __m128i *pv = (__m128i*)&z;
  __m128i *pl = (__m128i*)&l;
  
  uint8_t  Ilbrm    = 0;

  Kr = ulsch_harq->K;
  Kr_bytes = Kr>>3;
  K_bits_F = Kr-ulsch_harq->F;

  t_nrLDPC_time_stats procTime = {0};
  t_nrLDPC_time_stats* p_procTime     = &procTime ;
  DecIFConf dec_conf;
//FPGA ldpc dec param setting
  int mbmb=0,bg_len=0;
    if( p_decoderParms->BG == 1 ){
      dec_conf.nRows = 46;
      mbmb=68;
      bg_len = 22;
    } else {
      dec_conf.nRows = 42;
      mbmb=52;
      bg_len = 10;
    }

    // Calc input CB offset
  int input_CBoffset = p_decoderParms->Z*mbmb*8;
  if ((input_CBoffset & 0x7F) == 0)
    input_CBoffset = input_CBoffset/8;
  else
    input_CBoffset = 16*((input_CBoffset / 128) + 1);
    // Calc output CB offset
  int out_CBoffset = ulsch_harq->Z*bg_len;
  if ((out_CBoffset & 0x7F) == 0)
    out_CBoffset = out_CBoffset/8;
  else
    out_CBoffset = 16 * ((out_CBoffset / 128) + 1);

  //start_meas(&phy_vars_gNB->ulsch_deinterleaving_stats);

  ////////////////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////// nr_deinterleaving_ldpc ///////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////

  //////////////////////////// ulsch_llr =====> ulsch_harq->e //////////////////////////////
  int cutcb=0;
  for(cutcb = 0; cutcb < ulsch_harq->C;cutcb++){
    // r_offset = rdata->r_offset*cutcb;
    r_offset = E*cutcb;
  nr_deinterleaving_ldpc(E,
                         Qm,
                         ulsch_harq->e[r],
                         ulsch_llr+r_offset);

  //for (int i =0; i<16; i++)
  //          printf("rx output deinterleaving w[%d]= %d r_offset %d\n", i,ulsch_harq->w[r][i], r_offset);

  stop_meas(&phy_vars_gNB->ulsch_deinterleaving_stats);


  /*LOG_D(PHY,"HARQ_PID %d Rate Matching Segment %d (coded bits %d,unpunctured/repeated bits %d, TBS %d, mod_order %d, nb_rb %d, Nl %d, rv %d, round %d)...\n",
        harq_pid,r, G,
        Kr*3,
        ulsch_harq->TBS,
        Qm,
        nb_rb,
        n_layers,
        pusch_pdu->pusch_data.rv_index,
        ulsch_harq->round);*/
  //////////////////////////////////////////////////////////////////////////////////////////


  //////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////// nr_rate_matching_ldpc_rx ////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////

  ///////////////////////// ulsch_harq->e =====> ulsch_harq->d /////////////////////////

  //start_meas(&phy_vars_gNB->ulsch_rate_unmatching_stats);

  if (nr_rate_matching_ldpc_rx(Ilbrm,
                               Tbslbrm,
                               p_decoderParms->BG,
                               p_decoderParms->Z,
                               ulsch_harq->d[r],
                               ulsch_harq->e[r],
                               ulsch_harq->C,
                               rv_index,
                               ulsch_harq->new_rx,
                               E,
       ulsch_harq->F,
       Kr-ulsch_harq->F-2*(p_decoderParms->Z))==-1) {

    stop_meas(&phy_vars_gNB->ulsch_rate_unmatching_stats);

    LOG_E(PHY,"ulsch_decoding.c: Problem in rate_matching\n");
    rdata->decodeIterations = max_ldpc_iterations + 1;
    return;
  } else {
    stop_meas(&phy_vars_gNB->ulsch_rate_unmatching_stats);
  }

  memset(ulsch_harq->c[r],0,Kr_bytes);

  if (ulsch_harq->C == 1) {
    if (A > 3824)
      crc_type = CRC24_A;
    else
      crc_type = CRC16;

    length_dec = ulsch_harq->B;
  }
  else {
    crc_type = CRC24_B;
    length_dec = (ulsch_harq->B+24*ulsch_harq->C)/ulsch_harq->C;
  }

  //start_meas(&phy_vars_gNB->ulsch_ldpc_decoding_stats);

  //set first 2*Z_c bits to zeros
  memset(&z[0],0,2*ulsch_harq->Z*sizeof(int16_t));
  //set Filler bits
  memset((&z[0]+K_bits_F),127,ulsch_harq->F*sizeof(int16_t));
  //Move coded bits before filler bits
  memcpy((&z[0]+2*ulsch_harq->Z),ulsch_harq->d[r],(K_bits_F-2*ulsch_harq->Z)*sizeof(int16_t));
  //skip filler bits
  memcpy((&z[0]+Kr),ulsch_harq->d[r]+(Kr-2*ulsch_harq->Z),(kc*ulsch_harq->Z-Kr)*sizeof(int16_t));
  //Saturate coded bits before decoding into 8 bits values
  for (i=0, j=0; j < ((kc*ulsch_harq->Z)>>4)+1;  i+=2, j++)
  {
    pl[j] = _mm_packs_epi16(pv[i],pv[i+1]);
  }

  if(cutcb == 0){
    for(int ab=0;ab<input_CBoffset;ab++){
      if(l[ab] == -128){
        buffer_in[ab] = 0x81;
      } else {
        buffer_in[ab]=((int8_t)l[ab]);
      }
      buffer_in[ab]=((buffer_in[ab])^0xFF)+1;
    }
  }

  } // all cb


  for(int i=0;i<CB_TEST_NUM;i++){ // data format
    printf("*********** code block[%d] ***********\n",i+1);
    for(int ab=0;ab<input_CBoffset;ab++){
      multi_indata[ab+i*input_CBoffset] = buffer_in[ab];
      // printf("[%04d]%02x ",ab,(uint8_t)multi_indata[ab+i*input_CBoffset]);
      // if(ab%30==29)
      //   printf("\n");
    }
  }


  //////////////////////////////////////////////////////////////////////////////////////////


  //////////////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////// nrLDPC_decoder /////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////

  ////////////////////////////////// pl =====> llrProcBuf //////////////////////////////////
   /* LDPC ------[FPGA version]------ param set*/
  dec_conf.Zc = p_decoderParms->Z;
  dec_conf.BG = p_decoderParms->BG;
  dec_conf.max_iter = p_decoderParms->numMaxIter;
  //dec_conf.numCB = ulsch_harq->C;
  dec_conf.numCB = CB_TEST_NUM;
  dec_conf.numChannelLls = (K_bits_F-2*ulsch_harq->Z) + (kc*ulsch_harq->Z-Kr); // input soft bits length, Zc x 66 - length of filler bits
  dec_conf.numFillerBits = ulsch_harq->F; // filler bits length
  dec_conf.max_iter = 8;
  dec_conf.max_schedule = 0;
  dec_conf.SetIdx = 12;
  
  
    // nrLDPC_decoder_FPGA_PYM((int8_t *)&buffer_in[0],(int8_t *)&buffer_out[0],dec_conf);
    // memcpy(llrProcBuf,buffer_out,sizeof(uint8_t)*out_CBoffset);
    // for(int y=0;y<out_CBoffset;y++){
    //   printf("[%04d]%02x ",y,(uint8_t)llrProcBuf[y]);
    //   if(y%30 == 29)printf("\n");
    // }
  printf("FPGA accelator LDPC deocder multi code block \n");
  int test_num=0;
  int test_max=1000;
  int time_10kup=0;
  int time_1kup=0;
  int cb_error=0;
  int error_case=0;
    
  struct timespec ts_start, ts_end;
  double time_max=0.0,time_min=100000.0,time_ava=0.0,time_num=0.0,count=0.0,time_arr[1000]={0.0},time_sd=0.0;
  for(test_num=0;test_num<test_max;test_num++){
  // printf("test[%d/%d]\n",test_num,test_max);
    double cost;
    clock_gettime(CLOCK_MONOTONIC, &ts_start); // time start
    nrLDPC_decoder_FPGA_PYM((int8_t *)&multi_indata[0],(int8_t *)&multi_outdata[0],dec_conf);
    clock_gettime(CLOCK_MONOTONIC, &ts_end); // time end
    
    count++;
    // time_num = (fpga_end.tv_nsec - fpga_start.tv_nsec) *1.0 / 1000;
    timespec_sub(&ts_end, &ts_start);
    time_num = ts_end.tv_nsec/1000.0;
    if(time_num > 9999.99){
      time_10kup++;
      error_case = test_num;
    }
    if((time_num < 9999.99) && (time_num > 999.99)){
      time_1kup++;
      error_case = test_num;
    }
     
    if(count == 1){
      time_max = time_num;
      time_min = time_num;
    }
    if(time_max < time_num)
      time_max = time_num;
    
    if(time_min > time_num)
      time_min = time_num;
      
    time_ava = time_ava + time_num;
    time_arr[test_num]=time_num;

      for(int i=0;i<dec_conf.numCB;i++){
        
        printf("FPGA LDPC decoder mCB[%d/%d]\n",i+1,dec_conf.numCB); //CB Info
        for(int j=0;j<out_CBoffset;j++){
          llrProcBuf[j]=multi_outdata[j+i*out_CBoffset];
          // ulsch_harq->c[i][j]= (uint8_t) llrProcBuf[j];
          // printf("[%04d]%02x ",j,(uint8_t)llrProcBuf[j]);
          // if(j%10==9)printf("\n");
        }
        if (check_crc((uint8_t*)llrProcBuf,length_dec,ulsch_harq->F,crc_type)) {
          #ifdef PRINT_CRC_CHECK
            // printf("length_dec = %d，out_CBoffset = %d\n",length_dec,out_CBoffset);
            printf("\nFPGA LDPC [%d/%d] CRC check is OK \n",i+1,dec_conf.numCB); //CRC Info
          #endif
        }else{
          #ifdef PRINT_CRC_CHECK
            printf("\nFPGA LDPC [%d/%d] CRC check NOK\n",i+1,dec_conf.numCB); //CRC Info
            cb_error++;
          #endif
        }
        memset(llrProcBuf,0,27000);
      }
    }
    time_ava=time_ava/test_max;
    printf("------------------ time data information -----------------\n");
    for(int i=0;i<test_max;i++){
      // calculate [ standard deviation ]
      time_sd=time_sd+((time_arr[i]-time_ava)*(time_arr[i]-time_ava));
      time_sd = time_sd/test_max;
      // printf("time[%d]:%.2f  \n",i,time_arr[i]);
    }
    printf("\ntime_max:%.2f usec，time_min:%.2f usec，time_ava:%.2f usec，time_10kup =%d，SD = %.2f[%.2f]\n",time_max,time_min,time_ava,time_10kup,sqrt(time_sd),count);
    printf("time_1kup:%d\n",time_1kup);
    printf("code block error_num %d\n",cb_error); 
    printf("code block error_case %d\n",error_case);

  
  /*
  if (check_crc((uint8_t*)llrProcBuf,length_dec,ulsch_harq->F,crc_type)) {
#ifdef PRINT_CRC_CHECK
      LOG_I(PHY, "Segment %d CRC OK, iterations %d/%d\n",r,no_iteration_ldpc,max_ldpc_iterations);
#endif
    rdata->decodeIterations = no_iteration_ldpc;
    if (rdata->decodeIterations > p_decoderParms->numMaxIter) rdata->decodeIterations--;
  } else {
#ifdef PRINT_CRC_CHECK
      LOG_I(PHY, "CRC NOK\n");
#endif
    rdata->decodeIterations = max_ldpc_iterations + 1;
  }

  for (int m=0; m < Kr>>3; m ++) {
    ulsch_harq->c[r][m]= (uint8_t) llrProcBuf[m];
  }

  //stop_meas(&phy_vars_gNB->ulsch_ldpc_decoding_stats);

  memset(llrProcBuf,0,27000);
  */
  
}
#endif


uint32_t nr_ulsch_decoding(PHY_VARS_gNB *phy_vars_gNB,
                           uint8_t ULSCH_id,
                           short *ulsch_llr,
                           NR_DL_FRAME_PARMS *frame_parms,
                           nfapi_nr_pusch_pdu_t *pusch_pdu,
                           uint32_t frame,
                           uint8_t nr_tti_rx,
                           uint8_t harq_pid,
                           uint32_t G) {

  uint32_t A;
  uint32_t r;
  uint32_t r_offset;
  uint32_t offset;
  int kc;
  int Tbslbrm;
  int E;

#ifdef PRINT_CRC_CHECK
  prnt_crc_cnt++;
#endif
  

  NR_gNB_ULSCH_t                       *ulsch                 = phy_vars_gNB->ulsch[ULSCH_id][0];
  NR_gNB_PUSCH                         *pusch                 = phy_vars_gNB->pusch_vars[ULSCH_id];
  NR_UL_gNB_HARQ_t                     *harq_process          = ulsch->harq_processes[harq_pid];

  if (!harq_process) {
    LOG_E(PHY,"ulsch_decoding.c: NULL harq_process pointer\n");
    return 1;
  }

  t_nrLDPC_dec_params decParams;
  t_nrLDPC_dec_params* p_decParams    = &decParams;

  int Kr;
  int Kr_bytes;
    
  phy_vars_gNB->nbDecode = 0;
  harq_process->processedSegments = 0;

  double   Coderate = 0.0;
  
  // ------------------------------------------------------------------
  uint16_t nb_rb          = pusch_pdu->rb_size;
  uint8_t Qm              = pusch_pdu->qam_mod_order;
  uint16_t R              = pusch_pdu->target_code_rate;
  uint8_t mcs             = pusch_pdu->mcs_index;
  uint8_t n_layers        = pusch_pdu->nrOfLayers;
  // ------------------------------------------------------------------

   if (!ulsch_llr) {
    LOG_E(PHY,"ulsch_decoding.c: NULL ulsch_llr pointer\n");
    return 1;
  }

  VCD_SIGNAL_DUMPER_DUMP_FUNCTION_BY_NAME(VCD_SIGNAL_DUMPER_FUNCTIONS_PHY_gNB_ULSCH_DECODING,1);
  harq_process->TBS = pusch_pdu->pusch_data.tb_size;
  harq_process->round = nr_rv_round_map[pusch_pdu->pusch_data.rv_index];

  harq_process->new_rx = false; // flag to indicate if this is a new reception for this harq (initialized to false)
  if (harq_process->round == 0) {
    harq_process->new_rx = true;
    harq_process->ndi = pusch_pdu->pusch_data.new_data_indicator;
  }

  // this happens if there was a DTX in round 0
  if (harq_process->ndi != pusch_pdu->pusch_data.new_data_indicator) {
    harq_process->new_rx = true;
    harq_process->ndi = pusch_pdu->pusch_data.new_data_indicator;
    LOG_E(PHY,"Missed ULSCH detection. NDI toggled but rv %d does not correspond to first reception\n",pusch_pdu->pusch_data.rv_index);
  }

  A   = (harq_process->TBS)<<3;

  LOG_D(PHY,"ULSCH Decoding, harq_pid %d TBS %d G %d mcs %d Nl %d nb_rb %d, Qm %d, n_layers %d, Coderate %d\n",harq_pid,A,G, mcs, n_layers, nb_rb, Qm, n_layers, R);

  if (R<1024)
    Coderate = (float) R /(float) 1024;
  else
    Coderate = (float) R /(float) 2048;
  
  if ((A <=292) || ((A<=3824) && (Coderate <= 0.6667)) || Coderate <= 0.25){
    p_decParams->BG = 2;
    kc = 52;
    if (Coderate < 0.3333) {
      p_decParams->R = 15;
    }
    else if (Coderate <0.6667) {
      p_decParams->R = 13;
    }
    else {
      p_decParams->R = 23;
    }
  } else {
    p_decParams->BG = 1;
    kc = 68;
    if (Coderate < 0.6667) {
      p_decParams->R = 13;
    }
    else if (Coderate <0.8889) {
      p_decParams->R = 23;
    }
    else {
      p_decParams->R = 89;
    }
  }
  
  NR_gNB_SCH_STATS_t *stats=NULL;
  int first_free=-1;
  for (int i=0;i<NUMBER_OF_NR_SCH_STATS_MAX;i++) {
    if (phy_vars_gNB->ulsch_stats[i].rnti == 0 && first_free == -1) {
      first_free = i;
      stats=&phy_vars_gNB->ulsch_stats[i];
    }
    if (phy_vars_gNB->ulsch_stats[i].rnti == ulsch->rnti) {
      stats=&phy_vars_gNB->ulsch_stats[i];
      break;
    }
  }
  if (stats) {
    stats->frame = frame;
    stats->rnti = ulsch->rnti;
    stats->round_trials[harq_process->round]++;
    for (int aarx=0;aarx<frame_parms->nb_antennas_rx;aarx++) {
       stats->power[aarx]=dB_fixed_x10(pusch->ulsch_power[aarx]);
       stats->noise_power[aarx]=dB_fixed_x10(pusch->ulsch_noise_power[aarx]);
    }
    if (harq_process->new_rx == 0) {
      stats->current_Qm = Qm;
      stats->current_RI = n_layers;
      stats->total_bytes_tx += harq_process->TBS;
    }
  }
  if (A > 3824)
    harq_process->B = A+24;
  else
    harq_process->B = A+16;

// [hna] Perform nr_segmenation with input and output set to NULL to calculate only (B, C, K, Z, F)
  nr_segmentation(NULL,
                  NULL,
                  harq_process->B,
                  &harq_process->C,
                  &harq_process->K,
                  &harq_process->Z, // [hna] Z is Zc
                  &harq_process->F,
                  p_decParams->BG);

#ifdef DEBUG_ULSCH_DECODING
  printf("ulsch decoding nr segmentation Z %d\n", harq_process->Z);
  if (!frame%100)
    printf("K %d C %d Z %d \n", harq_process->K, harq_process->C, harq_process->Z);
#endif
  Tbslbrm = nr_compute_tbslbrm(0,nb_rb,n_layers);

  p_decParams->Z = harq_process->Z;


  p_decParams->numMaxIter = ulsch->max_ldpc_iterations;
  p_decParams->outMode= 0;

  r_offset = 0;

  uint16_t a_segments = MAX_NUM_NR_ULSCH_SEGMENTS;  //number of segments to be allocated

  if (nb_rb != 273) {
    a_segments = a_segments*nb_rb;
    a_segments = a_segments/273 +1;
  }

  if (harq_process->C > a_segments) {
    LOG_E(PHY,"Illegal harq_process->C %d > %d\n",harq_process->C,a_segments);
    return 1;
  }
#ifdef DEBUG_ULSCH_DECODING
  printf("Segmentation: C %d, K %d\n",harq_process->C,harq_process->K);
#endif
  Kr = harq_process->K;
  Kr_bytes = Kr>>3;
  offset = 0;
  void (*nr_processULSegment_ptr)(void*) = &nr_processULSegment;

  // for (r=0; r<harq_process->C; r++) {

    E = nr_get_E(G, harq_process->C, Qm, n_layers, r);

    union ldpcReqUnion id = {.s={ulsch->rnti,frame,nr_tti_rx,0,0}};
    notifiedFIFO_elt_t *req=newNotifiedFIFO_elt(sizeof(ldpcDecode_t), id.p, phy_vars_gNB->respDecode, nr_processULSegment_ptr);
    ldpcDecode_t * rdata=(ldpcDecode_t *) NotifiedFifoData(req);

    rdata->gNB = phy_vars_gNB;
    rdata->ulsch_harq = harq_process;
    rdata->decoderParms = decParams;
    rdata->ulsch_llr = ulsch_llr;
    rdata->Kc = kc;
    rdata->harq_pid = harq_pid;
    rdata->segment_r = r;
    rdata->nbSegments = harq_process->C;
    rdata->E = E;
    rdata->A = A;
    rdata->Qm = Qm;
    rdata->r_offset = r_offset;
    rdata->Kr_bytes = Kr_bytes;
    rdata->rv_index = pusch_pdu->pusch_data.rv_index;
    rdata->Tbslbrm = Tbslbrm;
    rdata->offset = offset;
    rdata->ulsch = ulsch;
    rdata->ulsch_id = ULSCH_id;
    pushTpool(phy_vars_gNB->threadPool,req);
    phy_vars_gNB->nbDecode++;
    LOG_D(PHY,"Added a block to decode, in pipe: %d\n",phy_vars_gNB->nbDecode);
    r_offset += E;
    offset += (Kr_bytes - (harq_process->F>>3) - ((harq_process->C>1)?3:0));
    //////////////////////////////////////////////////////////////////////////////////////////
  // }
  return 1;
}
