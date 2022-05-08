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
#include "common/utils/memcpy/FastMemcpy/FastMemcpy_Avx.h"
//#define DEBUG_CH_COMP
//#define DEBUG_RB_EXT
//#define DEBUG_CH_MAG

#define INVALID_VALUE 255
#define print_shorts(s,x) printf("%s %d,%d,%d,%d,%d,%d,%d,%d\n",s,(x)[0],(x)[1],(x)[2],(x)[3],(x)[4],(x)[5],(x)[6],(x)[7])

void nr_idft(int32_t *z, uint32_t Msc_PUSCH)
{

#if defined(__x86_64__) || defined(__i386__)
  __m128i idft_in128[1][3240], idft_out128[1][3240];
  __m128i norm128;
#elif defined(__arm__)
  int16x8_t idft_in128[1][3240], idft_out128[1][3240];
  int16x8_t norm128;
#endif
  int16_t *idft_in0 = (int16_t*)idft_in128[0], *idft_out0 = (int16_t*)idft_out128[0];

  int i, ip;

  LOG_T(PHY,"Doing lte_idft for Msc_PUSCH %d\n",Msc_PUSCH);

  if ((Msc_PUSCH % 1536) > 0) {
    // conjugate input
    for (i = 0; i < (Msc_PUSCH>>2); i++) {
#if defined(__x86_64__)||defined(__i386__)
      *&(((__m128i*)z)[i]) = _mm_sign_epi16(*&(((__m128i*)z)[i]), *(__m128i*)&conjugate2[0]);
#elif defined(__arm__)
      *&(((int16x8_t*)z)[i]) = vmulq_s16(*&(((int16x8_t*)z)[i]), *(int16x8_t*)&conjugate2[0]);
#endif
    }
    for (i = 0, ip = 0; i < Msc_PUSCH; i++, ip+=4)
      ((uint32_t*)idft_in0)[ip+0] = z[i];
  }

  switch (Msc_PUSCH) {
    case 12:
      dft(DFT_12,(int16_t *)idft_in0, (int16_t *)idft_out0,0);

#if defined(__x86_64__)||defined(__i386__)
      norm128 = _mm_set1_epi16(9459);
#elif defined(__arm__)
      norm128 = vdupq_n_s16(9459);
#endif

      for (i = 0; i < 12; i++) {
#if defined(__x86_64__)||defined(__i386__)
        ((__m128i*)idft_out0)[i] = _mm_slli_epi16(_mm_mulhi_epi16(((__m128i*)idft_out0)[i], norm128), 1);
#elif defined(__arm__)
        ((int16x8_t*)idft_out0)[i] = vqdmulhq_s16(((int16x8_t*)idft_out0)[i], norm128);
#endif
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
#if defined(__x86_64__) || defined(__i386__)
      ((__m128i*)z)[i] = _mm_sign_epi16(((__m128i*)z)[i], *(__m128i*)&conjugate2[0]);
#elif defined(__arm__)
      *&(((int16x8_t*)z)[i]) = vmulq_s16(*&(((int16x8_t*)z)[i]), *(int16x8_t*)&conjugate2[0]);
#endif
    }
  }

#if defined(__x86_64__) || defined(__i386__)
  _mm_empty();
  _m_empty();
#endif

}


void nr_ulsch_extract_rbs_single(int32_t **rxdataF,
                                 NR_gNB_PUSCH *pusch_vars,
                                 int slot,
                                 unsigned char symbol,
                                 uint8_t is_dmrs_symbol,
                                 nfapi_nr_pusch_pdu_t *pusch_pdu,
                                 NR_DL_FRAME_PARMS *frame_parms)
{

  int short re, nb_re_pusch;
  unsigned char aarx;
  uint32_t rxF_ext_index = 0;
  uint32_t ul_ch0_ext_index = 0;
  uint32_t ul_ch0_index = 0;
  uint8_t k_prime;
  uint16_t n;
  int16_t *rxF,*rxF_ext;
  int *ul_ch0,*ul_ch0_ext;
  uint8_t delta = 0;
  int soffset = (slot&3)*frame_parms->symbols_per_slot*frame_parms->ofdm_symbol_size;
#ifdef DEBUG_RB_EXT

  printf("--------------------symbol = %d-----------------------\n", symbol);
  printf("--------------------ch_ext_index = %d-----------------------\n", symbol*NR_NB_SC_PER_RB * pusch_pdu->rb_size);

#endif

  uint8_t is_dmrs_re;
  int start_re = (frame_parms->first_carrier_offset + (pusch_pdu->rb_start + pusch_pdu->bwp_start) * NR_NB_SC_PER_RB)%frame_parms->ofdm_symbol_size;
  nb_re_pusch = NR_NB_SC_PER_RB * pusch_pdu->rb_size;
#ifdef __AVX2__
  int nb_re_pusch2 = nb_re_pusch + (nb_re_pusch&7);
#else
  int nb_re_pusch2 = nb_re_pusch;
#endif

  for (aarx = 0; aarx < frame_parms->nb_antennas_rx; aarx++) {

    rxF       = (int16_t *)&rxdataF[aarx][soffset+(symbol * frame_parms->ofdm_symbol_size)];
    rxF_ext   = (int16_t *)&pusch_vars->rxdataF_ext[aarx][symbol * nb_re_pusch2]; // [hna] rxdataF_ext isn't contiguous in order to solve an alignment problem ib llr computation in case of mod_order = 4, 6

    ul_ch0     = &pusch_vars->ul_ch_estimates[aarx][pusch_vars->dmrs_symbol*frame_parms->ofdm_symbol_size]; // update channel estimates if new dmrs symbol are available

    ul_ch0_ext = &pusch_vars->ul_ch_estimates_ext[aarx][symbol*nb_re_pusch2];

    n = 0;
    k_prime = 0;
    rxF_ext_index = 0;
    ul_ch0_ext_index = 0;
    ul_ch0_index = 0;

    if (is_dmrs_symbol == 0) {
      //
      //rxF[ ((start_re + re)*2)      % (frame_parms->ofdm_symbol_size*2)]);
<<<<<<< HEAD
      if (start_re + nb_re_pusch < frame_parms->ofdm_symbol_size) {
        memcpy_fast((void*)rxF_ext,
=======
      if (start_re + nb_re_pusch <= frame_parms->ofdm_symbol_size) {
        memcpy1((void*)rxF_ext,
>>>>>>> origin/ldpc-decoder-codegen
                (void*)&rxF[start_re*2],
                nb_re_pusch*sizeof(int32_t));
      } else {
	      int neg_length = frame_parms->ofdm_symbol_size-start_re;
	      int pos_length = nb_re_pusch-neg_length;

	      memcpy_fast((void*)rxF_ext,(void*)&rxF[start_re*2],neg_length*sizeof(int32_t));
	      memcpy_fast((void*)&rxF_ext[2*neg_length],(void*)rxF,pos_length*sizeof(int32_t));
      }
      memcpy_fast((void*)ul_ch0_ext,(void*)ul_ch0,nb_re_pusch*sizeof(int32_t));
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
        for (idx=1-delta;idx<neg_length;idx+=2,idx2++,idx3++) {
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

        is_dmrs_re = (re == get_dmrs_freq_idx_ul(n, k_prime, delta, pusch_pdu->dmrs_config_type));

#ifdef DEBUG_RB_EXT
        printf("re = %d, kprime %d, n %d, is_dmrs_symbol = %d, symbol = %d\n", re, k_prime, n, is_dmrs_symbol, symbol);
#endif

        /* save only data and respective channel estimates */
        if (is_dmrs_re == 0) {
          rxF_ext[rxF_ext_index]     = (rxF[ ((start_re + re)*2)      % (frame_parms->ofdm_symbol_size*2)]);
          rxF_ext[rxF_ext_index + 1] = (rxF[(((start_re + re)*2) + 1) % (frame_parms->ofdm_symbol_size*2)]);
          ul_ch0_ext[ul_ch0_ext_index] = ul_ch0[ul_ch0_index];

#ifdef DEBUG_RB_EXT
          printf("dmrs symb %d: rxF_ext[%d] = (%d,%d), ul_ch0_ext[%d] = (%d,%d)\n",
                 is_dmrs_symbol,rxF_ext_index>>1, rxF_ext[rxF_ext_index],rxF_ext[rxF_ext_index+1],
                 ul_ch0_ext_index,  ((int16_t*)&ul_ch0_ext[ul_ch0_ext_index])[0],  ((int16_t*)&ul_ch0_ext[ul_ch0_ext_index])[1]);
#endif
          ul_ch0_ext_index++;
          rxF_ext_index +=2;
        } else {
          n += k_prime;
          k_prime ^= 1;
        }
        ul_ch0_index++;
      }
    }
#endif // __AVX2_
  }
}

void nr_ulsch_scale_channel(int **ul_ch_estimates_ext,
                            NR_DL_FRAME_PARMS *frame_parms,
                            NR_gNB_ULSCH_t *ulsch_gNB,
                            uint8_t symbol,
                            uint8_t is_dmrs_symbol,
                            unsigned short nb_rb,
                            pusch_dmrs_type_t pusch_dmrs_type)
{

#if defined(__x86_64__)||defined(__i386__)

  short rb, ch_amp;
  unsigned char aarx;
  __m128i *ul_ch128, ch_amp128;

  // Determine scaling amplitude based the symbol

  ch_amp = 1024*8; //((pilots) ? (ulsch_gNB->sqrt_rho_b) : (ulsch_gNB->sqrt_rho_a));

  LOG_D(PHY,"Scaling PUSCH Chest in OFDM symbol %d by %d, pilots %d nb_rb %d NCP %d symbol %d\n", symbol, ch_amp, is_dmrs_symbol, nb_rb, frame_parms->Ncp, symbol);
   // printf("Scaling PUSCH Chest in OFDM symbol %d by %d\n",symbol_mod,ch_amp);

  ch_amp128 = _mm_set1_epi16(ch_amp); // Q3.13

#ifdef __AVX2__
  int off = ((nb_rb&1) == 1)? 4:0;
#else
  int off = 0;
#endif

  for (aarx=0; aarx < frame_parms->nb_antennas_rx; aarx++) {

      ul_ch128 = (__m128i *)&ul_ch_estimates_ext[aarx][symbol*(off+(nb_rb*NR_NB_SC_PER_RB))];

      if (is_dmrs_symbol==1){
        if (pusch_dmrs_type == pusch_dmrs_type1)
          nb_rb = nb_rb>>1;
        else
          nb_rb = (2*nb_rb)/3;
      }

      for (rb=0;rb<nb_rb;rb++) {

        ul_ch128[0] = _mm_mulhi_epi16(ul_ch128[0], ch_amp128);
        ul_ch128[0] = _mm_slli_epi16(ul_ch128[0], 3);

        ul_ch128[1] = _mm_mulhi_epi16(ul_ch128[1], ch_amp128);
        ul_ch128[1] = _mm_slli_epi16(ul_ch128[1], 3);

        if (is_dmrs_symbol) {
          ul_ch128+=2;
        } else {
          ul_ch128[2] = _mm_mulhi_epi16(ul_ch128[2], ch_amp128);
          ul_ch128[2] = _mm_slli_epi16(ul_ch128[2], 3);
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

#if defined(__x86_64__)||defined(__i386__)

  short rb;
  unsigned char aatx, aarx;
  __m128i *ul_ch128, avg128U;

  int16_t x = factor2(len);
  int16_t y = (len)>>x;

#ifdef __AVX2__
  int off = ((nb_rb&1) == 1)? 4:0;
#else
  int off = 0;
#endif

  for (aatx = 0; aatx < nrOfLayers; aatx++)
    for (aarx = 0; aarx < frame_parms->nb_antennas_rx; aarx++) {
      //clear average level
      avg128U = _mm_setzero_si128();

      ul_ch128=(__m128i *)&ul_ch_estimates_ext[aatx*frame_parms->nb_antennas_rx+aarx][symbol*(off+(nb_rb*12))];

      for (rb = 0; rb < (len<12 ? 1 : len/12); rb++) {
        avg128U = _mm_add_epi32(avg128U, _mm_srai_epi32(_mm_madd_epi16(ul_ch128[0], ul_ch128[0]), x));
        avg128U = _mm_add_epi32(avg128U, _mm_srai_epi32(_mm_madd_epi16(ul_ch128[1], ul_ch128[1]), x));
        avg128U = _mm_add_epi32(avg128U, _mm_srai_epi32(_mm_madd_epi16(ul_ch128[2], ul_ch128[2]), x));
        ul_ch128+=3;
      }

      avg[aatx*frame_parms->nb_antennas_rx+aarx] = (((int32_t*)&avg128U)[0] +
                                                    ((int32_t*)&avg128U)[1] +
                                                    ((int32_t*)&avg128U)[2] +
                                                    ((int32_t*)&avg128U)[3]) / y;

    }

  _mm_empty();
  _m_empty();

#elif defined(__arm__)

  short rb;
  unsigned char aatx, aarx, nre = 12, symbol_mod;
  int32x4_t avg128U;
  int16x4_t *ul_ch128;

  symbol_mod = (symbol>=(7-frame_parms->Ncp)) ? symbol-(7-frame_parms->Ncp) : symbol;

  for (aatx=0; aatx<nrOfLayers; aatx++) {
    for (aarx=0; aarx<frame_parms->nb_antennas_rx; aarx++) {
      //clear average level
      avg128U = vdupq_n_s32(0);
      // 5 is always a symbol with no pilots for both normal and extended prefix

      ul_ch128 = (int16x4_t *)&ul_ch_estimates_ext[aatx*frame_parms->nb_antennas_rx+aarx][symbol*frame_parms->N_RB_UL*12];

      for (rb = 0; rb < nb_rb; rb++) {
        //  printf("rb %d : ",rb);
        //  print_shorts("ch",&ul_ch128[0]);
        avg128U = vqaddq_s32(avg128U, vmull_s16(ul_ch128[0], ul_ch128[0]));
        avg128U = vqaddq_s32(avg128U, vmull_s16(ul_ch128[1], ul_ch128[1]));
        avg128U = vqaddq_s32(avg128U, vmull_s16(ul_ch128[2], ul_ch128[2]));
        avg128U = vqaddq_s32(avg128U, vmull_s16(ul_ch128[3], ul_ch128[3]));

        if (((symbol_mod == 0) || (symbol_mod == (frame_parms->Ncp-1)))&&(nrOfLayers!=1)) {
          ul_ch128+=4;
        } else {
          avg128U = vqaddq_s32(avg128U, vmull_s16(ul_ch128[4], ul_ch128[4]));
          avg128U = vqaddq_s32(avg128U, vmull_s16(ul_ch128[5], ul_ch128[5]));
          ul_ch128+=6;
        }

        /*
          if (rb==0) {
          print_shorts("ul_ch128",&ul_ch128[0]);
          print_shorts("ul_ch128",&ul_ch128[1]);
          print_shorts("ul_ch128",&ul_ch128[2]);
          }
        */
      }

      if (symbol==2) //assume start symbol 2
          nre=6;
      else
          nre=12;

      avg[aatx*frame_parms->nb_antennas_rx+aarx] = (((int32_t*)&avg128U)[0] +
                                                    ((int32_t*)&avg128U)[1] +
                                                    ((int32_t*)&avg128U)[2] +
                                                    ((int32_t*)&avg128U)[3]) / (nb_rb*nre);
    }
  }
#endif
}

void nr_ulsch_channel_compensation(int **rxdataF_ext,
                                   int **ul_ch_estimates_ext,
                                   int **ul_ch_mag,
                                   int **ul_ch_magb,
                                   int **rxdataF_comp,
                                   int **rho,
                                   NR_DL_FRAME_PARMS *frame_parms,
                                   unsigned char symbol,
                                   uint8_t is_dmrs_symbol,
                                   int pusch_dmrs_type,
                                   unsigned char mod_order,
                                   uint8_t  nrOfLayers,
                                   unsigned short nb_rb,
                                   unsigned char output_shift) {

#ifdef __AVX2__
  int off = ((nb_rb&1) == 1)? 4:0;
#else
  int off = 0;
#endif

#ifdef DEBUG_CH_COMP
  int16_t *rxF, *ul_ch;
  int prnt_idx;

  for (int ant=0; ant<frame_parms->nb_antennas_rx; ant++) {
    rxF   = (int16_t *)&rxdataF_ext[ant][symbol*(off+(nb_rb*12))];
    ul_ch = (int16_t *)&ul_ch_estimates_ext[ant][symbol*(off+(nb_rb*12))];

    printf("--------------------symbol = %d, mod_order = %d, output_shift = %d-----------------------\n", symbol, mod_order, output_shift);
    printf("----------------Before compensation------------------\n");

    for (prnt_idx=0;prnt_idx<12*5*2;prnt_idx+=2){

      printf("rxF[%d] = (%d,%d)\n", prnt_idx>>1, rxF[prnt_idx],rxF[prnt_idx+1]);
      printf("ul_ch[%d] = (%d,%d)\n", prnt_idx>>1, ul_ch[prnt_idx],ul_ch[prnt_idx+1]);

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

#if defined(__i386) || defined(__x86_64__)

  unsigned short rb;
  unsigned char aatx,aarx;
#ifdef __AVX2__
  __m256i *ul_ch256,*ul_ch_mag256,*ul_ch_mag256b,*rxdataF256,*rxdataF_comp256;
  __m256i mmtmpD0,mmtmpD1,mmtmpD2,mmtmpD3,QAM_amp256={0},QAM_amp256b={0};
  QAM_amp256b = _mm256_setzero_si256();
#else
  __m128i *ul_ch128,*ul_ch128_2,*ul_ch_mag128,*ul_ch_mag128b,*rxdataF128,*rxdataF_comp128,*rho128;
  __m128i mmtmpD0,mmtmpD1,mmtmpD2,mmtmpD3,QAM_amp128={0},QAM_amp128b={0};
  QAM_amp128b = _mm_setzero_si128();
#endif
  for (aatx=0; aatx<nrOfLayers; aatx++) {

    if (mod_order == 4) {
#ifdef __AVX2__
      QAM_amp256  = _mm256_set1_epi16(QAM16_n1);  // 2/sqrt(10)
      QAM_amp256b = _mm256_setzero_si256();
#else
      QAM_amp128 = _mm_set1_epi16(QAM16_n1);  // 2/sqrt(10)
      QAM_amp128b = _mm_setzero_si128();
#endif
    } else if (mod_order == 6) {
#ifdef __AVX2__
      QAM_amp256  = _mm256_set1_epi16(QAM64_n1); //
      QAM_amp256b = _mm256_set1_epi16(QAM64_n2);
#else
      QAM_amp128  = _mm_set1_epi16(QAM64_n1); //
      QAM_amp128b = _mm_set1_epi16(QAM64_n2);
#endif
    }

    //    printf("comp: rxdataF_comp %p, symbol %d\n",rxdataF_comp[0],symbol);

    for (aarx=0; aarx<frame_parms->nb_antennas_rx; aarx++) {
#ifdef __AVX2__
      ul_ch256          = (__m256i *)&ul_ch_estimates_ext[aatx*frame_parms->nb_antennas_rx+aarx][symbol*(off+(nb_rb*12))];
      ul_ch_mag256      = (__m256i *)&ul_ch_mag[aatx*frame_parms->nb_antennas_rx+aarx][symbol*(off+(nb_rb*12))];
      ul_ch_mag256b     = (__m256i *)&ul_ch_magb[aatx*frame_parms->nb_antennas_rx+aarx][symbol*(off+(nb_rb*12))];
      rxdataF256        = (__m256i *)&rxdataF_ext[aarx][symbol*(off+(nb_rb*12))];
      rxdataF_comp256   = (__m256i *)&rxdataF_comp[aatx*frame_parms->nb_antennas_rx+aarx][symbol*(off+(nb_rb*12))];
#else
      ul_ch128          = (__m128i *)&ul_ch_estimates_ext[aatx*frame_parms->nb_antennas_rx+aarx][symbol*(off+(nb_rb*12))];
      ul_ch_mag128      = (__m128i *)&ul_ch_mag[aatx*frame_parms->nb_antennas_rx+aarx][symbol*(off+(nb_rb*12))];
      ul_ch_mag128b     = (__m128i *)&ul_ch_magb[aatx*frame_parms->nb_antennas_rx+aarx][symbol*(off+(nb_rb*12))];
      rxdataF128        = (__m128i *)&rxdataF_ext[aarx][symbol*(off+(nb_rb*12))];
      rxdataF_comp128   = (__m128i *)&rxdataF_comp[aatx*frame_parms->nb_antennas_rx+aarx][symbol*(off+(nb_rb*12))];
#endif



#ifdef __AVX2__
      register __m256i complex_shuffle = _mm256_set_epi8(29,28,31,30,25,24,27,26,21,20,23,22,17,16,19,18,13,12,15,14,9,8,11,10,5,4,7,6,1,0,3,2);
      register __m256i conj = _mm256_set_epi16(1,-1,1,-1,1,-1,1,-1,1,-1,1,-1,1,-1,1,-1);

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
          mmtmpD0 = _mm256_madd_epi16(ul_ch256[re8],rxdataF256[re8]);
          // mmtmpD0 contains real part of 8 consecutive outputs (32-bit)
          mmtmpD1 = _mm256_shuffle_epi8(ul_ch256[re8],complex_shuffle);
          mmtmpD1 = _mm256_sign_epi16(mmtmpD1,conj);
          mmtmpD1 = _mm256_madd_epi16(mmtmpD1,rxdataF256[re8]);
          // mmtmpD1 contains imag part of 4 consecutive outputs (32-bit)
          mmtmpD0 = _mm256_srai_epi32(mmtmpD0,output_shift);
          mmtmpD1 = _mm256_srai_epi32(mmtmpD1,output_shift);
          mmtmpD2 = _mm256_unpacklo_epi32(mmtmpD0,mmtmpD1);
          mmtmpD3 = _mm256_unpackhi_epi32(mmtmpD0,mmtmpD1);
          rxdataF_comp256[re8] = _mm256_packs_epi32(mmtmpD2,mmtmpD3);
        }
      }
      else if (mod_order==4) {
        for (int re8=0; re8<len8; re8++) {
          mmtmpD0 = _mm256_madd_epi16(ul_ch256[re8],ul_ch256[re8]);
          mmtmpD0 = _mm256_srai_epi32(mmtmpD0,output_shift);
          mmtmpD0 = _mm256_packs_epi32(mmtmpD0,mmtmpD0);

          // store channel magnitude here in a new field of ulsch

          ul_ch_mag256[re8] = _mm256_unpacklo_epi16(mmtmpD0,mmtmpD0);
          ul_ch_mag256[re8] = _mm256_mulhrs_epi16(ul_ch_mag256[re8],QAM_amp256);
          // multiply by conjugated channel
          mmtmpD0 = _mm256_madd_epi16(ul_ch256[re8],rxdataF256[re8]);
          // mmtmpD0 contains real part of 8 consecutive outputs (32-bit)
          mmtmpD1 = _mm256_shuffle_epi8(ul_ch256[re8],complex_shuffle);
          mmtmpD1 = _mm256_sign_epi16(mmtmpD1,conj);
          mmtmpD1 = _mm256_madd_epi16(mmtmpD1,rxdataF256[re8]);
          // mmtmpD1 contains imag part of 4 consecutive outputs (32-bit)
          mmtmpD0 = _mm256_srai_epi32(mmtmpD0,output_shift);
          mmtmpD1 = _mm256_srai_epi32(mmtmpD1,output_shift);
          mmtmpD2 = _mm256_unpacklo_epi32(mmtmpD0,mmtmpD1);
          mmtmpD3 = _mm256_unpackhi_epi32(mmtmpD0,mmtmpD1);
          rxdataF_comp256[re8] = _mm256_packs_epi32(mmtmpD2,mmtmpD3);
        }
      }
      else if (mod_order==6) {
        for (int re8=0; re8<len8; re8++) {
          mmtmpD0 = _mm256_madd_epi16(ul_ch256[re8],ul_ch256[re8]);
          mmtmpD0 = _mm256_srai_epi32(mmtmpD0,output_shift);
          mmtmpD0 = _mm256_packs_epi32(mmtmpD0,mmtmpD0);
          // store channel magnitude here in a new field of ulsch
          ul_ch_mag256[re8]  = _mm256_unpacklo_epi16(mmtmpD0,mmtmpD0);
          ul_ch_mag256b[re8] = ul_ch_mag256[re8];
          ul_ch_mag256[re8]  = _mm256_mulhrs_epi16(ul_ch_mag256[re8],QAM_amp256);
          ul_ch_mag256b[re8] = _mm256_mulhrs_epi16(ul_ch_mag256b[re8],QAM_amp256b);
          // multiply by conjugated channel
          mmtmpD0 = _mm256_madd_epi16(ul_ch256[re8],rxdataF256[re8]);
          // mmtmpD0 contains real part of 8 consecutive outputs (32-bit)
          mmtmpD1 = _mm256_shuffle_epi8(ul_ch256[re8],complex_shuffle);
          mmtmpD1 = _mm256_sign_epi16(mmtmpD1,conj);
          mmtmpD1 = _mm256_madd_epi16(mmtmpD1,rxdataF256[re8]);
          // mmtmpD1 contains imag part of 4 consecutive outputs (32-bit)
          mmtmpD0 = _mm256_srai_epi32(mmtmpD0,output_shift);
          mmtmpD1 = _mm256_srai_epi32(mmtmpD1,output_shift);
          mmtmpD2 = _mm256_unpacklo_epi32(mmtmpD0,mmtmpD1);
          mmtmpD3 = _mm256_unpackhi_epi32(mmtmpD0,mmtmpD1);
          rxdataF_comp256[re8] = _mm256_packs_epi32(mmtmpD2,mmtmpD3);
        }
      }
      else AssertFatal(1==0,"Unsupported mod_order %d\n",mod_order);
#else
      for (rb=0; rb<nb_rb; rb++) {
        if (mod_order>2) {
          // get channel amplitude if not QPSK
          //print_shorts("ch:",(int16_t*)&ul_ch128[0]);

          mmtmpD0 = _mm_madd_epi16(ul_ch128[0],ul_ch128[0]);
          mmtmpD0 = _mm_srai_epi32(mmtmpD0,output_shift);

          mmtmpD1 = _mm_madd_epi16(ul_ch128[1],ul_ch128[1]);
          mmtmpD1 = _mm_srai_epi32(mmtmpD1,output_shift);

          mmtmpD0 = _mm_packs_epi32(mmtmpD0,mmtmpD1);

          // store channel magnitude here in a new field of ulsch

          ul_ch_mag128[0] = _mm_unpacklo_epi16(mmtmpD0,mmtmpD0);
          ul_ch_mag128b[0] = ul_ch_mag128[0];
          ul_ch_mag128[0] = _mm_mulhi_epi16(ul_ch_mag128[0],QAM_amp128);
          ul_ch_mag128[0] = _mm_slli_epi16(ul_ch_mag128[0],1);

          // print_ints("ch: = ",(int32_t*)&mmtmpD0);
          // print_shorts("QAM_amp:",(int16_t*)&QAM_amp128);
          // print_shorts("mag:",(int16_t*)&ul_ch_mag128[0]);

          ul_ch_mag128[1] = _mm_unpackhi_epi16(mmtmpD0,mmtmpD0);
          ul_ch_mag128b[1] = ul_ch_mag128[1];
          ul_ch_mag128[1] = _mm_mulhi_epi16(ul_ch_mag128[1],QAM_amp128);
          ul_ch_mag128[1] = _mm_slli_epi16(ul_ch_mag128[1],1);

          if (is_dmrs_symbol==0) {
            mmtmpD0 = _mm_madd_epi16(ul_ch128[2],ul_ch128[2]);
            mmtmpD0 = _mm_srai_epi32(mmtmpD0,output_shift);
            mmtmpD1 = _mm_packs_epi32(mmtmpD0,mmtmpD0);

            ul_ch_mag128[2] = _mm_unpacklo_epi16(mmtmpD1,mmtmpD1);
            ul_ch_mag128b[2] = ul_ch_mag128[2];

            ul_ch_mag128[2] = _mm_mulhi_epi16(ul_ch_mag128[2],QAM_amp128);
            ul_ch_mag128[2] = _mm_slli_epi16(ul_ch_mag128[2],1);
          }

          ul_ch_mag128b[0] = _mm_mulhi_epi16(ul_ch_mag128b[0],QAM_amp128b);
          ul_ch_mag128b[0] = _mm_slli_epi16(ul_ch_mag128b[0],1);


          ul_ch_mag128b[1] = _mm_mulhi_epi16(ul_ch_mag128b[1],QAM_amp128b);
          ul_ch_mag128b[1] = _mm_slli_epi16(ul_ch_mag128b[1],1);

          if (is_dmrs_symbol==0) {
            ul_ch_mag128b[2] = _mm_mulhi_epi16(ul_ch_mag128b[2],QAM_amp128b);
            ul_ch_mag128b[2] = _mm_slli_epi16(ul_ch_mag128b[2],1);
          }
        }

        // multiply by conjugated channel
        mmtmpD0 = _mm_madd_epi16(ul_ch128[0],rxdataF128[0]);
        //  print_ints("re",&mmtmpD0);

        // mmtmpD0 contains real part of 4 consecutive outputs (32-bit)
        mmtmpD1 = _mm_shufflelo_epi16(ul_ch128[0],_MM_SHUFFLE(2,3,0,1));
        mmtmpD1 = _mm_shufflehi_epi16(mmtmpD1,_MM_SHUFFLE(2,3,0,1));
        mmtmpD1 = _mm_sign_epi16(mmtmpD1,*(__m128i*)&conjugate[0]);
        //  print_ints("im",&mmtmpD1);
        mmtmpD1 = _mm_madd_epi16(mmtmpD1,rxdataF128[0]);
        // mmtmpD1 contains imag part of 4 consecutive outputs (32-bit)
        mmtmpD0 = _mm_srai_epi32(mmtmpD0,output_shift);
        //  print_ints("re(shift)",&mmtmpD0);
        mmtmpD1 = _mm_srai_epi32(mmtmpD1,output_shift);
        //  print_ints("im(shift)",&mmtmpD1);
        mmtmpD2 = _mm_unpacklo_epi32(mmtmpD0,mmtmpD1);
        mmtmpD3 = _mm_unpackhi_epi32(mmtmpD0,mmtmpD1);
        //        print_ints("c0",&mmtmpD2);
        //  print_ints("c1",&mmtmpD3);
        rxdataF_comp128[0] = _mm_packs_epi32(mmtmpD2,mmtmpD3);
        //  print_shorts("rx:",rxdataF128);
        //  print_shorts("ch:",ul_ch128);
        //  print_shorts("pack:",rxdataF_comp128);

        // multiply by conjugated channel
        mmtmpD0 = _mm_madd_epi16(ul_ch128[1],rxdataF128[1]);
        // mmtmpD0 contains real part of 4 consecutive outputs (32-bit)
        mmtmpD1 = _mm_shufflelo_epi16(ul_ch128[1],_MM_SHUFFLE(2,3,0,1));
        mmtmpD1 = _mm_shufflehi_epi16(mmtmpD1,_MM_SHUFFLE(2,3,0,1));
        mmtmpD1 = _mm_sign_epi16(mmtmpD1,*(__m128i*)conjugate);
        mmtmpD1 = _mm_madd_epi16(mmtmpD1,rxdataF128[1]);
        // mmtmpD1 contains imag part of 4 consecutive outputs (32-bit)
        mmtmpD0 = _mm_srai_epi32(mmtmpD0,output_shift);
        mmtmpD1 = _mm_srai_epi32(mmtmpD1,output_shift);
        mmtmpD2 = _mm_unpacklo_epi32(mmtmpD0,mmtmpD1);
        mmtmpD3 = _mm_unpackhi_epi32(mmtmpD0,mmtmpD1);

        rxdataF_comp128[1] = _mm_packs_epi32(mmtmpD2,mmtmpD3);
        //  print_shorts("rx:",rxdataF128+1);
        //  print_shorts("ch:",ul_ch128+1);
        //  print_shorts("pack:",rxdataF_comp128+1);

        if (is_dmrs_symbol==0) {
          // multiply by conjugated channel
          mmtmpD0 = _mm_madd_epi16(ul_ch128[2],rxdataF128[2]);
          // mmtmpD0 contains real part of 4 consecutive outputs (32-bit)
          mmtmpD1 = _mm_shufflelo_epi16(ul_ch128[2],_MM_SHUFFLE(2,3,0,1));
          mmtmpD1 = _mm_shufflehi_epi16(mmtmpD1,_MM_SHUFFLE(2,3,0,1));
          mmtmpD1 = _mm_sign_epi16(mmtmpD1,*(__m128i*)conjugate);
          mmtmpD1 = _mm_madd_epi16(mmtmpD1,rxdataF128[2]);
          // mmtmpD1 contains imag part of 4 consecutive outputs (32-bit)
          mmtmpD0 = _mm_srai_epi32(mmtmpD0,output_shift);
          mmtmpD1 = _mm_srai_epi32(mmtmpD1,output_shift);
          mmtmpD2 = _mm_unpacklo_epi32(mmtmpD0,mmtmpD1);
          mmtmpD3 = _mm_unpackhi_epi32(mmtmpD0,mmtmpD1);

          rxdataF_comp128[2] = _mm_packs_epi32(mmtmpD2,mmtmpD3);
          //  print_shorts("rx:",rxdataF128+2);
          //  print_shorts("ch:",ul_ch128+2);
          //        print_shorts("pack:",rxdataF_comp128+2);

          ul_ch128+=3;
          ul_ch_mag128+=3;
          ul_ch_mag128b+=3;
          rxdataF128+=3;
          rxdataF_comp128+=3;
        } else { // we have a smaller PUSCH in symbols with pilots so skip last group of 4 REs and increment less
          ul_ch128+=2;
          ul_ch_mag128+=2;
          ul_ch_mag128b+=2;
          rxdataF128+=2;
          rxdataF_comp128+=2;
        }

      }
#endif
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
        mmtmpD0 = _mm_madd_epi16(ul_ch128[0],ul_ch128_2[0]);
        //  print_ints("re",&mmtmpD0);

        // mmtmpD0 contains real part of 4 consecutive outputs (32-bit)
        mmtmpD1 = _mm_shufflelo_epi16(ul_ch128[0],_MM_SHUFFLE(2,3,0,1));
        mmtmpD1 = _mm_shufflehi_epi16(mmtmpD1,_MM_SHUFFLE(2,3,0,1));
        mmtmpD1 = _mm_sign_epi16(mmtmpD1,*(__m128i*)&conjugate[0]);
        //  print_ints("im",&mmtmpD1);
        mmtmpD1 = _mm_madd_epi16(mmtmpD1,ul_ch128_2[0]);
        // mmtmpD1 contains imag part of 4 consecutive outputs (32-bit)
        mmtmpD0 = _mm_srai_epi32(mmtmpD0,output_shift);
        //  print_ints("re(shift)",&mmtmpD0);
        mmtmpD1 = _mm_srai_epi32(mmtmpD1,output_shift);
        //  print_ints("im(shift)",&mmtmpD1);
        mmtmpD2 = _mm_unpacklo_epi32(mmtmpD0,mmtmpD1);
        mmtmpD3 = _mm_unpackhi_epi32(mmtmpD0,mmtmpD1);
        //        print_ints("c0",&mmtmpD2);
        //  print_ints("c1",&mmtmpD3);
        rho128[0] = _mm_packs_epi32(mmtmpD2,mmtmpD3);

        //print_shorts("rx:",ul_ch128_2);
        //print_shorts("ch:",ul_ch128);
        //print_shorts("pack:",rho128);

        // multiply by conjugated channel
        mmtmpD0 = _mm_madd_epi16(ul_ch128[1],ul_ch128_2[1]);
        // mmtmpD0 contains real part of 4 consecutive outputs (32-bit)
        mmtmpD1 = _mm_shufflelo_epi16(ul_ch128[1],_MM_SHUFFLE(2,3,0,1));
        mmtmpD1 = _mm_shufflehi_epi16(mmtmpD1,_MM_SHUFFLE(2,3,0,1));
        mmtmpD1 = _mm_sign_epi16(mmtmpD1,*(__m128i*)conjugate);
        mmtmpD1 = _mm_madd_epi16(mmtmpD1,ul_ch128_2[1]);
        // mmtmpD1 contains imag part of 4 consecutive outputs (32-bit)
        mmtmpD0 = _mm_srai_epi32(mmtmpD0,output_shift);
        mmtmpD1 = _mm_srai_epi32(mmtmpD1,output_shift);
        mmtmpD2 = _mm_unpacklo_epi32(mmtmpD0,mmtmpD1);
        mmtmpD3 = _mm_unpackhi_epi32(mmtmpD0,mmtmpD1);


        rho128[1] =_mm_packs_epi32(mmtmpD2,mmtmpD3);
        //print_shorts("rx:",ul_ch128_2+1);
        //print_shorts("ch:",ul_ch128+1);
        //print_shorts("pack:",rho128+1);
        // multiply by conjugated channel
        mmtmpD0 = _mm_madd_epi16(ul_ch128[2],ul_ch128_2[2]);
        // mmtmpD0 contains real part of 4 consecutive outputs (32-bit)
        mmtmpD1 = _mm_shufflelo_epi16(ul_ch128[2],_MM_SHUFFLE(2,3,0,1));
        mmtmpD1 = _mm_shufflehi_epi16(mmtmpD1,_MM_SHUFFLE(2,3,0,1));
        mmtmpD1 = _mm_sign_epi16(mmtmpD1,*(__m128i*)conjugate);
        mmtmpD1 = _mm_madd_epi16(mmtmpD1,ul_ch128_2[2]);
        // mmtmpD1 contains imag part of 4 consecutive outputs (32-bit)
        mmtmpD0 = _mm_srai_epi32(mmtmpD0,output_shift);
        mmtmpD1 = _mm_srai_epi32(mmtmpD1,output_shift);
        mmtmpD2 = _mm_unpacklo_epi32(mmtmpD0,mmtmpD1);
        mmtmpD3 = _mm_unpackhi_epi32(mmtmpD0,mmtmpD1);

        rho128[2] = _mm_packs_epi32(mmtmpD2,mmtmpD3);
        //print_shorts("rx:",ul_ch128_2+2);
        //print_shorts("ch:",ul_ch128+2);
        //print_shorts("pack:",rho128+2);

        ul_ch128+=3;
        ul_ch128_2+=3;
        rho128+=3;

      }

    }
*/
  }

  _mm_empty();
  _m_empty();

#elif defined(__arm__)

  unsigned short rb;
  unsigned char aatx,aarx,symbol_mod,is_dmrs_symbol=0;

  int16x4_t *ul_ch128,*ul_ch128_2,*rxdataF128;
  int32x4_t mmtmpD0,mmtmpD1,mmtmpD0b,mmtmpD1b;
  int16x8_t *ul_ch_mag128,*ul_ch_mag128b,mmtmpD2,mmtmpD3,mmtmpD4;
  int16x8_t QAM_amp128,QAM_amp128b;
  int16x4x2_t *rxdataF_comp128,*rho128;

  int16_t conj[4]__attribute__((aligned(16))) = {1,-1,1,-1};
  int32x4_t output_shift128 = vmovq_n_s32(-(int32_t)output_shift);

  symbol_mod = (symbol>=(7-frame_parms->Ncp)) ? symbol-(7-frame_parms->Ncp) : symbol;

  if ((symbol_mod == 0) || (symbol_mod == (4-frame_parms->Ncp))) {
    if (nrOfLayers==1) { // 10 out of 12 so don't reduce size
      nb_rb=1+(5*nb_rb/6);
    }
    else {
      is_dmrs_symbol=1;
    }
  }

  for (aatx=0; aatx<nrOfLayers; aatx++) {
    if (mod_order == 4) {
      QAM_amp128  = vmovq_n_s16(QAM16_n1);  // 2/sqrt(10)
      QAM_amp128b = vmovq_n_s16(0);
    } else if (mod_order == 6) {
      QAM_amp128  = vmovq_n_s16(QAM64_n1); //
      QAM_amp128b = vmovq_n_s16(QAM64_n2);
    }
    //    printf("comp: rxdataF_comp %p, symbol %d\n",rxdataF_comp[0],symbol);

    for (aarx=0; aarx<frame_parms->nb_antennas_rx; aarx++) {
      ul_ch128          = (int16x4_t*)&ul_ch_estimates_ext[aatx*frame_parms->nb_antennas_rx+aarx][symbol*frame_parms->N_RB_UL*12];
      ul_ch_mag128      = (int16x8_t*)&ul_ch_mag[aatx*frame_parms->nb_antennas_rx+aarx][symbol*frame_parms->N_RB_UL*12];
      ul_ch_mag128b     = (int16x8_t*)&ul_ch_magb[aatx*frame_parms->nb_antennas_rx+aarx][symbol*frame_parms->N_RB_UL*12];
      rxdataF128        = (int16x4_t*)&rxdataF_ext[aarx][symbol*frame_parms->N_RB_UL*12];
      rxdataF_comp128   = (int16x4x2_t*)&rxdataF_comp[aatx*frame_parms->nb_antennas_rx+aarx][symbol*frame_parms->N_RB_UL*12];

      for (rb=0; rb<nb_rb; rb++) {
  if (mod_order>2) {
    // get channel amplitude if not QPSK
    mmtmpD0 = vmull_s16(ul_ch128[0], ul_ch128[0]);
    // mmtmpD0 = [ch0*ch0,ch1*ch1,ch2*ch2,ch3*ch3];
    mmtmpD0 = vqshlq_s32(vqaddq_s32(mmtmpD0,vrev64q_s32(mmtmpD0)),output_shift128);
    // mmtmpD0 = [ch0*ch0 + ch1*ch1,ch0*ch0 + ch1*ch1,ch2*ch2 + ch3*ch3,ch2*ch2 + ch3*ch3]>>output_shift128 on 32-bits
    mmtmpD1 = vmull_s16(ul_ch128[1], ul_ch128[1]);
    mmtmpD1 = vqshlq_s32(vqaddq_s32(mmtmpD1,vrev64q_s32(mmtmpD1)),output_shift128);
    mmtmpD2 = vcombine_s16(vmovn_s32(mmtmpD0),vmovn_s32(mmtmpD1));
    // mmtmpD2 = [ch0*ch0 + ch1*ch1,ch0*ch0 + ch1*ch1,ch2*ch2 + ch3*ch3,ch2*ch2 + ch3*ch3,ch4*ch4 + ch5*ch5,ch4*ch4 + ch5*ch5,ch6*ch6 + ch7*ch7,ch6*ch6 + ch7*ch7]>>output_shift128 on 16-bits
    mmtmpD0 = vmull_s16(ul_ch128[2], ul_ch128[2]);
    mmtmpD0 = vqshlq_s32(vqaddq_s32(mmtmpD0,vrev64q_s32(mmtmpD0)),output_shift128);
    mmtmpD1 = vmull_s16(ul_ch128[3], ul_ch128[3]);
    mmtmpD1 = vqshlq_s32(vqaddq_s32(mmtmpD1,vrev64q_s32(mmtmpD1)),output_shift128);
    mmtmpD3 = vcombine_s16(vmovn_s32(mmtmpD0),vmovn_s32(mmtmpD1));
    if (is_dmrs_symbol==0) {
      mmtmpD0 = vmull_s16(ul_ch128[4], ul_ch128[4]);
      mmtmpD0 = vqshlq_s32(vqaddq_s32(mmtmpD0,vrev64q_s32(mmtmpD0)),output_shift128);
      mmtmpD1 = vmull_s16(ul_ch128[5], ul_ch128[5]);
      mmtmpD1 = vqshlq_s32(vqaddq_s32(mmtmpD1,vrev64q_s32(mmtmpD1)),output_shift128);
      mmtmpD4 = vcombine_s16(vmovn_s32(mmtmpD0),vmovn_s32(mmtmpD1));
    }

    ul_ch_mag128b[0] = vqdmulhq_s16(mmtmpD2,QAM_amp128b);
    ul_ch_mag128b[1] = vqdmulhq_s16(mmtmpD3,QAM_amp128b);
    ul_ch_mag128[0] = vqdmulhq_s16(mmtmpD2,QAM_amp128);
    ul_ch_mag128[1] = vqdmulhq_s16(mmtmpD3,QAM_amp128);

    if (is_dmrs_symbol==0) {
      ul_ch_mag128b[2] = vqdmulhq_s16(mmtmpD4,QAM_amp128b);
      ul_ch_mag128[2]  = vqdmulhq_s16(mmtmpD4,QAM_amp128);
    }
  }

  mmtmpD0 = vmull_s16(ul_ch128[0], rxdataF128[0]);
  //mmtmpD0 = [Re(ch[0])Re(rx[0]) Im(ch[0])Im(ch[0]) Re(ch[1])Re(rx[1]) Im(ch[1])Im(ch[1])]
  mmtmpD1 = vmull_s16(ul_ch128[1], rxdataF128[1]);
  //mmtmpD1 = [Re(ch[2])Re(rx[2]) Im(ch[2])Im(ch[2]) Re(ch[3])Re(rx[3]) Im(ch[3])Im(ch[3])]
  mmtmpD0 = vcombine_s32(vpadd_s32(vget_low_s32(mmtmpD0),vget_high_s32(mmtmpD0)),
             vpadd_s32(vget_low_s32(mmtmpD1),vget_high_s32(mmtmpD1)));
  //mmtmpD0 = [Re(ch[0])Re(rx[0])+Im(ch[0])Im(ch[0]) Re(ch[1])Re(rx[1])+Im(ch[1])Im(ch[1]) Re(ch[2])Re(rx[2])+Im(ch[2])Im(ch[2]) Re(ch[3])Re(rx[3])+Im(ch[3])Im(ch[3])]

  mmtmpD0b = vmull_s16(vrev32_s16(vmul_s16(ul_ch128[0],*(int16x4_t*)conj)), rxdataF128[0]);
  //mmtmpD0 = [-Im(ch[0])Re(rx[0]) Re(ch[0])Im(rx[0]) -Im(ch[1])Re(rx[1]) Re(ch[1])Im(rx[1])]
  mmtmpD1b = vmull_s16(vrev32_s16(vmul_s16(ul_ch128[1],*(int16x4_t*)conj)), rxdataF128[1]);
  //mmtmpD0 = [-Im(ch[2])Re(rx[2]) Re(ch[2])Im(rx[2]) -Im(ch[3])Re(rx[3]) Re(ch[3])Im(rx[3])]
  mmtmpD1 = vcombine_s32(vpadd_s32(vget_low_s32(mmtmpD0b),vget_high_s32(mmtmpD0b)),
             vpadd_s32(vget_low_s32(mmtmpD1b),vget_high_s32(mmtmpD1b)));
  //mmtmpD1 = [-Im(ch[0])Re(rx[0])+Re(ch[0])Im(rx[0]) -Im(ch[1])Re(rx[1])+Re(ch[1])Im(rx[1]) -Im(ch[2])Re(rx[2])+Re(ch[2])Im(rx[2]) -Im(ch[3])Re(rx[3])+Re(ch[3])Im(rx[3])]

  mmtmpD0 = vqshlq_s32(mmtmpD0,output_shift128);
  mmtmpD1 = vqshlq_s32(mmtmpD1,output_shift128);
  rxdataF_comp128[0] = vzip_s16(vmovn_s32(mmtmpD0),vmovn_s32(mmtmpD1));
  mmtmpD0 = vmull_s16(ul_ch128[2], rxdataF128[2]);
  mmtmpD1 = vmull_s16(ul_ch128[3], rxdataF128[3]);
  mmtmpD0 = vcombine_s32(vpadd_s32(vget_low_s32(mmtmpD0),vget_high_s32(mmtmpD0)),
             vpadd_s32(vget_low_s32(mmtmpD1),vget_high_s32(mmtmpD1)));
  mmtmpD0b = vmull_s16(vrev32_s16(vmul_s16(ul_ch128[2],*(int16x4_t*)conj)), rxdataF128[2]);
  mmtmpD1b = vmull_s16(vrev32_s16(vmul_s16(ul_ch128[3],*(int16x4_t*)conj)), rxdataF128[3]);
  mmtmpD1 = vcombine_s32(vpadd_s32(vget_low_s32(mmtmpD0b),vget_high_s32(mmtmpD0b)),
             vpadd_s32(vget_low_s32(mmtmpD1b),vget_high_s32(mmtmpD1b)));
  mmtmpD0 = vqshlq_s32(mmtmpD0,output_shift128);
  mmtmpD1 = vqshlq_s32(mmtmpD1,output_shift128);
  rxdataF_comp128[1] = vzip_s16(vmovn_s32(mmtmpD0),vmovn_s32(mmtmpD1));

  if (is_dmrs_symbol==0) {
    mmtmpD0 = vmull_s16(ul_ch128[4], rxdataF128[4]);
    mmtmpD1 = vmull_s16(ul_ch128[5], rxdataF128[5]);
    mmtmpD0 = vcombine_s32(vpadd_s32(vget_low_s32(mmtmpD0),vget_high_s32(mmtmpD0)),
         vpadd_s32(vget_low_s32(mmtmpD1),vget_high_s32(mmtmpD1)));

    mmtmpD0b = vmull_s16(vrev32_s16(vmul_s16(ul_ch128[4],*(int16x4_t*)conj)), rxdataF128[4]);
    mmtmpD1b = vmull_s16(vrev32_s16(vmul_s16(ul_ch128[5],*(int16x4_t*)conj)), rxdataF128[5]);
    mmtmpD1 = vcombine_s32(vpadd_s32(vget_low_s32(mmtmpD0b),vget_high_s32(mmtmpD0b)),
         vpadd_s32(vget_low_s32(mmtmpD1b),vget_high_s32(mmtmpD1b)));


    mmtmpD0 = vqshlq_s32(mmtmpD0,output_shift128);
    mmtmpD1 = vqshlq_s32(mmtmpD1,output_shift128);
    rxdataF_comp128[2] = vzip_s16(vmovn_s32(mmtmpD0),vmovn_s32(mmtmpD1));


    ul_ch128+=6;
    ul_ch_mag128+=3;
    ul_ch_mag128b+=3;
    rxdataF128+=6;
    rxdataF_comp128+=3;

  } else { // we have a smaller PUSCH in symbols with pilots so skip last group of 4 REs and increment less
    ul_ch128+=4;
    ul_ch_mag128+=2;
    ul_ch_mag128b+=2;
    rxdataF128+=4;
    rxdataF_comp128+=2;
  }
      }
    }
  }

  if (rho) {
    for (aarx=0; aarx<frame_parms->nb_antennas_rx; aarx++) {
      rho128        = (int16x4x2_t*)&rho[aarx][symbol*frame_parms->N_RB_UL*12];
      ul_ch128      = (int16x4_t*)&ul_ch_estimates_ext[aarx][symbol*frame_parms->N_RB_UL*12];
      ul_ch128_2    = (int16x4_t*)&ul_ch_estimates_ext[2+aarx][symbol*frame_parms->N_RB_UL*12];
      for (rb=0; rb<nb_rb; rb++) {
  mmtmpD0 = vmull_s16(ul_ch128[0], ul_ch128_2[0]);
  mmtmpD1 = vmull_s16(ul_ch128[1], ul_ch128_2[1]);
  mmtmpD0 = vcombine_s32(vpadd_s32(vget_low_s32(mmtmpD0),vget_high_s32(mmtmpD0)),
             vpadd_s32(vget_low_s32(mmtmpD1),vget_high_s32(mmtmpD1)));
  mmtmpD0b = vmull_s16(vrev32_s16(vmul_s16(ul_ch128[0],*(int16x4_t*)conj)), ul_ch128_2[0]);
  mmtmpD1b = vmull_s16(vrev32_s16(vmul_s16(ul_ch128[1],*(int16x4_t*)conj)), ul_ch128_2[1]);
  mmtmpD1 = vcombine_s32(vpadd_s32(vget_low_s32(mmtmpD0b),vget_high_s32(mmtmpD0b)),
             vpadd_s32(vget_low_s32(mmtmpD1b),vget_high_s32(mmtmpD1b)));

  mmtmpD0 = vqshlq_s32(mmtmpD0,output_shift128);
  mmtmpD1 = vqshlq_s32(mmtmpD1,output_shift128);
  rho128[0] = vzip_s16(vmovn_s32(mmtmpD0),vmovn_s32(mmtmpD1));

  mmtmpD0 = vmull_s16(ul_ch128[2], ul_ch128_2[2]);
  mmtmpD1 = vmull_s16(ul_ch128[3], ul_ch128_2[3]);
  mmtmpD0 = vcombine_s32(vpadd_s32(vget_low_s32(mmtmpD0),vget_high_s32(mmtmpD0)),
             vpadd_s32(vget_low_s32(mmtmpD1),vget_high_s32(mmtmpD1)));
  mmtmpD0b = vmull_s16(vrev32_s16(vmul_s16(ul_ch128[2],*(int16x4_t*)conj)), ul_ch128_2[2]);
  mmtmpD1b = vmull_s16(vrev32_s16(vmul_s16(ul_ch128[3],*(int16x4_t*)conj)), ul_ch128_2[3]);
  mmtmpD1 = vcombine_s32(vpadd_s32(vget_low_s32(mmtmpD0b),vget_high_s32(mmtmpD0b)),
             vpadd_s32(vget_low_s32(mmtmpD1b),vget_high_s32(mmtmpD1b)));

  mmtmpD0 = vqshlq_s32(mmtmpD0,output_shift128);
  mmtmpD1 = vqshlq_s32(mmtmpD1,output_shift128);
  rho128[1] = vzip_s16(vmovn_s32(mmtmpD0),vmovn_s32(mmtmpD1));

  mmtmpD0 = vmull_s16(ul_ch128[0], ul_ch128_2[0]);
  mmtmpD1 = vmull_s16(ul_ch128[1], ul_ch128_2[1]);
  mmtmpD0 = vcombine_s32(vpadd_s32(vget_low_s32(mmtmpD0),vget_high_s32(mmtmpD0)),
             vpadd_s32(vget_low_s32(mmtmpD1),vget_high_s32(mmtmpD1)));
  mmtmpD0b = vmull_s16(vrev32_s16(vmul_s16(ul_ch128[4],*(int16x4_t*)conj)), ul_ch128_2[4]);
  mmtmpD1b = vmull_s16(vrev32_s16(vmul_s16(ul_ch128[5],*(int16x4_t*)conj)), ul_ch128_2[5]);
  mmtmpD1 = vcombine_s32(vpadd_s32(vget_low_s32(mmtmpD0b),vget_high_s32(mmtmpD0b)),
             vpadd_s32(vget_low_s32(mmtmpD1b),vget_high_s32(mmtmpD1b)));

  mmtmpD0 = vqshlq_s32(mmtmpD0,output_shift128);
  mmtmpD1 = vqshlq_s32(mmtmpD1,output_shift128);
  rho128[2] = vzip_s16(vmovn_s32(mmtmpD0),vmovn_s32(mmtmpD1));


  ul_ch128+=6;
  ul_ch128_2+=6;
  rho128+=3;
      }
    }
  }
#endif


#ifdef DEBUG_CH_COMP

  for (int ant=0; ant<frame_parms->nb_antennas_rx; ant++) {
    rxF   = (int16_t *)&rxdataF_comp[ant][(symbol*(off+(nb_rb*12)))];

    printf("----------------After compansation------------------\n");

    for (prnt_idx=0;prnt_idx<12*5*2;prnt_idx+=2){

      printf("rxF[%d] = (%d,%d)\n", prnt_idx>>1, rxF[prnt_idx],rxF[prnt_idx+1]);

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
			    uint8_t symbol,
			    uint16_t nb_rb) {
  int n_rx = frame_parms->nb_antennas_rx;
#if defined(__x86_64__) || defined(__i386__)
#ifdef __AVX2__
  __m256i *rxdataF_comp256[1+n_rx],*ul_ch_mag256[1+n_rx],*ul_ch_mag256b[1+n_rx];
#else
  __m128i *rxdataF_comp128[1+n_rx],*ul_ch_mag128[1+n_rx],*ul_ch_mag128b[1+n_rx];
#endif
#elif defined(__arm__)
  int16x8_t *rxdataF_comp128_0,*ul_ch_mag128_0,*ul_ch_mag128_0b;
  int16x8_t *rxdataF_comp128_1,*ul_ch_mag128_1,*ul_ch_mag128_1b;
#endif
  int32_t i;

#ifdef __AVX2__
  int off = ((nb_rb&1) == 1)? 4:0;
#else
  int off = 0;
#endif

  if (frame_parms->nb_antennas_rx>1) {
#if defined(__x86_64__) || defined(__i386__)
    int nb_re = nb_rb*12;
    if ((nb_re&7) > 0) nb_re+=8;
    for (int aa=0;aa<frame_parms->nb_antennas_rx;aa++) {
#ifdef __AVX2__
      rxdataF_comp256[aa]   = (__m256i *)&rxdataF_comp[aa][(symbol*(nb_re + off))];
      ul_ch_mag256[aa]      = (__m256i *)&ul_ch_mag[aa][(symbol*(nb_re + off))];
      ul_ch_mag256b[aa]     = (__m256i *)&ul_ch_magb[aa][(symbol*(nb_re + off))];
#else
      rxdataF_comp128[aa]   = (__m128i *)&rxdataF_comp[aa][(symbol*(nb_re + off))];
      ul_ch_mag128[aa]      = (__m128i *)&ul_ch_mag[aa][(symbol*(nb_re + off))];
      ul_ch_mag128b[aa]     = (__m128i *)&ul_ch_magb[aa][(symbol*(nb_re + off))];
#endif
    }
    for (int aa=1;aa<frame_parms->nb_antennas_rx;aa++) {      
      // MRC on each re of rb, both on MF output and magnitude (for 16QAM/64QAM llr computation)
#ifdef __AVX2__
      for (i=0; i<nb_re>>3; i++) {
	rxdataF_comp256[0][i] = _mm256_adds_epi16(rxdataF_comp256[0][i],rxdataF_comp256[aa][i]);
	ul_ch_mag256[0][i]    = _mm256_adds_epi16(ul_ch_mag256[0][i], ul_ch_mag256[aa][i]);
	ul_ch_mag256b[0][i]   = _mm256_adds_epi16(ul_ch_mag256b[0][i],ul_ch_mag256b[aa][i]);
      }

#else
      for (i=0; i<nb_rb*3; i++) {
	rxdataF_comp128[0][i] = _mm_adds_epi16(rxdataF_comp128[0][i],rxdataF_comp128[aa][i]);
	ul_ch_mag128[0][i]    = _mm_adds_epi16(ul_ch_mag128[0][i], ul_ch_mag128[aa][i]);
	ul_ch_mag128b[0][i]   = _mm_adds_epi16(ul_ch_mag128b[0][i],ul_ch_mag128b[aa][i]);
      }
#endif
    }
#elif defined(__arm__)
    rxdataF_comp128_0   = (int16x8_t *)&rxdataF_comp[0][symbol*frame_parms->N_RB_DL*12];
    rxdataF_comp128_1   = (int16x8_t *)&rxdataF_comp[1][symbol*frame_parms->N_RB_DL*12];
    ul_ch_mag128_0      = (int16x8_t *)&ul_ch_mag[0][symbol*frame_parms->N_RB_DL*12];
    ul_ch_mag128_1      = (int16x8_t *)&ul_ch_mag[1][symbol*frame_parms->N_RB_DL*12];
    ul_ch_mag128_0b     = (int16x8_t *)&ul_ch_magb[0][symbol*frame_parms->N_RB_DL*12];
    ul_ch_mag128_1b     = (int16x8_t *)&ul_ch_magb[1][symbol*frame_parms->N_RB_DL*12];
      
    // MRC on each re of rb, both on MF output and magnitude (for 16QAM/64QAM llr computation)
    for (i=0; i<nb_rb*3; i++) {
      rxdataF_comp128_0[i] = vhaddq_s16(rxdataF_comp128_0[i],rxdataF_comp128_1[i]);
      ul_ch_mag128_0[i]    = vhaddq_s16(ul_ch_mag128_0[i],ul_ch_mag128_1[i]);
      ul_ch_mag128_0b[i]   = vhaddq_s16(ul_ch_mag128_0b[i],ul_ch_mag128_1b[i]);
      rxdataF_comp128_0[i] = vqaddq_s16(rxdataF_comp128_0[i],(*(int16x8_t *)&jitterc[0]));
    }
#endif
  }

#if defined(__x86_64__) || defined(__i386__)
  _mm_empty();
  _m_empty();
#endif
}

<<<<<<< HEAD
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
=======
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
  stop_meas(&gNB->ulsch_channel_estimation_stats);

#ifdef __AVX2__
  int off = ((rel15_ul->rb_size&1) == 1)? 4:0;
#else
  int off = 0;
#endif
  uint32_t rxdataF_ext_offset = 0;

  for(uint8_t symbol = rel15_ul->start_symbol_index; symbol < (rel15_ul->start_symbol_index + rel15_ul->nr_of_symbols); symbol++) {
    uint8_t dmrs_symbol_flag = (rel15_ul->ul_dmrs_symb_pos >> symbol) & 0x01;
>>>>>>> origin/ldpc-decoder-codegen
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
        nr_ulsch_extract_rbs_single(gNB->common_vars.rxdataF,
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
                            rel15_ul->rb_size,
                            rel15_ul->dmrs_config_type);


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
                             symbol,
                             rel15_ul->rb_size);
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
    }
}

int nr_rx_pusch(PHY_VARS_gNB *gNB,
                uint8_t ulsch_id,
                uint32_t frame,
                uint8_t slot,
                unsigned char harq_pid)
{

  uint8_t aarx, aatx;
  uint32_t bwp_start_subcarrier;

  NR_DL_FRAME_PARMS *frame_parms = &gNB->frame_parms;
  nfapi_nr_pusch_pdu_t *rel15_ul = &gNB->ulsch[ulsch_id][0]->harq_processes[harq_pid]->ulsch_pdu;


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
  stop_meas(&gNB->ulsch_channel_estimation_stats);

  start_meas(&gNB->rx_pusch_init_stats);
  void (*nr_pusch_symbol_processing_ptr)(void*) = &nr_pusch_symbol_processing;

  // first the computation of channel levels

  
  int nb_re_pusch=0,meas_symbol=-1;
  for(meas_symbol = rel15_ul->start_symbol_index; 
      meas_symbol < (rel15_ul->start_symbol_index + rel15_ul->nr_of_symbols); 
      meas_symbol++) 
    if ((nb_re_pusch = get_nb_re_pusch(frame_parms,rel15_ul,meas_symbol)) > 0) break;

  AssertFatal(nb_re_pusch>0 && meas_symbol>=0,"nb_re_pusch %d cannot be 0 or meas_symbol %d cannot be negative here\n",nb_re_pusch,meas_symbol);


  start_meas(&gNB->ulsch_rbs_extraction_stats);
  nr_ulsch_extract_rbs_single(gNB->common_vars.rxdataF,
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
  start_meas(&gNB->rx_pusch_symbol_processing_stats);
  for(uint8_t symbol = rel15_ul->start_symbol_index; 
      symbol < (rel15_ul->start_symbol_index + rel15_ul->nr_of_symbols); 
      symbol++) {

    gNB->pusch_vars[ulsch_id]->ul_valid_re_per_slot[symbol] = get_nb_re_pusch(frame_parms,rel15_ul,symbol);
    gNB->pusch_vars[ulsch_id]->llr_offset[symbol] = (symbol==rel15_ul->start_symbol_index) ? 
                                                    0 : 
                                                    gNB->pusch_vars[ulsch_id]->llr_offset[symbol-1] + gNB->pusch_vars[ulsch_id]->ul_valid_re_per_slot[symbol-1] * rel15_ul->qam_mod_order;
    if (gNB->pusch_vars[ulsch_id]->ul_valid_re_per_slot[symbol] > 0)  {
      union puschSymbolReqUnion id = {.s={ulsch_id,frame,slot,0}};
      id.p=1+symbol;
      notifiedFIFO_elt_t *req=newNotifiedFIFO_elt(sizeof(puschSymbolProc_t),id.p,gNB->respPuschSymb,nr_pusch_symbol_processing_ptr);
      puschSymbolProc_t *rdata=(puschSymbolProc_t*)NotifiedFifoData(req);
      rdata->gNB = gNB;
      rdata->frame_parms=frame_parms;
      rdata->rel15_ul = rel15_ul;
      rdata->slot = slot;
      rdata->symbol = symbol;
      rdata->ulsch_id=ulsch_id;
      pushTpool(gNB->threadPool,req);
      gNB->nbSymb++;
      LOG_D(PHY,"%d.%d Added symbol %d (count %d) to process, in pipe\n",frame,slot,symbol,gNB->nbSymb);
    }
  } // symbol loop

  while (gNB->nbSymb > 0) {
    notifiedFIFO_elt_t *req=pullTpool(gNB->respPuschSymb, gNB->threadPool);
    gNB->nbSymb--;
    delNotifiedFIFO_elt(req);
  }
  stop_meas(&gNB->rx_pusch_symbol_processing_stats);
  return 0;
}
