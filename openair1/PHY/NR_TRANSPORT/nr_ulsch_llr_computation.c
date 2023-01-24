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

/*! \file PHY/NR_TRANSPORT/nr_ulsch_llr_computation.c
 * \brief Top-level routines for LLR computation of the PDSCH physical channel
 * \author Ahmed Hussein
 * \date 2019
 * \version 0.1
 * \company Fraunhofer IIS
 * \email: ahmed.hussein@iis.fraunhofer.de
 * \note
 * \warning
 */

#include "PHY/defs_nr_common.h"
#include "PHY/sse_intrin.h"
#include "PHY/impl_defs_top.h"



//----------------------------------------------------------------------------------------------
// QPSK
//----------------------------------------------------------------------------------------------
void nr_ulsch_qpsk_llr(int32_t *rxdataF_comp,
                      int16_t  *ulsch_llr,
                      uint32_t nb_re,
                      uint8_t  symbol)
{
  c16_t *rxF   = (c16_t *)rxdataF_comp;
  c16_t *llr32 = (c16_t *)ulsch_llr;

  if (!llr32) {
    LOG_E(PHY,"nr_ulsch_qpsk_llr: llr is null, symbol %d, llr32 = %p\n",symbol, llr32);
  }
  for (int i = 0; i < nb_re; i++) {
    //*llr32 = *rxF;
    llr32->r = rxF->r >> 3;
    llr32->i = rxF->i >> 3;
    rxF++;
    llr32++;
  }
}

//----------------------------------------------------------------------------------------------
// 16-QAM
//----------------------------------------------------------------------------------------------

void nr_ulsch_16qam_llr(int32_t *rxdataF_comp,
                        int32_t *ul_ch_mag,
                        int16_t  *ulsch_llr,
                        uint32_t nb_rb,
                        uint32_t nb_re,
                        uint8_t  symbol)
{

  simde__m256i *rxF = (simde__m256i*)rxdataF_comp;
  simde__m256i *ch_mag;
  simde__m256i llr256[2];
  register simde__m256i xmm0;
  uint32_t *llr32;
  int i;

  int off = ((nb_rb&1) == 1)? 4:0;

  llr32 = (uint32_t*)ulsch_llr;

  ch_mag = (simde__m256i*)&ul_ch_mag[(symbol*(off+(nb_rb*12)))];
  unsigned char len_mod8 = nb_re&7;
  nb_re >>= 3;  // length in quad words (4 REs)
  nb_re += (len_mod8 == 0 ? 0 : 1);

  for (i=0; i<nb_re; i++) {
    xmm0 = simde_mm256_abs_epi16(rxF[i]); // registers of even index in xmm0-> |y_R|, registers of odd index in xmm0-> |y_I|
    xmm0 = simde_mm256_subs_epi16(ch_mag[i],xmm0); // registers of even index in xmm0-> |y_R|-|h|^2, registers of odd index in xmm0-> |y_I|-|h|^2
 
    llr256[0] = simde_mm256_unpacklo_epi32(rxF[i],xmm0); // llr128[0] contains the llrs of the 1st,2nd,5th and 6th REs
    llr256[1] = simde_mm256_unpackhi_epi32(rxF[i],xmm0); // llr128[1] contains the llrs of the 3rd, 4th, 7th and 8th REs
    
    // 1st RE
    llr32[0] = simde_mm256_extract_epi32(llr256[0],0); // llr32[0] low 16 bits-> y_R        , high 16 bits-> y_I
    llr32[1] = simde_mm256_extract_epi32(llr256[0],1); // llr32[1] low 16 bits-> |h|-|y_R|^2, high 16 bits-> |h|-|y_I|^2

    // 2nd RE
    llr32[2] = simde_mm256_extract_epi32(llr256[0],2); // llr32[2] low 16 bits-> y_R        , high 16 bits-> y_I
    llr32[3] = simde_mm256_extract_epi32(llr256[0],3); // llr32[3] low 16 bits-> |h|-|y_R|^2, high 16 bits-> |h|-|y_I|^2

    // 3rd RE
    llr32[4] = simde_mm256_extract_epi32(llr256[1],0); // llr32[4] low 16 bits-> y_R        , high 16 bits-> y_I
    llr32[5] = simde_mm256_extract_epi32(llr256[1],1); // llr32[5] low 16 bits-> |h|-|y_R|^2, high 16 bits-> |h|-|y_I|^2

    // 4th RE
    llr32[6] = simde_mm256_extract_epi32(llr256[1],2); // llr32[6] low 16 bits-> y_R        , high 16 bits-> y_I
    llr32[7] = simde_mm256_extract_epi32(llr256[1],3); // llr32[7] low 16 bits-> |h|-|y_R|^2, high 16 bits-> |h|-|y_I|^2

    // 5th RE
    llr32[8] = simde_mm256_extract_epi32(llr256[0],4); // llr32[8] low 16 bits-> y_R        , high 16 bits-> y_I
    llr32[9] = simde_mm256_extract_epi32(llr256[0],5); // llr32[9] low 16 bits-> |h|-|y_R|^2, high 16 bits-> |h|-|y_I|^2

    // 6th RE
    llr32[10] = simde_mm256_extract_epi32(llr256[0],6); // llr32[10] low 16 bits-> y_R        , high 16 bits-> y_I
    llr32[11] = simde_mm256_extract_epi32(llr256[0],7); // llr32[11] low 16 bits-> |h|-|y_R|^2, high 16 bits-> |h|-|y_I|^2

    // 7th RE
    llr32[12] = simde_mm256_extract_epi32(llr256[1],4); // llr32[12] low 16 bits-> y_R        , high 16 bits-> y_I
    llr32[13] = simde_mm256_extract_epi32(llr256[1],5); // llr32[13] low 16 bits-> |h|-|y_R|^2, high 16 bits-> |h|-|y_I|^2

    // 8th RE
    llr32[14] = simde_mm256_extract_epi32(llr256[1],6); // llr32[14] low 16 bits-> y_R        , high 16 bits-> y_I
    llr32[15] = simde_mm256_extract_epi32(llr256[1],7); // llr32[15] low 16 bits-> |h|-|y_R|^2, high 16 bits-> |h|-|y_I|^2

    llr32+=16;

  }

  _mm_empty();
  _m_empty();
}

//----------------------------------------------------------------------------------------------
// 64-QAM
//----------------------------------------------------------------------------------------------

void nr_ulsch_64qam_llr(int32_t *rxdataF_comp,
                        int32_t *ul_ch_mag,
                        int32_t *ul_ch_magb,
                        int16_t  *ulsch_llr,
                        uint32_t nb_rb,
                        uint32_t nb_re,
                        uint8_t  symbol)
{
  int off = ((nb_rb&1) == 1)? 4:0;

  simde__m256i *rxF = (simde__m256i*)rxdataF_comp;
  simde__m256i *ch_mag,*ch_magb;
  register simde__m256i xmm0,xmm1,xmm2;

  int i;

  ch_mag = (simde__m256i*)&ul_ch_mag[(symbol*(off+(nb_rb*12)))];
  ch_magb = (simde__m256i*)&ul_ch_magb[(symbol*(off+(nb_rb*12)))];

  int len_mod8 = nb_re&7;
  nb_re    = nb_re>>3;  // length in 256-bit words (8 REs)
  nb_re   += ((len_mod8 == 0) ? 0 : 1);
  int32_t *ulsch_llr32=(int32_t*)ulsch_llr;

  for (i=0; i<nb_re; i++) {
    xmm0 = rxF[i];
    xmm1 = simde_mm256_abs_epi16(xmm0);
    xmm1 = simde_mm256_subs_epi16(ch_mag[i],xmm1);
    xmm2 = simde_mm256_abs_epi16(xmm1);
    xmm2 = simde_mm256_subs_epi16(ch_magb[i],xmm2);
    
    ulsch_llr32[0] = simde_mm256_extract_epi32(xmm0,0);
    ulsch_llr32[1] = simde_mm256_extract_epi32(xmm1,0);
    ulsch_llr32[2] = simde_mm256_extract_epi32(xmm2,0);

    ulsch_llr32[3] = simde_mm256_extract_epi32(xmm0,1);
    ulsch_llr32[4] = simde_mm256_extract_epi32(xmm1,1);
    ulsch_llr32[5] = simde_mm256_extract_epi32(xmm2,1);

    ulsch_llr32[6] = simde_mm256_extract_epi32(xmm0,2);
    ulsch_llr32[7] = simde_mm256_extract_epi32(xmm1,2);
    ulsch_llr32[8] = simde_mm256_extract_epi32(xmm2,2);

    ulsch_llr32[9] = simde_mm256_extract_epi32(xmm0,3);
    ulsch_llr32[10] = simde_mm256_extract_epi32(xmm1,3);
    ulsch_llr32[11] = simde_mm256_extract_epi32(xmm2,3);

    ulsch_llr32[12] = simde_mm256_extract_epi32(xmm0,4);
    ulsch_llr32[13] = simde_mm256_extract_epi32(xmm1,4);
    ulsch_llr32[14] = simde_mm256_extract_epi32(xmm2,4);

    ulsch_llr32[15] = simde_mm256_extract_epi32(xmm0,5);
    ulsch_llr32[16] = simde_mm256_extract_epi32(xmm1,5);
    ulsch_llr32[17] = simde_mm256_extract_epi32(xmm2,5);

    ulsch_llr32[18] = simde_mm256_extract_epi32(xmm0,6);
    ulsch_llr32[19] = simde_mm256_extract_epi32(xmm1,6);
    ulsch_llr32[20] = simde_mm256_extract_epi32(xmm2,6);

    ulsch_llr32[21] = simde_mm256_extract_epi32(xmm0,7);
    ulsch_llr32[22] = simde_mm256_extract_epi32(xmm1,7);
    ulsch_llr32[23] = simde_mm256_extract_epi32(xmm2,7);

    ulsch_llr32+=24;
  }

  _mm_empty();
  _m_empty();
}


void nr_ulsch_256qam_llr(int32_t *rxdataF_comp,
                         int32_t *ul_ch_mag,
                         int32_t *ul_ch_magb,
                         int32_t *ul_ch_magc,
                         int16_t  *ulsch_llr,
                         uint32_t nb_rb,
                         uint32_t nb_re,
                         uint8_t  symbol)
{
  int off = ((nb_rb&1) == 1)? 4:0;

  simde__m256i *rxF = (simde__m256i*)rxdataF_comp;
  simde__m256i *ch_mag,*ch_magb,*ch_magc;
  register simde__m256i xmm0,xmm1,xmm2,xmm3,xmm4,xmm5,xmm6;
  simde__m256i *llr256=(simde__m256i*)ulsch_llr;

  ch_mag  = (simde__m256i*)&ul_ch_mag[(symbol*(off+(nb_rb*12)))];
  ch_magb = (simde__m256i*)&ul_ch_magb[(symbol*(off+(nb_rb*12)))];
  ch_magc = (simde__m256i*)&ul_ch_magc[(symbol*(off+(nb_rb*12)))];
  int len_mod8 = nb_re&7;
  int nb_re256    = nb_re>>3;  // length in 256-bit words (8 REs)

  for (int i=0; i<nb_re256; i++) {
        xmm0 = simde_mm256_abs_epi16(rxF[i]); // registers of even index in xmm0-> |y_R|, registers of odd index in xmm0-> |y_I|
	xmm0 = simde_mm256_subs_epi16(ch_mag[i],xmm0); // registers of even index in xmm0-> |y_R|-|h|^2, registers of odd index in xmm0-> |y_I|-|h|^2
      //  xmmtmpD2 contains 16 LLRs
        xmm1 = simde_mm256_abs_epi16(xmm0);
        xmm1 = simde_mm256_subs_epi16(ch_magb[i],xmm1); // contains 16 LLRs
	xmm2 = simde_mm256_abs_epi16(xmm1);
	xmm2 = simde_mm256_subs_epi16(ch_magc[i],xmm2); // contains 16 LLRs
        // rxF[i] A0 A1 A2 A3 A4 A5 A6 A7 bits 7,6
        // xmm0   B0 B1 B2 B3 B4 B5 B6 B7 bits 5,4
	// xmm1   C0 C1 C2 C3 C4 C5 C6 C7 bits 3,2
	// xmm2   D0 D1 D2 D3 D4 D5 D6 D7 bits 1,0
	xmm3 = simde_mm256_unpacklo_epi32(rxF[i],xmm0); // A0 B0 A1 B1 A4 B4 A5 B5
	xmm4 = simde_mm256_unpackhi_epi32(rxF[i],xmm0); // A2 B2 A3 B3 A6 B6 A7 B7
	xmm5 = simde_mm256_unpacklo_epi32(xmm1,xmm2);   // C0 D0 C1 D1 C4 D4 C5 D5
	xmm6 = simde_mm256_unpackhi_epi32(xmm1,xmm2);   // C2 D2 C3 D3 C6 D6 C7 D7

        xmm0 = simde_mm256_unpacklo_epi64(xmm3,xmm5); // A0 B0 C0 D0 A4 B4 C4 D4
        xmm1 = simde_mm256_unpackhi_epi64(xmm3,xmm5); // A1 B1 C1 D1 A5 B5 C5 D5
        xmm2 = simde_mm256_unpacklo_epi64(xmm4,xmm6); // A2 B2 C2 D2 A6 B6 C6 D6
        xmm3 = simde_mm256_unpackhi_epi64(xmm4,xmm6); // A3 B3 C3 D3 A7 B7 C7 D7
	llr256[0] = simde_mm256_permute2x128_si256(xmm0, xmm1, 0x20); // A0 B0 C0 D0 A1 B1 C1 D1
	llr256[1] = simde_mm256_permute2x128_si256(xmm2, xmm3, 0x20); // A2 B2 C2 D2 A3 B3 C3 D3
	llr256[2] = simde_mm256_permute2x128_si256(xmm0, xmm1, 0x31); // A4 B4 C4 D4 A5 B5 C5 D5
	llr256[3] = simde_mm256_permute2x128_si256(xmm2, xmm3, 0x31); // A6 B6 C6 D6 A7 B7 C7 D7
        llr256+=4;

  }
  simde__m128i *llr128 = (simde__m128i*)llr256;	  
  if (len_mod8 >= 4) {
     int nb_re128 = nb_re>>2;	   
     simde__m128i xmm0,xmm1,xmm2,xmm3,xmm4,xmm5,xmm6;
     simde__m128i *rxF = (simde__m128i*)rxdataF_comp;
     simde__m128i *ch_mag  = (simde__m128i*)&ul_ch_mag[(symbol*(off+(nb_rb*12)))];
     simde__m128i *ch_magb = (simde__m128i*)&ul_ch_magb[(symbol*(off+(nb_rb*12)))];
     simde__m128i *ch_magc = (simde__m128i*)&ul_ch_magc[(symbol*(off+(nb_rb*12)))];

     xmm0 = simde_mm_abs_epi16(rxF[nb_re128-1]); // registers of even index in xmm0-> |y_R|, registers of odd index in xmm0-> |y_I|
     xmm0 = simde_mm_subs_epi16(ch_mag[nb_re128-1],xmm0); // registers of even index in xmm0-> |y_R|-|h|^2, registers of odd index in xmm0-> |y_I|-|h|^2
      //  xmmtmpD2 contains 8 LLRs
     xmm1 = simde_mm_abs_epi16(xmm0);
     xmm1 = simde_mm_subs_epi16(ch_magb[nb_re128-1],xmm1); // contains 8 LLRs
     xmm2 = simde_mm_abs_epi16(xmm1);
     xmm2 = simde_mm_subs_epi16(ch_magc[nb_re128-1],xmm2); // contains 8 LLRs
     // rxF[i] A0 A1 A2 A3
     // xmm0   B0 B1 B2 B3
     // xmm1   C0 C1 C2 C3
     // xmm2   D0 D1 D2 D3
     xmm3 = simde_mm_unpacklo_epi32(rxF[nb_re128-1],xmm0); // A0 B0 A1 B1 
     xmm4 = simde_mm_unpackhi_epi32(rxF[nb_re128-1],xmm0); // A2 B2 A3 B3
     xmm5 = simde_mm_unpacklo_epi32(xmm1,xmm2);   // C0 D0 C1 D1
     xmm6 = simde_mm_unpackhi_epi32(xmm1,xmm2);   // C2 D2 C3 D3

     llr128[0] = simde_mm_unpacklo_epi64(xmm3,xmm5); // A0 B0 C0 D0
     llr128[1] = simde_mm_unpackhi_epi64(xmm3,xmm5); // A1 B1 C1 D1 
     llr128[2] = simde_mm_unpacklo_epi64(xmm4,xmm6); // A2 B2 C2 D2 
     llr128[3] = simde_mm_unpackhi_epi64(xmm4,xmm6); // A3 B3 C3 D3
     llr128+=4;
  }
  if (len_mod8 == 6) {
     int nb_re64 = nb_re>>1;	   
     simde__m64 *llr64 = (simde__m64 *)llr128; 
     simde__m64 xmm0,xmm1,xmm2;
     simde__m64 *rxF = (simde__m64*)rxdataF_comp;
     simde__m64 *ch_mag  = (simde__m64*)&ul_ch_mag[(symbol*(off+(nb_rb*12)))];
     simde__m64 *ch_magb = (simde__m64*)&ul_ch_magb[(symbol*(off+(nb_rb*12)))];
     simde__m64 *ch_magc = (simde__m64*)&ul_ch_magc[(symbol*(off+(nb_rb*12)))];

     xmm0 = simde_mm_abs_pi16(rxF[nb_re64-1]); // registers of even index in xmm0-> |y_R|, registers of odd index in xmm0-> |y_I|
     xmm0 = simde_mm_subs_pi16(ch_mag[nb_re-1],xmm0); // registers of even index in xmm0-> |y_R|-|h|^2, registers of odd index in xmm0-> |y_I|-|h|^2
      //  xmmtmpD2 contains 4 LLRs
     xmm1 = simde_mm_abs_pi16(xmm0);
     xmm1 = simde_mm_subs_pi16(ch_magb[nb_re64-1],xmm1); // contains 4 LLRs
     xmm2 = simde_mm_abs_pi16(xmm1);
     xmm2 = simde_mm_subs_pi16(ch_magc[nb_re64-1],xmm2); // contains 4 LLRs
     // rxF[i] A0 A1
     // xmm0   B0 B1
     // xmm1   C0 C1
     // xmm2   D0 D1
     llr64[0] = simde_m_punpckldq(rxF[nb_re64-1],xmm0); // A0 B0 
     llr64[2] = simde_m_punpckhdq(rxF[nb_re64-1],xmm0);  // A1 B1
     llr64[1] = simde_m_punpckldq(xmm1,xmm2);         // C0 D0
     llr64[3] = simde_m_punpckhdq(xmm1,xmm2);         // C1 D1
  }

}
void nr_ulsch_compute_llr(int32_t *rxdataF_comp,
                          int32_t *ul_ch_mag,
                          int32_t *ul_ch_magb,
                          int32_t *ul_ch_magc,
                          int16_t *ulsch_llr,
                          uint32_t nb_rb,
                          uint32_t nb_re,
                          uint8_t  symbol,
                          uint8_t  mod_order)
{
  switch(mod_order){
    case 2:
      nr_ulsch_qpsk_llr(rxdataF_comp,
                        ulsch_llr,
                        nb_re,
                        symbol);
      break;
    case 4:
      nr_ulsch_16qam_llr(rxdataF_comp,
                         ul_ch_mag,
                         ulsch_llr,
                         nb_rb,
                         nb_re,
                         symbol);
      break;
    case 6:
    nr_ulsch_64qam_llr(rxdataF_comp,
                       ul_ch_mag,
                       ul_ch_magb,
                       ulsch_llr,
                       nb_rb,
                       nb_re,
                       symbol);
      break;
    case 8:
    nr_ulsch_256qam_llr(rxdataF_comp,
                        ul_ch_mag,
                        ul_ch_magb,
			ul_ch_magc,
                        ulsch_llr,
                        nb_rb,
                        nb_re,
                        symbol);
      break;
    default:
      LOG_E(PHY,"nr_ulsch_compute_llr: invalid Qm value, symbol = %d, Qm = %d\n",symbol, mod_order);
      break;
  }
}
