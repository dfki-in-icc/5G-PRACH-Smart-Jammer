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

/*! \file PHY/sse_intrin.h
 * \brief SSE includes and compatibility functions.
 *
 * This header collects all SSE compatibility functions. To use SSE inside a source file, include only sse_intrin.h.
 * The host CPU needs to have support for SSE2 at least. SSE3 and SSE4.1 functions are emulated if the CPU lacks support for them.
 * This will slow down the softmodem, but may be valuable if only offline signal processing is required.
 *
 * Has been revamped in July 2022 to rely on SIMD Everywhere (simde) by bruno.mongazon-cazavet@nokia-bell-labs.com
 *
 * \author S. Held, Laurent THOMAS
 * \email sebastian.held@imst.de, laurent.thomas@open-cells.com	
 * \company IMST GmbH, Open Cells Project
 * \date 2019
 * \version 0.2
*/

#ifndef SSE_INTRIN_H
#define SSE_INTRIN_H


#if defined(__x86_64) || defined(__i386__)

/* x86 processors */

#define SIMDE_X86_AVX2_ENABLE_NATIVE_ALIASES
#include <simde/x86/mmx.h>
#include <simde/x86/sse.h>
#include <simde/x86/sse2.h>
#include <simde/x86/sse3.h>
#include <simde/x86/ssse3.h>
#include <simde/x86/sse4.1.h>
#include <simde/x86/sse4.2.h>
#include <simde/x86/avx2.h>
#include <simde/x86/fma.h>

/* SIMDE requires inclusion of individual files */
#define SIMDE_X86_AVX512BW_ENABLE_NATIVE_ALIASES
#include <simde/x86/avx512/2intersect.h>
#include <simde/x86/avx512/4dpwssd.h>
#include <simde/x86/avx512/4dpwssds.h>
#include <simde/x86/avx512/abs.h>
#include <simde/x86/avx512/add.h>
#include <simde/x86/avx512/adds.h>
#include <simde/x86/avx512/and.h>
#include <simde/x86/avx512/andnot.h>
#include <simde/x86/avx512/avg.h>
#include <simde/x86/avx512/bitshuffle.h>
#include <simde/x86/avx512/blend.h>
#include <simde/x86/avx512/broadcast.h>
#include <simde/x86/avx512/cast.h>
#include <simde/x86/avx512/cmpeq.h>
#include <simde/x86/avx512/cmpge.h>
#include <simde/x86/avx512/cmpgt.h>
#include <simde/x86/avx512/cmp.h>
#include <simde/x86/avx512/cmple.h>
#include <simde/x86/avx512/cmplt.h>
#include <simde/x86/avx512/cmpneq.h>
#include <simde/x86/avx512/compress.h>
#include <simde/x86/avx512/conflict.h>
#include <simde/x86/avx512/copysign.h>
#include <simde/x86/avx512/cvt.h>
#include <simde/x86/avx512/cvts.h>
#include <simde/x86/avx512/cvtt.h>
#include <simde/x86/avx512/dbsad.h>
#include <simde/x86/avx512/div.h>
#include <simde/x86/avx512/dpbf16.h>
#include <simde/x86/avx512/dpbusd.h>
#include <simde/x86/avx512/dpbusds.h>
#include <simde/x86/avx512/dpwssd.h>
#include <simde/x86/avx512/dpwssds.h>
#include <simde/x86/avx512/expand.h>
#include <simde/x86/avx512/extract.h>
#include <simde/x86/avx512/fixupimm.h>
#include <simde/x86/avx512/fixupimm_round.h>
#include <simde/x86/avx512/flushsubnormal.h>
#include <simde/x86/avx512/fmadd.h>
#include <simde/x86/avx512/fmsub.h>
#include <simde/x86/avx512/fnmadd.h>
#include <simde/x86/avx512/fnmsub.h>
#include <simde/x86/avx512/insert.h>
#include <simde/x86/avx512/kshift.h>
#include <simde/x86/avx512/load.h>
#include <simde/x86/avx512/loadu.h>
#include <simde/x86/avx512/lzcnt.h>
#include <simde/x86/avx512/madd.h>
#include <simde/x86/avx512/maddubs.h>
#include <simde/x86/avx512/max.h>
#include <simde/x86/avx512/min.h>
#include <simde/x86/avx512/mov.h>
#include <simde/x86/avx512/mov_mask.h>
#include <simde/x86/avx512/movm.h>
#include <simde/x86/avx512/mul.h>
#include <simde/x86/avx512/mulhi.h>
#include <simde/x86/avx512/mulhrs.h>
#include <simde/x86/avx512/mullo.h>
#include <simde/x86/avx512/multishift.h>
#include <simde/x86/avx512/negate.h>
#include <simde/x86/avx512/or.h>
#include <simde/x86/avx512/packs.h>
#include <simde/x86/avx512/packus.h>
#include <simde/x86/avx512/permutex2var.h>
#include <simde/x86/avx512/permutexvar.h>
#include <simde/x86/avx512/popcnt.h>
#include <simde/x86/avx512/range.h>
#include <simde/x86/avx512/range_round.h>
#include <simde/x86/avx512/rol.h>
#include <simde/x86/avx512/rolv.h>
#include <simde/x86/avx512/ror.h>
#include <simde/x86/avx512/rorv.h>
#include <simde/x86/avx512/round.h>
#include <simde/x86/avx512/roundscale.h>
#include <simde/x86/avx512/roundscale_round.h>
#include <simde/x86/avx512/sad.h>
#include <simde/x86/avx512/set1.h>
#include <simde/x86/avx512/set4.h>
#include <simde/x86/avx512/set.h>
#include <simde/x86/avx512/setone.h>
#include <simde/x86/avx512/setr4.h>
#include <simde/x86/avx512/setr.h>
#include <simde/x86/avx512/setzero.h>
#include <simde/x86/avx512/shldv.h>
#include <simde/x86/avx512/shuffle.h>
#include <simde/x86/avx512/sll.h>
#include <simde/x86/avx512/slli.h>
#include <simde/x86/avx512/sllv.h>
#include <simde/x86/avx512/sqrt.h>
#include <simde/x86/avx512/sra.h>
#include <simde/x86/avx512/srai.h>
#include <simde/x86/avx512/srav.h>
#include <simde/x86/avx512/srl.h>
#include <simde/x86/avx512/srli.h>
#include <simde/x86/avx512/srlv.h>
#include <simde/x86/avx512/store.h>
#include <simde/x86/avx512/storeu.h>
#include <simde/x86/avx512/sub.h>
#include <simde/x86/avx512/subs.h>
#include <simde/x86/avx512/ternarylogic.h>
#include <simde/x86/avx512/test.h>
#include <simde/x86/avx512/testn.h>
#include <simde/x86/avx512/types.h>
#include <simde/x86/avx512/unpackhi.h>
#include <simde/x86/avx512/unpacklo.h>
#include <simde/x86/avx512/xor.h>
#include <simde/x86/avx512/xorsign.h>

#elif defined(__arm__)

#define SIMDE_ARM_NEON_A64V8_ENABLE_NATIVE_ALIASES

#include <simde/arm/neon.h>

#endif // x86_64 || i386

/*
 * OAI specific
 */

#if defined(__x86_64__) || defined(__i386__)
  #define vect128 __m128i
#elif defined(__arm__)
  #define vect128 int16x8_t
#endif

static const short minusConjug128[8]__attribute__((aligned(16))) = {-1,1,-1,1,-1,1,-1,1};
static inline vect128 mulByConjugate128(vect128 *a, vect128 *b, int8_t output_shift) {

#if defined(__x86_64__) || defined(__i386__)
  vect128 realPart = _mm_madd_epi16(*a,*b);
  realPart = _mm_srai_epi32(realPart,output_shift);
  vect128 imagPart = _mm_shufflelo_epi16(*b,_MM_SHUFFLE(2,3,0,1));
  imagPart = _mm_shufflehi_epi16(imagPart,_MM_SHUFFLE(2,3,0,1));
  imagPart = _mm_sign_epi16(imagPart,*(vect128 *)minusConjug128);
  imagPart = _mm_madd_epi16(imagPart,*a);
  imagPart = _mm_srai_epi32(imagPart,output_shift);
  vect128 lowPart = _mm_unpacklo_epi32(realPart,imagPart);
  vect128 highPart = _mm_unpackhi_epi32(realPart,imagPart);
  return ( _mm_packs_epi32(lowPart,highPart));
#elif defined(__arm__)
  AssertFatal(false, "not developped\n");
#endif
}

#if defined(__x86_64__) || defined(__i386__)
#define displaySamples128(vect)  {\
    __m128i x=vect;                                       \
    printf("vector: %s = (%hd,%hd) (%hd,%hd) (%hd,%hd) (%hd,%hd)\n", #vect, \
           _mm_extract_epi16(x,0),                                  \
           _mm_extract_epi16(x,1),\
           _mm_extract_epi16(x,2),\
           _mm_extract_epi16(x,3),\
           _mm_extract_epi16(x,4),\
           _mm_extract_epi16(x,5),\
           _mm_extract_epi16(x,6),\
           _mm_extract_epi16(x,7));\
  }
#elif defined(__arm__)
  displaySamples128(vect) {}
//TBD
#endif
#endif // SSE_INTRIN_H
