#include "PHY/defs_gNB.h"
#include "PHY/phy_extern.h"
#include "nr_transport_proto.h"
#include "PHY/impl_defs_top.h"
#include "PHY/NR_TRANSPORT/nr_sch_dmrs.h"
#include "PHY/NR_REFSIG/dmrs_nr.h"
#include "PHY/NR_REFSIG/ptrs_nr.h"
#include "PHY/NR_ESTIMATION/nr_ul_estimation.h"
#include "PHY/defs_nr_common.h"
#include "common/utils/nr/nr_common.h"
#include "PHY/sse_intrin.h"

//#define DEBUG_CH_COMP
//#define DEBUG_RB_EXT
//#define DEBUG_CH_MAG

#define INVALID_VALUE 255
#define print_shorts(s,x) printf("%s %d,%d,%d,%d,%d,%d,%d,%d\n",s,(x)[0],(x)[1],(x)[2],(x)[3],(x)[4],(x)[5],(x)[6],(x)[7])
#define print_shorts64(s,x) printf("%s %d,%d,%d,%d\n",s,(x)[0],(x)[1],(x)[2],(x)[3])

#include <time.h>
#include <stdint.h>

#include "common/utils/thread_pool/task_manager.h"

#include "nr_ulsch_demodulation.h"

#include <omp.h>


static inline
int64_t time_now_us(void)
{
  struct timespec tms;

  /* The C11 way */
  /* if (! timespec_get(&tms, TIME_UTC))  */

  /* POSIX.1-2008 way */
  if (clock_gettime(CLOCK_REALTIME,&tms)) {
    return -1;
  }
  /* seconds, multiplied with 1 million */
  int64_t micros = tms.tv_sec * 1000000;
  /* Add full microseconds */
  micros += tms.tv_nsec/1000;
  /* round up if necessary */
  if (tms.tv_nsec % 1000 >= 500) {
    ++micros;
  }
  return micros;
}








void nr_idft(int32_t *z, uint32_t Msc_PUSCH)
{

  simde__m128i idft_in128[1][3240], idft_out128[1][3240];
  simde__m128i norm128;
  int16_t *idft_in0 = (int16_t*)idft_in128[0], *idft_out0 = (int16_t*)idft_out128[0];

  int i, ip;

  LOG_T(PHY,"Doing lte_idft for Msc_PUSCH %d\n",Msc_PUSCH);

  if ((Msc_PUSCH % 1536) > 0) {
    // conjugate input
    for (i = 0; i < (Msc_PUSCH>>2); i++) {
      *&(((simde__m128i*)z)[i]) = simde_mm_sign_epi16(*&(((simde__m128i*)z)[i]), *(simde__m128i*)&conjugate2[0]);
    }
    for (i = 0, ip = 0; i < Msc_PUSCH; i++, ip+=4)
      ((uint32_t*)idft_in0)[ip+0] = z[i];
  }

  switch (Msc_PUSCH) {
    case 12:
      dft(DFT_12,(int16_t *)idft_in0, (int16_t *)idft_out0,0);

      norm128 = simde_mm_set1_epi16(9459);

      for (i = 0; i < 12; i++) {
        ((simde__m128i*)idft_out0)[i] = simde_mm_slli_epi16(simde_mm_mulhi_epi16(((simde__m128i*)idft_out0)[i], norm128), 1);
      }

      break;

    case 24:
      dft(DFT_24,idft_in0, idft_out0, 1);
      break;

    case 36:
      dft(DFT_36,idft_in0, idft_out0, 1);
      break;

    case 48:
      dft(DFT_48,idft_in0, idft_out0, 1);
      break;

    case 60:
      dft(DFT_60,idft_in0, idft_out0, 1);
      break;

    case 72:
      dft(DFT_72,idft_in0, idft_out0, 1);
      break;

    case 96:
      dft(DFT_96,idft_in0, idft_out0, 1);
      break;

    case 108:
      dft(DFT_108,idft_in0, idft_out0, 1);
      break;

    case 120:
      dft(DFT_120,idft_in0, idft_out0, 1);
      break;

    case 144:
      dft(DFT_144,idft_in0, idft_out0, 1);
      break;

    case 180:
      dft(DFT_180,idft_in0, idft_out0, 1);
      break;

    case 192:
      dft(DFT_192,idft_in0, idft_out0, 1);
      break;

    case 216:
      dft(DFT_216,idft_in0, idft_out0, 1);
      break;

    case 240:
      dft(DFT_240,idft_in0, idft_out0, 1);
      break;

    case 288:
      dft(DFT_288,idft_in0, idft_out0, 1);
      break;

    case 300:
      dft(DFT_300,idft_in0, idft_out0, 1);
      break;

    case 324:
      dft(DFT_324,(int16_t*)idft_in0, (int16_t*)idft_out0, 1);
      break;

    case 360:
      dft(DFT_360,(int16_t*)idft_in0, (int16_t*)idft_out0, 1);
      break;

    case 384:
      dft(DFT_384,(int16_t*)idft_in0, (int16_t*)idft_out0, 1);
      break;

    case 432:
      dft(DFT_432,(int16_t*)idft_in0, (int16_t*)idft_out0, 1);
      break;

    case 480:
      dft(DFT_480,(int16_t*)idft_in0, (int16_t*)idft_out0, 1);
      break;

    case 540:
      dft(DFT_540,(int16_t*)idft_in0, (int16_t*)idft_out0, 1);
      break;

    case 576:
      dft(DFT_576,(int16_t*)idft_in0, (int16_t*)idft_out0, 1);
      break;

    case 600:
      dft(DFT_600,(int16_t*)idft_in0, (int16_t*)idft_out0, 1);
      break;

    case 648:
      dft(DFT_648,(int16_t*)idft_in0, (int16_t*)idft_out0, 1);
      break;

    case 720:
      dft(DFT_720,(int16_t*)idft_in0, (int16_t*)idft_out0, 1);
      break;

    case 768:
      dft(DFT_768,(int16_t*)idft_in0, (int16_t*)idft_out0, 1);
      break;

    case 864:
      dft(DFT_864,(int16_t*)idft_in0, (int16_t*)idft_out0, 1);
      break;

    case 900:
      dft(DFT_900,(int16_t*)idft_in0, (int16_t*)idft_out0, 1);
      break;

    case 960:
      dft(DFT_960,(int16_t*)idft_in0, (int16_t*)idft_out0, 1);
      break;

    case 972:
      dft(DFT_972,(int16_t*)idft_in0, (int16_t*)idft_out0, 1);
      break;

    case 1080:
      dft(DFT_1080,(int16_t*)idft_in0, (int16_t*)idft_out0, 1);
      break;

    case 1152:
      dft(DFT_1152,(int16_t*)idft_in0, (int16_t*)idft_out0, 1);
      break;

    case 1200:
      dft(DFT_1200,idft_in0, idft_out0, 1);
      break;

    case 1296:
      dft(DFT_1296,(int16_t*)idft_in0, (int16_t*)idft_out0, 1);
      break;

    case 1440:
      dft(DFT_1440,(int16_t*)idft_in0, (int16_t*)idft_out0, 1);
      break;

    case 1500:
      dft(DFT_1500,(int16_t*)idft_in0, (int16_t*)idft_out0, 1);
      break;

    case 1536:
      //dft(DFT_1536,(int16_t*)idft_in0, (int16_t*)idft_out0, 1);
      idft(IDFT_1536,(int16_t*)z, (int16_t*)z, 1);
      break;

    case 1620:
      dft(DFT_1620,(int16_t*)idft_in0, (int16_t*)idft_out0, 1);
      break;

    case 1728:
      dft(DFT_1728,(int16_t*)idft_in0, (int16_t*)idft_out0, 1);
      break;

    case 1800:
      dft(DFT_1800,(int16_t*)idft_in0, (int16_t*)idft_out0, 1);
      break;

    case 1920:
      dft(DFT_1920,(int16_t*)idft_in0, (int16_t*)idft_out0, 1);
      break;

    case 1944:
      dft(DFT_1944,(int16_t*)idft_in0, (int16_t*)idft_out0, 1);
      break;

    case 2160:
      dft(DFT_2160,(int16_t*)idft_in0, (int16_t*)idft_out0, 1);
      break;

    case 2304:
      dft(DFT_2304,(int16_t*)idft_in0, (int16_t*)idft_out0, 1);
      break;

    case 2400:
      dft(DFT_2400,(int16_t*)idft_in0, (int16_t*)idft_out0, 1);
      break;

    case 2592:
      dft(DFT_2592,(int16_t*)idft_in0, (int16_t*)idft_out0, 1);
      break;

    case 2700:
      dft(DFT_2700,(int16_t*)idft_in0, (int16_t*)idft_out0, 1);
      break;

    case 2880:
      dft(DFT_2880,(int16_t*)idft_in0, (int16_t*)idft_out0, 1);
      break;

    case 2916:
      dft(DFT_2916,(int16_t*)idft_in0, (int16_t*)idft_out0, 1);
      break;

    case 3000:
      dft(DFT_3000,(int16_t*)idft_in0, (int16_t*)idft_out0, 1);
      break;

    case 3072:
      //dft(DFT_3072,(int16_t*)idft_in0, (int16_t*)idft_out0, 1);
      idft(IDFT_3072,(int16_t*)z, (int16_t*)z, 1);
      break;

    case 3240:
      dft(DFT_3240,(int16_t*)idft_in0, (int16_t*)idft_out0, 1);
      break;

    default:
      // should not be reached
      LOG_E( PHY, "Unsupported Msc_PUSCH value of %"PRIu16"\n", Msc_PUSCH );
      return;
  }

  if ((Msc_PUSCH % 1536) > 0) {
    for (i = 0, ip = 0; i < Msc_PUSCH; i++, ip+=4)
      z[i] = ((uint32_t*)idft_out0)[ip];

    // conjugate output
    for (i = 0; i < (Msc_PUSCH>>2); i++) {
      ((simde__m128i*)z)[i] = simde_mm_sign_epi16(((simde__m128i*)z)[i], *(simde__m128i*)&conjugate2[0]);
    }
  }

  simde_mm_empty();
  _m_empty();

}


void nr_ulsch_extract_rbs0(int32_t *rxdataF,
                           int32_t *chF,
			   int32_t *rxFext,
			   int32_t *chFext,
			   int rxoffset,
			   int choffset,
			   int aarx,
			   int is_dmrs_symbol,
                           nfapi_nr_pusch_pdu_t *pusch_pdu,
                           NR_DL_FRAME_PARMS *frame_parms)
{

  uint8_t delta = 0;

  int start_re = (frame_parms->first_carrier_offset + (pusch_pdu->rb_start + pusch_pdu->bwp_start) * NR_NB_SC_PER_RB)%frame_parms->ofdm_symbol_size;
  int nb_re_pusch = NR_NB_SC_PER_RB * pusch_pdu->rb_size;


  int32_t *rxF        = &rxdataF[rxoffset];
  int32_t *rxF_ext    = &rxFext[0];
  int32_t *ul_ch0     = &chF[choffset]; 
  int32_t *ul_ch0_ext = &chFext[0];


  if (is_dmrs_symbol == 0) {
    if (start_re + nb_re_pusch <= frame_parms->ofdm_symbol_size) {
      memcpy((void*)rxF_ext,
             (void*)&rxF[start_re],
             nb_re_pusch*sizeof(int32_t));
    } else {
    int neg_length = frame_parms->ofdm_symbol_size-start_re;
    int pos_length = nb_re_pusch-neg_length;

    memcpy((void*)rxF_ext,(void*)&rxF[start_re],neg_length*sizeof(int32_t));
    memcpy((void*)&rxF_ext[neg_length],(void*)rxF,pos_length*sizeof(int32_t));
    }
   memcpy((void*)ul_ch0_ext,(void*)ul_ch0,nb_re_pusch*sizeof(int32_t));
  }
  else if (pusch_pdu->dmrs_config_type == pusch_dmrs_type1) { // 6 REs / PRB
    AssertFatal(delta==0||delta==1,"Illegal delta %d\n",delta);
    int32_t *rxF32        = &rxF[start_re];
    int32_t *rxF_ext32    = rxF_ext;
    int32_t *ul_ch032     = ul_ch0;
    int32_t *ul_ch0_ext32 = ul_ch0_ext;
    int idx,idx2,idx3;
    if (start_re + nb_re_pusch < frame_parms->ofdm_symbol_size) {
      for (idx=1-delta,idx2=0;idx<nb_re_pusch;idx+=2,idx2++) {
        rxF_ext32[idx2] = rxF32[idx];
        ul_ch0_ext32[idx2]= ul_ch032[idx];
      }
    }
    else { // handle the two pieces around DC
      int neg_length = frame_parms->ofdm_symbol_size-start_re;
      int pos_length = nb_re_pusch-neg_length;
        
      for (idx=1-delta,idx2=0;idx<neg_length;idx+=2,idx2++) {
        rxF_ext32[idx2] = rxF32[idx];
        ul_ch0_ext32[idx2]= ul_ch032[idx];
      }
      rxF32=(int32_t*)rxF;
      idx3=idx;
      for (idx=1-delta;idx<pos_length;idx+=2,idx2++,idx3++) {
        rxF_ext32[idx2] = rxF32[idx];
        ul_ch0_ext32[idx2]= ul_ch032[idx3];
      }
    }
  }
  else if (pusch_pdu->dmrs_config_type == pusch_dmrs_type2)  { // 8 REs / PRB
    AssertFatal(delta==0||delta==2||delta==4,"Illegal delta %d\n",delta);
    if (start_re + nb_re_pusch < frame_parms->ofdm_symbol_size) {
      int64_t *rxF64        = (int64_t*)&rxF[start_re];
      int64_t *rxF_ext64    = (int64_t*)rxF_ext;
      int64_t *ul_ch064     = (int64_t*)ul_ch0;
      int64_t *ul_ch0_ext64 = (int64_t*)ul_ch0_ext;
      if (delta==0) {
        for (int idx=0;idx<nb_re_pusch>>1;idx+=6) {
           rxF_ext64[idx]=rxF64[idx+1];
           rxF_ext64[idx+1]=rxF64[idx+2];
           rxF_ext64[idx+2]=rxF64[idx+4];
           rxF_ext64[idx+3]=rxF64[idx+5];
           ul_ch0_ext64[idx]=ul_ch064[idx+1];
           ul_ch0_ext64[idx+1]=ul_ch064[idx+2];
           ul_ch0_ext64[idx+2]=ul_ch064[idx+4];
           ul_ch0_ext64[idx+3]=ul_ch064[idx+5];
        }
      }
      else if (delta==2) {
        for (int idx=0;idx<nb_re_pusch>>1;idx+=6) {
           rxF_ext64[idx]=rxF64[idx+0];
           rxF_ext64[idx+1]=rxF64[idx+2];
           rxF_ext64[idx+2]=rxF64[idx+3];
           rxF_ext64[idx+3]=rxF64[idx+5];
           ul_ch0_ext64[idx]=ul_ch064[idx+0];
           ul_ch0_ext64[idx+1]=ul_ch064[idx+2];
           ul_ch0_ext64[idx+2]=ul_ch064[idx+3];
           ul_ch0_ext64[idx+3]=ul_ch064[idx+5];
        }
      }
      else if (delta==4) {
        for (int idx=0;idx<nb_re_pusch>>1;idx+=6) {
           rxF_ext64[idx]=rxF64[idx+0];
           rxF_ext64[idx+1]=rxF64[idx+1];
           rxF_ext64[idx+2]=rxF64[idx+3];
           rxF_ext64[idx+3]=rxF64[idx+4];
           ul_ch0_ext64[idx]=ul_ch064[idx+0];
           ul_ch0_ext64[idx+1]=ul_ch064[idx+1];
           ul_ch0_ext64[idx+2]=ul_ch064[idx+3];
           ul_ch0_ext64[idx+3]=ul_ch064[idx+4];
        }
      }
    }
    else {
      int neg_length = frame_parms->ofdm_symbol_size-start_re;
      int pos_length = nb_re_pusch-neg_length;
      if ((pos_length%12) > 0 ) pos_length+=12;
      int64_t *rxF64        = (int64_t*)&rxF[start_re];
      int64_t *rxF_ext64    = (int64_t*)rxF_ext;
      int64_t *ul_ch064     = (int64_t*)ul_ch0;
      int64_t *ul_ch0_ext64 = (int64_t*)ul_ch0_ext;
      int idx=0;
      if (delta==0) {
        for (idx=0;idx<neg_length>>1;idx+=6) {
           rxF_ext64[idx]  =rxF64[idx+1];
           rxF_ext64[idx+1]=rxF64[idx+2];
           rxF_ext64[idx+2]=rxF64[idx+4];
           rxF_ext64[idx+3]=rxF64[idx+5];
           ul_ch0_ext64[idx]=ul_ch064[idx+1];
           ul_ch0_ext64[idx+1]=ul_ch064[idx+2];
           ul_ch0_ext64[idx+2]=ul_ch064[idx+4];
           ul_ch0_ext64[idx+3]=ul_ch064[idx+5];
        }
        if ((neg_length%12) > 0) {
          rxF_ext64[idx+4]=rxF64[idx+7];
          rxF_ext64[idx+5]=rxF64[idx+8];
          ul_ch0_ext64[idx+4]=ul_ch064[idx+7];
          ul_ch0_ext64[idx+5]=ul_ch064[idx+8];
        }
        rxF_ext64+=(neg_length/3);
        rxF64=(int64_t*)rxF;
        ul_ch0_ext64+=(neg_length/3);
        ul_ch064+=(neg_length>>1);
        for (idx=0;idx<pos_length>>1;idx+=6) {
           rxF_ext64[idx]  =rxF64[idx+1];
           rxF_ext64[idx+1]=rxF64[idx+2];
           rxF_ext64[idx+2]=rxF64[idx+4];
           rxF_ext64[idx+3]=rxF64[idx+5];
           ul_ch0_ext64[idx]=ul_ch064[idx+1];
           ul_ch0_ext64[idx+1]=ul_ch064[idx+2];
           ul_ch0_ext64[idx+2]=ul_ch064[idx+4];
           ul_ch0_ext64[idx+3]=ul_ch064[idx+5];
        }
      }
      else if (delta==2) {
        for (idx=0;idx<neg_length>>1;idx+=6) {
           rxF_ext64[idx]  =rxF64[idx+0];
           rxF_ext64[idx+1]=rxF64[idx+2];
           rxF_ext64[idx+2]=rxF64[idx+3];
           rxF_ext64[idx+3]=rxF64[idx+5];
           ul_ch0_ext64[idx]=ul_ch064[idx+0];
           ul_ch0_ext64[idx+1]=ul_ch064[idx+2];
           ul_ch0_ext64[idx+2]=ul_ch064[idx+3];
           ul_ch0_ext64[idx+3]=ul_ch064[idx+5];
        }
        if ((neg_length%12) > 0) {
          rxF_ext64[idx+4]=rxF64[idx+6];
          rxF_ext64[idx+5]=rxF64[idx+8];
          ul_ch0_ext64[idx+4]=ul_ch064[idx+6];
          ul_ch0_ext64[idx+5]=ul_ch064[idx+8];
        }
        rxF_ext64+=(neg_length/3);
        rxF64=(int64_t*)rxF;
        ul_ch0_ext64+=(neg_length/3);
        ul_ch064+=(neg_length>>1);
        for (idx=0;idx<pos_length>>1;idx+=6) {
           rxF_ext64[idx]  =rxF64[idx+0];
           rxF_ext64[idx+1]=rxF64[idx+2];
           rxF_ext64[idx+2]=rxF64[idx+3];
           rxF_ext64[idx+3]=rxF64[idx+5];
           ul_ch0_ext64[idx]=ul_ch064[idx+0];
           ul_ch0_ext64[idx+1]=ul_ch064[idx+2];
           ul_ch0_ext64[idx+2]=ul_ch064[idx+3];
           ul_ch0_ext64[idx+3]=ul_ch064[idx+5];
        }
      }
      else if (delta==4) {
        for (idx=0;idx<neg_length>>1;idx+=6) {
           rxF_ext64[idx]  =rxF64[idx+0];
           rxF_ext64[idx+1]=rxF64[idx+1];
           rxF_ext64[idx+2]=rxF64[idx+3];
           rxF_ext64[idx+3]=rxF64[idx+4];
           ul_ch0_ext64[idx]=ul_ch064[idx+0];
           ul_ch0_ext64[idx+1]=ul_ch064[idx+1];
           ul_ch0_ext64[idx+2]=ul_ch064[idx+3];
           ul_ch0_ext64[idx+3]=ul_ch064[idx+4];
        }
        if ((neg_length%12) > 0) {
          rxF_ext64[idx+4]=rxF64[idx+6];
          rxF_ext64[idx+5]=rxF64[idx+7];
          ul_ch0_ext64[idx+4]=ul_ch064[idx+6];
          ul_ch0_ext64[idx+5]=ul_ch064[idx+7];
        }
        rxF_ext64+=(neg_length/3);
        rxF64=(int64_t*)rxF;
        ul_ch0_ext64+=(neg_length/3);
        ul_ch064+=(neg_length>>1);
        for (idx=0;idx<pos_length>>1;idx+=6) {
           rxF_ext64[idx]  =rxF64[idx+0];
           rxF_ext64[idx+1]=rxF64[idx+1];
           rxF_ext64[idx+2]=rxF64[idx+3];
           rxF_ext64[idx+3]=rxF64[idx+4];
           ul_ch0_ext64[idx]=ul_ch064[idx+0];
           ul_ch0_ext64[idx+1]=ul_ch064[idx+1];
           ul_ch0_ext64[idx+2]=ul_ch064[idx+3];
           ul_ch0_ext64[idx+3]=ul_ch064[idx+4];
        }
      }
    }
  }
}

void nr_ulsch_extract_rbs(int32_t **rxdataF,
                          NR_gNB_PUSCH *pusch_vars,
                          int slot,
                          unsigned char symbol,
                          uint8_t is_dmrs_symbol,
                          nfapi_nr_pusch_pdu_t *pusch_pdu,
                          NR_DL_FRAME_PARMS *frame_parms) {

  unsigned char aarx;
  int16_t *rxF,*rxF_ext;
  int *ul_ch0,*ul_ch0_ext;
  int soffset = (slot&3)*frame_parms->symbols_per_slot*frame_parms->ofdm_symbol_size;

#ifdef DEBUG_RB_EXT
  printf("--------------------symbol = %d-----------------------\n", symbol);
  printf("--------------------ch_ext_index = %d-----------------------\n", symbol*NR_NB_SC_PER_RB * pusch_pdu->rb_size);
#endif
  int delta=0;
  int start_re = (frame_parms->first_carrier_offset + (pusch_pdu->rb_start + pusch_pdu->bwp_start) * NR_NB_SC_PER_RB)%frame_parms->ofdm_symbol_size;
  int nb_re_pusch = NR_NB_SC_PER_RB * pusch_pdu->rb_size;

  int nb_re_pusch2 = nb_re_pusch + (nb_re_pusch&7);

  for (aarx = 0; aarx < frame_parms->nb_antennas_rx; aarx++) {

    rxF       = (int16_t *)&rxdataF[aarx][soffset+(symbol * frame_parms->ofdm_symbol_size)];
    rxF_ext   = (int16_t *)&pusch_vars->rxdataF_ext[aarx][symbol * nb_re_pusch2]; // [hna] rxdataF_ext isn't contiguous in order to solve an alignment problem ib llr computation in case of mod_order = 4, 6

    ul_ch0     = &pusch_vars->ul_ch_estimates[aarx][pusch_vars->dmrs_symbol*frame_parms->ofdm_symbol_size]; // update channel estimates if new dmrs symbol are available

    ul_ch0_ext = &pusch_vars->ul_ch_estimates_ext[aarx][symbol*nb_re_pusch2];


    if (is_dmrs_symbol == 0) {
      if (start_re + nb_re_pusch <= frame_parms->ofdm_symbol_size) {
        memcpy((void*)rxF_ext,
                (void*)&rxF[start_re*2],
                nb_re_pusch*sizeof(int32_t));
      } else {
	      int neg_length = frame_parms->ofdm_symbol_size-start_re;
	      int pos_length = nb_re_pusch-neg_length;

	      memcpy((void*)rxF_ext,(void*)&rxF[start_re*2],neg_length*sizeof(int32_t));
      	memcpy((void*)&rxF_ext[2*neg_length],(void*)rxF,pos_length*sizeof(int32_t));
      }
      memcpy((void*)ul_ch0_ext,(void*)ul_ch0,nb_re_pusch*sizeof(int32_t));
    }
    else if (pusch_pdu->dmrs_config_type == pusch_dmrs_type1) { // 6 REs / PRB
      AssertFatal(delta==0||delta==1,"Illegal delta %d\n",delta);
      int32_t *rxF32 = (int32_t*)&rxF[start_re*2];
      int32_t *rxF_ext32 =(int32_t*)rxF_ext;
      int32_t *ul_ch032 = (int32_t*)ul_ch0;
      int32_t *ul_ch0_ext32 =(int32_t*)ul_ch0_ext;
      int idx,idx2,idx3;
      if (start_re + nb_re_pusch < frame_parms->ofdm_symbol_size) {
        for (idx=1-delta,idx2=0;idx<nb_re_pusch;idx+=2,idx2++) {
          rxF_ext32[idx2] = rxF32[idx];
          ul_ch0_ext32[idx2]= ul_ch032[idx];
        }
      }
      else { // handle the two pieces around DC
        LOG_D(PHY,"Running extraction with DMRS for config 1, allocation around DC, start_re %d\n",start_re);
	int neg_length = frame_parms->ofdm_symbol_size-start_re;
	int pos_length = nb_re_pusch-neg_length;
        
        for (idx=1-delta,idx2=0;idx<neg_length;idx+=2,idx2++) {
          rxF_ext32[idx2] = rxF32[idx];
          ul_ch0_ext32[idx2]= ul_ch032[idx];
        }
        rxF32=(int32_t*)rxF;
        idx3=idx;
        for (idx=1-delta;idx<pos_length;idx+=2,idx2++,idx3++) {
          rxF_ext32[idx2] = rxF32[idx];
          ul_ch0_ext32[idx2]= ul_ch032[idx3];
        }
      }
    }
    else if (pusch_pdu->dmrs_config_type == pusch_dmrs_type2)  { // 8 REs / PRB
      AssertFatal(delta==0||delta==2||delta==4,"Illegal delta %d\n",delta);
      if (start_re + nb_re_pusch < frame_parms->ofdm_symbol_size) {
        int64_t *rxF64 = (int64_t*)&rxF[start_re*2];
        int64_t *rxF_ext64 =(int64_t*)rxF_ext;
        int64_t *ul_ch064 = (int64_t*)ul_ch0;
        int64_t *ul_ch0_ext64 =(int64_t*)ul_ch0_ext;
        if (delta==0) {
          for (int idx=0;idx<nb_re_pusch>>1;idx+=6) {
             rxF_ext64[idx]=rxF64[idx+1];
             rxF_ext64[idx+1]=rxF64[idx+2];
             rxF_ext64[idx+2]=rxF64[idx+4];
             rxF_ext64[idx+3]=rxF64[idx+5];
             ul_ch0_ext64[idx]=ul_ch064[idx+1];
             ul_ch0_ext64[idx+1]=ul_ch064[idx+2];
             ul_ch0_ext64[idx+2]=ul_ch064[idx+4];
             ul_ch0_ext64[idx+3]=ul_ch064[idx+5];
          }
        }
        else if (delta==2) {
          for (int idx=0;idx<nb_re_pusch>>1;idx+=6) {
             rxF_ext64[idx]=rxF64[idx+0];
             rxF_ext64[idx+1]=rxF64[idx+2];
             rxF_ext64[idx+2]=rxF64[idx+3];
             rxF_ext64[idx+3]=rxF64[idx+5];
             ul_ch0_ext64[idx]=ul_ch064[idx+0];
             ul_ch0_ext64[idx+1]=ul_ch064[idx+2];
             ul_ch0_ext64[idx+2]=ul_ch064[idx+3];
             ul_ch0_ext64[idx+3]=ul_ch064[idx+5];
          }
        }
        else if (delta==4) {
          for (int idx=0;idx<nb_re_pusch>>1;idx+=6) {
             rxF_ext64[idx]=rxF64[idx+0];
             rxF_ext64[idx+1]=rxF64[idx+1];
             rxF_ext64[idx+2]=rxF64[idx+3];
             rxF_ext64[idx+3]=rxF64[idx+4];
             ul_ch0_ext64[idx]=ul_ch064[idx+0];
             ul_ch0_ext64[idx+1]=ul_ch064[idx+1];
             ul_ch0_ext64[idx+2]=ul_ch064[idx+3];
             ul_ch0_ext64[idx+3]=ul_ch064[idx+4];
          }
        }
      }
      else {
	      int neg_length = frame_parms->ofdm_symbol_size-start_re;
	      int pos_length = nb_re_pusch-neg_length;
        if ((pos_length%12) > 0 ) pos_length+=12;
        int64_t *rxF64 = (int64_t*)&rxF[start_re*2];
        int64_t *rxF_ext64 =(int64_t*)rxF_ext;
        int64_t *ul_ch064 = (int64_t*)ul_ch0;
        int64_t *ul_ch0_ext64 =(int64_t*)ul_ch0_ext;
        int idx=0;
        if (delta==0) {
          for (idx=0;idx<neg_length>>1;idx+=6) {
             rxF_ext64[idx]  =rxF64[idx+1];
             rxF_ext64[idx+1]=rxF64[idx+2];
             rxF_ext64[idx+2]=rxF64[idx+4];
             rxF_ext64[idx+3]=rxF64[idx+5];
             ul_ch0_ext64[idx]=ul_ch064[idx+1];
             ul_ch0_ext64[idx+1]=ul_ch064[idx+2];
             ul_ch0_ext64[idx+2]=ul_ch064[idx+4];
             ul_ch0_ext64[idx+3]=ul_ch064[idx+5];
          }
          if ((neg_length%12) > 0) {
            rxF_ext64[idx+4]=rxF64[idx+7];
            rxF_ext64[idx+5]=rxF64[idx+8];
            ul_ch0_ext64[idx+4]=ul_ch064[idx+7];
            ul_ch0_ext64[idx+5]=ul_ch064[idx+8];
          }
          rxF_ext64+=(neg_length/3);
          rxF64=(int64_t*)rxF;
          ul_ch0_ext64+=(neg_length/3);
          ul_ch064+=(neg_length>>1);
          for (idx=0;idx<pos_length>>1;idx+=6) {
             rxF_ext64[idx]  =rxF64[idx+1];
             rxF_ext64[idx+1]=rxF64[idx+2];
             rxF_ext64[idx+2]=rxF64[idx+4];
             rxF_ext64[idx+3]=rxF64[idx+5];
             ul_ch0_ext64[idx]=ul_ch064[idx+1];
             ul_ch0_ext64[idx+1]=ul_ch064[idx+2];
             ul_ch0_ext64[idx+2]=ul_ch064[idx+4];
             ul_ch0_ext64[idx+3]=ul_ch064[idx+5];
          }
        }
        else if (delta==2) {
          for (idx=0;idx<neg_length>>1;idx+=6) {
             rxF_ext64[idx]  =rxF64[idx+0];
             rxF_ext64[idx+1]=rxF64[idx+2];
             rxF_ext64[idx+2]=rxF64[idx+3];
             rxF_ext64[idx+3]=rxF64[idx+5];
             ul_ch0_ext64[idx]=ul_ch064[idx+0];
             ul_ch0_ext64[idx+1]=ul_ch064[idx+2];
             ul_ch0_ext64[idx+2]=ul_ch064[idx+3];
             ul_ch0_ext64[idx+3]=ul_ch064[idx+5];
          }
          if ((neg_length%12) > 0) {
            rxF_ext64[idx+4]=rxF64[idx+6];
            rxF_ext64[idx+5]=rxF64[idx+8];
            ul_ch0_ext64[idx+4]=ul_ch064[idx+6];
            ul_ch0_ext64[idx+5]=ul_ch064[idx+8];
          }
          rxF_ext64+=(neg_length/3);
          rxF64=(int64_t*)rxF;
          ul_ch0_ext64+=(neg_length/3);
          ul_ch064+=(neg_length>>1);
          for (idx=0;idx<pos_length>>1;idx+=6) {
             rxF_ext64[idx]  =rxF64[idx+0];
             rxF_ext64[idx+1]=rxF64[idx+2];
             rxF_ext64[idx+2]=rxF64[idx+3];
             rxF_ext64[idx+3]=rxF64[idx+5];
             ul_ch0_ext64[idx]=ul_ch064[idx+0];
             ul_ch0_ext64[idx+1]=ul_ch064[idx+2];
             ul_ch0_ext64[idx+2]=ul_ch064[idx+3];
             ul_ch0_ext64[idx+3]=ul_ch064[idx+5];
          }
        }
        else if (delta==4) {
          for (idx=0;idx<neg_length>>1;idx+=6) {
             rxF_ext64[idx]  =rxF64[idx+0];
             rxF_ext64[idx+1]=rxF64[idx+1];
             rxF_ext64[idx+2]=rxF64[idx+3];
             rxF_ext64[idx+3]=rxF64[idx+4];
             ul_ch0_ext64[idx]=ul_ch064[idx+0];
             ul_ch0_ext64[idx+1]=ul_ch064[idx+1];
             ul_ch0_ext64[idx+2]=ul_ch064[idx+3];
             ul_ch0_ext64[idx+3]=ul_ch064[idx+4];
          }
          if ((neg_length%12) > 0) {
            rxF_ext64[idx+4]=rxF64[idx+6];
            rxF_ext64[idx+5]=rxF64[idx+7];
            ul_ch0_ext64[idx+4]=ul_ch064[idx+6];
            ul_ch0_ext64[idx+5]=ul_ch064[idx+7];
          }
          rxF_ext64+=(neg_length/3);
          rxF64=(int64_t*)rxF;
          ul_ch0_ext64+=(neg_length/3);
          ul_ch064+=(neg_length>>1);
          for (idx=0;idx<pos_length>>1;idx+=6) {
             rxF_ext64[idx]  =rxF64[idx+0];
             rxF_ext64[idx+1]=rxF64[idx+1];
             rxF_ext64[idx+2]=rxF64[idx+3];
             rxF_ext64[idx+3]=rxF64[idx+4];
             ul_ch0_ext64[idx]=ul_ch064[idx+0];
             ul_ch0_ext64[idx+1]=ul_ch064[idx+1];
             ul_ch0_ext64[idx+2]=ul_ch064[idx+3];
             ul_ch0_ext64[idx+3]=ul_ch064[idx+4];
          }
        }
      }
    }
#if 0      
    else {

      for (re = 0; re < nb_re_pusch; re++) {

      for (aatx = 0; aatx < pusch_pdu->nrOfLayers; aatx++) {
        ul_ch0 = &pusch_vars->ul_ch_estimates[aatx*frame_parms->nb_antennas_rx+aarx][pusch_vars->dmrs_symbol*frame_parms->ofdm_symbol_size]; // update channel estimates if new dmrs symbol are available
        ul_ch0_ext = &pusch_vars->ul_ch_estimates_ext[aatx*frame_parms->nb_antennas_rx+aarx][symbol*nb_re_pusch2];
        memcpy((void*)ul_ch0_ext,(void*)ul_ch0,nb_re_pusch*sizeof(int32_t));
      }

    } else {

      for (aatx = 0; aatx < pusch_pdu->nrOfLayers; aatx++) {
        ul_ch0 = &pusch_vars->ul_ch_estimates[aatx*frame_parms->nb_antennas_rx+aarx][pusch_vars->dmrs_symbol*frame_parms->ofdm_symbol_size]; // update channel estimates if new dmrs symbol are available
        ul_ch0_ext = &pusch_vars->ul_ch_estimates_ext[aatx*frame_parms->nb_antennas_rx+aarx][symbol*nb_re_pusch2];

        rxF_ext_index = 0;
        ul_ch0_ext_index = 0;
        ul_ch0_index = 0;
        for (re = 0; re < nb_re_pusch; re++) {
          uint16_t k = start_re + re;
          is_data_re = allowed_xlsch_re_in_dmrs_symbol(k, start_re, frame_parms->ofdm_symbol_size, pusch_pdu->num_dmrs_cdm_grps_no_data, pusch_pdu->dmrs_config_type);
          if (++k >= frame_parms->ofdm_symbol_size) {
            k -= frame_parms->ofdm_symbol_size;
          }

          #ifdef DEBUG_RB_EXT
          printf("re = %d, is_dmrs_symbol = %d, symbol = %d\n", re, is_dmrs_symbol, symbol);
          #endif

          // save only data and respective channel estimates
          if (is_data_re == 1) {
            if (aatx == 0) {
              rxF_ext[rxF_ext_index]     = (rxF[ ((start_re + re)*2)      % (frame_parms->ofdm_symbol_size*2)]);
              rxF_ext[rxF_ext_index + 1] = (rxF[(((start_re + re)*2) + 1) % (frame_parms->ofdm_symbol_size*2)]);
              rxF_ext_index +=2;
            }
          
            ul_ch0_ext[ul_ch0_ext_index] = ul_ch0[ul_ch0_index];
            ul_ch0_ext_index++;

            #ifdef DEBUG_RB_EXT
            printf("dmrs symb %d: rxF_ext[%d] = (%d,%d), ul_ch0_ext[%d] = (%d,%d)\n",
                 is_dmrs_symbol,rxF_ext_index>>1, rxF_ext[rxF_ext_index],rxF_ext[rxF_ext_index+1],
                 ul_ch0_ext_index,  ((int16_t*)&ul_ch0_ext[ul_ch0_ext_index])[0],  ((int16_t*)&ul_ch0_ext[ul_ch0_ext_index])[1]);
            #endif          
          } 
          ul_ch0_index++;
        }
      }
    }
#endif      
  }
}

void nr_ulsch_scale_channel(int **ul_ch_estimates_ext,
                            NR_DL_FRAME_PARMS *frame_parms,
                            NR_gNB_ULSCH_t *ulsch_gNB,
                            uint8_t symbol,
                            uint8_t is_dmrs_symbol,
                            uint32_t len,
                            uint8_t nrOfLayers,
                            unsigned short nb_rb)
{

#if defined(__x86_64__)||defined(__i386__)

  short rb, ch_amp;
  unsigned char aarx,aatx;
  simde__m128i *ul_ch128, ch_amp128;

  uint32_t nb_rb_0 = len/12 + ((len%12)?1:0);

  // Determine scaling amplitude based the symbol

  ch_amp = 1024*8;

  LOG_D(PHY,"Scaling PUSCH Chest in OFDM symbol %d by %d, pilots %d nb_rb %d NCP %d symbol %d\n", symbol, ch_amp, is_dmrs_symbol, nb_rb, frame_parms->Ncp, symbol);
   // printf("Scaling PUSCH Chest in OFDM symbol %d by %d\n",symbol_mod,ch_amp);

  ch_amp128 = simde_mm_set1_epi16(ch_amp); // Q3.13

  int off = ((nb_rb&1) == 1)? 4:0;

  for (aatx = 0; aatx < nrOfLayers; aatx++) {
    for (aarx=0; aarx < frame_parms->nb_antennas_rx; aarx++) {
      ul_ch128 = (simde__m128i *)&ul_ch_estimates_ext[aatx*frame_parms->nb_antennas_rx+aarx][symbol*(off+(nb_rb*NR_NB_SC_PER_RB))];
      for (rb=0;rb < nb_rb_0;rb++) {
        ul_ch128[0] = simde_mm_mulhi_epi16(ul_ch128[0], ch_amp128);
        ul_ch128[0] = simde_mm_slli_epi16(ul_ch128[0], 3);

        ul_ch128[1] = simde_mm_mulhi_epi16(ul_ch128[1], ch_amp128);
        ul_ch128[1] = simde_mm_slli_epi16(ul_ch128[1], 3);

        ul_ch128[2] = simde_mm_mulhi_epi16(ul_ch128[2], ch_amp128);
        ul_ch128[2] = simde_mm_slli_epi16(ul_ch128[2], 3);
        ul_ch128+=3;
      }
    }
  }
#endif
}

int get_nb_re_pusch(NR_DL_FRAME_PARMS *frame_parms, nfapi_nr_pusch_pdu_t *rel15_ul,int symbol) {

  uint8_t dmrs_symbol_flag = (rel15_ul->ul_dmrs_symb_pos >> symbol) & 0x01;
  if (dmrs_symbol_flag == 1) {
    if ((rel15_ul->ul_dmrs_symb_pos >> ((symbol + 1) % frame_parms->symbols_per_slot)) & 0x01)
      AssertFatal(1==0,"Double DMRS configuration is not yet supported\n");

    if (rel15_ul->dmrs_config_type == 0) {
      // if no data in dmrs cdm group is 1 only even REs have no data
      // if no data in dmrs cdm group is 2 both odd and even REs have no data
      return(rel15_ul->rb_size *(12 - (rel15_ul->num_dmrs_cdm_grps_no_data*6)));
    }
    else return(rel15_ul->rb_size *(12 - (rel15_ul->num_dmrs_cdm_grps_no_data*4)));
  } else return(rel15_ul->rb_size * NR_NB_SC_PER_RB);
}

//compute average channel_level on each (TX,RX) antenna pair
void nr_ulsch_channel_level(int **ul_ch_estimates_ext,
                            NR_DL_FRAME_PARMS *frame_parms,
                            int32_t *avg,
                            uint8_t symbol,
                            uint32_t len,
                            uint8_t nrOfLayers,
                            unsigned short nb_rb)
{


  short rb;
  unsigned char aatx, aarx;
  simde__m128i *ul_ch128, avg128U;

  int16_t x = factor2(len);
  int16_t y = (len)>>x;
  
  uint32_t nb_rb_0 = len/12 + ((len%12)?1:0);

  int off = ((nb_rb_0&1) == 1)? 4:0;

  for (aatx = 0; aatx < nrOfLayers; aatx++) {
    for (aarx = 0; aarx < frame_parms->nb_antennas_rx; aarx++) {
      //clear average level
      avg128U = simde_mm_setzero_si128();

      ul_ch128=(simde__m128i *)&ul_ch_estimates_ext[aatx*frame_parms->nb_antennas_rx+aarx][symbol*(off+(nb_rb*12))];

      for (rb = 0; rb < nb_rb_0; rb++) {
        avg128U = simde_mm_add_epi32(avg128U, simde_mm_srai_epi32(simde_mm_madd_epi16(ul_ch128[0], ul_ch128[0]), x));
        avg128U = simde_mm_add_epi32(avg128U, simde_mm_srai_epi32(simde_mm_madd_epi16(ul_ch128[1], ul_ch128[1]), x));
        avg128U = simde_mm_add_epi32(avg128U, simde_mm_srai_epi32(simde_mm_madd_epi16(ul_ch128[2], ul_ch128[2]), x));
        ul_ch128+=3;
      }

      avg[aatx*frame_parms->nb_antennas_rx+aarx] = (((int32_t*)&avg128U)[0] +
                                                    ((int32_t*)&avg128U)[1] +
                                                    ((int32_t*)&avg128U)[2] +
                                                    ((int32_t*)&avg128U)[3]) / y;
    }
  }

  simde_mm_empty();
  _m_empty();

}


 
//==============================================================================================
// Pre-processing for LLR computation
//==============================================================================================
void nr_ulsch_channel_compensation(int **rxdataF_ext,
                                   int **ul_ch_estimates_ext,
                                   int **ul_ch_mag,
                                   int **ul_ch_magb,
                                   int **ul_ch_magc,
                                   int **rxdataF_comp,
                                   int ***rho,
                                   NR_DL_FRAME_PARMS *frame_parms,
                                   unsigned char symbol,
                                   int length,
                                   uint8_t is_dmrs_symbol,
                                   int pusch_dmrs_type,
                                   unsigned char mod_order,
                                   uint8_t  nrOfLayers,
                                   unsigned short nb_rb,
                                   unsigned char output_shift) {

  int off = ((nb_rb&1) == 1)? 4:0;

#ifdef DEBUG_CH_COMP
  int16_t *rxF, *ul_ch;
  int prnt_idx;
  for (int nl=0; nl<nrOfLayers; nl++) {
    for (int aarx = 0; aarx < frame_parms->nb_antennas_rx; aarx++) {
      rxF = (int16_t *) &rxdataF_ext[aarx][symbol * (off + (nb_rb * 12))];
      ul_ch = (int16_t *) &ul_ch_estimates_ext[nl * frame_parms->nb_antennas_rx + aarx][symbol * (off + (nb_rb * 12))];

      printf("--------symbol = %d, mod_order = %d, output_shift = %d, layer %i, antenna rx = %d -----------\n",
             symbol, mod_order, output_shift, nl, aarx);
      printf("----------------Before compensation------------------\n");

      for (prnt_idx = 0; prnt_idx < 12 * 5 * 2; prnt_idx += 2) {
        printf("rxF[%d] = (%d,%d)\n", prnt_idx >> 1, rxF[prnt_idx], rxF[prnt_idx + 1]);
        printf("ul_ch[%d] = (%d,%d)\n", prnt_idx >> 1, ul_ch[prnt_idx], ul_ch[prnt_idx + 1]);
      }
    }
  }
#endif

#ifdef DEBUG_CH_MAG
  int16_t *ch_mag;
  int print_idx;


  for (int ant=0; ant<frame_parms->nb_antennas_rx; ant++) {
    ch_mag   = (int16_t *)&ul_ch_mag[ant][symbol*(off+(nb_rb*12))];

    printf("--------------------symbol = %d, mod_order = %d-----------------------\n", symbol, mod_order);
    printf("----------------Before computation------------------\n");

    for (print_idx=0;print_idx<5;print_idx++){

      printf("ch_mag[%d] = %d\n", print_idx, ch_mag[print_idx]);

    }
  }

#endif


  int aatx,aarx;
  simde__m256i *ul_ch256,*ul_ch_mag256,*ul_ch_mag256b,*ul_ch_mag256c,*rxdataF256,*rxdataF_comp256;
  simde__m256i mmtmpD0,mmtmpD1,mmtmpD2,mmtmpD3,QAM_amp256={0},QAM_amp256b={0},QAM_amp256c={0};
  QAM_amp256b = simde_mm256_setzero_si256();
  for (aatx=0; aatx<nrOfLayers; aatx++) {
    if (mod_order == 4) {
      QAM_amp256  = simde_mm256_set1_epi16(QAM16_n1);  // 2/sqrt(10)
      QAM_amp256b = simde_mm256_setzero_si256();
    } else if (mod_order == 6) {
      QAM_amp256  = simde_mm256_set1_epi16(QAM64_n1); //
      QAM_amp256b = simde_mm256_set1_epi16(QAM64_n2);
    } else if (mod_order == 8) {
      QAM_amp256  = simde_mm256_set1_epi16(QAM256_n1); //
      QAM_amp256b = simde_mm256_set1_epi16(QAM256_n2);
      QAM_amp256c = simde_mm256_set1_epi16(QAM256_n3);
    }

    for (aarx=0; aarx<frame_parms->nb_antennas_rx; aarx++) {
      ul_ch256          = (simde__m256i *)&ul_ch_estimates_ext[aatx*frame_parms->nb_antennas_rx+aarx][symbol*(off+(nb_rb*12))];
      ul_ch_mag256      = (simde__m256i *)&ul_ch_mag[aatx*frame_parms->nb_antennas_rx+aarx][symbol*(off+(nb_rb*12))];
      ul_ch_mag256b     = (simde__m256i *)&ul_ch_magb[aatx*frame_parms->nb_antennas_rx+aarx][symbol*(off+(nb_rb*12))];
      ul_ch_mag256c     = (simde__m256i *)&ul_ch_magc[aatx*frame_parms->nb_antennas_rx+aarx][symbol*(off+(nb_rb*12))];
      rxdataF256        = (simde__m256i *)&rxdataF_ext[aarx][symbol*(off+(nb_rb*12))];
      rxdataF_comp256   = (simde__m256i *)&rxdataF_comp[aatx*frame_parms->nb_antennas_rx+aarx][symbol*(off+(nb_rb*12))];



      register simde__m256i complex_shuffle = simde_mm256_set_epi8(29,28,31,30,25,24,27,26,21,20,23,22,17,16,19,18,13,12,15,14,9,8,11,10,5,4,7,6,1,0,3,2);
      register simde__m256i conj = simde_mm256_set_epi16(1,-1,1,-1,1,-1,1,-1,1,-1,1,-1,1,-1,1,-1);

      // compute length of symbol vector in 256 bit words
      int len8=(nb_rb*12);
      if (aarx == 0) LOG_D(PHY,"Compensation: symbol %d, dmrs %d, len %d\n",symbol,is_dmrs_symbol,len8);
      if (is_dmrs_symbol==1){
        if (pusch_dmrs_type == pusch_dmrs_type1)
          len8>>=1;
        else
          len8 = (2*len8)/3;
      }
      if ((len8&7) > 0) len8+=8;
      len8>>=3;
      if (aarx==0) LOG_D(PHY,"Compensation: len8 %d\n",len8);
      if (mod_order==2) {
        for (int re8=0; re8<len8; re8++) {
          // multiply by conjugated channel
          mmtmpD0 = simde_mm256_madd_epi16(ul_ch256[re8],rxdataF256[re8]);
          // mmtmpD0 contains real part of 8 consecutive outputs (32-bit)
          mmtmpD1 = simde_mm256_shuffle_epi8(ul_ch256[re8],complex_shuffle);
          mmtmpD1 = simde_mm256_sign_epi16(mmtmpD1,conj);
          mmtmpD1 = simde_mm256_madd_epi16(mmtmpD1,rxdataF256[re8]);
          // mmtmpD1 contains imag part of 4 consecutive outputs (32-bit)
          mmtmpD0 = simde_mm256_srai_epi32(mmtmpD0,output_shift);
          mmtmpD1 = simde_mm256_srai_epi32(mmtmpD1,output_shift);
          mmtmpD2 = simde_mm256_unpacklo_epi32(mmtmpD0,mmtmpD1);
          mmtmpD3 = simde_mm256_unpackhi_epi32(mmtmpD0,mmtmpD1);
          rxdataF_comp256[re8] = simde_mm256_packs_epi32(mmtmpD2,mmtmpD3);
        }
      }
      else if (mod_order==4) {
        for (int re8=0; re8<len8; re8++) {
          mmtmpD0 = simde_mm256_madd_epi16(ul_ch256[re8],ul_ch256[re8]);
          mmtmpD0 = simde_mm256_srai_epi32(mmtmpD0,output_shift);
          mmtmpD0 = simde_mm256_packs_epi32(mmtmpD0,mmtmpD0);

          // store channel magnitude here in a new field of ulsch

          ul_ch_mag256[re8] = simde_mm256_unpacklo_epi16(mmtmpD0,mmtmpD0);
          ul_ch_mag256[re8] = simde_mm256_mulhrs_epi16(ul_ch_mag256[re8],QAM_amp256);
          // multiply by conjugated channel
          mmtmpD0 = simde_mm256_madd_epi16(ul_ch256[re8],rxdataF256[re8]);
          // mmtmpD0 contains real part of 8 consecutive outputs (32-bit)
          mmtmpD1 = simde_mm256_shuffle_epi8(ul_ch256[re8],complex_shuffle);
          mmtmpD1 = simde_mm256_sign_epi16(mmtmpD1,conj);
          mmtmpD1 = simde_mm256_madd_epi16(mmtmpD1,rxdataF256[re8]);
          // mmtmpD1 contains imag part of 4 consecutive outputs (32-bit)
          mmtmpD0 = simde_mm256_srai_epi32(mmtmpD0,output_shift);
          mmtmpD1 = simde_mm256_srai_epi32(mmtmpD1,output_shift);
          mmtmpD2 = simde_mm256_unpacklo_epi32(mmtmpD0,mmtmpD1);
          mmtmpD3 = simde_mm256_unpackhi_epi32(mmtmpD0,mmtmpD1);
          rxdataF_comp256[re8] = simde_mm256_packs_epi32(mmtmpD2,mmtmpD3);
        }
      }
      else if (mod_order==6) {
        for (int re8=0; re8<len8; re8++) {
          mmtmpD0 = simde_mm256_madd_epi16(ul_ch256[re8],ul_ch256[re8]);
          mmtmpD0 = simde_mm256_srai_epi32(mmtmpD0,output_shift);
          mmtmpD0 = simde_mm256_packs_epi32(mmtmpD0,mmtmpD0);
          // store channel magnitude here in a new field of ulsch
          ul_ch_mag256[re8]  = simde_mm256_unpacklo_epi16(mmtmpD0,mmtmpD0);
          ul_ch_mag256b[re8] = ul_ch_mag256[re8];
          ul_ch_mag256[re8]  = simde_mm256_mulhrs_epi16(ul_ch_mag256[re8],QAM_amp256);
          ul_ch_mag256b[re8] = simde_mm256_mulhrs_epi16(ul_ch_mag256b[re8],QAM_amp256b);
          // multiply by conjugated channel
          mmtmpD0 = simde_mm256_madd_epi16(ul_ch256[re8],rxdataF256[re8]);
          // mmtmpD0 contains real part of 8 consecutive outputs (32-bit)
          mmtmpD1 = simde_mm256_shuffle_epi8(ul_ch256[re8],complex_shuffle);
          mmtmpD1 = simde_mm256_sign_epi16(mmtmpD1,conj);
          mmtmpD1 = simde_mm256_madd_epi16(mmtmpD1,rxdataF256[re8]);
          // mmtmpD1 contains imag part of 4 consecutive outputs (32-bit)
          mmtmpD0 = simde_mm256_srai_epi32(mmtmpD0,output_shift);
          mmtmpD1 = simde_mm256_srai_epi32(mmtmpD1,output_shift);
          mmtmpD2 = simde_mm256_unpacklo_epi32(mmtmpD0,mmtmpD1);
          mmtmpD3 = simde_mm256_unpackhi_epi32(mmtmpD0,mmtmpD1);
          rxdataF_comp256[re8] = simde_mm256_packs_epi32(mmtmpD2,mmtmpD3);
        }
      }
      else if (mod_order==8) {
        for (int re8=0; re8<len8; re8++) {
          mmtmpD0 = simde_mm256_madd_epi16(ul_ch256[re8],ul_ch256[re8]);
          mmtmpD0 = simde_mm256_srai_epi32(mmtmpD0,output_shift);
          mmtmpD0 = simde_mm256_packs_epi32(mmtmpD0,mmtmpD0);
          // store channel magnitude here in a new field of ulsch
          ul_ch_mag256[re8]  = simde_mm256_unpacklo_epi16(mmtmpD0,mmtmpD0);
          ul_ch_mag256b[re8] = ul_ch_mag256[re8];
          ul_ch_mag256c[re8] = ul_ch_mag256[re8];
          ul_ch_mag256[re8]  = simde_mm256_mulhrs_epi16(ul_ch_mag256[re8],QAM_amp256);
          ul_ch_mag256b[re8] = simde_mm256_mulhrs_epi16(ul_ch_mag256b[re8],QAM_amp256b);
          ul_ch_mag256c[re8] = simde_mm256_mulhrs_epi16(ul_ch_mag256c[re8],QAM_amp256c);
          // multiply by conjugated channel
          mmtmpD0 = simde_mm256_madd_epi16(ul_ch256[re8],rxdataF256[re8]);
          // mmtmpD0 contains real part of 8 consecutive outputs (32-bit)
          mmtmpD1 = simde_mm256_shuffle_epi8(ul_ch256[re8],complex_shuffle);
          mmtmpD1 = simde_mm256_sign_epi16(mmtmpD1,conj);
          mmtmpD1 = simde_mm256_madd_epi16(mmtmpD1,rxdataF256[re8]);
          // mmtmpD1 contains imag part of 4 consecutive outputs (32-bit)
          mmtmpD0 = simde_mm256_srai_epi32(mmtmpD0,output_shift);
          mmtmpD1 = simde_mm256_srai_epi32(mmtmpD1,output_shift);
          mmtmpD2 = simde_mm256_unpacklo_epi32(mmtmpD0,mmtmpD1);
          mmtmpD3 = simde_mm256_unpackhi_epi32(mmtmpD0,mmtmpD1);
          rxdataF_comp256[re8] = simde_mm256_packs_epi32(mmtmpD2,mmtmpD3);
        }
      }
      else AssertFatal(1==0,"Unsupported mod_order %d\n",mod_order);
    } // nb_antennas_rx
  } // nb_layers

  if (rho) {
/*
    for (aarx=0; aarx<frame_parms->nb_antennas_rx; aarx++) {
      rho128        = (__m128i *)&rho[aarx][symbol*frame_parms->N_RB_UL*12];
      ul_ch128      = (__m128i *)&ul_ch_estimates_ext[aarx][symbol*frame_parms->N_RB_UL*12];
      ul_ch128_2    = (__m128i *)&ul_ch_estimates_ext[2+aarx][symbol*frame_parms->N_RB_UL*12];

      for (rb=0; rb<nb_rb; rb++) {
        // multiply by conjugated channel
        mmtmpD0 = simde_mm_madd_epi16(ul_ch128[0],rxdataF128[0]);
        //  print_ints("re",&mmtmpD0);

        // mmtmpD0 contains real part of 4 consecutive outputs (32-bit)
        mmtmpD1 = simde_mm_shufflelo_epi16(ul_ch128[0],_MM_SHUFFLE(2,3,0,1));
        mmtmpD1 = simde_mm_shufflehi_epi16(mmtmpD1,_MM_SHUFFLE(2,3,0,1));
        mmtmpD1 = simde_mm_sign_epi16(mmtmpD1,*(simde__m128i*)&conjugate[0]);
        //  print_ints("im",&mmtmpD1);
        mmtmpD1 = simde_mm_madd_epi16(mmtmpD1,rxdataF128[0]);
        // mmtmpD1 contains imag part of 4 consecutive outputs (32-bit)
        mmtmpD0 = simde_mm_srai_epi32(mmtmpD0,output_shift);
        //  print_ints("re(shift)",&mmtmpD0);
        mmtmpD1 = simde_mm_srai_epi32(mmtmpD1,output_shift);
        //  print_ints("im(shift)",&mmtmpD1);
        mmtmpD2 = simde_mm_unpacklo_epi32(mmtmpD0,mmtmpD1);
        mmtmpD3 = simde_mm_unpackhi_epi32(mmtmpD0,mmtmpD1);
        //        print_ints("c0",&mmtmpD2);
        //  print_ints("c1",&mmtmpD3);
        rxdataF_comp128[0] = simde_mm_packs_epi32(mmtmpD2,mmtmpD3);
        //  print_shorts("rx:",rxdataF128);
        //  print_shorts("ch:",ul_ch128);
        //  print_shorts("pack:",rxdataF_comp128);

        // multiply by conjugated channel
        mmtmpD0 = simde_mm_madd_epi16(ul_ch128[1],rxdataF128[1]);
        // mmtmpD0 contains real part of 4 consecutive outputs (32-bit)
        mmtmpD1 = simde_mm_shufflelo_epi16(ul_ch128[1],_MM_SHUFFLE(2,3,0,1));
        mmtmpD1 = simde_mm_shufflehi_epi16(mmtmpD1,_MM_SHUFFLE(2,3,0,1));
        mmtmpD1 = simde_mm_sign_epi16(mmtmpD1,*(simde__m128i*)conjugate);
        mmtmpD1 = simde_mm_madd_epi16(mmtmpD1,rxdataF128[1]);
        // mmtmpD1 contains imag part of 4 consecutive outputs (32-bit)
        mmtmpD0 = simde_mm_srai_epi32(mmtmpD0,output_shift);
        mmtmpD1 = simde_mm_srai_epi32(mmtmpD1,output_shift);
        mmtmpD2 = simde_mm_unpacklo_epi32(mmtmpD0,mmtmpD1);
        mmtmpD3 = simde_mm_unpackhi_epi32(mmtmpD0,mmtmpD1);

        rxdataF_comp128[1] = simde_mm_packs_epi32(mmtmpD2,mmtmpD3);
        //  print_shorts("rx:",rxdataF128+1);
        //  print_shorts("ch:",ul_ch128+1);
        //  print_shorts("pack:",rxdataF_comp128+1);

        rho128[1] =simde_mm_packs_epi32(mmtmpD2,mmtmpD3);
        //print_shorts("rx:",ul_ch128_2+1);
        //print_shorts("ch:",ul_ch128+1);
        //print_shorts("pack:",rho128+1);
        // multiply by conjugated channel
        mmtmpD0 = simde_mm_madd_epi16(ul_ch128[2],rxdataF128[2]);
        // mmtmpD0 contains real part of 4 consecutive outputs (32-bit)
        mmtmpD1 = simde_mm_shufflelo_epi16(ul_ch128[2],_MM_SHUFFLE(2,3,0,1));
        mmtmpD1 = simde_mm_shufflehi_epi16(mmtmpD1,_MM_SHUFFLE(2,3,0,1));
        mmtmpD1 = simde_mm_sign_epi16(mmtmpD1,*(simde__m128i*)conjugate);
        mmtmpD1 = simde_mm_madd_epi16(mmtmpD1,rxdataF128[2]);
        // mmtmpD1 contains imag part of 4 consecutive outputs (32-bit)
        mmtmpD0 = simde_mm_srai_epi32(mmtmpD0,output_shift);
        mmtmpD1 = simde_mm_srai_epi32(mmtmpD1,output_shift);
        mmtmpD2 = simde_mm_unpacklo_epi32(mmtmpD0,mmtmpD1);
        mmtmpD3 = simde_mm_unpackhi_epi32(mmtmpD0,mmtmpD1);

        rxdataF_comp128[2] = simde_mm_packs_epi32(mmtmpD2,mmtmpD3);
        //print_shorts("rx:",rxdataF128+2);
        //print_shorts("ch:",ul_ch128+2);
        //print_shorts("pack:",rxdataF_comp128+2);

        ul_ch128+=3;
        ul_ch_mag128+=3;
        ul_ch_mag128b+=3;
        rxdataF128+=3;
        rxdataF_comp128+=3;
      }
    }
  }

  if (rho) {
    //we compute the Tx correlation matrix for each Rx antenna
    //As an example the 2x2 MIMO case requires
    //rho[aarx][nb_aatx*nb_aatx] = [cov(H_aarx_0,H_aarx_0) cov(H_aarx_0,H_aarx_1)
    //                              cov(H_aarx_1,H_aarx_0) cov(H_aarx_1,H_aarx_1)], aarx=0,...,nb_antennas_rx-1

    int avg_rho_re[frame_parms->nb_antennas_rx][nrOfLayers*nrOfLayers];
    int avg_rho_im[frame_parms->nb_antennas_rx][nrOfLayers*nrOfLayers];

    for (aarx=0; aarx < frame_parms->nb_antennas_rx; aarx++) {
      for (aatx=0; aatx < nrOfLayers; aatx++) {
        for (int atx=0; atx< nrOfLayers; atx++) {

          avg_rho_re[aarx][aatx*nrOfLayers+atx] = 0;
          avg_rho_im[aarx][aatx*nrOfLayers+atx] = 0;
          rho128        = (simde__m128i *)&rho[aarx][aatx*nrOfLayers+atx][symbol*(off+(nb_rb*12))];
          ul_ch128      = (simde__m128i *)&ul_ch_estimates_ext[aatx*frame_parms->nb_antennas_rx+aarx][symbol*(off+(nb_rb*12))];
          ul_ch128_2    = (simde__m128i *)&ul_ch_estimates_ext[atx*frame_parms->nb_antennas_rx+aarx][symbol*(off+(nb_rb*12))];

          for (rb=0; rb<nb_rb_0; rb++) {
            // multiply by conjugated channel
            mmtmpD0 = simde_mm_madd_epi16(ul_ch128[0],ul_ch128_2[0]);
            //  print_ints("re",&mmtmpD0);

            // mmtmpD0 contains real part of 4 consecutive outputs (32-bit)
            mmtmpD1 = simde_mm_shufflelo_epi16(ul_ch128[0],_MM_SHUFFLE(2,3,0,1));
            mmtmpD1 = simde_mm_shufflehi_epi16(mmtmpD1,_MM_SHUFFLE(2,3,0,1));
            mmtmpD1 = simde_mm_sign_epi16(mmtmpD1,*(simde__m128i*)&conjugate[0]);
            //  print_ints("im",&mmtmpD1);
            mmtmpD1 = simde_mm_madd_epi16(mmtmpD1,ul_ch128_2[0]);
            // mmtmpD1 contains imag part of 4 consecutive outputs (32-bit)
            mmtmpD0 = simde_mm_srai_epi32(mmtmpD0,output_shift);
            //  print_ints("re(shift)",&mmtmpD0);
            mmtmpD1 = simde_mm_srai_epi32(mmtmpD1,output_shift);
            //  print_ints("im(shift)",&mmtmpD1);
            mmtmpD2 = simde_mm_unpacklo_epi32(mmtmpD0,mmtmpD1);
            mmtmpD3 = simde_mm_unpackhi_epi32(mmtmpD0,mmtmpD1);
            //        print_ints("c0",&mmtmpD2);
            //  print_ints("c1",&mmtmpD3);
            rho128[0] = simde_mm_packs_epi32(mmtmpD2,mmtmpD3);

            //print_shorts("rx:",ul_ch128_2);
            //print_shorts("ch:",ul_ch128);
            //print_shorts("pack:",rho128);

            avg_rho_re[aarx][aatx*nrOfLayers+atx] +=(((int16_t*)&rho128[0])[0]+
              ((int16_t*)&rho128[0])[2] +
              ((int16_t*)&rho128[0])[4] +
              ((int16_t*)&rho128[0])[6])/16;//

            avg_rho_im[aarx][aatx*nrOfLayers+atx] +=(((int16_t*)&rho128[0])[1]+
              ((int16_t*)&rho128[0])[3] +
              ((int16_t*)&rho128[0])[5] +
              ((int16_t*)&rho128[0])[7])/16;//
            // multiply by conjugated channel
            mmtmpD0 = simde_mm_madd_epi16(ul_ch128[1],ul_ch128_2[1]);
            // mmtmpD0 contains real part of 4 consecutive outputs (32-bit)
            mmtmpD1 = simde_mm_shufflelo_epi16(ul_ch128[1],_MM_SHUFFLE(2,3,0,1));
            mmtmpD1 = simde_mm_shufflehi_epi16(mmtmpD1,_MM_SHUFFLE(2,3,0,1));
            mmtmpD1 = simde_mm_sign_epi16(mmtmpD1,*(simde__m128i*)conjugate);
            mmtmpD1 = simde_mm_madd_epi16(mmtmpD1,ul_ch128_2[1]);
            // mmtmpD1 contains imag part of 4 consecutive outputs (32-bit)
            mmtmpD0 = simde_mm_srai_epi32(mmtmpD0,output_shift);
            mmtmpD1 = simde_mm_srai_epi32(mmtmpD1,output_shift);
            mmtmpD2 = simde_mm_unpacklo_epi32(mmtmpD0,mmtmpD1);
            mmtmpD3 = simde_mm_unpackhi_epi32(mmtmpD0,mmtmpD1);
            rho128[1] =simde_mm_packs_epi32(mmtmpD2,mmtmpD3);
            //print_shorts("rx:",ul_ch128_2+1);
            //print_shorts("ch:",ul_ch128+1);
            //print_shorts("pack:",rho128+1);

            // multiply by conjugated channel
            avg_rho_re[aarx][aatx*nrOfLayers+atx] +=(((int16_t*)&rho128[1])[0]+
              ((int16_t*)&rho128[1])[2] +
              ((int16_t*)&rho128[1])[4] +
              ((int16_t*)&rho128[1])[6])/16;

            avg_rho_im[aarx][aatx*nrOfLayers+atx] +=(((int16_t*)&rho128[1])[1]+
              ((int16_t*)&rho128[1])[3] +
              ((int16_t*)&rho128[1])[5] +
              ((int16_t*)&rho128[1])[7])/16;

            mmtmpD0 = simde_mm_madd_epi16(ul_ch128[2],ul_ch128_2[2]);
            // mmtmpD0 contains real part of 4 consecutive outputs (32-bit)
            mmtmpD1 = simde_mm_shufflelo_epi16(ul_ch128[2],_MM_SHUFFLE(2,3,0,1));
            mmtmpD1 = simde_mm_shufflehi_epi16(mmtmpD1,_MM_SHUFFLE(2,3,0,1));
            mmtmpD1 = simde_mm_sign_epi16(mmtmpD1,*(simde__m128i*)conjugate);
            mmtmpD1 = simde_mm_madd_epi16(mmtmpD1,ul_ch128_2[2]);
            // mmtmpD1 contains imag part of 4 consecutive outputs (32-bit)
            mmtmpD0 = simde_mm_srai_epi32(mmtmpD0,output_shift);
            mmtmpD1 = simde_mm_srai_epi32(mmtmpD1,output_shift);
            mmtmpD2 = simde_mm_unpacklo_epi32(mmtmpD0,mmtmpD1);
            mmtmpD3 = simde_mm_unpackhi_epi32(mmtmpD0,mmtmpD1);

            rho128[2] = simde_mm_packs_epi32(mmtmpD2,mmtmpD3);
            //print_shorts("rx:",ul_ch128_2+2);
            //print_shorts("ch:",ul_ch128+2);
            //print_shorts("pack:",rho128+2);
            avg_rho_re[aarx][aatx*nrOfLayers+atx] +=(((int16_t*)&rho128[2])[0]+
              ((int16_t*)&rho128[2])[2] +
              ((int16_t*)&rho128[2])[4] +
              ((int16_t*)&rho128[2])[6])/16;

            avg_rho_im[aarx][aatx*nrOfLayers+atx] +=(((int16_t*)&rho128[2])[1]+
              ((int16_t*)&rho128[2])[3] +
              ((int16_t*)&rho128[2])[5] +
              ((int16_t*)&rho128[2])[7])/16;

            ul_ch128+=3;
            ul_ch128_2+=3;
            rho128+=3;
          }
          if (is_dmrs_symbol==1) {
            //measurements->rx_correlation[0][0][aarx] = signal_energy(&rho[aarx][aatx*nb_aatx+atx][symbol*nb_rb*12],rb*12);
            avg_rho_re[aarx][aatx*nrOfLayers+atx] = 16*avg_rho_re[aarx][aatx*nrOfLayers+atx]/(nb_rb*12);
            avg_rho_im[aarx][aatx*nrOfLayers+atx] = 16*avg_rho_im[aarx][aatx*nrOfLayers+atx]/(nb_rb*12);
            //printf("rho[rx]%d tx%d tx%d = Re: %d Im: %d\n",aarx, aatx,atx, avg_rho_re[aarx][aatx*nb_aatx+atx], avg_rho_im[aarx][aatx*nb_aatx+atx]);
          }
        }
      }
    }
*/
  }

  simde_mm_empty();
  _m_empty();

#ifdef DEBUG_CH_COMP
  for (int nl2=0; nl2<nrOfLayers; nl2++) {
    for (int aarx2=0; aarx2<frame_parms->nb_antennas_rx; aarx2++) {
      rxF   = (int16_t *)&rxdataF_comp[nl2*frame_parms->nb_antennas_rx+aarx2][(symbol*(off+(nb_rb*12)))];

      printf("--------After compansation, layer %i, antenna rx %i----------\n", nl2, aarx2);

      for (prnt_idx=0;prnt_idx<12*5*2;prnt_idx+=2){
        printf("rxF[%d] = (%d,%d)\n", prnt_idx>>1, rxF[prnt_idx],rxF[prnt_idx+1]);
      }
    }
  }
#endif

#ifdef DEBUG_CH_MAG


  for (int ant=0; ant<frame_parms->nb_antennas_rx; ant++) {
    ch_mag   = (int16_t *)&ul_ch_mag[ant][(symbol*(off+(nb_rb*12)))];

    printf("----------------After computation------------------\n");

    for (print_idx=0;print_idx<12*5*2;print_idx+=2){

      printf("ch_mag[%d] = (%d,%d)\n", print_idx>>1, ch_mag[print_idx],ch_mag[print_idx+1]);

    }
  }

#endif

}

void nr_ulsch_detection_mrc(NR_DL_FRAME_PARMS *frame_parms,
                int32_t **rxdataF_comp,
                int32_t **ul_ch_mag,
                int32_t **ul_ch_magb,
                int32_t **ul_ch_magc,
                int32_t ***rho,                
                uint8_t  nrOfLayers,
                uint8_t symbol,
                uint16_t nb_rb,
                int length) {
  int n_rx = frame_parms->nb_antennas_rx;
  simde__m256i *rxdataF_comp256[1+n_rx],*ul_ch_mag256[1+n_rx],*ul_ch_mag256b[1+n_rx],*ul_ch_mag256c[1+n_rx];
  int32_t i;

  int off = ((nb_rb&1) == 1)? 4:0;

  if (frame_parms->nb_antennas_rx>1) {
    int nb_re = nb_rb*12;
    if ((nb_re&7) > 0) nb_re+=8;
    for (int aa=0;aa<frame_parms->nb_antennas_rx;aa++) {
      rxdataF_comp256[aa]   = (simde__m256i *)&rxdataF_comp[aa][(symbol*(nb_re + off))];
      ul_ch_mag256[aa]      = (simde__m256i *)&ul_ch_mag[aa][(symbol*(nb_re + off))];
      ul_ch_mag256b[aa]     = (simde__m256i *)&ul_ch_magb[aa][(symbol*(nb_re + off))];
      ul_ch_mag256c[aa]     = (simde__m256i *)&ul_ch_magc[aa][(symbol*(nb_re + off))];
    }
    for (int aa=1;aa<frame_parms->nb_antennas_rx;aa++) {      
      // MRC on each re of rb, both on MF output and magnitude (for 16QAM/64QAM llr computation)
      for (i=0; i<nb_re>>3; i++) {
	rxdataF_comp256[0][i] = simde_mm256_adds_epi16(rxdataF_comp256[0][i],rxdataF_comp256[aa][i]);
	ul_ch_mag256[0][i]    = simde_mm256_adds_epi16(ul_ch_mag256[0][i], ul_ch_mag256[aa][i]);
	ul_ch_mag256b[0][i]   = simde_mm256_adds_epi16(ul_ch_mag256b[0][i],ul_ch_mag256b[aa][i]);
	ul_ch_mag256c[0][i]   = simde_mm256_adds_epi16(ul_ch_mag256c[0][i],ul_ch_mag256c[aa][i]);
      }

    }
  }

  simde_mm_empty();
  simde_m_empty();
}

void inner_rx_qpsk(int *rxF, 
	           int *ul_ch,
		   int16_t *llr,
		   int aarx, 
                   int length,
		   int output_shift) {

   register simde__m256i xmmtmpD0,xmmtmpD1,xmmtmpD2,xmmtmpD3,xmmtmpD4;
   register simde__m256i complex_shuffle256 = simde_mm256_set_epi8(29,28,31,30,25,24,27,26,21,20,23,22,17,16,19,18,13,12,15,14,9,8,11,10,5,4,7,6,1,0,3,2);
   register simde__m256i conj256 = simde_mm256_set_epi16(1,-1,1,-1,1,-1,1,-1,1,-1,1,-1,1,-1,1,-1);

   simde__m256i *rxF256  = (simde__m256i*)rxF;
   simde__m256i *ulch256 = (simde__m256i*)ul_ch;
   // need to use simde__m64 because llr output is not necessarily aligned to 256 bits, but it is always to 64 bits
   simde__m64   *llr64 = (simde__m64 *)llr;   
 
   if (aarx==0)  
     for (int i=0;i<((length>>3)+((length&7)>0?1:0));i++) {
        xmmtmpD0  = simde_mm256_madd_epi16(ulch256[i],rxF256[i]);
        // xmmtmpD0 contains real part of 8 consecutive outputs (32-bit) of conj(H_m[i])*R_m[i]
        xmmtmpD1  = simde_mm256_shuffle_epi8(ulch256[i],complex_shuffle256);
        xmmtmpD1  = simde_mm256_sign_epi16(xmmtmpD1,conj256);
        xmmtmpD1  = simde_mm256_madd_epi16(xmmtmpD1,rxF256[i]);
        // xmmtmpD1 contains imag part of 8 consecutive outputs (32-bit) of conj(H_m[i])*R_m[i]
        xmmtmpD0  = simde_mm256_srai_epi32(xmmtmpD0,output_shift);
        xmmtmpD1  = simde_mm256_srai_epi32(xmmtmpD1,output_shift);
        xmmtmpD2  = simde_mm256_unpacklo_epi32(xmmtmpD0,xmmtmpD1);
        xmmtmpD3  = simde_mm256_unpackhi_epi32(xmmtmpD0,xmmtmpD1);
        xmmtmpD4  = simde_mm256_packs_epi32(xmmtmpD2,xmmtmpD3);
	*llr64    = (simde__m64)simde_mm256_extract_epi64(xmmtmpD4,0); llr64++;
	*llr64    = (simde__m64)simde_mm256_extract_epi64(xmmtmpD4,1); llr64++;
	*llr64    = (simde__m64)simde_mm256_extract_epi64(xmmtmpD4,2); llr64++;
	*llr64    = (simde__m64)simde_mm256_extract_epi64(xmmtmpD4,3); llr64++;
     }
   else
     for (int i=0;i<((length>>3)+((length&7)>0?1:0));i++) {
        xmmtmpD0 = simde_mm256_madd_epi16(ulch256[i],rxF256[i]);
        // xmmtmpD0 contains real part of 8 consecutive outputs (32-bit) of conj(H_m[i])*R_m[i]
        xmmtmpD1 = simde_mm256_shuffle_epi8(ulch256[i],complex_shuffle256);
        xmmtmpD1 = simde_mm256_sign_epi16(xmmtmpD1,conj256);
        xmmtmpD1 = simde_mm256_madd_epi16(xmmtmpD1,rxF256[i]);
        // xmmtmpD1 contains imag part of 8 consecutive outputs (32-bit) of conj(H_m[i])*R_m[i]
        xmmtmpD0 = simde_mm256_srai_epi32(xmmtmpD0,output_shift);
        xmmtmpD1 = simde_mm256_srai_epi32(xmmtmpD1,output_shift);
        xmmtmpD2 = simde_mm256_unpacklo_epi32(xmmtmpD0,xmmtmpD1);
        xmmtmpD3 = simde_mm256_unpackhi_epi32(xmmtmpD0,xmmtmpD1);
        xmmtmpD4 = simde_mm256_packs_epi32(xmmtmpD2,xmmtmpD3);
	*llr64   = simde_mm_adds_pi16(*llr64,(simde__m64)(simde_mm256_extract_epi64(xmmtmpD4,0))); llr64++;
	*llr64   = simde_mm_adds_pi16(*llr64,(simde__m64)(simde_mm256_extract_epi64(xmmtmpD4,1))); llr64++;
	*llr64   = simde_mm_adds_pi16(*llr64,(simde__m64)(simde_mm256_extract_epi64(xmmtmpD4,2))); llr64++;
	*llr64   = simde_mm_adds_pi16(*llr64,(simde__m64)(simde_mm256_extract_epi64(xmmtmpD4,3))); llr64++;
     }
}

void inner_rx_256qam(int *rxF, int *ul_ch, int16_t *llr, int aarx, int length,int output_shift) {
   register simde__m256i xmmtmpD0,xmmtmpD1,xmmtmpD2,xmmtmpD3,xmmtmpD4,xmmtmpD5,xmmtmpD6,xmmtmpD7,xmmtmpD8,xmmtmpD9;
   register simde__m256i complex_shuffle256 = simde_mm256_set_epi8(29,28,31,30,25,24,27,26,21,20,23,22,17,16,19,18,13,12,15,14,9,8,11,10,5,4,7,6,1,0,3,2);
   register simde__m256i conj256 = simde_mm256_set_epi16(1,-1,1,-1,1,-1,1,-1,1,-1,1,-1,1,-1,1,-1);
  
   register simde__m256i QAM_amp256  = simde_mm256_set1_epi16(QAM256_n1);  // 2/sqrt(10)
   register simde__m256i QAM_amp256b = simde_mm256_set1_epi16(QAM256_n2);
   register simde__m256i QAM_amp256c = simde_mm256_set1_epi16(QAM256_n3);
   simde__m256i *rxF256  = (simde__m256i*)rxF;
   simde__m256i *ulch256 = (simde__m256i*)ul_ch;
   // need to use simde__m64 because llr output is not necessarily aligned to 256 bits, but it is always to 64 bits
 
   if (aarx==0) {
     simde__m256i *llr256 = (simde__m256i *)llr;   
     for (int i=0;i<((length>>4)+((length&15)>0?1:0));i++) {
        xmmtmpD0  = simde_mm256_madd_epi16(ulch256[i],rxF256[i]);
        // xmmtmpD0 contains real part of 8 consecutive outputs (32-bit) of conj(H_m[i])*R_m[i]
        xmmtmpD1  = simde_mm256_shuffle_epi8(ulch256[i],complex_shuffle256);
        xmmtmpD1  = simde_mm256_sign_epi16(xmmtmpD1,conj256);
        xmmtmpD1  = simde_mm256_madd_epi16(xmmtmpD1,rxF256[i]);
        // xmmtmpD1 contains imag part of 8 consecutive outputs (32-bit) of conj(H_m[i])*R_m[i]
        xmmtmpD0  = simde_mm256_srai_epi32(xmmtmpD0,output_shift);
        xmmtmpD1  = simde_mm256_srai_epi32(xmmtmpD1,output_shift);
        xmmtmpD2  = simde_mm256_unpacklo_epi32(xmmtmpD0,xmmtmpD1);
        xmmtmpD3  = simde_mm256_unpackhi_epi32(xmmtmpD0,xmmtmpD1);
        xmmtmpD4  = simde_mm256_packs_epi32(xmmtmpD2,xmmtmpD3);

	// compute channel amplitude for LLR
        xmmtmpD0 = simde_mm256_madd_epi16(ulch256[i],ulch256[i]);
        xmmtmpD0 = simde_mm256_srai_epi32(xmmtmpD0,output_shift);
        xmmtmpD0 = simde_mm256_packs_epi32(xmmtmpD0,xmmtmpD0);   // contains 16 LLRs 
        xmmtmpD2 = simde_mm256_unpacklo_epi16(xmmtmpD0,xmmtmpD0);
        xmmtmpD1 = simde_mm256_mulhrs_epi16(xmmtmpD2,QAM_amp256);
        xmmtmpD6 = simde_mm256_mulhrs_epi16(xmmtmpD2,QAM_amp256b);
        xmmtmpD8 = simde_mm256_mulhrs_epi16(xmmtmpD2,QAM_amp256c);

        xmmtmpD2 = simde_mm256_abs_epi16(xmmtmpD4); // registers of even index in xmm0-> |y_R|, registers of odd index in xmm0-> |y_I|
        xmmtmpD2 = simde_mm256_subs_epi16(xmmtmpD1,xmmtmpD2); // registers of even index in xmm0-> |y_R|-|h|^2, registers of odd index in xmm0-> |y_I|-|h|^2
	// xmmtmpD2 contains 16 LLRs
        xmmtmpD7 = simde_mm256_abs_epi16(xmmtmpD2);
        xmmtmpD7 = simde_mm256_subs_epi16(xmmtmpD6,xmmtmpD7); // contains 16 LLRs
        xmmtmpD9 = simde_mm256_abs_epi16(xmmtmpD7);
        xmmtmpD9 = simde_mm256_subs_epi16(xmmtmpD8,xmmtmpD9); // contains 16 LLRs

        // xmmtmpD4 A0 A1 A2 A3 A4 A5 A6 A7
	// xmmtmpD2 B0 B1 B2 B3 B4 B5 B6 B7
	// xmmtmpD7 C0 C1 C2 C3 C4 C5 C6 C7
	// xmmtmpD9 D0 D1 D2 D3 D4 D5 D6 D7	
        xmmtmpD1 = simde_mm256_unpacklo_epi32(xmmtmpD4,xmmtmpD2); // A0 B0 A1 B1 A4 B4 A5 B5
        xmmtmpD3 = simde_mm256_unpackhi_epi32(xmmtmpD4,xmmtmpD2); // A2 B2 A3 B3 A6 B6 A7 B7
        xmmtmpD5 = simde_mm256_unpacklo_epi32(xmmtmpD7,xmmtmpD9); // C0 D0 C1 D1 C4 D4 C5 D5
        xmmtmpD6 = simde_mm256_unpackhi_epi32(xmmtmpD7,xmmtmpD9); // C2 D2 C3 D3 C6 D6 C7 D7

        xmmtmpD2 = simde_mm256_unpacklo_epi64(xmmtmpD1,xmmtmpD5); // A0 B0 C0 D0 A4 B4 C4 D4
        xmmtmpD4 = simde_mm256_unpackhi_epi64(xmmtmpD1,xmmtmpD5); // A1 B1 C1 D1 A5 B5 C5 D5
        xmmtmpD1 = simde_mm256_unpacklo_epi64(xmmtmpD3,xmmtmpD6); // A2 B2 C2 D2 A6 B6 C6 D6
        xmmtmpD5 = simde_mm256_unpackhi_epi64(xmmtmpD3,xmmtmpD6); // A3 B3 C3 D3 A7 B7 C7 D7
	llr256[0] = simde_mm256_permute2x128_si256(xmmtmpD2, xmmtmpD4, 0x20); // A0 B0 C0 D0 A1 B1 C1 D1
	llr256[1] = simde_mm256_permute2x128_si256(xmmtmpD1, xmmtmpD5, 0x20); // A2 B2 C2 D2 A3 B3 C3 D3
	llr256[2] = simde_mm256_permute2x128_si256(xmmtmpD2, xmmtmpD4, 0x31); // A4 B4 C4 D4 A5 B5 C5 D5
	llr256[3] = simde_mm256_permute2x128_si256(xmmtmpD1, xmmtmpD5, 0x31); // A6 B6 C6 D6 A7 B7 C7 D7
        llr256+=4;
     }
   } else {
     simde__m256i *llr256 = (simde__m256i *)llr;   
     for (int i=0;i<((length>>4)+((length&15)>0?1:0));i++) {
        xmmtmpD0  = simde_mm256_madd_epi16(ulch256[i],rxF256[i]);
        // xmmtmpD0 contains real part of 8 consecutive outputs (32-bit) of conj(H_m[i])*R_m[i]
        xmmtmpD1  = simde_mm256_shuffle_epi8(ulch256[i],complex_shuffle256);
        xmmtmpD1  = simde_mm256_sign_epi16(xmmtmpD1,conj256);
        xmmtmpD1  = simde_mm256_madd_epi16(xmmtmpD1,rxF256[i]);
        // xmmtmpD1 contains imag part of 8 consecutive outputs (32-bit) of conj(H_m[i])*R_m[i]
        xmmtmpD0  = simde_mm256_srai_epi32(xmmtmpD0,output_shift);
        xmmtmpD1  = simde_mm256_srai_epi32(xmmtmpD1,output_shift);
        xmmtmpD2  = simde_mm256_unpacklo_epi32(xmmtmpD0,xmmtmpD1);
        xmmtmpD3  = simde_mm256_unpackhi_epi32(xmmtmpD0,xmmtmpD1);
        xmmtmpD4  = simde_mm256_packs_epi32(xmmtmpD2,xmmtmpD3);

	// compute channel amplitude for LLR
        xmmtmpD0 = simde_mm256_madd_epi16(ulch256[i],ulch256[i]);
        xmmtmpD0 = simde_mm256_srai_epi32(xmmtmpD0,output_shift);
        xmmtmpD0 = simde_mm256_packs_epi32(xmmtmpD0,xmmtmpD0);
        xmmtmpD2 = simde_mm256_unpacklo_epi16(xmmtmpD0,xmmtmpD0);
        xmmtmpD1 = simde_mm256_mulhrs_epi16(xmmtmpD2,QAM_amp256);
        xmmtmpD6 = simde_mm256_mulhrs_epi16(xmmtmpD2,QAM_amp256b);
        xmmtmpD8 = simde_mm256_mulhrs_epi16(xmmtmpD2,QAM_amp256c);

        xmmtmpD2 = simde_mm256_abs_epi16(xmmtmpD4); // registers of even index in xmm0-> |y_R|, registers of odd index in xmm0-> |y_I|
        xmmtmpD2 = simde_mm256_subs_epi16(xmmtmpD1,xmmtmpD2); // registers of even index in xmm0-> |y_R|-|h|^2, registers of odd index in xmm0-> |y_I|-|h|^2
        xmmtmpD7 = simde_mm256_abs_epi16(xmmtmpD2);
        xmmtmpD7 = simde_mm256_subs_epi16(xmmtmpD6,xmmtmpD7);
        xmmtmpD9 = simde_mm256_abs_epi16(xmmtmpD7);
        xmmtmpD9 = simde_mm256_subs_epi16(xmmtmpD8,xmmtmpD9);
 
        // xmmtmpD4 A0 A1 A2 A3 A4 A5 A6 A7
	// xmmtmpD2 B0 B1 B2 B3 B4 B5 B6 B7
	// xmmtmpD7 C0 C1 C2 C3 C4 C5 C6 C7
	// xmmtmpD9 D0 D1 D2 D3 D4 D5 D6 D7	
        xmmtmpD1 = simde_mm256_unpacklo_epi32(xmmtmpD4,xmmtmpD2); // A0 B0 A1 B1 A4 B4 A5 B5
        xmmtmpD3 = simde_mm256_unpackhi_epi32(xmmtmpD4,xmmtmpD2); // A2 B2 A3 B3 A6 B6 A7 B7
        xmmtmpD5 = simde_mm256_unpacklo_epi32(xmmtmpD7,xmmtmpD9); // C0 D0 C1 D1 C4 D4 C5 D5
        xmmtmpD6 = simde_mm256_unpackhi_epi32(xmmtmpD7,xmmtmpD9); // C2 D2 C3 D3 C6 D6 C7 D7

        xmmtmpD2 = simde_mm256_unpacklo_epi64(xmmtmpD1,xmmtmpD5); // A0 B0 C0 D0 A4 B4 C4 D4
        xmmtmpD4 = simde_mm256_unpackhi_epi64(xmmtmpD1,xmmtmpD5); // A1 B1 C1 D1 A5 B5 C5 D5
        xmmtmpD1 = simde_mm256_unpacklo_epi64(xmmtmpD3,xmmtmpD6); // A2 B2 C2 D2 A6 B6 C6 D6
        xmmtmpD5 = simde_mm256_unpackhi_epi64(xmmtmpD3,xmmtmpD6); // A3 B3 C3 D3 A7 B7 C7 D7
	llr256[0] = simde_mm256_adds_epi16(llr256[0],simde_mm256_permute2x128_si256(xmmtmpD2, xmmtmpD4, 0x20)); // A0 B0 C0 D0 A1 B1 C1 D1
	llr256[1] = simde_mm256_adds_epi16(llr256[1],simde_mm256_permute2x128_si256(xmmtmpD1, xmmtmpD5, 0x20)); // A2 B2 C2 D2 A3 B3 C3 D3
	llr256[2] = simde_mm256_adds_epi16(llr256[2],simde_mm256_permute2x128_si256(xmmtmpD2, xmmtmpD4, 0x31)); // A4 B4 C4 D4 A5 B5 C5 D5
	llr256[3] = simde_mm256_adds_epi16(llr256[3],simde_mm256_permute2x128_si256(xmmtmpD1, xmmtmpD5, 0x31)); // A6 B6 C6 D6 A7 B7 C7 D7
        llr256+=4;
     }
   }  
}



void inner_rx_64qam(int * restrict rxF, int * restrict ul_ch, int16_t *restrict llr, int aarx, int length,int output_shift) {
   register simde__m256i xmmtmpD0,xmmtmpD1,xmmtmpD2,xmmtmpD3,xmmtmpD4,xmmtmpD6,xmmtmpD7;
   register simde__m256i complex_shuffle256 = simde_mm256_set_epi8(29,28,31,30,25,24,27,26,21,20,23,22,17,16,19,18,13,12,15,14,9,8,11,10,5,4,7,6,1,0,3,2);
   register simde__m256i conj256 = simde_mm256_set_epi16(1,-1,1,-1,1,-1,1,-1,1,-1,1,-1,1,-1,1,-1);
  
   register simde__m256i QAM_amp256  = simde_mm256_set1_epi16(QAM64_n1);  // 2/sqrt(10)
   register simde__m256i QAM_amp256b = simde_mm256_set1_epi16(QAM64_n2);
   simde__m256i *rxF256  = (simde__m256i*)rxF;
   simde__m256i *ulch256 = (simde__m256i*)ul_ch;
   // need to use simde__m64 because llr output is not necessarily aligned to 256 bits, but it is always to 64 bits

   
   if (aarx==0) {
     uint32_t *llr32 = (uint32_t *)llr;   
     for (int i=0;i<((length>>3)+((length&7)>0?1:0));i++) {
        xmmtmpD0  = simde_mm256_madd_epi16(ulch256[i],rxF256[i]);
        // xmmtmpD0 contains real part of 8 consecutive outputs (32-bit) of conj(H_m[i])*R_m[i]
        xmmtmpD1  = simde_mm256_shuffle_epi8(ulch256[i],complex_shuffle256);
        xmmtmpD1  = simde_mm256_sign_epi16(xmmtmpD1,conj256);
        xmmtmpD1  = simde_mm256_madd_epi16(xmmtmpD1,rxF256[i]);
        // xmmtmpD1 contains imag part of 8 consecutive outputs (32-bit) of conj(H_m[i])*R_m[i]
        xmmtmpD0  = simde_mm256_srai_epi32(xmmtmpD0,output_shift);
        xmmtmpD1  = simde_mm256_srai_epi32(xmmtmpD1,output_shift);
        xmmtmpD2  = simde_mm256_unpacklo_epi32(xmmtmpD0,xmmtmpD1);
        xmmtmpD3  = simde_mm256_unpackhi_epi32(xmmtmpD0,xmmtmpD1);
        xmmtmpD4  = simde_mm256_packs_epi32(xmmtmpD2,xmmtmpD3);

	// compute channel amplitude for LLR
        xmmtmpD0 = simde_mm256_madd_epi16(ulch256[i],ulch256[i]);
        xmmtmpD0 = simde_mm256_srai_epi32(xmmtmpD0,output_shift);
        xmmtmpD0 = simde_mm256_packs_epi32(xmmtmpD0,xmmtmpD0);
        xmmtmpD2 = simde_mm256_unpacklo_epi16(xmmtmpD0,xmmtmpD0);
        xmmtmpD1 = simde_mm256_mulhrs_epi16(xmmtmpD2,QAM_amp256);
        xmmtmpD6 = simde_mm256_mulhrs_epi16(xmmtmpD2,QAM_amp256b);

        xmmtmpD2 = simde_mm256_abs_epi16(xmmtmpD4); // registers of even index in xmm0-> |y_R|, registers of odd index in xmm0-> |y_I|
        xmmtmpD2 = simde_mm256_subs_epi16(xmmtmpD1,xmmtmpD2); // registers of even index in xmm0-> |y_R|-|h|^2, registers of odd index in xmm0-> |y_I|-|h|^2
        xmmtmpD7 = simde_mm256_abs_epi16(xmmtmpD2);
        xmmtmpD7 = simde_mm256_subs_epi16(xmmtmpD6,xmmtmpD7);
 
        llr32[0] = simde_mm256_extract_epi32(xmmtmpD4,0);
        llr32[1] = simde_mm256_extract_epi32(xmmtmpD2,0);
        llr32[2] = simde_mm256_extract_epi32(xmmtmpD7,0);
        llr32[3] = simde_mm256_extract_epi32(xmmtmpD4,1);
        llr32[4] = simde_mm256_extract_epi32(xmmtmpD2,1);
        llr32[5] = simde_mm256_extract_epi32(xmmtmpD7,1);
        llr32[6] = simde_mm256_extract_epi32(xmmtmpD4,2);
        llr32[7] = simde_mm256_extract_epi32(xmmtmpD2,2);
        llr32[8] = simde_mm256_extract_epi32(xmmtmpD7,2);
        llr32[9] = simde_mm256_extract_epi32(xmmtmpD4,3);
        llr32[10] = simde_mm256_extract_epi32(xmmtmpD2,3);
        llr32[11] = simde_mm256_extract_epi32(xmmtmpD7,3);
        llr32[12] = simde_mm256_extract_epi32(xmmtmpD4,4);
        llr32[13] = simde_mm256_extract_epi32(xmmtmpD2,4);
        llr32[14] = simde_mm256_extract_epi32(xmmtmpD7,4);
        llr32[15] = simde_mm256_extract_epi32(xmmtmpD4,5);
        llr32[16] = simde_mm256_extract_epi32(xmmtmpD2,5);
        llr32[17] = simde_mm256_extract_epi32(xmmtmpD7,5);
        llr32[18] = simde_mm256_extract_epi32(xmmtmpD4,6);
        llr32[19] = simde_mm256_extract_epi32(xmmtmpD2,6);
        llr32[20] = simde_mm256_extract_epi32(xmmtmpD7,6);
        llr32[21] = simde_mm256_extract_epi32(xmmtmpD4,7);
        llr32[22] = simde_mm256_extract_epi32(xmmtmpD2,7);
        llr32[23] = simde_mm256_extract_epi32(xmmtmpD7,7);
        llr32+=24;
     }
   } else {
   
     simde__m64 *llr64 = (simde__m64 *)llr;   
     for (int i=0;i<((length>>3)+((length&7)>0?1:0));i++) {
        xmmtmpD0  = simde_mm256_madd_epi16(ulch256[i],rxF256[i]);
        // xmmtmpD0 contains real part of 8 consecutive outputs (32-bit) of conj(H_m[i])*R_m[i]
        xmmtmpD1  = simde_mm256_shuffle_epi8(ulch256[i],complex_shuffle256);
        xmmtmpD1  = simde_mm256_sign_epi16(xmmtmpD1,conj256);
        xmmtmpD1  = simde_mm256_madd_epi16(xmmtmpD1,rxF256[i]);
        // xmmtmpD1 contains imag part of 8 consecutive outputs (32-bit) of conj(H_m[i])*R_m[i]
        xmmtmpD0  = simde_mm256_srai_epi32(xmmtmpD0,output_shift);
        xmmtmpD1  = simde_mm256_srai_epi32(xmmtmpD1,output_shift);
        xmmtmpD2  = simde_mm256_unpacklo_epi32(xmmtmpD0,xmmtmpD1);
        xmmtmpD3  = simde_mm256_unpackhi_epi32(xmmtmpD0,xmmtmpD1);
        xmmtmpD4  = simde_mm256_packs_epi32(xmmtmpD2,xmmtmpD3);

	// compute channel amplitude for LLR
        xmmtmpD0 = simde_mm256_madd_epi16(ulch256[i],ulch256[i]);
        xmmtmpD0 = simde_mm256_srai_epi32(xmmtmpD0,output_shift);
        xmmtmpD0 = simde_mm256_packs_epi32(xmmtmpD0,xmmtmpD0);
        xmmtmpD2 = simde_mm256_unpacklo_epi16(xmmtmpD0,xmmtmpD0);
        xmmtmpD1 = simde_mm256_mulhrs_epi16(xmmtmpD2,QAM_amp256);
        xmmtmpD6 = simde_mm256_mulhrs_epi16(xmmtmpD2,QAM_amp256b);

        xmmtmpD2 = simde_mm256_abs_epi16(xmmtmpD4); // registers of even index in xmm0-> |y_R|, registers of odd index in xmm0-> |y_I|
        xmmtmpD2 = simde_mm256_subs_epi16(xmmtmpD1,xmmtmpD2); // registers of even index in xmm0-> |y_R|-|h|^2, registers of odd index in xmm0-> |y_I|-|h|^2
        xmmtmpD7 = simde_mm256_abs_epi16(xmmtmpD2);
        xmmtmpD7 = simde_mm256_subs_epi16(xmmtmpD6,xmmtmpD7);
        llr64[0] = simde_mm_adds_pi16(llr64[0],simde_mm_set_pi32(simde_mm256_extract_epi32(xmmtmpD2,0),simde_mm256_extract_epi32(xmmtmpD4,0)));
        llr64[1] = simde_mm_adds_pi16(llr64[1],simde_mm_set_pi32(simde_mm256_extract_epi32(xmmtmpD4,1),simde_mm256_extract_epi32(xmmtmpD7,0)));
        llr64[2] = simde_mm_adds_pi16(llr64[2],simde_mm_set_pi32(simde_mm256_extract_epi32(xmmtmpD7,1),simde_mm256_extract_epi32(xmmtmpD2,1)));
        llr64[3] = simde_mm_adds_pi16(llr64[3],simde_mm_set_pi32(simde_mm256_extract_epi32(xmmtmpD2,2),simde_mm256_extract_epi32(xmmtmpD4,2)));
        llr64[4] = simde_mm_adds_pi16(llr64[4],simde_mm_set_pi32(simde_mm256_extract_epi32(xmmtmpD4,3),simde_mm256_extract_epi32(xmmtmpD7,2)));
        llr64[5] = simde_mm_adds_pi16(llr64[5],simde_mm_set_pi32(simde_mm256_extract_epi32(xmmtmpD7,3),simde_mm256_extract_epi32(xmmtmpD2,3)));
        llr64[6] = simde_mm_adds_pi16(llr64[6],simde_mm_set_pi32(simde_mm256_extract_epi32(xmmtmpD2,4),simde_mm256_extract_epi32(xmmtmpD4,4)));
        llr64[7] = simde_mm_adds_pi16(llr64[7],simde_mm_set_pi32(simde_mm256_extract_epi32(xmmtmpD4,5),simde_mm256_extract_epi32(xmmtmpD7,4)));
        llr64[8] = simde_mm_adds_pi16(llr64[8],simde_mm_set_pi32(simde_mm256_extract_epi32(xmmtmpD7,5),simde_mm256_extract_epi32(xmmtmpD2,5)));
        llr64[9] = simde_mm_adds_pi16(llr64[9],simde_mm_set_pi32(simde_mm256_extract_epi32(xmmtmpD2,6),simde_mm256_extract_epi32(xmmtmpD4,6)));
        llr64[10] = simde_mm_adds_pi16(llr64[10],simde_mm_set_pi32(simde_mm256_extract_epi32(xmmtmpD4,7),simde_mm256_extract_epi32(xmmtmpD7,6)));
        llr64[11] = simde_mm_adds_pi16(llr64[11],simde_mm_set_pi32(simde_mm256_extract_epi32(xmmtmpD7,7),simde_mm256_extract_epi32(xmmtmpD2,7)));
        llr64+=12;
     }
   }  
}


/*
void inner_rx_64qam(int* rxF, int*  ul_ch, int16_t *  llr, int aarx, int length,int output_shift) {
  simde__m256i xmmtmpD0,xmmtmpD1,xmmtmpD2,xmmtmpD3,xmmtmpD4,xmmtmpD6,xmmtmpD7;
  simde__m256i complex_shuffle256 = simde_mm256_set_epi8(29,28,31,30,25,24,27,26,21,20,23,22,17,16,19,18,13,12,15,14,9,8,11,10,5,4,7,6,1,0,3,2);
  simde__m256i conj256 = simde_mm256_set_epi16(1,-1,1,-1,1,-1,1,-1,1,-1,1,-1,1,-1,1,-1);

  simde__m256i QAM_amp256  = simde_mm256_set1_epi16(QAM64_n1);  // 2/sqrt(10)
  simde__m256i QAM_amp256b = simde_mm256_set1_epi16(QAM64_n2);
  simde__m256i *rxF256  = (simde__m256i*)rxF;
  simde__m256i *ulch256 = (simde__m256i*)ul_ch;
  // need to use simde__m64 because llr output is not necessarily aligned to 256 bits, but it is always to 64 bits
  simde__m256i shuffle_a = simde_mm256_set_epi32(0,3,6,1,4,7,2,5);
  simde__m256i shuffle_b = simde_mm256_set_epi32(5,0,3,6,1,4,7,2);
  simde__m256i shuffle_c = simde_mm256_set_epi32(2,5,0,3,6,1,4,7);

  uint32_t  *llr32 = (uint32_t *)llr;   
  simde__m256i *llr256 = (simde__m256i*)llr;
  int const lim = (length>>3)+(length&0x7) > 0 ? 1 : 0;
  for (int i=0; i< lim ;i++) {
    xmmtmpD0  = simde_mm256_madd_epi16(ulch256[i],rxF256[i]);
    // xmmtmpD0 ontains real part of 8 consecutive outputs (32-bit) of conj(H_m[i])*R_m[i]
    xmmtmpD1  = simde_mm256_shuffle_epi8(ulch256[i],complex_shuffle256);
    xmmtmpD1  = simde_mm256_sign_epi16(xmmtmpD1,conj256);
    xmmtmpD1  = simde_mm256_madd_epi16(xmmtmpD1,rxF256[i]);
    // xmmtmpD1 contains imag part of 8 consecutive outputs (32-bit) of conj(H_m[i])*R_m[i]
    xmmtmpD0  = simde_mm256_srai_epi32(xmmtmpD0,output_shift);
    xmmtmpD1  = simde_mm256_srai_epi32(xmmtmpD1,output_shift);
    xmmtmpD2  = simde_mm256_unpacklo_epi32(xmmtmpD0,xmmtmpD1);
    xmmtmpD3  = simde_mm256_unpackhi_epi32(xmmtmpD0,xmmtmpD1);
    xmmtmpD4  = simde_mm256_packs_epi32(xmmtmpD2,xmmtmpD3);

    // compute channel amplitude for LLR
    xmmtmpD0 = simde_mm256_madd_epi16(ulch256[i],ulch256[i]);
    xmmtmpD0 = simde_mm256_srai_epi32(xmmtmpD0,output_shift);
    xmmtmpD0 = simde_mm256_packs_epi32(xmmtmpD0,xmmtmpD0);
    xmmtmpD2 = simde_mm256_unpacklo_epi16(xmmtmpD0,xmmtmpD0);
    xmmtmpD1 = simde_mm256_mulhrs_epi16(xmmtmpD2,QAM_amp256);
    xmmtmpD6 = simde_mm256_mulhrs_epi16(xmmtmpD2,QAM_amp256b);

    xmmtmpD2 = simde_mm256_abs_epi16(xmmtmpD4); // registers of even index in xmm0-> |y_R|, registers of odd index in xmm0-> |y_I|
    xmmtmpD2 = simde_mm256_subs_epi16(xmmtmpD1,xmmtmpD2); // registers of even index in xmm0-> |y_R|-|h|^2, registers of odd index in xmm0-> |y_I|-|h|^2
    xmmtmpD7 = simde_mm256_abs_epi16(xmmtmpD2);
    xmmtmpD7 = simde_mm256_subs_epi16(xmmtmpD6,xmmtmpD7);

    simde__m256i a =  simde_mm256_permutevar8x32_epi32(xmmtmpD4, shuffle_a);  
    simde__m256i b =  simde_mm256_permutevar8x32_epi32(xmmtmpD2, shuffle_b);
    simde__m256i c =  simde_mm256_permutevar8x32_epi32(xmmtmpD7, shuffle_c);

    simde__m256i tmp = simde_mm256_blend_epi32(a, b, 0b10010010); 
    llr256[0] = simde_mm256_blend_epi32(c, tmp, 0b00100100);

    tmp = simde_mm256_blend_epi32(a, b, 0b01001001);
    llr256[1] = simde_mm256_blend_epi32(c, tmp, 0b10010010);

    tmp = simde_mm256_blend_epi32(a, b, 0b00100100);
    llr256[2] = simde_mm256_blend_epi32(c,tmp, 0b01001001 );
    llr256 += 3;
  }
}
*/


void inner_rx_16qam( int * rxF, int * ul_ch, int16_t * llr, int aarx, int length,int output_shift) {
   register simde__m256i xmmtmpD0,xmmtmpD1,xmmtmpD2,xmmtmpD3,xmmtmpD4,xmmtmpD5;
   register simde__m256i complex_shuffle256 = simde_mm256_set_epi8(29,28,31,30,25,24,27,26,21,20,23,22,17,16,19,18,13,12,15,14,9,8,11,10,5,4,7,6,1,0,3,2);
   register simde__m256i conj256 = simde_mm256_set_epi16(1,-1,1,-1,1,-1,1,-1,1,-1,1,-1,1,-1,1,-1);
  
   register simde__m256i QAM_amp256  = simde_mm256_set1_epi16(QAM64_n1);  // 2/sqrt(10)
   simde__m256i *rxF256  = (simde__m256i*)rxF;
   simde__m256i *ulch256 = (simde__m256i*)ul_ch;
   // need to use simde__m64 because llr output is not necessarily aligned to 256 bits, but it is always to 64 bits
   simde__m64 *llr64 = (simde__m64 *)llr;   
 
   if (aarx==0)  
     for (int i=0;i<((length>>3)+((length&7)>0?1:0));i++) {
        xmmtmpD0  = simde_mm256_madd_epi16(ulch256[i],rxF256[i]);
        // xmmtmpD0 contains real part of 8 consecutive outputs (32-bit) of conj(H_m[i])*R_m[i]
        xmmtmpD1  = simde_mm256_shuffle_epi8(ulch256[i],complex_shuffle256);
        xmmtmpD1  = simde_mm256_sign_epi16(xmmtmpD1,conj256);
        xmmtmpD1  = simde_mm256_madd_epi16(xmmtmpD1,rxF256[i]);
        // xmmtmpD1 contains imag part of 8 consecutive outputs (32-bit) of conj(H_m[i])*R_m[i]
        xmmtmpD0  = simde_mm256_srai_epi32(xmmtmpD0,output_shift);
        xmmtmpD1  = simde_mm256_srai_epi32(xmmtmpD1,output_shift);
        xmmtmpD2  = simde_mm256_unpacklo_epi32(xmmtmpD0,xmmtmpD1);
        xmmtmpD3  = simde_mm256_unpackhi_epi32(xmmtmpD0,xmmtmpD1);
        xmmtmpD4  = simde_mm256_packs_epi32(xmmtmpD2,xmmtmpD3);

	// compute channel amplitude for LLR
        xmmtmpD0 = simde_mm256_madd_epi16(ulch256[i],ulch256[i]);
        xmmtmpD0 = simde_mm256_srai_epi32(xmmtmpD0,output_shift);
        xmmtmpD0 = simde_mm256_packs_epi32(xmmtmpD0,xmmtmpD0);
        xmmtmpD1 = simde_mm256_unpacklo_epi16(xmmtmpD0,xmmtmpD0);
        xmmtmpD1 = simde_mm256_mulhrs_epi16(xmmtmpD1,QAM_amp256);

        xmmtmpD2 = simde_mm256_abs_epi16(xmmtmpD4); // registers of even index in xmm0-> |y_R|, registers of odd index in xmm0-> |y_I|
        xmmtmpD2 = simde_mm256_subs_epi16(xmmtmpD1,xmmtmpD2); // registers of even index in xmm0-> |y_R|-|h|^2, registers of odd index in xmm0-> |y_I|-|h|^2
 
        xmmtmpD3 = simde_mm256_unpacklo_epi32(xmmtmpD4,xmmtmpD2); // llr128[0] contains the llrs of the 1st,2nd,5th and 6th REs
        xmmtmpD5 = simde_mm256_unpackhi_epi32(xmmtmpD4,xmmtmpD2); // llr128[1] contains the llrs of the 3rd, 4th, 7th and 8th REs
        // 1st/2nd RE
        llr64[0] = (simde__m64)simde_mm256_extract_epi64(xmmtmpD3,0); // llr32[0] low 16 bits-> y_R        , high 16 bits-> y_I
        // 3rd/4th RE
        llr64[1] = (simde__m64)simde_mm256_extract_epi64(xmmtmpD3,1); // llr32[2] low 16 bits-> y_R        , high 16 bits-> y_I
        // 4th/5th RE
        llr64[2] = (simde__m64)simde_mm256_extract_epi64(xmmtmpD5,0); // llr32[4] low 16 bits-> y_R        , high 16 bits-> y_I
        // 6th/7th RE
        llr64[3] = (simde__m64)simde_mm256_extract_epi64(xmmtmpD5,1); // llr32[6] low 16 bits-> y_R        , high 16 bits-> y_I
        // 8th/9th RE
        llr64[4] = (simde__m64)simde_mm256_extract_epi64(xmmtmpD3,2); // llr32[8] low 16 bits-> y_R        , high 16 bits-> y_I
        // 10th/11th RE
        llr64[5] = (simde__m64)simde_mm256_extract_epi64(xmmtmpD3,3); // llr32[10] low 16 bits-> y_R        , high 16 bits-> y_I
        // 12th/13th RE
        llr64[6] = (simde__m64)simde_mm256_extract_epi64(xmmtmpD5,2); // llr32[12] low 16 bits-> y_R        , high 16 bits-> y_I
        // 14th/15th RE
        llr64[7] = (simde__m64)simde_mm256_extract_epi64(xmmtmpD5,3); // llr32[14] low 16 bits-> y_R        , high 16 bits-> y_I
        llr64+=8;
     }
   else
     for (int i=0;i<((length>>3)+((length&7)>0?1:0));i++) {
        xmmtmpD0 = simde_mm256_madd_epi16(ulch256[i],rxF256[i]);
        // xmmtmpD0 contains real part of 8 consecutive outputs (32-bit) of conj(H_m[i])*R_m[i]
        xmmtmpD1 = simde_mm256_shuffle_epi8(ulch256[i],complex_shuffle256);
        xmmtmpD1 = simde_mm256_sign_epi16(xmmtmpD1,conj256);
        xmmtmpD1 = simde_mm256_madd_epi16(xmmtmpD1,rxF256[i]);
        // xmmtmpD1 contains imag part of 8 consecutive outputs (32-bit) of conj(H_m[i])*R_m[i]
        xmmtmpD0 = simde_mm256_srai_epi32(xmmtmpD0,output_shift);
        xmmtmpD1 = simde_mm256_srai_epi32(xmmtmpD1,output_shift);
        xmmtmpD2 = simde_mm256_unpacklo_epi32(xmmtmpD0,xmmtmpD1);
        xmmtmpD3 = simde_mm256_unpackhi_epi32(xmmtmpD0,xmmtmpD1);
        xmmtmpD4 = simde_mm256_packs_epi32(xmmtmpD2,xmmtmpD3);

        xmmtmpD0 = simde_mm256_madd_epi16(ulch256[i],ulch256[i]);
        xmmtmpD0 = simde_mm256_srai_epi32(xmmtmpD0,output_shift);
        xmmtmpD0 = simde_mm256_packs_epi32(xmmtmpD0,xmmtmpD0);
        xmmtmpD1 = simde_mm256_unpacklo_epi16(xmmtmpD0,xmmtmpD0);
        xmmtmpD1 = simde_mm256_mulhrs_epi16(xmmtmpD1,QAM_amp256);

        xmmtmpD2 = simde_mm256_abs_epi16(xmmtmpD4); // registers of even index in xmm0-> |y_R|, registers of odd index in xmm0-> |y_I|
        xmmtmpD2 = simde_mm256_subs_epi16(xmmtmpD1,xmmtmpD2); // registers of even index in xmm0-> |y_R|-|h|^2, registers of odd index in xmm0-> |y_I|-|h|^2
 
        xmmtmpD3 = simde_mm256_unpacklo_epi32(xmmtmpD4,xmmtmpD2); // llr128[0] contains the llrs of the 1st,2nd,5th and 6th REs
        xmmtmpD5 = simde_mm256_unpackhi_epi32(xmmtmpD4,xmmtmpD2); // llr128[1] contains the llrs of the 3rd, 4th, 7th and 8th REs
        // 1st/2nd RE
        llr64[0] = simde_mm_adds_pi16(llr64[0],(simde__m64)simde_mm256_extract_epi64(xmmtmpD3,0)); 
        // 3rd/4th RE
        llr64[1] = simde_mm_adds_pi16(llr64[1],(simde__m64)simde_mm256_extract_epi64(xmmtmpD3,1)); 
        // 4th/5th RE
        llr64[2] = simde_mm_adds_pi16(llr64[2],(simde__m64)simde_mm256_extract_epi64(xmmtmpD5,0)); 
        // 6th/7th RE
        llr64[3] = simde_mm_adds_pi16(llr64[3],(simde__m64)simde_mm256_extract_epi64(xmmtmpD5,1)); 
        // 8th/9th RE
        llr64[4] = simde_mm_adds_pi16(llr64[4],(simde__m64)simde_mm256_extract_epi64(xmmtmpD3,2)); 
        // 10th/11th RE
        llr64[5] = simde_mm_adds_pi16(llr64[5],(simde__m64)simde_mm256_extract_epi64(xmmtmpD3,3)); 
        // 12th/13th RE
        llr64[6] = simde_mm_adds_pi16(llr64[6],(simde__m64)simde_mm256_extract_epi64(xmmtmpD5,2)); 
        // 14th/15th RE
        llr64[7] = simde_mm_adds_pi16(llr64[7],(simde__m64)simde_mm256_extract_epi64(xmmtmpD5,3)); 
        llr64+=8;
     }
}


void nr_pusch_symbol_processing_noprecoding(void *arg)
{

//  int64_t now = time_now_us();
//  printf("Into  nr_pusch_symbol_processing_noprecoding %ld \n ", now);

  puschSymbolProc_t *rdata=(puschSymbolProc_t*)arg;

  PHY_VARS_gNB *gNB=rdata->gNB;
  NR_DL_FRAME_PARMS *frame_parms=rdata->frame_parms;
  nfapi_nr_pusch_pdu_t *rel15_ul=rdata->rel15_ul;
  int ulsch_id=rdata->ulsch_id;
  int slot=rdata->slot;
  int16_t *llr=rdata->llr;
  int16_t *s=rdata->s;

  for (int symbol = rdata->startSymbol;symbol < rdata->startSymbol+rdata->numSymbols;symbol++) {
    int dmrs_symbol_flag = (rel15_ul->ul_dmrs_symb_pos >> symbol) & 0x01;
    int nb_re_pusch = gNB->pusch_vars[ulsch_id]->ul_valid_re_per_slot[symbol];
    // this needs to be reworded for parrellization, we need a table which give dmrs symbol location
    // used for chennel estimate, they are being run in parallel!
    if (dmrs_symbol_flag == 1) {
      if ((rel15_ul->ul_dmrs_symb_pos >> ((symbol + 1) % frame_parms->symbols_per_slot)) & 0x01)
        AssertFatal(1==0,"Double DMRS configuration is not yet supported\n");

      gNB->pusch_vars[ulsch_id]->dmrs_symbol = symbol;
    }

    LOG_D(PHY,"symbol %d: nb_re_pusch %d, DMRS symbl used for Chest :%d \n", symbol, nb_re_pusch, gNB->pusch_vars[ulsch_id]->dmrs_symbol);

    if (nb_re_pusch == 0) continue;


    void (*inner_rx)(int *,int *,int16_t *,int,int,int);

    if      (rel15_ul->qam_mod_order == 2) inner_rx = inner_rx_qpsk;
    else if (rel15_ul->qam_mod_order == 4) inner_rx = inner_rx_16qam;
    else if (rel15_ul->qam_mod_order == 6) inner_rx = inner_rx_64qam;
    else if (rel15_ul->qam_mod_order == 8) inner_rx = inner_rx_256qam;
    else    AssertFatal(1==0,"rel15_ul->qam_mod_order %d, pusch_pdu->dmrs_config_type %d\n",
  	   	        rel15_ul->qam_mod_order,rel15_ul->dmrs_config_type);

    int soffset   = (slot&3)*frame_parms->symbols_per_slot*frame_parms->ofdm_symbol_size;
    int32_t rxFext[nb_re_pusch] __attribute__((aligned(32)));
    int32_t chFext[nb_re_pusch] __attribute__((aligned(32)));
    int16_t llr16[nb_re_pusch*rel15_ul->qam_mod_order] __attribute__((aligned(32)));
    for (int aa=0;aa<frame_parms->nb_antennas_rx;aa++) {
      nr_ulsch_extract_rbs0(gNB->common_vars.rxdataF[aa],
                            gNB->pusch_vars[ulsch_id]->ul_ch_estimates[aa],
                            rxFext,
                            chFext,
  	    		    soffset+(symbol * frame_parms->ofdm_symbol_size),
	 		    gNB->pusch_vars[ulsch_id]->dmrs_symbol*frame_parms->ofdm_symbol_size,
			    aa,
                            dmrs_symbol_flag, 
                            rel15_ul,
                            frame_parms);
      inner_rx(rxFext,chFext,llr16,aa,nb_re_pusch,gNB->pusch_vars[ulsch_id]->log2_maxh);
    }
    // unscrambling
    simde__m64 *llr64 = (simde__m64 *) llr;
    for (int i=0;i<(nb_re_pusch*rel15_ul->qam_mod_order)>>2;i++) {
       llr64[i] = simde_mm_mullo_pi16(((simde__m64 *)llr16)[i],((simde__m64 *)s)[i]);
    }
    s+=(nb_re_pusch*rel15_ul->qam_mod_order);
    llr+=(nb_re_pusch*rel15_ul->qam_mod_order);
  }    

//  int64_t end = time_now_us(); 
//  printf("Elapsed time = %ld tstamp %ld  id %lu \n", end - now, end,  pthread_self());
}

/*
void nr_pusch_symbol_processing(void *arg) {
    puschSymbolProc_t *rdata=(puschSymbolProc_t*)arg;

    PHY_VARS_gNB *gNB=rdata->gNB;
    NR_DL_FRAME_PARMS *frame_parms=rdata->frame_parms;
    nfapi_nr_pusch_pdu_t *rel15_ul=rdata->rel15_ul;
    int ulsch_id=rdata->ulsch_id;
    int slot=rdata->slot;
    int symbol=rdata->symbol;
    int dmrs_symbol_flag = (rel15_ul->ul_dmrs_symb_pos >> symbol) & 0x01;
    int nb_re_pusch = gNB->pusch_vars[ulsch_id]->ul_valid_re_per_slot[symbol];
    if (dmrs_symbol_flag == 1) {
      if ((rel15_ul->ul_dmrs_symb_pos >> ((symbol + 1) % frame_parms->symbols_per_slot)) & 0x01)
        AssertFatal(1==0,"Double DMRS configuration is not yet supported\n");

      gNB->pusch_vars[ulsch_id]->dmrs_symbol = symbol;
    }

    LOG_D(PHY,"symbol %d: nb_re_pusch %d, DMRS symbl used for Chest :%d \n", symbol, nb_re_pusch, gNB->pusch_vars[ulsch_id]->dmrs_symbol);

#ifdef __AVX2__
    int off = ((rel15_ul->rb_size&1) == 1)? 4:0;
#else
    int off = 0;
#endif
    //----------------------------------------------------------
    //--------------------- RBs extraction ---------------------
    //----------------------------------------------------------
    if (nb_re_pusch > 0) {

      if (gNB->pusch_vars[ulsch_id]->extraction_done[symbol]!=1) {
        start_meas(&gNB->ulsch_rbs_extraction_stats);
        nr_ulsch_extract_rbs(gNB->common_vars.rxdataF,
                             gNB->pusch_vars[ulsch_id],
                             slot,
                             symbol,
                             dmrs_symbol_flag,
                             rel15_ul,
                             frame_parms);
        stop_meas(&gNB->ulsch_rbs_extraction_stats);
      }
      //----------------------------------------------------------
      //--------------------- Channel Scaling --------------------
      //----------------------------------------------------------
      nr_ulsch_scale_channel(gNB->pusch_vars[ulsch_id]->ul_ch_estimates_ext,
                            frame_parms,
                            gNB->ulsch[ulsch_id],
                            symbol,
                            dmrs_symbol_flag,
			                      nb_re_pusch,
			                      rel15_ul->nrOfLayers,
			                      rel15_ul->rb_size);


      //----------------------------------------------------------
      //--------------------- Channel Compensation ---------------
      //----------------------------------------------------------
      start_meas(&gNB->ulsch_channel_compensation_stats);
      LOG_D(PHY,"Doing channel compensations log2_maxh %d\n",gNB->pusch_vars[ulsch_id]->log2_maxh);
      nr_ulsch_channel_compensation(gNB->pusch_vars[ulsch_id]->rxdataF_ext,
                                    gNB->pusch_vars[ulsch_id]->ul_ch_estimates_ext,
                                    gNB->pusch_vars[ulsch_id]->ul_ch_mag0,
                                    gNB->pusch_vars[ulsch_id]->ul_ch_magb0,
                                    gNB->pusch_vars[ulsch_id]->rxdataF_comp,
                                    (rel15_ul->nrOfLayers>1) ? gNB->pusch_vars[ulsch_id]->rho : NULL,
                                    frame_parms,
                                    symbol,
                                    nb_re_pusch,
                                    dmrs_symbol_flag,
                                    rel15_ul->dmrs_config_type,
                                    rel15_ul->qam_mod_order,
                                    rel15_ul->nrOfLayers,
                                    rel15_ul->rb_size,
                                    gNB->pusch_vars[ulsch_id]->log2_maxh);
      stop_meas(&gNB->ulsch_channel_compensation_stats);

      start_meas(&gNB->ulsch_mrc_stats);
      nr_ulsch_detection_mrc(frame_parms,
			      gNB->pusch_vars[ulsch_id]->rxdataF_comp,
			      gNB->pusch_vars[ulsch_id]->ul_ch_mag0,
			      gNB->pusch_vars[ulsch_id]->ul_ch_magb0,
			      (rel15_ul->nrOfLayers>1) ? gNB->pusch_vars[ulsch_id]->rho : NULL,
			      rel15_ul->nrOfLayers,
			      symbol,
			      rel15_ul->rb_size,
			      nb_re_pusch);
      stop_meas(&gNB->ulsch_mrc_stats);

      // transform precoding = 0 means enabled
      if (rel15_ul->transform_precoding == 0) {

      #ifdef __AVX2__
        // For odd number of resource blocks need byte alignment to multiple of 8
        int nb_re_pusch2 = nb_re_pusch + (nb_re_pusch&7);
      #else
        int nb_re_pusch2 = nb_re_pusch;
      #endif

        // perform IDFT operation on the compensated rxdata if transform precoding is enabled
        nr_idft(&gNB->pusch_vars[ulsch_id]->rxdataF_comp[0][symbol * nb_re_pusch2], nb_re_pusch);
        LOG_D(PHY,"Transform precoding being done on data- symbol: %d, nb_re_pusch: %d\n", symbol, nb_re_pusch);
      }

      start_meas(&gNB->ulsch_llr_stats);
      nr_ulsch_compute_llr(&gNB->pusch_vars[ulsch_id]->rxdataF_comp[0][symbol * (off + rel15_ul->rb_size * NR_NB_SC_PER_RB)],
                           gNB->pusch_vars[ulsch_id]->ul_ch_mag0,
                           gNB->pusch_vars[ulsch_id]->ul_ch_magb0,
                           &gNB->pusch_vars[ulsch_id]->llr[gNB->pusch_vars[ulsch_id]->llr_offset[symbol]],
                           rel15_ul->rb_size,
                           gNB->pusch_vars[ulsch_id]->ul_valid_re_per_slot[symbol],
                           symbol,
                           rel15_ul->qam_mod_order);
      stop_meas(&gNB->ulsch_llr_stats);
      //----------------------------------------------------------
      //--------------------- PTRS Processing --------------------
      //----------------------------------------------------------
      // In case PTRS is enabled then LLR will be calculated after PTRS symbols are processed 
      //otherwise LLR are calculated for each symbol based upon DMRS channel estimates only. 
      if (rel15_ul->pdu_bit_map & PUSCH_PDU_BITMAP_PUSCH_PTRS) {
        start_meas(&gNB->ulsch_ptrs_processing_stats);
        nr_pusch_ptrs_processing(gNB,
                                 frame_parms,
                                 rel15_ul,
                                 ulsch_id,
                                 slot,
                                 symbol,
                                 nb_re_pusch);
        stop_meas(&gNB->ulsch_ptrs_processing_stats);

        //  Subtract total PTRS RE's in the symbol from PUSCH RE's 
        gNB->pusch_vars[ulsch_id]->ul_valid_re_per_slot[symbol] -= gNB->pusch_vars[ulsch_id]->ptrs_re_per_slot;
      }
    }
}
*/
/* Zero Forcing Rx function: nr_det_HhH()
 *
 *
 * */
void nr_ulsch_det_HhH(int32_t *after_mf_00,//a
                int32_t *after_mf_01,//b
                int32_t *after_mf_10,//c
                int32_t *after_mf_11,//d
                int32_t *det_fin,//1/ad-bc
                unsigned short nb_rb,
                unsigned char symbol,
                int32_t shift)
{
  int16_t nr_conjug2[8]__attribute__((aligned(16))) = {1,-1,1,-1,1,-1,1,-1} ;
  unsigned short rb;
  simde__m128i *after_mf_00_128,*after_mf_01_128, *after_mf_10_128, *after_mf_11_128, ad_re_128, bc_re_128; //ad_im_128, bc_im_128;
  simde__m128i *det_fin_128, det_re_128; //det_im_128, tmp_det0, tmp_det1;

  after_mf_00_128 = (simde__m128i *)after_mf_00;
  after_mf_01_128 = (simde__m128i *)after_mf_01;
  after_mf_10_128 = (simde__m128i *)after_mf_10;
  after_mf_11_128 = (simde__m128i *)after_mf_11;

  det_fin_128 = (simde__m128i *)det_fin;

  for (rb=0; rb<3*nb_rb; rb++) {

    //complex multiplication (I_a+jQ_a)(I_d+jQ_d) = (I_aI_d - Q_aQ_d) + j(Q_aI_d + I_aQ_d)
    //The imag part is often zero, we compute only the real part
    ad_re_128 = simde_mm_sign_epi16(after_mf_00_128[0],*(simde__m128i*)&nr_conjug2[0]);
    ad_re_128 = simde_mm_madd_epi16(ad_re_128,after_mf_11_128[0]); //Re: I_a0*I_d0 - Q_a1*Q_d1
    //ad_im_128 = simde_mm_shufflelo_epi16(after_mf_00_128[0],_MM_SHUFFLE(2,3,0,1));//permutes IQs for the low 64 bits as [I_a0 Q_a1 I_a2 Q_a3]_64bits to [Q_a1 I_a0 Q_a3 I_a2]_64bits
    //ad_im_128 = simde_mm_shufflehi_epi16(ad_im_128,_MM_SHUFFLE(2,3,0,1));//permutes IQs for the high 64 bits as [I_a0 Q_a1 I_a2 Q_a3]_64bits to [Q_a1 I_a0 Q_a3 I_a2]_64bits
    //ad_im_128 = simde_mm_madd_epi16(ad_im_128,after_mf_11_128[0]);//Im: (Q_aI_d + I_aQ_d)

    //complex multiplication (I_b+jQ_b)(I_c+jQ_c) = (I_bI_c - Q_bQ_c) + j(Q_bI_c + I_bQ_c)
    //The imag part is often zero, we compute only the real part
    bc_re_128 = simde_mm_sign_epi16(after_mf_01_128[0],*(simde__m128i*)&nr_conjug2[0]);
    bc_re_128 = simde_mm_madd_epi16(bc_re_128,after_mf_10_128[0]); //Re: I_b0*I_c0 - Q_b1*Q_c1
    //bc_im_128 = simde_mm_shufflelo_epi16(after_mf_01_128[0],_MM_SHUFFLE(2,3,0,1));//permutes IQs for the low 64 bits as [I_b0 Q_b1 I_b2 Q_b3]_64bits to [Q_b1 I_b0 Q_b3 I_b2]_64bits
    //bc_im_128 = simde_mm_shufflehi_epi16(bc_im_128,_MM_SHUFFLE(2,3,0,1));//permutes IQs for the high 64 bits as [I_b0 Q_b1 I_b2 Q_b3]_64bits to [Q_b1 I_b0 Q_b3 I_b2]_64bits
    //bc_im_128 = simde_mm_madd_epi16(bc_im_128,after_mf_10_128[0]);//Im: (Q_bI_c + I_bQ_c)

    det_re_128 = simde_mm_sub_epi32(ad_re_128, bc_re_128);
    //det_im_128 = simde_mm_sub_epi32(ad_im_128, bc_im_128);

    //det in Q30 format
    det_fin_128[0] = simde_mm_abs_epi32(det_re_128);


#ifdef DEBUG_DLSCH_DEMOD
     printf("\n Computing det_HhH_inv \n");
     //print_ints("det_re_128:",(int32_t*)&det_re_128);
     //print_ints("det_im_128:",(int32_t*)&det_im_128);
     print_ints("det_fin_128:",(int32_t*)&det_fin_128[0]);
#endif
    det_fin_128+=1;
    after_mf_00_128+=1;
    after_mf_01_128+=1;
    after_mf_10_128+=1;
    after_mf_11_128+=1;
  }
  simde_mm_empty();
  _m_empty();
}

/* Zero Forcing Rx function: nr_inv_comp_muli
 * Complex number multi: z = x*y
 *                         = (x_re*y_re - x_im*y_im) + j(x_im*y_re + x_re*y_im)
 * */
simde__m128i nr_ulsch_inv_comp_muli(simde__m128i input_x,
                         simde__m128i input_y)
{
  int16_t nr_conjug2[8]__attribute__((aligned(16))) = {1,-1,1,-1,1,-1,1,-1} ;

  simde__m128i xy_re_128, xy_im_128;
  simde__m128i output_z, tmp_z0, tmp_z1;

  // complex multiplication (x_re + jx_im)*(y_re + jy_im) = (x_re*y_re - x_im*y_im) + j(x_im*y_re + x_re*y_im)

  // the real part
  xy_re_128 = simde_mm_sign_epi16(input_x,*(simde__m128i*)&nr_conjug2[0]);
  xy_re_128 = simde_mm_madd_epi16(xy_re_128,input_y); //Re: (x_re*y_re - x_im*y_im)

  // the imag part
  xy_im_128 = simde_mm_shufflelo_epi16(input_x,_MM_SHUFFLE(2,3,0,1));//permutes IQs for the low 64 bits as [I_a0 Q_a1 I_a2 Q_a3]_64bits to [Q_a1 I_a0 Q_a3 I_a2]_64bits
  xy_im_128 = simde_mm_shufflehi_epi16(xy_im_128,_MM_SHUFFLE(2,3,0,1));//permutes IQs for the high 64 bits as [I_a0 Q_a1 I_a2 Q_a3]_64bits to [Q_a1 I_a0 Q_a3 I_a2]_64bits
  xy_im_128 = simde_mm_madd_epi16(xy_im_128,input_y);//Im: (x_im*y_re + x_re*y_im)

  //convert back to Q15 before packing
  xy_re_128 = simde_mm_srai_epi32(xy_re_128,4);//(2^15/64*2*16)
  xy_im_128 = simde_mm_srai_epi32(xy_im_128,4);

  tmp_z0  = simde_mm_unpacklo_epi32(xy_re_128,xy_im_128);
  //print_ints("unpack lo:",&tmp_z0[0]);
  tmp_z1  = simde_mm_unpackhi_epi32(xy_re_128,xy_im_128);
  //print_ints("unpack hi:",&tmp_z1[0]);
  output_z = simde_mm_packs_epi32(tmp_z0,tmp_z1);

  simde_mm_empty();
  _m_empty();
  return(output_z);
}

/* Zero Forcing Rx function: nr_conjch0_mult_ch1()
 *
 *
 * */
void nr_ulsch_conjch0_mult_ch1(int *ch0,
                         int *ch1,
                         int32_t *ch0conj_ch1,
                         unsigned short nb_rb,
                         unsigned char output_shift0)
{
  //This function is used to compute multiplications in H_hermitian * H matrix
  short nr_conjugate[8]__attribute__((aligned(16))) = {-1,1,-1,1,-1,1,-1,1};
  unsigned short rb;
  simde__m128i *dl_ch0_128,*dl_ch1_128, *ch0conj_ch1_128, mmtmpD0,mmtmpD1,mmtmpD2,mmtmpD3;

  dl_ch0_128 = (simde__m128i *)ch0;
  dl_ch1_128 = (simde__m128i *)ch1;

  ch0conj_ch1_128 = (simde__m128i *)ch0conj_ch1;

  for (rb=0; rb<3*nb_rb; rb++) {

    mmtmpD0 = simde_mm_madd_epi16(dl_ch0_128[0],dl_ch1_128[0]);
    mmtmpD1 = simde_mm_shufflelo_epi16(dl_ch0_128[0],_MM_SHUFFLE(2,3,0,1));
    mmtmpD1 = simde_mm_shufflehi_epi16(mmtmpD1,_MM_SHUFFLE(2,3,0,1));
    mmtmpD1 = simde_mm_sign_epi16(mmtmpD1,*(simde__m128i*)&nr_conjugate[0]);
    mmtmpD1 = simde_mm_madd_epi16(mmtmpD1,dl_ch1_128[0]);
    mmtmpD0 = simde_mm_srai_epi32(mmtmpD0,output_shift0);
    mmtmpD1 = simde_mm_srai_epi32(mmtmpD1,output_shift0);
    mmtmpD2 = simde_mm_unpacklo_epi32(mmtmpD0,mmtmpD1);
    mmtmpD3 = simde_mm_unpackhi_epi32(mmtmpD0,mmtmpD1);

    ch0conj_ch1_128[0] = simde_mm_packs_epi32(mmtmpD2,mmtmpD3);

    /*printf("\n Computing conjugates \n");
    print_shorts("ch0:",(int16_t*)&dl_ch0_128[0]);
    print_shorts("ch1:",(int16_t*)&dl_ch1_128[0]);
    print_shorts("pack:",(int16_t*)&ch0conj_ch1_128[0]);*/

    dl_ch0_128+=1;
    dl_ch1_128+=1;
    ch0conj_ch1_128+=1;
  }
  simde_mm_empty();
  _m_empty();
}
simde__m128i nr_ulsch_comp_muli_sum(simde__m128i input_x,
                         simde__m128i input_y,
                         simde__m128i input_w,
                         simde__m128i input_z,
                         simde__m128i det)
{
  int16_t nr_conjug2[8]__attribute__((aligned(16))) = {1,-1,1,-1,1,-1,1,-1} ;

  simde__m128i xy_re_128, xy_im_128, wz_re_128, wz_im_128;
  simde__m128i output, tmp_z0, tmp_z1;

  // complex multiplication (x_re + jx_im)*(y_re + jy_im) = (x_re*y_re - x_im*y_im) + j(x_im*y_re + x_re*y_im)
  // the real part
  xy_re_128 = simde_mm_sign_epi16(input_x,*(simde__m128i*)&nr_conjug2[0]);
  xy_re_128 = simde_mm_madd_epi16(xy_re_128,input_y); //Re: (x_re*y_re - x_im*y_im)

  // the imag part
  xy_im_128 = simde_mm_shufflelo_epi16(input_x,_MM_SHUFFLE(2,3,0,1));//permutes IQs for the low 64 bits as [I_a0 Q_a1 I_a2 Q_a3]_64bits to [Q_a1 I_a0 Q_a3 I_a2]_64bits
  xy_im_128 = simde_mm_shufflehi_epi16(xy_im_128,_MM_SHUFFLE(2,3,0,1));//permutes IQs for the high 64 bits as [I_a0 Q_a1 I_a2 Q_a3]_64bits to [Q_a1 I_a0 Q_a3 I_a2]_64bits
  xy_im_128 = simde_mm_madd_epi16(xy_im_128,input_y);//Im: (x_im*y_re + x_re*y_im)

  // complex multiplication (w_re + jw_im)*(z_re + jz_im) = (w_re*z_re - w_im*z_im) + j(w_im*z_re + w_re*z_im)
  // the real part
  wz_re_128 = simde_mm_sign_epi16(input_w,*(simde__m128i*)&nr_conjug2[0]);
  wz_re_128 = simde_mm_madd_epi16(wz_re_128,input_z); //Re: (w_re*z_re - w_im*z_im)

  // the imag part
  wz_im_128 = simde_mm_shufflelo_epi16(input_w,_MM_SHUFFLE(2,3,0,1));//permutes IQs for the low 64 bits as [I_a0 Q_a1 I_a2 Q_a3]_64bits to [Q_a1 I_a0 Q_a3 I_a2]_64bits
  wz_im_128 = simde_mm_shufflehi_epi16(wz_im_128,_MM_SHUFFLE(2,3,0,1));//permutes IQs for the high 64 bits as [I_a0 Q_a1 I_a2 Q_a3]_64bits to [Q_a1 I_a0 Q_a3 I_a2]_64bits
  wz_im_128 = simde_mm_madd_epi16(wz_im_128,input_z);//Im: (w_im*z_re + w_re*z_im)


  xy_re_128 = simde_mm_sub_epi32(xy_re_128, wz_re_128);
  xy_im_128 = simde_mm_sub_epi32(xy_im_128, wz_im_128);
  //print_ints("rx_re:",(int32_t*)&xy_re_128[0]);
  //print_ints("rx_Img:",(int32_t*)&xy_im_128[0]);
  //divide by matrix det and convert back to Q15 before packing
  int sum_det =0;
  for (int k=0; k<4;k++) {
    sum_det += ((((int *)&det[0])[k])>>2);
    //printf("det_%d = %d log2 =%d \n",k,(((int *)&det[0])[k]),log2_approx(((int *)&det[0])[k]));
    }

  xy_re_128 = simde_mm_slli_epi32(xy_re_128,5);
  xy_re_128 = simde_mm_srai_epi32(xy_re_128,log2_approx(sum_det));
  xy_re_128 = simde_mm_slli_epi32(xy_re_128,5);

  xy_im_128 = simde_mm_slli_epi32(xy_im_128,5);
  xy_im_128 = simde_mm_srai_epi32(xy_im_128,log2_approx(sum_det));
  xy_im_128 = simde_mm_slli_epi32(xy_im_128,5);

  tmp_z0  = simde_mm_unpacklo_epi32(xy_re_128,xy_im_128);
  //print_ints("unpack lo:",&tmp_z0[0]);
  tmp_z1  = simde_mm_unpackhi_epi32(xy_re_128,xy_im_128);
  //print_ints("unpack hi:",&tmp_z1[0]);
  output = simde_mm_packs_epi32(tmp_z0,tmp_z1);

  simde_mm_empty();
  _m_empty();
  return(output);
}
/* Zero Forcing Rx function: nr_construct_HhH_elements()
 *
 *
 * */
void nr_ulsch_construct_HhH_elements(int *conjch00_ch00,
                               int *conjch01_ch01,
                               int *conjch11_ch11,
                               int *conjch10_ch10,//
                               int *conjch20_ch20,
                               int *conjch21_ch21,
                               int *conjch30_ch30,
                               int *conjch31_ch31,
                               int *conjch00_ch01,//00_01
                               int *conjch01_ch00,//01_00
                               int *conjch10_ch11,//10_11
                               int *conjch11_ch10,//11_10
                               int *conjch20_ch21,
                               int *conjch21_ch20,
                               int *conjch30_ch31,
                               int *conjch31_ch30,
                               int32_t *after_mf_00,
                               int32_t *after_mf_01,
                               int32_t *after_mf_10,
                               int32_t *after_mf_11,
                               unsigned short nb_rb,
                               unsigned char symbol)
{
  //This function is used to construct the (H_hermitian * H matrix) matrix elements
  unsigned short rb;
  simde__m128i *conjch00_ch00_128, *conjch01_ch01_128, *conjch11_ch11_128, *conjch10_ch10_128;
  simde__m128i *conjch20_ch20_128, *conjch21_ch21_128, *conjch30_ch30_128, *conjch31_ch31_128;
  simde__m128i *conjch00_ch01_128, *conjch01_ch00_128, *conjch10_ch11_128, *conjch11_ch10_128;
  simde__m128i *conjch20_ch21_128, *conjch21_ch20_128, *conjch30_ch31_128, *conjch31_ch30_128;
  simde__m128i *after_mf_00_128, *after_mf_01_128, *after_mf_10_128, *after_mf_11_128;

  conjch00_ch00_128 = (simde__m128i *)conjch00_ch00;
  conjch01_ch01_128 = (simde__m128i *)conjch01_ch01;
  conjch11_ch11_128 = (simde__m128i *)conjch11_ch11;
  conjch10_ch10_128 = (simde__m128i *)conjch10_ch10;

  conjch20_ch20_128 = (simde__m128i *)conjch20_ch20;
  conjch21_ch21_128 = (simde__m128i *)conjch21_ch21;
  conjch30_ch30_128 = (simde__m128i *)conjch30_ch30;
  conjch31_ch31_128 = (simde__m128i *)conjch31_ch31;

  conjch00_ch01_128 = (simde__m128i *)conjch00_ch01;
  conjch01_ch00_128 = (simde__m128i *)conjch01_ch00;
  conjch10_ch11_128 = (simde__m128i *)conjch10_ch11;
  conjch11_ch10_128 = (simde__m128i *)conjch11_ch10;

  conjch20_ch21_128 = (simde__m128i *)conjch20_ch21;
  conjch21_ch20_128 = (simde__m128i *)conjch21_ch20;
  conjch30_ch31_128 = (simde__m128i *)conjch30_ch31;
  conjch31_ch30_128 = (simde__m128i *)conjch31_ch30;

  after_mf_00_128 = (simde__m128i *)after_mf_00;
  after_mf_01_128 = (simde__m128i *)after_mf_01;
  after_mf_10_128 = (simde__m128i *)after_mf_10;
  after_mf_11_128 = (simde__m128i *)after_mf_11;

  for (rb=0; rb<3*nb_rb; rb++) {

    after_mf_00_128[0] =simde_mm_adds_epi16(conjch00_ch00_128[0],conjch10_ch10_128[0]);//00_00 + 10_10
    if (conjch20_ch20 != NULL) after_mf_00_128[0] =simde_mm_adds_epi16(after_mf_00_128[0],conjch20_ch20_128[0]);
    if (conjch30_ch30 != NULL) after_mf_00_128[0] =simde_mm_adds_epi16(after_mf_00_128[0],conjch30_ch30_128[0]);

    after_mf_11_128[0] =simde_mm_adds_epi16(conjch01_ch01_128[0], conjch11_ch11_128[0]); //01_01 + 11_11
    if (conjch21_ch21 != NULL) after_mf_11_128[0] =simde_mm_adds_epi16(after_mf_11_128[0],conjch21_ch21_128[0]);
    if (conjch31_ch31 != NULL) after_mf_11_128[0] =simde_mm_adds_epi16(after_mf_11_128[0],conjch31_ch31_128[0]);

    after_mf_01_128[0] =simde_mm_adds_epi16(conjch00_ch01_128[0], conjch10_ch11_128[0]);//00_01 + 10_11
    if (conjch20_ch21 != NULL) after_mf_01_128[0] =simde_mm_adds_epi16(after_mf_01_128[0],conjch20_ch21_128[0]);
    if (conjch30_ch31 != NULL) after_mf_01_128[0] =simde_mm_adds_epi16(after_mf_01_128[0],conjch30_ch31_128[0]);

    after_mf_10_128[0] =simde_mm_adds_epi16(conjch01_ch00_128[0], conjch11_ch10_128[0]);//01_00 + 11_10
    if (conjch21_ch20 != NULL) after_mf_10_128[0] =simde_mm_adds_epi16(after_mf_10_128[0],conjch21_ch20_128[0]);
    if (conjch31_ch30 != NULL) after_mf_10_128[0] =simde_mm_adds_epi16(after_mf_10_128[0],conjch31_ch30_128[0]);

#ifdef DEBUG_DLSCH_DEMOD
    if ((rb<=30))
    {
      printf(" \n construct_HhH_elements \n");
      print_shorts("after_mf_00_128:",(int16_t*)&after_mf_00_128[0]);
      print_shorts("after_mf_01_128:",(int16_t*)&after_mf_01_128[0]);
      print_shorts("after_mf_10_128:",(int16_t*)&after_mf_10_128[0]);
      print_shorts("after_mf_11_128:",(int16_t*)&after_mf_11_128[0]);
    }
#endif
    conjch00_ch00_128+=1;
    conjch10_ch10_128+=1;
    conjch01_ch01_128+=1;
    conjch11_ch11_128+=1;

    if (conjch20_ch20 != NULL) conjch20_ch20_128+=1;
    if (conjch21_ch21 != NULL) conjch21_ch21_128+=1;
    if (conjch30_ch30 != NULL) conjch30_ch30_128+=1;
    if (conjch31_ch31 != NULL) conjch31_ch31_128+=1;

    conjch00_ch01_128+=1;
    conjch01_ch00_128+=1;
    conjch10_ch11_128+=1;
    conjch11_ch10_128+=1;

    if (conjch20_ch21 != NULL) conjch20_ch21_128+=1;
    if (conjch21_ch20 != NULL) conjch21_ch20_128+=1;
    if (conjch30_ch31 != NULL) conjch30_ch31_128+=1;
    if (conjch31_ch30 != NULL) conjch31_ch30_128+=1;

    after_mf_00_128 += 1;
    after_mf_01_128 += 1;
    after_mf_10_128 += 1;
    after_mf_11_128 += 1;
  }
  simde_mm_empty();
  _m_empty();
}

/* Zero Forcing Rx function: nr_ulsch_zero_forcing_rx_2layers()
 *
 *
 * */
uint8_t nr_ulsch_zero_forcing_rx_2layers(int **rxdataF_comp,
                                   int **ul_ch_mag,
                                   int **ul_ch_magb,                                   
                                   int **ul_ch_estimates_ext,
                                   unsigned short nb_rb,
                                   unsigned char n_rx,
                                   unsigned char mod_order,
                                   int shift,
                                   unsigned char symbol,
                                   int length)
{
  int *ch00, *ch01, *ch10, *ch11;
  int *ch20, *ch30, *ch21, *ch31;
  uint32_t nb_rb_0 = length/12 + ((length%12)?1:0);

  int off = ((nb_rb&1) == 1)? 4:0;

  /* we need at least alignment to 16 bytes, let's put 32 to be sure
   * (maybe not necessary but doesn't hurt)
   */
  int32_t conjch00_ch01[12*nb_rb] __attribute__((aligned(32)));
  int32_t conjch01_ch00[12*nb_rb] __attribute__((aligned(32)));
  int32_t conjch10_ch11[12*nb_rb] __attribute__((aligned(32)));
  int32_t conjch11_ch10[12*nb_rb] __attribute__((aligned(32)));
  int32_t conjch00_ch00[12*nb_rb] __attribute__((aligned(32)));
  int32_t conjch01_ch01[12*nb_rb] __attribute__((aligned(32)));
  int32_t conjch10_ch10[12*nb_rb] __attribute__((aligned(32)));
  int32_t conjch11_ch11[12*nb_rb] __attribute__((aligned(32)));
  int32_t conjch20_ch20[12*nb_rb] __attribute__((aligned(32)));
  int32_t conjch21_ch21[12*nb_rb] __attribute__((aligned(32)));
  int32_t conjch30_ch30[12*nb_rb] __attribute__((aligned(32)));
  int32_t conjch31_ch31[12*nb_rb] __attribute__((aligned(32)));
  int32_t conjch20_ch21[12*nb_rb] __attribute__((aligned(32)));
  int32_t conjch30_ch31[12*nb_rb] __attribute__((aligned(32)));
  int32_t conjch21_ch20[12*nb_rb] __attribute__((aligned(32)));
  int32_t conjch31_ch30[12*nb_rb] __attribute__((aligned(32)));

  int32_t af_mf_00[12*nb_rb] __attribute__((aligned(32)));
  int32_t af_mf_01[12*nb_rb] __attribute__((aligned(32)));
  int32_t af_mf_10[12*nb_rb] __attribute__((aligned(32)));
  int32_t af_mf_11[12*nb_rb] __attribute__((aligned(32)));
  int32_t determ_fin[12*nb_rb] __attribute__((aligned(32)));

  switch (n_rx) {
    case 2://
      ch00 = (int *)&ul_ch_estimates_ext[0][symbol*(off+nb_rb*12)];
      ch01 = (int *)&ul_ch_estimates_ext[2][symbol*(off+nb_rb*12)];
      ch10 = (int *)&ul_ch_estimates_ext[1][symbol*(off+nb_rb*12)];
      ch11 = (int *)&ul_ch_estimates_ext[3][symbol*(off+nb_rb*12)];
      ch20 = NULL;
      ch21 = NULL;
      ch30 = NULL;
      ch31 = NULL;
      break;

    case 4://
      ch00 = (int *)&ul_ch_estimates_ext[0][symbol*(off+nb_rb*12)];
      ch01 = (int *)&ul_ch_estimates_ext[4][symbol*(off+nb_rb*12)];
      ch10 = (int *)&ul_ch_estimates_ext[1][symbol*(off+nb_rb*12)];
      ch11 = (int *)&ul_ch_estimates_ext[5][symbol*(off+nb_rb*12)];
      ch20 = (int *)&ul_ch_estimates_ext[2][symbol*(off+nb_rb*12)];
      ch21 = (int *)&ul_ch_estimates_ext[6][symbol*(off+nb_rb*12)];
      ch30 = (int *)&ul_ch_estimates_ext[3][symbol*(off+nb_rb*12)];
      ch31 = (int *)&ul_ch_estimates_ext[7][symbol*(off+nb_rb*12)];
      break;

    default:
      return -1;
      break;
  }

  /* 1- Compute the rx channel matrix after compensation: (1/2^log2_max)x(H_herm x H)
   * for n_rx = 2
   * |conj_H_00       conj_H_10|    | H_00         H_01|   |(conj_H_00xH_00+conj_H_10xH_10)   (conj_H_00xH_01+conj_H_10xH_11)|
   * |                         |  x |                  | = |                                                                 |
   * |conj_H_01       conj_H_11|    | H_10         H_11|   |(conj_H_01xH_00+conj_H_11xH_10)   (conj_H_01xH_01+conj_H_11xH_11)|
   *
   */

  if (n_rx>=2){
    // (1/2^log2_maxh)*conj_H_00xH_00: (1/(64*2))conjH_00*H_00*2^15
    nr_ulsch_conjch0_mult_ch1(ch00,
                        ch00,
                        conjch00_ch00,
                        nb_rb_0,
                        shift);
    // (1/2^log2_maxh)*conj_H_10xH_10: (1/(64*2))conjH_10*H_10*2^15
    nr_ulsch_conjch0_mult_ch1(ch10,
                        ch10,
                        conjch10_ch10,
                        nb_rb_0,
                        shift);
    // conj_H_00xH_01
    nr_ulsch_conjch0_mult_ch1(ch00,
                        ch01,
                        conjch00_ch01,
                        nb_rb_0,
                        shift); // this shift is equal to the channel level log2_maxh
    // conj_H_10xH_11
    nr_ulsch_conjch0_mult_ch1(ch10,
                        ch11,
                        conjch10_ch11,
                        nb_rb_0,
                        shift);
    // conj_H_01xH_01
    nr_ulsch_conjch0_mult_ch1(ch01,
                        ch01,
                        conjch01_ch01,
                        nb_rb_0,
                        shift);
    // conj_H_11xH_11
    nr_ulsch_conjch0_mult_ch1(ch11,
                        ch11,
                        conjch11_ch11,
                        nb_rb_0,
                        shift);
    // conj_H_01xH_00
    nr_ulsch_conjch0_mult_ch1(ch01,
                        ch00,
                        conjch01_ch00,
                        nb_rb_0,
                        shift);
    // conj_H_11xH_10
    nr_ulsch_conjch0_mult_ch1(ch11,
                        ch10,
                        conjch11_ch10,
                        nb_rb_0,
                        shift);
  }
  if (n_rx==4){
    // (1/2^log2_maxh)*conj_H_20xH_20: (1/(64*2*16))conjH_20*H_20*2^15
    nr_ulsch_conjch0_mult_ch1(ch20,
                        ch20,
                        conjch20_ch20,
                        nb_rb_0,
                        shift);

    // (1/2^log2_maxh)*conj_H_30xH_30: (1/(64*2*4))conjH_30*H_30*2^15
    nr_ulsch_conjch0_mult_ch1(ch30,
                        ch30,
                        conjch30_ch30,
                        nb_rb_0,
                        shift);

    // (1/2^log2_maxh)*conj_H_20xH_20: (1/(64*2))conjH_20*H_20*2^15
    nr_ulsch_conjch0_mult_ch1(ch20,
                        ch21,
                        conjch20_ch21,
                        nb_rb_0,
                        shift);

    nr_ulsch_conjch0_mult_ch1(ch30,
                        ch31,
                        conjch30_ch31,
                        nb_rb_0,
                        shift);

    nr_ulsch_conjch0_mult_ch1(ch21,
                        ch21,
                        conjch21_ch21,
                        nb_rb_0,
                        shift);

    nr_ulsch_conjch0_mult_ch1(ch31,
                        ch31,
                        conjch31_ch31,
                        nb_rb_0,
                        shift);

    // (1/2^log2_maxh)*conj_H_20xH_20: (1/(64*2))conjH_20*H_20*2^15
    nr_ulsch_conjch0_mult_ch1(ch21,
                        ch20,
                        conjch21_ch20,
                        nb_rb_0,
                        shift);

    nr_ulsch_conjch0_mult_ch1(ch31,
                        ch30,
                        conjch31_ch30,
                        nb_rb_0,
                        shift);

    nr_ulsch_construct_HhH_elements(conjch00_ch00,
                              conjch01_ch01,
                              conjch11_ch11,
                              conjch10_ch10,//
                              conjch20_ch20,
                              conjch21_ch21,
                              conjch30_ch30,
                              conjch31_ch31,
                              conjch00_ch01,
                              conjch01_ch00,
                              conjch10_ch11,
                              conjch11_ch10,//
                              conjch20_ch21,
                              conjch21_ch20,
                              conjch30_ch31,
                              conjch31_ch30,
                              af_mf_00,
                              af_mf_01,
                              af_mf_10,
                              af_mf_11,
                              nb_rb_0,
                              symbol);
  }
  if (n_rx==2){
    nr_ulsch_construct_HhH_elements(conjch00_ch00,
                              conjch01_ch01,
                              conjch11_ch11,
                              conjch10_ch10,//
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              conjch00_ch01,
                              conjch01_ch00,
                              conjch10_ch11,
                              conjch11_ch10,//
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              af_mf_00,
                              af_mf_01,
                              af_mf_10,
                              af_mf_11,
                              nb_rb_0,
                              symbol);
  }
  //det_HhH = ad -bc
  nr_ulsch_det_HhH(af_mf_00,//a
             af_mf_01,//b
             af_mf_10,//c
             af_mf_11,//d
             determ_fin,
             nb_rb_0,
             symbol,
             shift);
  /* 2- Compute the channel matrix inversion **********************************
   *
     *    |(conj_H_00xH_00+conj_H_10xH_10)   (conj_H_00xH_01+conj_H_10xH_11)|
     * A= |                                                                 |
     *    |(conj_H_01xH_00+conj_H_11xH_10)   (conj_H_01xH_01+conj_H_11xH_11)|
     *
     *
     *
     *inv(A) =(1/det)*[d  -b
     *                 -c  a]
     *
     *
     **************************************************************************/
  simde__m128i *rxdataF_comp128_0,*rxdataF_comp128_1,*ul_ch_mag128_0=NULL,*ul_ch_mag128b_0=NULL,*determ_fin_128;//*dl_ch_mag128_1,*dl_ch_mag128b_1,*dl_ch_mag128r_1
  simde__m128i mmtmpD0,mmtmpD1,mmtmpD2,mmtmpD3;
  simde__m128i *after_mf_a_128,*after_mf_b_128, *after_mf_c_128, *after_mf_d_128;
  simde__m128i QAM_amp128={0},QAM_amp128b={0};

  determ_fin_128      = (simde__m128i *)&determ_fin[0];

  rxdataF_comp128_0   = (simde__m128i *)&rxdataF_comp[0][symbol*(off+nb_rb*12)];//aatx=0 @ aarx =0
  rxdataF_comp128_1   = (simde__m128i *)&rxdataF_comp[n_rx][symbol*(off+nb_rb*12)];//aatx=1 @ aarx =0

  after_mf_a_128 = (simde__m128i *)af_mf_00;
  after_mf_b_128 = (simde__m128i *)af_mf_01;
  after_mf_c_128 = (simde__m128i *)af_mf_10;
  after_mf_d_128 = (simde__m128i *)af_mf_11;

  if (mod_order>2) {
    if (mod_order == 4) {
      QAM_amp128 = simde_mm_set1_epi16(QAM16_n1);  //2/sqrt(10)
      QAM_amp128b = simde_mm_setzero_si128();
    } else if (mod_order == 6) {
      QAM_amp128  = simde_mm_set1_epi16(QAM64_n1); //4/sqrt{42}
      QAM_amp128b = simde_mm_set1_epi16(QAM64_n2); //2/sqrt{42}
    } 
    ul_ch_mag128_0      = (simde__m128i *)&ul_ch_mag[0][symbol*(off+nb_rb*12)];
    ul_ch_mag128b_0     = (simde__m128i *)&ul_ch_magb[0][symbol*(off+nb_rb*12)];
  }

  for (int rb=0; rb<3*nb_rb_0; rb++) {
    if (mod_order>2) {
      int sum_det =0;
      for (int k=0; k<4;k++) {
        sum_det += ((((int *)&determ_fin_128[0])[k])>>2);
        //printf("det_%d = %d\n",k,sum_det);
        }

      mmtmpD2 = simde_mm_slli_epi32(determ_fin_128[0],5);
      mmtmpD2 = simde_mm_srai_epi32(mmtmpD2,log2_approx(sum_det));
      mmtmpD2 = simde_mm_slli_epi32(mmtmpD2,5);

      mmtmpD3 = simde_mm_unpacklo_epi32(mmtmpD2,mmtmpD2);

      mmtmpD2 = simde_mm_unpackhi_epi32(mmtmpD2,mmtmpD2);

      mmtmpD2 = simde_mm_packs_epi32(mmtmpD3,mmtmpD2);

      ul_ch_mag128_0[0] = mmtmpD2;
      ul_ch_mag128b_0[0] = mmtmpD2;

      ul_ch_mag128_0[0] = simde_mm_mulhi_epi16(ul_ch_mag128_0[0],QAM_amp128);
      ul_ch_mag128_0[0] = simde_mm_slli_epi16(ul_ch_mag128_0[0],1);

      ul_ch_mag128b_0[0] = simde_mm_mulhi_epi16(ul_ch_mag128b_0[0],QAM_amp128b);
      ul_ch_mag128b_0[0] = simde_mm_slli_epi16(ul_ch_mag128b_0[0],1);

      //print_shorts("mag layer 1:",(int16_t*)&dl_ch_mag128_0[0]);
      //print_shorts("mag layer 2:",(int16_t*)&dl_ch_mag128_1[0]);
      //print_shorts("magb layer 1:",(int16_t*)&dl_ch_mag128b_0[0]);
      //print_shorts("magb layer 2:",(int16_t*)&dl_ch_mag128b_1[0]);
      //print_shorts("magr layer 1:",(int16_t*)&dl_ch_mag128r_0[0]);
      //print_shorts("magr layer 2:",(int16_t*)&dl_ch_mag128r_1[0]);
    }
    // multiply by channel Inv
    //rxdataF_zf128_0 = rxdataF_comp128_0*d - b*rxdataF_comp128_1
    //rxdataF_zf128_1 = rxdataF_comp128_1*a - c*rxdataF_comp128_0
    //printf("layer_1 \n");
    mmtmpD0 = nr_ulsch_comp_muli_sum(rxdataF_comp128_0[0],
                               after_mf_d_128[0],
                               rxdataF_comp128_1[0],
                               after_mf_b_128[0],
                               determ_fin_128[0]);

    //printf("layer_2 \n");
    mmtmpD1 = nr_ulsch_comp_muli_sum(rxdataF_comp128_1[0],
                               after_mf_a_128[0],
                               rxdataF_comp128_0[0],
                               after_mf_c_128[0],
                               determ_fin_128[0]);

    rxdataF_comp128_0[0] = mmtmpD0;
    if (mod_order > 2) {
      // We need to check why it is a shift of 3
      rxdataF_comp128_1[0] = simde_mm_srai_epi16(mmtmpD1, 3);
    } else {
      rxdataF_comp128_1[0] = mmtmpD1;
    }

#ifdef DEBUG_DLSCH_DEMOD
    printf("\n Rx signal after ZF l%d rb%d\n",symbol,rb);
    print_shorts(" Rx layer 1:",(int16_t*)&rxdataF_comp128_0[0]);
    print_shorts(" Rx layer 2:",(int16_t*)&rxdataF_comp128_1[0]);
#endif
    determ_fin_128 += 1;
    ul_ch_mag128_0 += 1;
    ul_ch_mag128b_0 += 1;    
    rxdataF_comp128_0 += 1;
    rxdataF_comp128_1 += 1;
    after_mf_a_128 += 1;
    after_mf_b_128 += 1;
    after_mf_c_128 += 1;
    after_mf_d_128 += 1;
  }
  simde_mm_empty();
  _m_empty();
   return(0);
}

/* Main Function */
int nr_rx_pusch(PHY_VARS_gNB *gNB,
		 uint8_t ulsch_id,
		 uint32_t frame,
		 uint8_t slot,
		 unsigned char harq_pid)
 {

   uint8_t aarx, aatx;
   uint32_t nb_re_pusch, bwp_start_subcarrier;
   int avgs = 0;

   NR_DL_FRAME_PARMS *frame_parms = &gNB->frame_parms;
   nfapi_nr_pusch_pdu_t *rel15_ul = &gNB->ulsch[ulsch_id]->harq_processes[harq_pid]->ulsch_pdu;
   int avg[frame_parms->nb_antennas_rx*rel15_ul->nrOfLayers];

   gNB->pusch_vars[ulsch_id]->dmrs_symbol = INVALID_VALUE;
   gNB->pusch_vars[ulsch_id]->cl_done = 0;

   bwp_start_subcarrier = ((rel15_ul->rb_start + rel15_ul->bwp_start)*NR_NB_SC_PER_RB + frame_parms->first_carrier_offset) % frame_parms->ofdm_symbol_size;
   LOG_D(PHY,"pusch %d.%d : bwp_start_subcarrier %d, rb_start %d, first_carrier_offset %d\n", frame,slot,bwp_start_subcarrier, rel15_ul->rb_start, frame_parms->first_carrier_offset);
   LOG_D(PHY,"pusch %d.%d : ul_dmrs_symb_pos %x\n",frame,slot,rel15_ul->ul_dmrs_symb_pos);
   LOG_D(PHY,"ulsch RX %x : start_rb %d nb_rb %d mcs %d Nl %d Tpmi %d bwp_start %d start_sc %d start_symbol %d num_symbols %d cdmgrpsnodata %d num_dmrs %d dmrs_ports %d\n",
	 rel15_ul->rnti,rel15_ul->rb_start,rel15_ul->rb_size,rel15_ul->mcs_index,
	 rel15_ul->nrOfLayers,0,rel15_ul->bwp_start,0,rel15_ul->start_symbol_index,rel15_ul->nr_of_symbols,
	 rel15_ul->num_dmrs_cdm_grps_no_data,rel15_ul->ul_dmrs_symb_pos,rel15_ul->dmrs_ports);
   //----------------------------------------------------------
   //--------------------- Channel estimation ---------------------
   //----------------------------------------------------------
   start_meas(&gNB->ulsch_channel_estimation_stats);


   for(uint8_t symbol = rel15_ul->start_symbol_index; symbol < (rel15_ul->start_symbol_index + rel15_ul->nr_of_symbols); symbol++) {
     uint8_t dmrs_symbol_flag = (rel15_ul->ul_dmrs_symb_pos >> symbol) & 0x01;
     LOG_D(PHY, "symbol %d, dmrs_symbol_flag :%d\n", symbol, dmrs_symbol_flag);
     if (dmrs_symbol_flag == 1) {
       if (gNB->pusch_vars[ulsch_id]->dmrs_symbol == INVALID_VALUE)
         gNB->pusch_vars[ulsch_id]->dmrs_symbol = symbol;

       for (int nl=0; nl<rel15_ul->nrOfLayers; nl++) {

         nr_pusch_channel_estimation(gNB,
             slot,
             get_dmrs_port(nl,rel15_ul->dmrs_ports),
             symbol,
             ulsch_id,
             bwp_start_subcarrier,
             rel15_ul);
       }

       nr_gnb_measurements(gNB, ulsch_id, harq_pid, symbol,rel15_ul->nrOfLayers);
      

       for (aarx = 0; aarx < frame_parms->nb_antennas_rx; aarx++) {
         if (symbol == rel15_ul->start_symbol_index) {
           gNB->pusch_vars[ulsch_id]->ulsch_power[aarx] = 0;
           gNB->pusch_vars[ulsch_id]->ulsch_noise_power[aarx] = 0;
         }
         for (aatx = 0; aatx < rel15_ul->nrOfLayers; aatx++) {
           gNB->pusch_vars[ulsch_id]->ulsch_power[aarx] += signal_energy_nodc(
               &gNB->pusch_vars[ulsch_id]->ul_ch_estimates[aatx*gNB->frame_parms.nb_antennas_rx+aarx][symbol * frame_parms->ofdm_symbol_size],
               rel15_ul->rb_size * 12);
         }
         for (int rb = 0; rb < rel15_ul->rb_size; rb++) {
           gNB->pusch_vars[ulsch_id]->ulsch_noise_power[aarx] +=
             gNB->measurements.n0_subband_power[aarx][rel15_ul->bwp_start + rel15_ul->rb_start + rb] /
             rel15_ul->rb_size;
         }
         LOG_D(PHY,"aa %d, bwp_start%d, rb_start %d, rb_size %d: ulsch_power %d, ulsch_noise_power %d\n",aarx,
             rel15_ul->bwp_start,rel15_ul->rb_start,rel15_ul->rb_size,
             gNB->pusch_vars[ulsch_id]->ulsch_power[aarx],
             gNB->pusch_vars[ulsch_id]->ulsch_noise_power[aarx]);
       }
     }
   }
   stop_meas(&gNB->ulsch_channel_estimation_stats);

   int off = ((rel15_ul->rb_size&1) == 1)? 4:0;
   uint32_t rxdataF_ext_offset = 0;

   for(uint8_t symbol = rel15_ul->start_symbol_index; symbol < (rel15_ul->start_symbol_index + rel15_ul->nr_of_symbols); symbol++) {
     uint8_t dmrs_symbol_flag = (rel15_ul->ul_dmrs_symb_pos >> symbol) & 0x01;
     if (dmrs_symbol_flag == 1) {
       if ((rel15_ul->ul_dmrs_symb_pos >> ((symbol + 1) % frame_parms->symbols_per_slot)) & 0x01)
	 AssertFatal(1==0,"Double DMRS configuration is not yet supported\n");

       gNB->pusch_vars[ulsch_id]->dmrs_symbol = symbol;

       if (rel15_ul->dmrs_config_type == 0) {
	 // if no data in dmrs cdm group is 1 only even REs have no data
	 // if no data in dmrs cdm group is 2 both odd and even REs have no data
	 nb_re_pusch = rel15_ul->rb_size *(12 - (rel15_ul->num_dmrs_cdm_grps_no_data*6));
       }
       else {
	 nb_re_pusch = rel15_ul->rb_size *(12 - (rel15_ul->num_dmrs_cdm_grps_no_data*4));
       }
     }
     else {
       nb_re_pusch = rel15_ul->rb_size * NR_NB_SC_PER_RB;
     }

     gNB->pusch_vars[ulsch_id]->ul_valid_re_per_slot[symbol] = nb_re_pusch;
     LOG_D(PHY,"symbol %d: nb_re_pusch %d, DMRS symbl used for Chest :%d \n", symbol, nb_re_pusch, gNB->pusch_vars[ulsch_id]->dmrs_symbol);

     //----------------------------------------------------------
     //--------------------- RBs extraction ---------------------
     //----------------------------------------------------------
     if (nb_re_pusch > 0) {
       start_meas(&gNB->ulsch_rbs_extraction_stats);
       nr_ulsch_extract_rbs(gNB->common_vars.rxdataF,
			    gNB->pusch_vars[ulsch_id],
			    slot,
			    symbol,
			    dmrs_symbol_flag,
			    rel15_ul,
			    frame_parms);
       stop_meas(&gNB->ulsch_rbs_extraction_stats);

       //----------------------------------------------------------
       //--------------------- Channel Scaling --------------------
       //----------------------------------------------------------
       nr_ulsch_scale_channel(gNB->pusch_vars[ulsch_id]->ul_ch_estimates_ext,
			      frame_parms,
			      gNB->ulsch[ulsch_id],
			      symbol,
			      dmrs_symbol_flag,
			      nb_re_pusch,
			      rel15_ul->nrOfLayers,
			      rel15_ul->rb_size);

       if (gNB->pusch_vars[ulsch_id]->cl_done==0) {
	 nr_ulsch_channel_level(gNB->pusch_vars[ulsch_id]->ul_ch_estimates_ext,
				frame_parms,
				avg,
				symbol,
				nb_re_pusch,
				rel15_ul->nrOfLayers,
				rel15_ul->rb_size);
	 avgs = 0;

	 for (aatx=0;aatx<rel15_ul->nrOfLayers;aatx++)
	   for (aarx=0;aarx<frame_parms->nb_antennas_rx;aarx++)
	     avgs = cmax(avgs,avg[aatx*frame_parms->nb_antennas_rx+aarx]);

	 gNB->pusch_vars[ulsch_id]->log2_maxh = (log2_approx(avgs)/2)+2;
	 gNB->pusch_vars[ulsch_id]->cl_done = 1;
       }

       //----------------------------------------------------------
       //--------------------- Channel Compensation ---------------
       //----------------------------------------------------------
       start_meas(&gNB->ulsch_channel_compensation_stats);
       LOG_D(PHY,"Doing channel compensations log2_maxh %d, avgs %d (%d,%d)\n",gNB->pusch_vars[ulsch_id]->log2_maxh,avgs,avg[0],avg[1]);
       nr_ulsch_channel_compensation(gNB->pusch_vars[ulsch_id]->rxdataF_ext,
				     gNB->pusch_vars[ulsch_id]->ul_ch_estimates_ext,
				     gNB->pusch_vars[ulsch_id]->ul_ch_mag0,
				     gNB->pusch_vars[ulsch_id]->ul_ch_magb0,
				     gNB->pusch_vars[ulsch_id]->ul_ch_magc0,
				     gNB->pusch_vars[ulsch_id]->rxdataF_comp,
				     (rel15_ul->nrOfLayers>1) ? gNB->pusch_vars[ulsch_id]->rho : NULL,
				     frame_parms,
				     symbol,
				     nb_re_pusch,
				     dmrs_symbol_flag,
             rel15_ul->dmrs_config_type,
				     rel15_ul->qam_mod_order,
				     rel15_ul->nrOfLayers,
				     rel15_ul->rb_size,
				     gNB->pusch_vars[ulsch_id]->log2_maxh);
       stop_meas(&gNB->ulsch_channel_compensation_stats);

       start_meas(&gNB->ulsch_mrc_stats);
       nr_ulsch_detection_mrc(frame_parms,
			      gNB->pusch_vars[ulsch_id]->rxdataF_comp,
			      gNB->pusch_vars[ulsch_id]->ul_ch_mag0,
			      gNB->pusch_vars[ulsch_id]->ul_ch_magb0,
			      gNB->pusch_vars[ulsch_id]->ul_ch_magc0,
			      (rel15_ul->nrOfLayers>1) ? gNB->pusch_vars[ulsch_id]->rho : NULL,
			      rel15_ul->nrOfLayers,
			      symbol,
			      rel15_ul->rb_size,
			      nb_re_pusch);

       if (rel15_ul->nrOfLayers == 2)//Apply zero forcing for 2 Tx layers
	 nr_ulsch_zero_forcing_rx_2layers(gNB->pusch_vars[ulsch_id]->rxdataF_comp,
					  gNB->pusch_vars[ulsch_id]->ul_ch_mag0,
					  gNB->pusch_vars[ulsch_id]->ul_ch_magb0,
					  gNB->pusch_vars[ulsch_id]->ul_ch_estimates_ext,
					  rel15_ul->rb_size,
					  frame_parms->nb_antennas_rx,
					  rel15_ul->qam_mod_order,
					  gNB->pusch_vars[ulsch_id]->log2_maxh,
					  symbol,
					  nb_re_pusch);
       stop_meas(&gNB->ulsch_mrc_stats);

       if (rel15_ul->transform_precoding == transformPrecoder_enabled) {
	 // For odd number of resource blocks need byte alignment to multiple of 8
	 int nb_re_pusch2 = nb_re_pusch + (nb_re_pusch&7);

	 // perform IDFT operation on the compensated rxdata if transform precoding is enabled
	 nr_idft(&gNB->pusch_vars[ulsch_id]->rxdataF_comp[0][symbol * nb_re_pusch2], nb_re_pusch);
	 LOG_D(PHY,"Transform precoding being done on data- symbol: %d, nb_re_pusch: %d\n", symbol, nb_re_pusch);
       }
       //----------------------------------------------------------
       //--------------------- PTRS Processing --------------------
       //----------------------------------------------------------
       /* In case PTRS is enabled then LLR will be calculated after PTRS symbols are processed *
	* otherwise LLR are calculated for each symbol based upon DMRS channel estimates only. */
       if (rel15_ul->pdu_bit_map & PUSCH_PDU_BITMAP_PUSCH_PTRS) {
	 start_meas(&gNB->ulsch_ptrs_processing_stats);
	 nr_pusch_ptrs_processing(gNB,
				  frame_parms,
				  rel15_ul,
				  ulsch_id,
				  slot,
				  symbol,
				  nb_re_pusch);
	 stop_meas(&gNB->ulsch_ptrs_processing_stats);

	 /*  Subtract total PTRS RE's in the symbol from PUSCH RE's */
	 gNB->pusch_vars[ulsch_id]->ul_valid_re_per_slot[symbol] -= gNB->pusch_vars[ulsch_id]->ptrs_re_per_slot;
       }

       /*---------------------------------------------------------------------------------------------------- */
       /*--------------------  LLRs computation  -------------------------------------------------------------*/
       /*-----------------------------------------------------------------------------------------------------*/
       start_meas(&gNB->ulsch_llr_stats);
       for (aatx=0; aatx < rel15_ul->nrOfLayers; aatx++) {
	 nr_ulsch_compute_llr(&gNB->pusch_vars[ulsch_id]->rxdataF_comp[aatx*frame_parms->nb_antennas_rx][symbol * (off + rel15_ul->rb_size * NR_NB_SC_PER_RB)],
			      gNB->pusch_vars[ulsch_id]->ul_ch_mag0[aatx*frame_parms->nb_antennas_rx],
			      gNB->pusch_vars[ulsch_id]->ul_ch_magb0[aatx*frame_parms->nb_antennas_rx],
			      gNB->pusch_vars[ulsch_id]->ul_ch_magc0[aatx*frame_parms->nb_antennas_rx],
			      &gNB->pusch_vars[ulsch_id]->llr_layers[aatx][rxdataF_ext_offset * rel15_ul->qam_mod_order],
			      rel15_ul->rb_size,
			      gNB->pusch_vars[ulsch_id]->ul_valid_re_per_slot[symbol],
			      symbol,
			      rel15_ul->qam_mod_order);
       }
       stop_meas(&gNB->ulsch_llr_stats);
       rxdataF_ext_offset += gNB->pusch_vars[ulsch_id]->ul_valid_re_per_slot[symbol];
     }
   } // symbol loop

   return 0;
 }




int nr_rx_pusch_tp(PHY_VARS_gNB *gNB,
                   uint8_t ulsch_id,
                   uint32_t frame,
                   uint8_t slot,
                   unsigned char harq_pid)
{

#ifdef TASK_MANAGER
  wake_and_spin_task_manager(&gNB->man);
#endif

  uint8_t aarx;
  uint32_t bwp_start_subcarrier;

  NR_DL_FRAME_PARMS *frame_parms = &gNB->frame_parms;
  nfapi_nr_pusch_pdu_t *rel15_ul = &gNB->ulsch[ulsch_id]->harq_processes[harq_pid]->ulsch_pdu;



  gNB->pusch_vars[ulsch_id]->dmrs_symbol = INVALID_VALUE;
  gNB->pusch_vars[ulsch_id]->cl_done = 0;
  memset(gNB->pusch_vars[ulsch_id]->extraction_done,0,14*sizeof(int));
  gNB->nbSymb=0;
  bwp_start_subcarrier = ((rel15_ul->rb_start + rel15_ul->bwp_start)*NR_NB_SC_PER_RB + frame_parms->first_carrier_offset) % frame_parms->ofdm_symbol_size;
  LOG_D(PHY,"pusch %d.%d : bwp_start_subcarrier %d, rb_start %d, first_carrier_offset %d\n", frame,slot,bwp_start_subcarrier, rel15_ul->rb_start, frame_parms->first_carrier_offset);
  LOG_D(PHY,"pusch %d.%d : ul_dmrs_symb_pos %x\n",frame,slot,rel15_ul->ul_dmrs_symb_pos);

  //----------------------------------------------------------
  //--------------------- Channel estimation ---------------------
  //----------------------------------------------------------
  start_meas(&gNB->ulsch_channel_estimation_stats);
  for(uint8_t symbol = rel15_ul->start_symbol_index; symbol < (rel15_ul->start_symbol_index + rel15_ul->nr_of_symbols); symbol++) {

    uint8_t dmrs_symbol_flag = (rel15_ul->ul_dmrs_symb_pos >> symbol) & 0x01;
    LOG_D(PHY, "symbol %d, dmrs_symbol_flag :%d\n", symbol, dmrs_symbol_flag);
    if (dmrs_symbol_flag == 1) {

      if (gNB->pusch_vars[ulsch_id]->dmrs_symbol == INVALID_VALUE)
        gNB->pusch_vars[ulsch_id]->dmrs_symbol = symbol;

      for (int nl=0; nl<rel15_ul->nrOfLayers; nl++)
        nr_pusch_channel_estimation(gNB,
                                    slot,
                                    get_dmrs_port(nl,rel15_ul->dmrs_ports),
                                    symbol,
                                    ulsch_id,
                                    bwp_start_subcarrier,
                                    rel15_ul);


      nr_gnb_measurements(gNB, ulsch_id, harq_pid, symbol,rel15_ul->nrOfLayers);

      for (aarx = 0; aarx < frame_parms->nb_antennas_rx; aarx++) {
        if (symbol == rel15_ul->start_symbol_index) {
          gNB->pusch_vars[ulsch_id]->ulsch_power[aarx] = 0;
          gNB->pusch_vars[ulsch_id]->ulsch_noise_power[aarx] = 0;
        }
        gNB->pusch_vars[ulsch_id]->ulsch_power[aarx] += signal_energy_nodc(
            &gNB->pusch_vars[ulsch_id]->ul_ch_estimates[aarx][symbol * frame_parms->ofdm_symbol_size],
            rel15_ul->rb_size * 12);
        for (int rb = 0; rb < rel15_ul->rb_size; rb++) {
          gNB->pusch_vars[ulsch_id]->ulsch_noise_power[aarx] +=
              gNB->measurements.n0_subband_power[aarx][rel15_ul->bwp_start + rel15_ul->rb_start + rb] /
              rel15_ul->rb_size;
        }
      }
    }

  }

  if (gNB->chest_time == 1) { // averaging time domain channel estimates
    nr_chest_time_domain_avg(frame_parms,
                             gNB->pusch_vars[ulsch_id]->ul_ch_estimates,
                             rel15_ul->nr_of_symbols,
                             rel15_ul->start_symbol_index,
                             rel15_ul->ul_dmrs_symb_pos,
                             rel15_ul->rb_size);

    gNB->pusch_vars[ulsch_id]->dmrs_symbol = get_next_dmrs_symbol_in_slot(rel15_ul->ul_dmrs_symb_pos, rel15_ul->start_symbol_index, rel15_ul->nr_of_symbols);
  }

  stop_meas(&gNB->ulsch_channel_estimation_stats);

  start_meas(&gNB->rx_pusch_init_stats);

  // Scrambling initialization
  int number_dmrs_symbols=0;
  for (int l = rel15_ul->start_symbol_index; l < rel15_ul->start_symbol_index + rel15_ul->nr_of_symbols; l++)
    number_dmrs_symbols += ((rel15_ul->ul_dmrs_symb_pos)>>l)&0x01;

  int nb_re_dmrs;
  if (rel15_ul->dmrs_config_type==pusch_dmrs_type1)
    nb_re_dmrs = 6*rel15_ul->num_dmrs_cdm_grps_no_data;
  else
    nb_re_dmrs = 4*rel15_ul->num_dmrs_cdm_grps_no_data;

  int G = nr_get_G(rel15_ul->rb_size,
               rel15_ul->nr_of_symbols,
               nb_re_dmrs,
               number_dmrs_symbols, // number of dmrs symbols irrespective of single or double symbol dmrs
               rel15_ul->qam_mod_order,
               rel15_ul->nrOfLayers);
  // initialize scrambling sequence
  int16_t s[G+16] __attribute__((aligned(32)));

  nr_codeword_unscrambling_init(s,G,0,rel15_ul->data_scrambling_id,rel15_ul->rnti); 
  void (*nr_pusch_symbol_processing_ptr)(void*) = &nr_pusch_symbol_processing_noprecoding;
//  void (*nr_pusch_symbol_processing_ptr)(void*) = &nr_pusch_symbol_processing;

  // first the computation of channel levels

  
  int nb_re_pusch=0,meas_symbol=-1;
  for(meas_symbol = rel15_ul->start_symbol_index; 
      meas_symbol < (rel15_ul->start_symbol_index + rel15_ul->nr_of_symbols); 
      meas_symbol++) 
    if ((nb_re_pusch = get_nb_re_pusch(frame_parms,rel15_ul,meas_symbol)) > 0) break;

  AssertFatal(nb_re_pusch>0 && meas_symbol>=0,"nb_re_pusch %d cannot be 0 or meas_symbol %d cannot be negative here\n",nb_re_pusch,meas_symbol);


  start_meas(&gNB->ulsch_rbs_extraction_stats);
  nr_ulsch_extract_rbs(gNB->common_vars.rxdataF,
                              gNB->pusch_vars[ulsch_id],
                              slot,
                              meas_symbol,
                              (rel15_ul->ul_dmrs_symb_pos >> meas_symbol) & 0x01,
                              rel15_ul,
                              frame_parms);
  stop_meas(&gNB->ulsch_rbs_extraction_stats);

  int avgs;
  int avg[frame_parms->nb_antennas_rx*rel15_ul->nrOfLayers];

  nr_ulsch_channel_level(gNB->pusch_vars[ulsch_id]->ul_ch_estimates_ext,
                         frame_parms,
                         avg,
                         meas_symbol,
                         nb_re_pusch,
                         rel15_ul->nrOfLayers,
                         rel15_ul->rb_size);

  avgs = 0;

  for (int aatx=0;aatx<rel15_ul->nrOfLayers;aatx++)
    for (int aarx=0;aarx<frame_parms->nb_antennas_rx;aarx++) {
       avgs = cmax(avgs,avg[aatx*frame_parms->nb_antennas_rx+aarx]);
    }
  gNB->pusch_vars[ulsch_id]->log2_maxh = (log2_approx(avgs)/2)+2;
  gNB->pusch_vars[ulsch_id]->cl_done = 1;
  gNB->pusch_vars[ulsch_id]->extraction_done[meas_symbol]=1;
  stop_meas(&gNB->rx_pusch_init_stats);


//  int64_t start = time_now_us(); 
//  printf("Tasks started %ld \n", start );

  start_meas(&gNB->rx_pusch_symbol_processing_stats);
  int numSymbols=gNB->num_pusch_symbols_per_thread;

#ifdef TASK_MANAGER
  puschSymbolProc_t arr[rel15_ul->nr_of_symbols];
  int idx_arr = 0;
#elif OMP_TP
  puschSymbolProc_t arr[rel15_ul->nr_of_symbols];
  int idx_arr = 0;
#endif

#ifdef OMP_TP
  omp_set_num_threads(4);

#pragma omp parallel
{
#pragma omp single
{

#endif

  for(uint8_t symbol = rel15_ul->start_symbol_index; 
      symbol < (rel15_ul->start_symbol_index + rel15_ul->nr_of_symbols); 
      symbol+=numSymbols) {
    int total_res=0;
    for (int s = 0; s<numSymbols;s++) { 
	gNB->pusch_vars[ulsch_id]->ul_valid_re_per_slot[symbol+s] = get_nb_re_pusch(frame_parms,rel15_ul,symbol+s);
        gNB->pusch_vars[ulsch_id]->llr_offset[symbol+s] = ((symbol+s)==rel15_ul->start_symbol_index) ? 
                                                          0 : 
                                                          gNB->pusch_vars[ulsch_id]->llr_offset[symbol+s-1] + gNB->pusch_vars[ulsch_id]->ul_valid_re_per_slot[symbol+s-1] * rel15_ul->qam_mod_order;
	total_res+=gNB->pusch_vars[ulsch_id]->ul_valid_re_per_slot[symbol+s];
    }	
    if (total_res > 0)  {
#ifdef TASK_MANAGER 
      puschSymbolProc_t *rdata = &arr[idx_arr];
      idx_arr++;
#elif OMP_TP
      puschSymbolProc_t *rdata = &arr[idx_arr];
      idx_arr++;
#else
      union puschSymbolReqUnion id = {.s={ulsch_id,frame,slot,0}};
      id.p=1+symbol;
      notifiedFIFO_elt_t *req=newNotifiedFIFO_elt(sizeof(puschSymbolProc_t),id.p,gNB->respPuschSymb,nr_pusch_symbol_processing_ptr);
      puschSymbolProc_t *rdata=(puschSymbolProc_t*)NotifiedFifoData(req);
#endif
      rdata->gNB = gNB;
      rdata->frame_parms=frame_parms;
      rdata->rel15_ul = rel15_ul;
      rdata->slot = slot;
      rdata->startSymbol = symbol;
      rdata->numSymbols = numSymbols;
      rdata->ulsch_id=ulsch_id;
      rdata->llr = &gNB->pusch_vars[ulsch_id]->llr[gNB->pusch_vars[ulsch_id]->llr_offset[symbol]];
      rdata->s   = &s[gNB->pusch_vars[ulsch_id]->llr_offset[symbol]];

#ifdef TASK_MANAGER
    task_t const t = {.args = rdata, .func = &nr_pusch_symbol_processing_noprecoding };
    async_task_manager(&gNB->man, t);
#elif OMP_TP
#pragma omp task
nr_pusch_symbol_processing_noprecoding(rdata);
#else
    pushTpool(&gNB->threadPool,req);
    gNB->nbSymb++;
#endif
      LOG_D(PHY,"%d.%d Added symbol %d (count %d) to process, in pipe\n",frame,slot,symbol,gNB->nbSymb);
    }
  } // symbol loop

#ifdef OMP_TP
}
}
#endif


//  printf("Waiting %ld \n", time_now_us());


#ifdef TASK_MANAGER
 stop_spin_task_manager(&gNB->man);
 wait_all_spin_task_manager(&gNB->man);
#elif OMP_TP
#pragma omp taskwait
#else
  while (gNB->nbSymb > 0) {
    notifiedFIFO_elt_t *req=pullTpool(gNB->respPuschSymb, &gNB->threadPool);
    gNB->nbSymb--;
    delNotifiedFIFO_elt(req);
  }
#endif

//  int64_t const finish = time_now_us();
//  printf("Tasks finished %ld delay %ld \n", finish , finish-start );

  stop_meas(&gNB->rx_pusch_symbol_processing_stats);
  return 0;
}

