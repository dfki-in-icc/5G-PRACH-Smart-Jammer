/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this file
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


#include <stdio.h>
#include "oran_isolate.h"
#include "xran_lib_wrap.hpp"
#include "common.hpp"
#include "xran_compression.h"
#include "xran_sync_api.h"

// Declare variable useful for the send buffer function
struct xran_device_ctx *p_xran_dev_ctx_2;
volatile uint8_t first_call_set = 0;
volatile uint8_t first_rx_set = 0;

// Variable declaration useful for fill IQ samples from file
#define IQ_PLAYBACK_BUFFER_BYTES (XRAN_NUM_OF_SLOT_IN_TDD_LOOP*N_SYM_PER_SLOT*XRAN_MAX_PRBS*N_SC_PER_PRB*4L)
int16_t    *p_tx_play_buffer[MAX_ANT_CARRIER_SUPPORTED];
int        iq_playback_buffer_size_dl = IQ_PLAYBACK_BUFFER_BYTES;
int32_t    tx_play_buffer_size[MAX_ANT_CARRIER_SUPPORTED];
int32_t    tx_play_buffer_position[MAX_ANT_CARRIER_SUPPORTED];
int rx_tti;
int rx_sym;
volatile uint32_t rx_cb_tti = 0;
volatile uint32_t rx_cb_frame = 0;
volatile uint32_t rx_cb_subframe = 0;
volatile uint32_t rx_cb_slot = 0;

#define GetFrameNum(tti,SFNatSecStart,numSubFramePerSystemFrame, numSlotPerSubFrame)  ((((uint32_t)tti / ((uint32_t)numSubFramePerSystemFrame * (uint32_t)numSlotPerSubFrame)) + SFNatSecStart) & 0x3FF)
#define GetSlotNum(tti, numSlotPerSfn) ((uint32_t)tti % ((uint32_t)numSlotPerSfn))

// Declare the function useful to load IQs from file
int sys_load_file_to_buff(char *filename, char *bufname, unsigned char *pBuffer, unsigned int size, unsigned int buffers_num)
{
    unsigned int  file_size = 0;
    int  num= 0;

    if (size)
    {
        if (filename && bufname)
        {
            FILE           *file;
            printf("Loading file %s to  %s: ", filename, bufname);
            file = fopen(filename, "rb");


            if (file == NULL)
            {
                printf("can't open file %s!!!", filename);
                exit(-1);
            }
            else
            {
                fseek(file, 0, SEEK_END);
                file_size = ftell(file);
                fseek(file, 0, SEEK_SET);

                if ((file_size > size) || (file_size == 0))
                    file_size = size;

                printf("Reading IQ samples from file: File Size: %d [Buffer Size: %d]\n", file_size, size);

                num = fread(pBuffer, buffers_num, size, file);
                fflush(file);
                fclose(file);
                printf("from addr (0x%lx) size (%d) bytes num (%d)", (uint64_t)pBuffer, file_size, num);
            }
            printf(" \n");

        }
        else
        {
            printf(" the file name, buffer name are not set!!!");
        }
    }
    else
    {
        printf(" the %s is free: size = %d bytes!!!", bufname, size);
    }
    return num;
}


void xran_fh_rx_callback(void *pCallbackTag, xran_status_t status){
    xran_cb_tag *callback_tag = (xran_cb_tag *)pCallbackTag;
    uint64_t second;
    uint32_t tti;
    uint32_t frame;
    uint32_t subframe;
    uint32_t slot;
    tti = xran_get_slot_idx(&frame,&subframe,&slot,&second);

    rx_tti = callback_tag->slotiId;
    rx_sym = callback_tag->symbol;
    if (rx_sym == 7) {
      if (first_call_set) {
        if (!first_rx_set) {
          printf("first_rx is set\n");
        }
        first_rx_set = 1;
      }
      rx_cb_tti = tti;
      rx_cb_frame = frame;
      rx_cb_subframe = subframe;
      rx_cb_slot = slot;
    }
}
void xran_fh_srs_callback(void *pCallbackTag, xran_status_t status){
    rte_pause();
}
void xran_fh_rx_prach_callback(void *pCallbackTag, xran_status_t status){
    rte_pause();
}


int physide_dl_tti_call_back(void * param)
{
  if (!first_call_set)
    printf("first_call set from phy cb first_call_set=%p\n",&first_call_set);
  first_call_set = 1;
  return 0;
}

int physide_ul_half_slot_call_back(void * param)
{
    rte_pause();
    return 0;
}

int physide_ul_full_slot_call_back(void * param)
{
    rte_pause();
    return 0;
}


#ifdef __cplusplus
extern "C"
{
#endif
void* define_oran_pointer(){
   xranLibWraper *xranlib;
   xranlib = new xranLibWraper;
  //xranLibWraper *xranlib = (xranLibWraper*) calloc(1,sizeof(xranLibWraper));

  return xranlib;
}
#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
extern "C"
{
#endif
void dump_oran_config(void *xranlib_){
   xranLibWraper *xranlib = ((xranLibWraper *) xranlib_);
   int numerology  = xranlib->get_numerology();
   int duplex_type = xranlib->get_duplextype();
   int num_cc      = xranlib->get_num_cc();
   int num_eaxc    = xranlib->get_num_eaxc();
   int num_eaxc_ul = xranlib->get_num_eaxc_ul();
   int ndlrbs      = xranlib->get_num_dlrbs();
   int nulrbs      = xranlib->get_num_ulrbs();

   printf("**--**--**--**--**--**--**--**--**--**--**--**\n");
   printf("Dump ORAN Configuration\n");
   printf("* Numerology     = %d\n", numerology);
   printf("* Duplex Type    = %d\n", duplex_type);
   printf("* Number CC      = %d\n", num_cc);
   printf("* Number eAxc    = %d\n", num_eaxc);
   printf("* Number eAxc UL = %d\n", num_eaxc_ul);
   printf("* Number RBs DL  = %d\n", ndlrbs);
   printf("* Number RBs UL  = %d\n", nulrbs);
   printf("**--**--**--**--**--**--**--**--**--**--**--**\n");
}
#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
extern "C"
{
#endif
int setup_oran( void *xranlib_ ){
  xranLibWraper *xranlib = ((xranLibWraper *) xranlib_);
  if(xranlib->SetUp() < 0) {
     return (-1);
  }
  return (0);
}
#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
extern "C"
{
#endif
int open_oran_callback(void *xranlib_){
  xranLibWraper *xranlib = ((xranLibWraper *) xranlib_);
  xranlib->Open(nullptr,
            nullptr,
            (void *)xran_fh_rx_callback,
            (void *)xran_fh_rx_prach_callback,
            (void *)xran_fh_srs_callback);
  
  return(0);

}
#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
extern "C"
{
#endif
int open_oran(void *xranlib_){
  xranLibWraper *xranlib = ((xranLibWraper *) xranlib_);
  struct xran_fh_config *pCfg = (struct xran_fh_config*) malloc(sizeof(struct xran_fh_config));
  assert(pCfg != NULL);
  xranlib->get_cfg_fh(pCfg);
  xran_open(xranlib->get_xranhandle(),pCfg);

  return(0);
}
#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
extern "C"
{
#endif
int initialize_oran(void *xranlib_){
  xranLibWraper *xranlib = ((xranLibWraper *) xranlib_);

  xranlib->Init();

  return(0);
}
#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
extern "C"
{
#endif
int start_oran(void *xranlib_){
  xranLibWraper *xranlib = ((xranLibWraper *) xranlib_);
  xranlib->Start();

  return (0);

}
#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
extern "C"
{
#endif
int stop_oran(void *xranlib_){
  xranLibWraper *xranlib = ((xranLibWraper *) xranlib_);
  xranlib->Stop();

  return (0);

}
#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
extern "C"
{
#endif
int close_oran(void *xranlib_){
  xranLibWraper *xranlib = ((xranLibWraper *) xranlib_);
  xranlib->Close();

  return (0);

}
#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
extern "C"
{
#endif
int register_physide_callbacks(void *xranlib_){
  xranLibWraper *xranlib = ((xranLibWraper *) xranlib_);
  
  xran_reg_physide_cb(xranlib->get_xranhandle(), physide_dl_tti_call_back, xranlib, 1, XRAN_CB_TTI);
  xran_reg_physide_cb(xranlib->get_xranhandle(), physide_ul_half_slot_call_back, NULL, 10, XRAN_CB_HALF_SLOT_RX);
  xran_reg_physide_cb(xranlib->get_xranhandle(), physide_ul_full_slot_call_back, NULL, 10, XRAN_CB_FULL_SLOT_RX);

  return (0);

}
#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
extern "C"
{
#endif
int load_iq_from_file(void *xranlib_){
   xranLibWraper *xranlib = ((xranLibWraper *) xranlib_);

   int  numCCPorts_ = xranlib->get_num_cc();
   int  num_eAxc_   = xranlib->get_num_eaxc();

   printf("numCCPorts_ =%d, num_eAxc_=%d, MAX_ANT_CARRIER_SUPPORTED =%d\n",numCCPorts_,num_eAxc_,MAX_ANT_CARRIER_SUPPORTED);

   int i;
   const char *IQ_filename[MAX_ANT_CARRIER_SUPPORTED];
   for(i=0; i<MAX_ANT_CARRIER_SUPPORTED; i++){
      if( (i==0) || (i==1) || (i==2) || (i==3) ){
         IQ_filename[0] = "/home/oba/PISONS/phy/fhi_lib/app/usecase/mu0_5mhz/ant_0.bin";
         IQ_filename[1] = "/home/oba/PISONS/phy/fhi_lib/app/usecase/mu0_5mhz/ant_1.bin";
         IQ_filename[2] = "/home/oba/PISONS/phy/fhi_lib/app/usecase/mu0_5mhz/ant_2.bin";
         IQ_filename[3] = "/home/oba/PISONS/phy/fhi_lib/app/usecase/mu0_5mhz/ant_3.bin";
      }else{
          IQ_filename[i] = "/0";
      }
   }

   int32_t number_slots =  40;                         // According to wrapper.hpp  uint32_t m_nSlots = 10; but for the file 5MHz is set to 40 
   uint32_t numerology   =  xranlib->get_numerology(); // According to the conf file is mu number
   uint32_t bandwidth    =  5;                         // According to the wrapper.hpp since we are reading the 5MHz files
   uint32_t sub6         =  xranlib->get_sub6();
   iq_playback_buffer_size_dl = (number_slots * N_SYM_PER_SLOT * N_SC_PER_PRB * xranlib->get_num_rbs(numerology,bandwidth,sub6)*4L);
   const char *string_msg = "DL IFFT IN IQ Samples in binary format";

   for(i = 0; i < MAX_ANT_CARRIER_SUPPORTED && i < (int32_t)(numCCPorts_ * num_eAxc_); i++) {
        char *name   = const_cast<char*>(IQ_filename[i]);
        char *string = const_cast<char*>(string_msg); 
        if(((uint8_t *)IQ_filename[i])[0]!=0){

                p_tx_play_buffer[i]    = (int16_t*)malloc(iq_playback_buffer_size_dl);
                assert (NULL != (p_tx_play_buffer[i]));
                tx_play_buffer_size[i] = (int32_t)iq_playback_buffer_size_dl;

                printf("Loading file [%d] %s \n",i,IQ_filename[i]);
                tx_play_buffer_size[i] = sys_load_file_to_buff( name,
                                                               string,
                                                               (uint8_t*) p_tx_play_buffer[i],
                                                               tx_play_buffer_size[i],
                                                               1);
                tx_play_buffer_position[i] = 0;
        } else {

                p_tx_play_buffer[i]=(int16_t*)malloc(iq_playback_buffer_size_dl);
                tx_play_buffer_size[i]=0;
                tx_play_buffer_position[i] = 0;
        }
   }

return(0);

}
#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
extern "C"
{
#endif
int xran_fh_tx_send_buffer(void *xranlib_){
  xranLibWraper *xranlib = ((xranLibWraper *) xranlib_);

  int32_t flowId;
  void *ptr = NULL;
  char *pos = NULL;

        p_xran_dev_ctx_2 = xran_dev_get_ctx();
       if (p_xran_dev_ctx_2 != NULL){
          printf("p_xran_dev_ctx_2=%p\n",p_xran_dev_ctx_2);
       }

       int num_eaxc = xranlib->get_num_eaxc();
       int num_eaxc_ul = xranlib->get_num_eaxc_ul();
       uint32_t xran_max_antenna_nr = RTE_MAX(num_eaxc, num_eaxc_ul);
       int32_t nSectorNum;

       /*
       for (nSectorNum = 0; nSectorNum < XRAN_MAX_SECTOR_NR; nSectorNum++)
       {
           nSectorIndex[nSectorNum] = nSectorNum;
       }
       */
       nSectorNum = xranlib->get_num_cc();

       int maxflowid = num_eaxc * (nSectorNum-1) + (xran_max_antenna_nr-1);
       printf("the maximum flowID will be=%d\n",maxflowid);

       for(uint16_t cc_id=0; cc_id<nSectorNum; cc_id++){
          for(int32_t tti  = 0; tti  < XRAN_N_FE_BUF_LEN; tti++) {
             for(uint8_t ant_id = 0; ant_id < xran_max_antenna_nr; ant_id++){
                for(int32_t sym_idx = 0; sym_idx < XRAN_NUM_OF_SYMBOL_PER_SLOT; sym_idx++) {

                   flowId = num_eaxc * cc_id + ant_id;
                   uint8_t *pData = p_xran_dev_ctx_2->sFrontHaulTxBbuIoBufCtrl[tti % XRAN_N_FE_BUF_LEN][cc_id][ant_id].sBufferList.pBuffers[sym_idx%XRAN_NUM_OF_SYMBOL_PER_SLOT].pData;
                   uint8_t *pPrbMapData = p_xran_dev_ctx_2->sFrontHaulTxPrbMapBbuIoBufCtrl[tti % XRAN_N_FE_BUF_LEN][cc_id][ant_id].sBufferList.pBuffers->pData;
                   struct xran_prb_map *pPrbMap = (struct xran_prb_map *)pPrbMapData;
                   ptr = pData;
                   pos = ((char*)p_tx_play_buffer[flowId]) + tx_play_buffer_position[flowId];

                   uint8_t *u8dptr;
                   struct xran_prb_map *pRbMap = pPrbMap;
                   int32_t sym_id = sym_idx%XRAN_NUM_OF_SYMBOL_PER_SLOT;
                   if(ptr && pos){
                      uint32_t idxElm = 0;
                      u8dptr = (uint8_t*)ptr;
                      int16_t payload_len = 0;

                      uint8_t  *dst = (uint8_t *)u8dptr;
                      uint8_t  *src = (uint8_t *)pos;
                      struct xran_prb_elm* p_prbMapElm = &pRbMap->prbMap[idxElm];

                      dst =  xran_add_hdr_offset(dst, p_prbMapElm->compMethod);
                      for (idxElm = 0;  idxElm < pRbMap->nPrbElm; idxElm++) {
                         struct xran_section_desc *p_sec_desc = NULL;
                         p_prbMapElm = &pRbMap->prbMap[idxElm];
                         p_sec_desc =  p_prbMapElm->p_sec_desc[sym_id];

                         if(p_sec_desc == NULL){
                            printf ("p_sec_desc == NULL\n");
                            exit(-1);
                         }
                         src = (uint8_t *)(pos + p_prbMapElm->nRBStart*N_SC_PER_PRB*4L);

                         if(p_prbMapElm->compMethod == XRAN_COMPMETHOD_NONE) {
                            payload_len = p_prbMapElm->nRBSize*N_SC_PER_PRB*4L;
                            rte_memcpy(dst, src, payload_len);
                         } else if (p_prbMapElm->compMethod == XRAN_COMPMETHOD_BLKFLOAT) {
                            printf("idxElm=%d, compMeth==BLKFLOAT\n",idxElm);
                            struct xranlib_compress_request  bfp_com_req;
                            struct xranlib_compress_response bfp_com_rsp;

                            memset(&bfp_com_req, 0, sizeof(struct xranlib_compress_request));
                            memset(&bfp_com_rsp, 0, sizeof(struct xranlib_compress_response));

                            bfp_com_req.data_in    = (int16_t*)src;
                            bfp_com_req.numRBs     = p_prbMapElm->nRBSize;
                            bfp_com_req.len        = p_prbMapElm->nRBSize*N_SC_PER_PRB*4L;
                            bfp_com_req.compMethod = p_prbMapElm->compMethod;
                            bfp_com_req.iqWidth    = p_prbMapElm->iqWidth;

                            bfp_com_rsp.data_out   = (int8_t*)dst;
                            bfp_com_rsp.len        = 0;

                            xranlib_compress_avx512(&bfp_com_req, &bfp_com_rsp);
                            payload_len = bfp_com_rsp.len;
                         }else {
                            printf ("p_prbMapElm->compMethod == %d is not supported\n",
                                     p_prbMapElm->compMethod);
                            exit(-1);
                         }
                         p_sec_desc->iq_buffer_offset = RTE_PTR_DIFF(dst, u8dptr);
                         p_sec_desc->iq_buffer_len = payload_len;
                         
                         dst += payload_len;
                         dst  = xran_add_hdr_offset(dst, p_prbMapElm->compMethod);
                     }
                   } else {
                       exit(-1);
                       printf("ptr ==NULL\n");
                   }
                }
              }
            }
          }
return(0);                                   

}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C"
{
#endif
	int read_prach_data(ru_info_t *ru, int frame, int slot)
	{
		p_xran_dev_ctx_2 = xran_dev_get_ctx();
		struct rte_mbuf *mb;

		/* calculate tti and subframe_id from frame, slot num */
		int tti = 20 * (frame) + (slot);
		uint32_t subframe = XranGetSubFrameNum(tti, 2, 10);
		uint32_t is_prach_slot = xran_is_prach_slot(subframe, (slot % 2));
		int sym_idx = 0;

		struct xran_prach_cp_config *pPrachCPConfig = &(p_xran_dev_ctx_2->PrachCPConfig);

		/* If it is PRACH slot, copy prach IQ from XRAN PRACH buffer to OAI PRACH buffer */
		if(is_prach_slot) {

			for(sym_idx = 0; sym_idx < pPrachCPConfig->numSymbol; sym_idx++) {
				mb = (struct rte_mbuf *) p_xran_dev_ctx_2->sFHPrachRxBbuIoBufCtrl[tti % 40][0][0].sBufferList.pBuffers[sym_idx].pCtrl;
				if(mb) {
					uint16_t *dst, *src;
					int idx = 0;
					dst = (uint16_t * )((uint8_t *)ru->prach_buf + (sym_idx*576));
					src = (uint16_t *) ((uint8_t *) p_xran_dev_ctx_2->sFHPrachRxBbuIoBufCtrl[tti % 40][0][0].sBufferList.pBuffers[sym_idx].pData);

					/* convert Network order to host order */
					for (idx = 0; idx < 576/2; idx++)
					{
						dst[idx] = ntohs(src[idx]);
					}

				} else {
					/* TODO: Unlikely this code never gets executed */
					printf("%s():%d, There is no prach ctrl data for symb %d\n", __func__, __LINE__, sym_idx);
				}
			}
		}

		return(0);
	}
#ifdef __cplusplus
}
#endif




#ifdef __cplusplus
extern "C"
{
#endif
int xran_fh_rx_read_slot(void *xranlib_, ru_info_t *ru, int *frame, int *slot, int oframe, int oslot, uint8_t sync){
  xranLibWraper *xranlib = ((xranLibWraper *) xranlib_);

  void *ptr = NULL;
  int32_t  *pos = NULL;
  int idx = 0;
  static int print_tmp = 1;
  static int print_tmp_1 = 1;

  while (first_call_set != 1){
    if (print_tmp) {
      print_tmp = 0;
      printf("wait in ru_thread() till first_call_set is set in xran\n");
    }
  }

  while (first_rx_set != 1){
    if (print_tmp_1) {
      print_tmp_1 = 0;
      printf("wait in ru_thread for first_rx_set to set in xran\n");
    }
  }

  volatile uint32_t prev_rx_cb_tti;
  int tti;

  prev_rx_cb_tti = rx_cb_tti;
  *frame = rx_cb_frame;
  *slot  = (rx_cb_subframe*2)+rx_cb_slot;
  tti    = (*frame*20) + *slot;

  while (rx_cb_tti == prev_rx_cb_tti) {
  }


  read_prach_data(ru, *frame, *slot);
        p_xran_dev_ctx_2 = xran_dev_get_ctx();

       int num_eaxc = xranlib->get_num_eaxc();
       int num_eaxc_ul = xranlib->get_num_eaxc_ul();
       uint32_t xran_max_antenna_nr = RTE_MAX(num_eaxc, num_eaxc_ul);
       
       int slot_offset_rxdata = 3&(*slot);
       uint32_t slot_size = 4*14*4096;
       uint8_t *rx_data = (uint8_t *)ru->rxdataF[0];
       uint8_t *start_ptr = NULL;
       start_ptr = rx_data + (slot_size*slot_offset_rxdata);
       for(uint16_t cc_id=0; cc_id<1/*nSectorNum*/; cc_id++){ // OAI does not support multiple CC yet.
           for(uint8_t ant_id = 0; ant_id < xran_max_antenna_nr && ant_id<ru->nb_rx; ant_id++){
              // This loop would better be more inner to avoid confusion and maybe also errors.
              for(int32_t sym_idx = 0; sym_idx < XRAN_NUM_OF_SYMBOL_PER_SLOT; sym_idx++) {

                 uint8_t *pData = p_xran_dev_ctx_2->sFrontHaulRxBbuIoBufCtrl[tti % XRAN_N_FE_BUF_LEN][cc_id][ant_id].sBufferList.pBuffers[sym_idx%XRAN_NUM_OF_SYMBOL_PER_SLOT].pData;
                 uint8_t *pPrbMapData = p_xran_dev_ctx_2->sFrontHaulRxPrbMapBbuIoBufCtrl[tti % XRAN_N_FE_BUF_LEN][cc_id][ant_id].sBufferList.pBuffers->pData;
                 struct xran_prb_map *pPrbMap = (struct xran_prb_map *)pPrbMapData;
                 ptr = pData;
                 pos = (int32_t *)(start_ptr + (4*sym_idx*4096));

                 uint8_t *u8dptr;
                 struct xran_prb_map *pRbMap = pPrbMap;
                 int32_t sym_id = sym_idx%XRAN_NUM_OF_SYMBOL_PER_SLOT;
                 if(ptr && pos){
                    uint32_t idxElm = 0;
                    u8dptr = (uint8_t*)ptr;
                    int16_t payload_len = 0;

                    uint8_t *src = (uint8_t *)u8dptr;
                    // first half
                    uint8_t  *src1 = (uint8_t *)u8dptr;
                    uint8_t  *dst1 = (uint8_t *)pos;
                    // second half
                    uint8_t  *src2 = (uint8_t *)u8dptr;
                    uint8_t  *dst2 = (uint8_t *)pos;

                    struct xran_prb_elm* p_prbMapElm = &pRbMap->prbMap[idxElm];

                    for (idxElm = 0;  idxElm < pRbMap->nPrbElm; idxElm++) {
                       struct xran_section_desc *p_sec_desc = NULL;
                       p_prbMapElm = &pRbMap->prbMap[idxElm];
                       p_sec_desc =  p_prbMapElm->p_sec_desc[sym_id];

                       if(pRbMap->nPrbElm==1 && idxElm==0){
                         src = pData;
                       }
                       else if(p_sec_desc->pData==NULL){
                         return -1;
                       }else{
                         src =  p_sec_desc->pData;
                       }

                       src2 = src;

                       if(p_sec_desc == NULL){
                          printf ("p_sec_desc == NULL\n");
                          exit(-1);
                       }
                       // Calculation of the pointer for the section in the buffer.
                       // first half
                       dst1 = (uint8_t *)(pos + p_prbMapElm->nRBStart*N_SC_PER_PRB);
                       // second half
                       dst2 = (uint8_t *)(pos + (p_prbMapElm->nRBStart*N_SC_PER_PRB + 3276/2) + 4096 - 3276);
                       if(p_prbMapElm->compMethod == XRAN_COMPMETHOD_NONE) {
                          payload_len = p_prbMapElm->nRBSize*N_SC_PER_PRB*4L;
                          src1 = src2 + payload_len/2;
                          for (idx = 0; idx < payload_len/(2*sizeof(int16_t)); idx++) {
                            ((uint16_t *)dst1)[idx] = ntohs(((uint16_t *)src1)[idx]);
                            ((uint16_t *)dst2)[idx] = ntohs(((uint16_t *)src2)[idx]);
                          }
                       } else if (p_prbMapElm->compMethod == XRAN_COMPMETHOD_BLKFLOAT) {
                          struct xranlib_decompress_request  bfp_decom_req_2;
                          struct xranlib_decompress_response bfp_decom_rsp_2;
                          struct xranlib_decompress_request  bfp_decom_req_1;
                          struct xranlib_decompress_response bfp_decom_rsp_1;


                          payload_len = (3* p_prbMapElm->iqWidth + 1)*p_prbMapElm->nRBSize;

                          memset(&bfp_decom_req_2, 0, sizeof(struct xranlib_decompress_request));
                          memset(&bfp_decom_rsp_2, 0, sizeof(struct xranlib_decompress_response));
                          memset(&bfp_decom_req_1, 0, sizeof(struct xranlib_decompress_request));
                          memset(&bfp_decom_rsp_1, 0, sizeof(struct xranlib_decompress_response));

                          bfp_decom_req_2.data_in    = (int8_t*)src2;
                          bfp_decom_req_2.numRBs     = p_prbMapElm->nRBSize/2;
                          bfp_decom_req_2.len        = payload_len/2;
                          bfp_decom_req_2.compMethod = p_prbMapElm->compMethod;
                          bfp_decom_req_2.iqWidth    = p_prbMapElm->iqWidth;

                          bfp_decom_rsp_2.data_out   = (int16_t*)dst2;
                          bfp_decom_rsp_2.len        = 0;

                          xranlib_decompress_avx512(&bfp_decom_req_2, &bfp_decom_rsp_2);
                          
                          int16_t first_half_len = bfp_decom_rsp_2.len;
                          src1 = src2+(payload_len/2);

                          bfp_decom_req_1.data_in    = (int8_t*)src1;
                          bfp_decom_req_1.numRBs     = p_prbMapElm->nRBSize/2;
                          bfp_decom_req_1.len        = payload_len/2;
                          bfp_decom_req_1.compMethod = p_prbMapElm->compMethod;
                          bfp_decom_req_1.iqWidth    = p_prbMapElm->iqWidth;

                          bfp_decom_rsp_1.data_out   = (int16_t*)dst1;
                          bfp_decom_rsp_1.len        = 0;

                          xranlib_decompress_avx512(&bfp_decom_req_1, &bfp_decom_rsp_1);
                          payload_len = bfp_decom_rsp_1.len+first_half_len;
                       }else {
                          printf ("p_prbMapElm->compMethod == %d is not supported\n",
                                   p_prbMapElm->compMethod);
                          exit(-1);
                       }
                   }

                } else {
                     exit(-1);
                     printf("ptr ==NULL\n");
                }
              }
            }
          }
return(0);                                   
}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C"
{
#endif
int xran_fh_tx_send_slot(void *xranlib_, ru_info_t *ru, int frame, int slot, uint64_t timestamp){
  xranLibWraper *xranlib = ((xranLibWraper *) xranlib_);


  int tti = /*frame*SUBFRAMES_PER_SYSTEMFRAME*SLOTNUM_PER_SUBFRAME+*/20*frame+slot; //commented out temporarily to check that compilation of oran 5g is working.

  void *ptr = NULL;
  int32_t  *pos = NULL;
  int idx = 0;

        p_xran_dev_ctx_2 = xran_dev_get_ctx();
#if 0
       if (p_xran_dev_ctx_2 != NULL){
          printf("p_xran_dev_ctx_2=%d\n",p_xran_dev_ctx_2);
       }
#endif

       int num_eaxc = xranlib->get_num_eaxc();
       int num_eaxc_ul = xranlib->get_num_eaxc_ul();
       uint32_t xran_max_antenna_nr = RTE_MAX(num_eaxc, num_eaxc_ul);
       /*
       for (nSectorNum = 0; nSectorNum < XRAN_MAX_SECTOR_NR; nSectorNum++)
       {
           nSectorIndex[nSectorNum] = nSectorNum;
       }
       */

       for(uint16_t cc_id=0; cc_id<1/*nSectorNum*/; cc_id++){ // OAI does not support multiple CC yet.
           for(uint8_t ant_id = 0; ant_id < xran_max_antenna_nr && ant_id<ru->nb_tx; ant_id++){
              // This loop would better be more inner to avoid confusion and maybe also errors.
              for(int32_t sym_idx = 0; sym_idx < XRAN_NUM_OF_SYMBOL_PER_SLOT; sym_idx++) {

                 uint8_t *pData = p_xran_dev_ctx_2->sFrontHaulTxBbuIoBufCtrl[tti % XRAN_N_FE_BUF_LEN][cc_id][ant_id].sBufferList.pBuffers[sym_idx%XRAN_NUM_OF_SYMBOL_PER_SLOT].pData;
                 uint8_t *pPrbMapData = p_xran_dev_ctx_2->sFrontHaulTxPrbMapBbuIoBufCtrl[tti % XRAN_N_FE_BUF_LEN][cc_id][ant_id].sBufferList.pBuffers->pData;
                 struct xran_prb_map *pPrbMap = (struct xran_prb_map *)pPrbMapData;
                 ptr = pData;
                 pos = &ru->txdataF_BF[ant_id][sym_idx * 4096 /*fp->ofdm_symbol_size*/]; // We had to use a different ru structure than benetel so the access to the buffer is not the same.

                 uint8_t *u8dptr;
                 struct xran_prb_map *pRbMap = pPrbMap;
                 int32_t sym_id = sym_idx%XRAN_NUM_OF_SYMBOL_PER_SLOT;
                 if(ptr && pos){
                    uint32_t idxElm = 0;
                    u8dptr = (uint8_t*)ptr;
                    int16_t payload_len = 0;

                    uint8_t *dst = (uint8_t *)u8dptr;
                    // first half
                    uint8_t  *dst1 = (uint8_t *)u8dptr;
                    uint8_t  *src1 = (uint8_t *)pos;
                    // second half
                    uint8_t  *dst2 = (uint8_t *)u8dptr;
                    uint8_t  *src2 = (uint8_t *)pos;

                    struct xran_prb_elm* p_prbMapElm = &pRbMap->prbMap[idxElm];

                    for (idxElm = 0;  idxElm < pRbMap->nPrbElm; idxElm++) {
                       struct xran_section_desc *p_sec_desc = NULL;
                       p_prbMapElm = &pRbMap->prbMap[idxElm];
                       p_sec_desc =  p_prbMapElm->p_sec_desc[sym_id];

                       payload_len = p_prbMapElm->nRBSize*N_SC_PER_PRB*4L;
                       dst =  xran_add_hdr_offset(dst, p_prbMapElm->compMethod);
                       dst2 = dst;
                       dst1 = dst2 + payload_len/2;

                       if(p_sec_desc == NULL){
                          printf ("p_sec_desc == NULL\n");
                          exit(-1);
                       }
                       // Calculation of the pointer for the section in the buffer.
                       // first half
                       src1 = (uint8_t *)(pos + p_prbMapElm->nRBStart*N_SC_PER_PRB);
                       // second half
                       src2 = (uint8_t *)(pos + (p_prbMapElm->nRBStart*N_SC_PER_PRB + 3276/2) + 4096 - 3276);
                       if(p_prbMapElm->compMethod == XRAN_COMPMETHOD_NONE) {
                         /* convert to Network order */
                         for (idx = 0; idx < payload_len/(2*sizeof(uint16_t)); idx++)
                         {
                           ((uint16_t *)dst1)[idx] = htons(((uint16_t *)src1)[idx]);
                           ((uint16_t *)dst2)[idx] = htons(((uint16_t *)src2)[idx]);
                         }
                       } else if (p_prbMapElm->compMethod == XRAN_COMPMETHOD_BLKFLOAT) {
                          printf("idxElm=%d, compMeth==BLKFLOAT\n",idxElm);
                          struct xranlib_compress_request  bfp_com_req;
                          struct xranlib_compress_response bfp_com_rsp;

                          memset(&bfp_com_req, 0, sizeof(struct xranlib_compress_request));
                          memset(&bfp_com_rsp, 0, sizeof(struct xranlib_compress_response));

                          bfp_com_req.data_in    = (int16_t*)src2;
                          bfp_com_req.numRBs     = p_prbMapElm->nRBSize/2;
                          bfp_com_req.len        = payload_len/2;
                          bfp_com_req.compMethod = p_prbMapElm->compMethod;
                          bfp_com_req.iqWidth    = p_prbMapElm->iqWidth;

                          bfp_com_rsp.data_out   = (int8_t*)dst2;
                          bfp_com_rsp.len        = 0;

                          xranlib_compress_avx512(&bfp_com_req, &bfp_com_rsp);
                          
                          int16_t first_half_len = bfp_com_rsp.len;

                          dst1 = dst2 + first_half_len;

                          bfp_com_req.data_in    = (int16_t*)src1;
                          bfp_com_req.numRBs     = p_prbMapElm->nRBSize/2;
                          bfp_com_req.len        = payload_len/2;
                          bfp_com_req.compMethod = p_prbMapElm->compMethod;
                          bfp_com_req.iqWidth    = p_prbMapElm->iqWidth;

                          bfp_com_rsp.data_out   = (int8_t*)dst1;
                          bfp_com_rsp.len        = 0;

                          xranlib_compress_avx512(&bfp_com_req, &bfp_com_rsp);
                          payload_len = bfp_com_rsp.len+first_half_len;
                       }else {
                          printf ("p_prbMapElm->compMethod == %d is not supported\n",
                                   p_prbMapElm->compMethod);
                          exit(-1);
                       }

                       p_sec_desc->iq_buffer_offset = RTE_PTR_DIFF(dst, u8dptr);
                       p_sec_desc->iq_buffer_len = payload_len;
                       
                       dst += payload_len;
                       dst  = xran_add_hdr_offset(dst, p_prbMapElm->compMethod);
                   }

                   // The tti should be updated as it increased.
                   pRbMap->tti_id = tti;

                } else {
                     exit(-1);
                     printf("ptr ==NULL\n");
                }
              }
            }
          }
return(0);                                   

}
#ifdef __cplusplus
}
#endif



int64_t count_sec =0;
struct xran_common_counters x_counters;
uint64_t nTotalTime;
uint64_t nUsedTime;
uint32_t nCoreUsed;
float nUsedPercent;
uint64_t old_rx_counter = 0;
uint64_t old_tx_counter = 0;

#ifdef __cplusplus
extern "C"
{
#endif
int compute_xran_statistics(void *xranlib_){
  xranLibWraper *xranlib = ((xranLibWraper *) xranlib_);
           
  if(xran_get_common_counters(xranlib->get_xranhandle(), &x_counters) == XRAN_STATUS_SUCCESS) {
     xran_get_time_stats(&nTotalTime, &nUsedTime, &nCoreUsed, 1);
     nUsedPercent = ((float)nUsedTime * 100.0) / (float)nTotalTime;

     printf("[rx %7ld pps %7ld kbps %7ld][tx %7ld pps %7ld kbps %7ld] [on_time %ld early %ld late %ld corrupt %ld pkt_dupl %ld Total %ld] IO Util: %5.2f %%\n",
                                 x_counters.rx_counter,
                                 x_counters.rx_counter-old_rx_counter,
                                 x_counters.rx_bytes_per_sec*8/1000L,
                                 x_counters.tx_counter,
                                 x_counters.tx_counter-old_tx_counter,
                                 x_counters.tx_bytes_per_sec*8/1000L,
                                 x_counters.Rx_on_time,
                                 x_counters.Rx_early,
                                 x_counters.Rx_late,
                                 x_counters.Rx_corrupt,
                                 x_counters.Rx_pkt_dupl,
                                 x_counters.Total_msgs_rcvd,
                                 nUsedPercent);

      if(x_counters.rx_counter > old_rx_counter)
         old_rx_counter = x_counters.rx_counter;
      if(x_counters.tx_counter > old_tx_counter)
         old_tx_counter = x_counters.tx_counter;
  } else {
      printf("error xran_get_common_counters\n");
      return(1);
  }

  return (0);

}
#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
extern "C"
{
#endif
void check_xran_ptp_sync(){

   if (xran_is_synchronized() != 0)
        printf("Machine is not synchronized using PTP!\n");
    else
        printf("Machine is synchronized using PTP!\n");

}
#ifdef __cplusplus
}
#endif
