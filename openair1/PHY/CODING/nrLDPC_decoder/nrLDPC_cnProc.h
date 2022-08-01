
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

/*!\file nrLDPC_cnProc.h
 * \brief Defines the functions for check node processing
 * \author Sebastian Wagner (TCL Communications) Email: <mailto:sebastian.wagner@tcl.com>
 * \date 30-09-2019
 * \version 1.0
 * \note
 * \warning
 */

#ifndef __NR_LDPC_DECODER_CNPROC__H__
#define __NR_LDPC_DECODER_CNPROC__H__

/**
   \brief Performs CN processing for BG2 on the CN processing buffer and stores the results in the CN processing results buffer.
   \param p_lut Pointer to decoder LUTs
   \param p_procBuf Pointer to processing buffers
   \param Z Lifting size
*/


#include "nrLDPC_cnProc_avx512.h"

/**
   \brief Performs parity check for BG1 on the CN processing buffer. Stops as soon as error is detected.
   \param p_lut Pointer to decoder LUTs
   \param Z Lifting size
   \return 32-bit parity check indicator
*/
static inline uint32_t nrLDPC_cnProcPc_BG1(t_nrLDPC_lut* p_lut, int8_t* cnProcBuf, int8_t* cnProcBufRes, uint16_t Z)
{
    const uint8_t*  lut_numCnInCnGroups   = p_lut->numCnInCnGroups;
    const uint32_t* lut_startAddrCnGroups = p_lut->startAddrCnGroups;

    __m256i* p_cnProcBuf;
    __m256i* p_cnProcBufRes;

    // Number of CNs in Groups
    uint32_t M;
    uint32_t i;
    uint32_t j;
    uint32_t pcRes = 0;
    uint32_t pcResSum = 0;
    uint32_t Mrem;
    uint32_t M32;

    __m256i ymm0, ymm1;

    // =====================================================================
    // Process group with 3 BNs

    if (lut_numCnInCnGroups[0] > 0)
    {
        // Reset results
        pcResSum = 0;

        // Number of CNs in group
        M = lut_numCnInCnGroups[0]*Z;
        // Remainder modulo 32
        Mrem = M&31;
        // Number of groups of 32 CNs for parallel processing
        // Ceil for values not divisible by 32
        M32 = (M + 31)>>5;

        // Set pointers to start of group 3
        p_cnProcBuf    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[0]];
        p_cnProcBufRes = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[0]];

        // Loop over CNs
        for (i=0; i<(M32-1); i++)
        {
            pcRes = 0;
            // Loop over every BN
            // Compute PC for 32 CNs at once
            for (j=0; j<3; j++)
            {
                // BN offset is units of (1*384/32) = 12
                ymm0 = p_cnProcBuf   [j*12 + i];
                ymm1 = p_cnProcBufRes[j*12 + i];

                // Add BN and input LLR, extract the sign bit
                // and add in GF(2) (xor)
                pcRes ^= _mm256_movemask_epi8(_mm256_adds_epi8(ymm0,ymm1));
            }

            // If no error pcRes should be 0
            pcResSum |= pcRes;
        }

        // Last 32 CNs might not be full valid 32 depending on Z
        pcRes = 0;
        // Loop over every BN
        // Compute PC for 32 CNs at once
        for (j=0; j<3; j++)
        {
            // BN offset is units of (1*384/32) = 12
            ymm0 = p_cnProcBuf   [j*12 + i];
            ymm1 = p_cnProcBufRes[j*12 + i];

            // Add BN and input LLR, extract the sign bit
            // and add in GF(2) (xor)
            pcRes ^= _mm256_movemask_epi8(_mm256_adds_epi8(ymm0,ymm1));
        }

        // If no error pcRes should be 0
        // Only use valid CNs
        pcResSum |= (pcRes&(0xFFFFFFFF>>(32-Mrem)));

        // If PC failed we can stop here
        if (pcResSum > 0)
        {
            return pcResSum;
        }
    }

    // =====================================================================
    // Process group with 4 BNs

    if (lut_numCnInCnGroups[1] > 0)
    {
        // Reset results
        pcResSum = 0;

        // Number of CNs in group
        M = lut_numCnInCnGroups[1]*Z;
        // Remainder modulo 32
        Mrem = M&31;
        // Number of groups of 32 CNs for parallel processing
        // Ceil for values not divisible by 32
        M32 = (M + 31)>>5;

        // Set pointers to start of group 4
        p_cnProcBuf    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[1]];
        p_cnProcBufRes = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[1]];

        // Loop over CNs
        for (i=0; i<(M32-1); i++)
        {
            pcRes = 0;
            // Loop over every BN
            // Compute PC for 32 CNs at once
            for (j=0; j<4; j++)
            {
                // BN offset is units of 5*384/32 = 60
                ymm0 = p_cnProcBuf   [j*60 + i];
                ymm1 = p_cnProcBufRes[j*60 + i];

                // Add BN and input LLR, extract the sign bit
                // and add in GF(2) (xor)
                pcRes ^= _mm256_movemask_epi8(_mm256_adds_epi8(ymm0,ymm1));
            }

            // If no error pcRes should be 0
            pcResSum |= pcRes;
        }

        // Last 32 CNs might not be full valid 32 depending on Z
        pcRes = 0;
        // Loop over every BN
        // Compute PC for 32 CNs at once
        for (j=0; j<4; j++)
        {
            // BN offset is units of 5*384/32 = 60
            ymm0 = p_cnProcBuf   [j*60 + i];
            ymm1 = p_cnProcBufRes[j*60 + i];

            // Add BN and input LLR, extract the sign bit
            // and add in GF(2) (xor)
            pcRes ^= _mm256_movemask_epi8(_mm256_adds_epi8(ymm0,ymm1));
        }

        // If no error pcRes should be 0
        // Only use valid CNs
        pcResSum |= (pcRes&(0xFFFFFFFF>>(32-Mrem)));

        // If PC failed we can stop here
        if (pcResSum > 0)
        {
            return pcResSum;
        }
    }

    // =====================================================================
    // Process group with 5 BNs

    if (lut_numCnInCnGroups[2] > 0)
    {
        // Reset results
        pcResSum = 0;

        // Number of CNs in group
        M = lut_numCnInCnGroups[2]*Z;
        // Remainder modulo 32
        Mrem = M&31;
        // Number of groups of 32 CNs for parallel processing
        // Ceil for values not divisible by 32
        M32 = (M + 31)>>5;

        // Set pointers to start of group 5
        p_cnProcBuf    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[2]];
        p_cnProcBufRes = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[2]];

        // Loop over CNs
        for (i=0; i<(M32-1); i++)
        {
            pcRes = 0;
            // Loop over every BN
            // Compute PC for 32 CNs at once
            for (j=0; j<5; j++)
            {
                // BN offset is units of 18*384/32 = 216
                ymm0 = p_cnProcBuf   [j*216 + i];
                ymm1 = p_cnProcBufRes[j*216 + i];

                // Add BN and input LLR, extract the sign bit
                // and add in GF(2) (xor)
                pcRes ^= _mm256_movemask_epi8(_mm256_adds_epi8(ymm0,ymm1));
            }

            // If no error pcRes should be 0
            pcResSum |= pcRes;
        }

        // Last 32 CNs might not be full valid 32 depending on Z
        pcRes = 0;

        // Loop over every BN
        // Compute PC for 32 CNs at once
        for (j=0; j<5; j++)
        {
            // BN offset is units of 18*384/32 = 216
            ymm0 = p_cnProcBuf   [j*216 + i];
            ymm1 = p_cnProcBufRes[j*216 + i];

            // Add BN and input LLR, extract the sign bit
            // and add in GF(2) (xor)
            pcRes ^= _mm256_movemask_epi8(_mm256_adds_epi8(ymm0,ymm1));
        }

        // If no error pcRes should be 0
        // Only use valid CNs
        pcResSum |= (pcRes&(0xFFFFFFFF>>(32-Mrem)));

        // If PC failed we can stop here
        if (pcResSum > 0)
        {
            return pcResSum;
        }
    }

    // =====================================================================
    // Process group with 6 BNs

    if (lut_numCnInCnGroups[3] > 0)
    {
        // Reset results
        pcResSum = 0;

        // Number of CNs in group
        M = lut_numCnInCnGroups[3]*Z;
        // Remainder modulo 32
        Mrem = M&31;
        // Number of groups of 32 CNs for parallel processing
        // Ceil for values not divisible by 32
        M32 = (M + 31)>>5;

        // Set pointers to start of group 6
        p_cnProcBuf    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[3]];
        p_cnProcBufRes = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[3]];

        // Loop over CNs
        for (i=0; i<(M32-1); i++)
        {
            pcRes = 0;
            // Loop over every BN
            // Compute PC for 32 CNs at once
            for (j=0; j<6; j++)
            {
                // BN offset is units of 8*384/32 = 96
                ymm0 = p_cnProcBuf   [j*96 + i];
                ymm1 = p_cnProcBufRes[j*96 + i];

                // Add BN and input LLR, extract the sign bit
                // and add in GF(2) (xor)
                pcRes ^= _mm256_movemask_epi8(_mm256_adds_epi8(ymm0,ymm1));
            }

            // If no error pcRes should be 0
            pcResSum |= pcRes;
        }

        // Last 32 CNs might not be full valid 32 depending on Z
        pcRes = 0;
        // Loop over every BN
        // Compute PC for 32 CNs at once
        for (j=0; j<6; j++)
        {
            // BN offset is units of 8*384/32 = 96
            ymm0 = p_cnProcBuf   [j*96 + i];
            ymm1 = p_cnProcBufRes[j*96 + i];

            // Add BN and input LLR, extract the sign bit
            // and add in GF(2) (xor)
            pcRes ^= _mm256_movemask_epi8(_mm256_adds_epi8(ymm0,ymm1));
        }

        // If no error pcRes should be 0
        // Only use valid CNs
        pcResSum |= (pcRes&(0xFFFFFFFF>>(32-Mrem)));

        // If PC failed we can stop here
        if (pcResSum > 0)
        {
            return pcResSum;
        }
    }

    // =====================================================================
    // Process group with 7 BNs

    if (lut_numCnInCnGroups[4] > 0)
    {
        // Reset results
        pcResSum = 0;

        // Number of CNs in group
        M = lut_numCnInCnGroups[4]*Z;
        // Remainder modulo 32
        Mrem = M&31;
        // Number of groups of 32 CNs for parallel processing
        // Ceil for values not divisible by 32
        M32 = (M + 31)>>5;

        // Set pointers to start of group 7
        p_cnProcBuf    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[4]];
        p_cnProcBufRes = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[4]];

        // Loop over CNs
        for (i=0; i<(M32-1); i++)
        {
            pcRes = 0;
            // Loop over every BN
            // Compute PC for 32 CNs at once
            for (j=0; j<7; j++)
            {
                // BN offset is units of 5*384/32 = 60
                ymm0 = p_cnProcBuf   [j*60 + i];
                ymm1 = p_cnProcBufRes[j*60 + i];

                // Add BN and input LLR, extract the sign bit
                // and add in GF(2) (xor)
                pcRes ^= _mm256_movemask_epi8(_mm256_adds_epi8(ymm0,ymm1));
            }

            // If no error pcRes should be 0
            pcResSum |= pcRes;
        }

        // Last 32 CNs might not be full valid 32 depending on Z
        pcRes = 0;
        // Loop over every BN
        // Compute PC for 32 CNs at once
        for (j=0; j<7; j++)
        {
            // BN offset is units of 5*384/32 = 60
            ymm0 = p_cnProcBuf   [j*60 + i];
            ymm1 = p_cnProcBufRes[j*60 + i];

            // Add BN and input LLR, extract the sign bit
            // and add in GF(2) (xor)
            pcRes ^= _mm256_movemask_epi8(_mm256_adds_epi8(ymm0,ymm1));
        }

        // If no error pcRes should be 0
        // Only use valid CNs
        pcResSum |= (pcRes&(0xFFFFFFFF>>(32-Mrem)));

        // If PC failed we can stop here
        if (pcResSum > 0)
        {
            return pcResSum;
        }
    }

    // =====================================================================
    // Process group with 8 BNs

    if (lut_numCnInCnGroups[5] > 0)
    {
        // Reset results
        pcResSum = 0;

        // Number of CNs in group
        M = lut_numCnInCnGroups[5]*Z;
        // Remainder modulo 32
        Mrem = M&31;
        // Number of groups of 32 CNs for parallel processing
        // Ceil for values not divisible by 32
        M32 = (M + 31)>>5;

        // Set pointers to start of group 8
        p_cnProcBuf    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[5]];
        p_cnProcBufRes = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[5]];

        // Loop over CNs
        for (i=0; i<(M32-1); i++)
        {
            pcRes = 0;
            // Loop over every BN
            // Compute PC for 32 CNs at once
            for (j=0; j<8; j++)
            {
                // BN offset is units of 2*384/32 = 24
                ymm0 = p_cnProcBuf   [j*24 + i];
                ymm1 = p_cnProcBufRes[j*24 + i];

                // Add BN and input LLR, extract the sign bit
                // and add in GF(2) (xor)
                pcRes ^= _mm256_movemask_epi8(_mm256_adds_epi8(ymm0,ymm1));
            }

            // If no error pcRes should be 0
            pcResSum |= pcRes;
        }

        // Last 32 CNs might not be full valid 32 depending on Z
        pcRes = 0;
        // Loop over every BN
        // Compute PC for 32 CNs at once
        for (j=0; j<8; j++)
        {
            // BN offset is units of 2*384/32 = 24
            ymm0 = p_cnProcBuf   [j*24 + i];
            ymm1 = p_cnProcBufRes[j*24 + i];

            // Add BN and input LLR, extract the sign bit
            // and add in GF(2) (xor)
            pcRes ^= _mm256_movemask_epi8(_mm256_adds_epi8(ymm0,ymm1));
        }

        // If no error pcRes should be 0
        // Only use valid CNs
        pcResSum |= (pcRes&(0xFFFFFFFF>>(32-Mrem)));

        // If PC failed we can stop here
        if (pcResSum > 0)
        {
            return pcResSum;
        }
    }

    // =====================================================================
    // Process group with 9 BNs

    if (lut_numCnInCnGroups[6] > 0)
    {
        // Reset results
        pcResSum = 0;

        // Number of CNs in group
        M = lut_numCnInCnGroups[6]*Z;
        // Remainder modulo 32
        Mrem = M&31;
        // Number of groups of 32 CNs for parallel processing
        // Ceil for values not divisible by 32
        M32 = (M + 31)>>5;

        // Set pointers to start of group 9
        p_cnProcBuf    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[6]];
        p_cnProcBufRes = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[6]];

        // Loop over CNs
        for (i=0; i<(M32-1); i++)
        {
            pcRes = 0;
            // Loop over every BN
            // Compute PC for 32 CNs at once
            for (j=0; j<9; j++)
            {
                // BN offset is units of 2*384/32 = 24
                ymm0 = p_cnProcBuf   [j*24 + i];
                ymm1 = p_cnProcBufRes[j*24 + i];

                // Add BN and input LLR, extract the sign bit
                // and add in GF(2) (xor)
                pcRes ^= _mm256_movemask_epi8(_mm256_adds_epi8(ymm0,ymm1));
            }

            // If no error pcRes should be 0
            pcResSum |= pcRes;
        }

        // Last 32 CNs might not be full valid 32 depending on Z
        pcRes = 0;
        // Loop over every BN
        // Compute PC for 32 CNs at once
        for (j=0; j<9; j++)
        {
            // BN offset is units of 2*384/32 = 24
            ymm0 = p_cnProcBuf   [j*24 + i];
            ymm1 = p_cnProcBufRes[j*24 + i];

            // Add BN and input LLR, extract the sign bit
            // and add in GF(2) (xor)
            pcRes ^= _mm256_movemask_epi8(_mm256_adds_epi8(ymm0,ymm1));
        }

        // If no error pcRes should be 0
        // Only use valid CNs
        pcResSum |= (pcRes&(0xFFFFFFFF>>(32-Mrem)));

        // If PC failed we can stop here
        if (pcResSum > 0)
        {
            return pcResSum;
        }
    }

    // =====================================================================
    // Process group with 10 BNs

    if (lut_numCnInCnGroups[7] > 0)
    {
        // Reset results
        pcResSum = 0;

        // Number of CNs in group
        M = lut_numCnInCnGroups[7]*Z;
        // Remainder modulo 32
        Mrem = M&31;
        // Number of groups of 32 CNs for parallel processing
        // Ceil for values not divisible by 32
        M32 = (M + 31)>>5;

        // Set pointers to start of group 10
        p_cnProcBuf    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[7]];
        p_cnProcBufRes = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[7]];

        // Loop over CNs
        for (i=0; i<(M32-1); i++)
        {
            pcRes = 0;
            // Loop over every BN
            // Compute PC for 32 CNs at once
            for (j=0; j<10; j++)
            {
                // BN offset is units of 1*384/32 = 12
                ymm0 = p_cnProcBuf   [j*12 + i];
                ymm1 = p_cnProcBufRes[j*12 + i];

                // Add BN and input LLR, extract the sign bit
                // and add in GF(2) (xor)
                pcRes ^= _mm256_movemask_epi8(_mm256_adds_epi8(ymm0,ymm1));
            }

            // If no error pcRes should be 0
            pcResSum |= pcRes;
        }

        // Last 32 CNs might not be full valid 32 depending on Z
        pcRes = 0;
        // Loop over every BN
        // Compute PC for 32 CNs at once
        for (j=0; j<10; j++)
        {
            // BN offset is units of 1*384/32 = 12
            ymm0 = p_cnProcBuf   [j*12 + i];
            ymm1 = p_cnProcBufRes[j*12 + i];

            // Add BN and input LLR, extract the sign bit
            // and add in GF(2) (xor)
            pcRes ^= _mm256_movemask_epi8(_mm256_adds_epi8(ymm0,ymm1));
        }

        // If no error pcRes should be 0
        // Only use valid CNs
        pcResSum |= (pcRes&(0xFFFFFFFF>>(32-Mrem)));

        // If PC failed we can stop here
        if (pcResSum > 0)
        {
            return pcResSum;
        }
    }

    // =====================================================================
    // Process group with 19 BNs

    if (lut_numCnInCnGroups[8] > 0)
    {
        // Reset results
        pcResSum = 0;

        // Number of CNs in group
        M = lut_numCnInCnGroups[8]*Z;
        // Remainder modulo 32
        Mrem = M&31;
        // Number of groups of 32 CNs for parallel processing
        // Ceil for values not divisible by 32
        M32 = (M + 31)>>5;

        // Set pointers to start of group 19
        p_cnProcBuf    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[8]];
        p_cnProcBufRes = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[8]];

        // Loop over CNs
        for (i=0; i<(M32-1); i++)
        {
            pcRes = 0;
            // Loop over every BN (Last BN is connected to multiple CNs)
            // Compute PC for 32 CNs at once
            for (j=0; j<19; j++)
            {
                // BN offset is units of 4*384/32 = 48
                ymm0 = p_cnProcBuf   [j*48 + i];
                ymm1 = p_cnProcBufRes[j*48 + i];

                // Add BN and input LLR, extract the sign bit
                // and add in GF(2) (xor)
                pcRes ^= _mm256_movemask_epi8(_mm256_adds_epi8(ymm0,ymm1));
            }

            // If no error pcRes should be 0
            pcResSum |= pcRes;
        }

        // Last 32 CNs might not be full valid 32 depending on Z
        pcRes = 0;
        // Loop over every BN (Last BN is connected to multiple CNs)
        // Compute PC for 32 CNs at once
        for (j=0; j<19; j++)
        {
            // BN offset is units of 4*384/32 = 48
            ymm0 = p_cnProcBuf   [j*48 + i];
            ymm1 = p_cnProcBufRes[j*48 + i];

            // Add BN and input LLR, extract the sign bit
            // and add in GF(2) (xor)
            pcRes ^= _mm256_movemask_epi8(_mm256_adds_epi8(ymm0,ymm1));
        }

        // If no error pcRes should be 0
        // Only use valid CNs
        pcResSum |= (pcRes&(0xFFFFFFFF>>(32-Mrem)));

        // If PC failed we can stop here
        if (pcResSum > 0)
        {
            return pcResSum;
        }
    }

    return pcResSum;
}

/**
   \brief Performs parity check for BG2 on the CN processing buffer. Stops as soon as error is detected.
   \param p_lut Pointer to decoder LUTs
   \param Z Lifting size
   \return 32-bit parity check indicator
*/
static inline uint32_t nrLDPC_cnProcPc_BG2(t_nrLDPC_lut* p_lut, int8_t* cnProcBuf, int8_t* cnProcBufRes, uint16_t Z)
{
    const uint8_t*  lut_numCnInCnGroups   = p_lut->numCnInCnGroups;
    const uint32_t* lut_startAddrCnGroups = p_lut->startAddrCnGroups;

    __m256i* p_cnProcBuf;
    __m256i* p_cnProcBufRes;

    // Number of CNs in Groups
    uint32_t M;
    uint32_t i;
    uint32_t j;
    uint32_t pcRes = 0;
    uint32_t pcResSum = 0;
    uint32_t Mrem;
    uint32_t M32;

    __m256i ymm0, ymm1;

    // =====================================================================
    // Process group with 3 BNs

    if (lut_numCnInCnGroups[0] > 0)
    {
        // Reset results
        pcResSum = 0;

        // Number of CNs in group
        M = lut_numCnInCnGroups[0]*Z;
        // Remainder modulo 32
        Mrem = M&31;
        // Number of groups of 32 CNs for parallel processing
        // Ceil for values not divisible by 32
        M32 = (M + 31)>>5;

        // Set pointers to start of group 3
        p_cnProcBuf    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[0]];
        p_cnProcBufRes = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[0]];

        // Loop over CNs
        for (i=0; i<(M32-1); i++)
        {
            pcRes = 0;
            // Loop over every BN
            // Compute PC for 32 CNs at once
            for (j=0; j<3; j++)
            {
                // BN offset is units of (6*384/32) = 72
                ymm0 = p_cnProcBuf   [j*72 + i];
                ymm1 = p_cnProcBufRes[j*72 + i];

                // Add BN and input LLR, extract the sign bit
                // and add in GF(2) (xor)
                pcRes ^= _mm256_movemask_epi8(_mm256_adds_epi8(ymm0,ymm1));
            }

            // If no error pcRes should be 0
            pcResSum |= pcRes;
        }

        // Last 32 CNs might not be full valid 32 depending on Z
        pcRes = 0;
        // Loop over every BN
        // Compute PC for 32 CNs at once
        for (j=0; j<3; j++)
        {
            // BN offset is units of (6*384/32) = 72
            ymm0 = p_cnProcBuf   [j*72 + i];
            ymm1 = p_cnProcBufRes[j*72 + i];

            // Add BN and input LLR, extract the sign bit
            // and add in GF(2) (xor)
            pcRes ^= _mm256_movemask_epi8(_mm256_adds_epi8(ymm0,ymm1));
        }

        // If no error pcRes should be 0
        // Only use valid CNs
        pcResSum |= (pcRes&(0xFFFFFFFF>>(32-Mrem)));

        // If PC failed we can stop here
        if (pcResSum > 0)
        {
            return pcResSum;
        }
    }

    // =====================================================================
    // Process group with 4 BNs

    if (lut_numCnInCnGroups[1] > 0)
    {
        // Reset results
        pcResSum = 0;

        // Number of CNs in group
        M = lut_numCnInCnGroups[1]*Z;
        // Remainder modulo 32
        Mrem = M&31;
        // Number of groups of 32 CNs for parallel processing
        // Ceil for values not divisible by 32
        M32 = (M + 31)>>5;

        // Set pointers to start of group 4
        p_cnProcBuf    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[1]];
        p_cnProcBufRes = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[1]];

        // Loop over CNs
        for (i=0; i<(M32-1); i++)
        {
            pcRes = 0;
            // Loop over every BN
            // Compute PC for 32 CNs at once
            for (j=0; j<4; j++)
            {
                // BN offset is units of 20*384/32 = 240
                ymm0 = p_cnProcBuf   [j*240 + i];
                ymm1 = p_cnProcBufRes[j*240 + i];

                // Add BN and input LLR, extract the sign bit
                // and add in GF(2) (xor)
                pcRes ^= _mm256_movemask_epi8(_mm256_adds_epi8(ymm0,ymm1));
            }

            // If no error pcRes should be 0
            pcResSum |= pcRes;
        }

        // Last 32 CNs might not be full valid 32 depending on Z
        pcRes = 0;
        // Loop over every BN
        // Compute PC for 32 CNs at once
        for (j=0; j<4; j++)
        {
            // BN offset is units of 20*384/32 = 240
            ymm0 = p_cnProcBuf   [j*240 + i];
            ymm1 = p_cnProcBufRes[j*240 + i];

            // Add BN and input LLR, extract the sign bit
            // and add in GF(2) (xor)
            pcRes ^= _mm256_movemask_epi8(_mm256_adds_epi8(ymm0,ymm1));
        }

        // If no error pcRes should be 0
        // Only use valid CNs
        pcResSum |= (pcRes&(0xFFFFFFFF>>(32-Mrem)));

        // If PC failed we can stop here
        if (pcResSum > 0)
        {
            return pcResSum;
        }
    }

    // =====================================================================
    // Process group with 5 BNs

    if (lut_numCnInCnGroups[2] > 0)
    {
        // Reset results
        pcResSum = 0;

        // Number of CNs in group
        M = lut_numCnInCnGroups[2]*Z;
        // Remainder modulo 32
        Mrem = M&31;
        // Number of groups of 32 CNs for parallel processing
        // Ceil for values not divisible by 32
        M32 = (M + 31)>>5;

        // Set pointers to start of group 5
        p_cnProcBuf    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[2]];
        p_cnProcBufRes = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[2]];

        // Loop over CNs
        for (i=0; i<(M32-1); i++)
        {
            pcRes = 0;
            // Loop over every BN
            // Compute PC for 32 CNs at once
            for (j=0; j<5; j++)
            {
                // BN offset is units of 9*384/32 = 108
                ymm0 = p_cnProcBuf   [j*108 + i];
                ymm1 = p_cnProcBufRes[j*108 + i];

                // Add BN and input LLR, extract the sign bit
                // and add in GF(2) (xor)
                pcRes ^= _mm256_movemask_epi8(_mm256_adds_epi8(ymm0,ymm1));
            }

            // If no error pcRes should be 0
            pcResSum |= pcRes;
        }

        // Last 32 CNs might not be full valid 32 depending on Z
        pcRes = 0;
        // Loop over every BN
        // Compute PC for 32 CNs at once
        for (j=0; j<5; j++)
        {
            // BN offset is units of 9*384/32 = 108
            ymm0 = p_cnProcBuf   [j*108 + i];
            ymm1 = p_cnProcBufRes[j*108 + i];

            // Add BN and input LLR, extract the sign bit
            // and add in GF(2) (xor)
            pcRes ^= _mm256_movemask_epi8(_mm256_adds_epi8(ymm0,ymm1));
        }

        // If no error pcRes should be 0
        // Only use valid CNs
        pcResSum |= (pcRes&(0xFFFFFFFF>>(32-Mrem)));

        // If PC failed we can stop here
        if (pcResSum > 0)
        {
            return pcResSum;
        }
    }

    // =====================================================================
    // Process group with 6 BNs

    if (lut_numCnInCnGroups[3] > 0)
    {
        // Reset results
        pcResSum = 0;

        // Number of CNs in group
        M = lut_numCnInCnGroups[3]*Z;
        // Remainder modulo 32
        Mrem = M&31;
        // Number of groups of 32 CNs for parallel processing
        // Ceil for values not divisible by 32
        M32 = (M + 31)>>5;

        // Set pointers to start of group 6
        p_cnProcBuf    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[3]];
        p_cnProcBufRes = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[3]];

        // Loop over CNs
        for (i=0; i<(M32-1); i++)
        {
            pcRes = 0;
            // Loop over every BN
            // Compute PC for 32 CNs at once
            for (j=0; j<6; j++)
            {
                // BN offset is units of 3*384/32 = 36
                ymm0 = p_cnProcBuf   [j*36 + i];
                ymm1 = p_cnProcBufRes[j*36 + i];

                // Add BN and input LLR, extract the sign bit
                // and add in GF(2) (xor)
                pcRes ^= _mm256_movemask_epi8(_mm256_adds_epi8(ymm0,ymm1));
            }

            // If no error pcRes should be 0
            pcResSum |= pcRes;
        }

        // Last 32 CNs might not be full valid 32 depending on Z
        pcRes = 0;
        // Loop over every BN
        // Compute PC for 32 CNs at once
        for (j=0; j<6; j++)
        {
            // BN offset is units of 3*384/32 = 36
            ymm0 = p_cnProcBuf   [j*36 + i];
            ymm1 = p_cnProcBufRes[j*36 + i];

            // Add BN and input LLR, extract the sign bit
            // and add in GF(2) (xor)
            pcRes ^= _mm256_movemask_epi8(_mm256_adds_epi8(ymm0,ymm1));
        }

        // If no error pcRes should be 0
        // Only use valid CNs
        pcResSum |= (pcRes&(0xFFFFFFFF>>(32-Mrem)));

        // If PC failed we can stop here
        if (pcResSum > 0)
        {
            return pcResSum;
        }
    }

    // =====================================================================
    // Process group with 8 BNs

    if (lut_numCnInCnGroups[4] > 0)
    {
        // Reset results
        pcResSum = 0;

        // Number of CNs in group
        M = lut_numCnInCnGroups[4]*Z;
        // Remainder modulo 32
        Mrem = M&31;
        // Number of groups of 32 CNs for parallel processing
        // Ceil for values not divisible by 32
        M32 = (M + 31)>>5;

        // Set pointers to start of group 8
        p_cnProcBuf    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[4]];
        p_cnProcBufRes = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[4]];

        // Loop over CNs
        for (i=0; i<(M32-1); i++)
        {
            pcRes = 0;
            // Loop over every BN
            // Compute PC for 32 CNs at once
            for (j=0; j<8; j++)
            {
                // BN offset is units of 2*384/32 = 24
                ymm0 = p_cnProcBuf   [j*24 + i];
                ymm1 = p_cnProcBufRes[j*24 + i];

                // Add BN and input LLR, extract the sign bit
                // and add in GF(2) (xor)
                pcRes ^= _mm256_movemask_epi8(_mm256_adds_epi8(ymm0,ymm1));
            }

            // If no error pcRes should be 0
            pcResSum |= pcRes;
        }

        // Last 32 CNs might not be full valid 32 depending on Z
        pcRes = 0;
        // Loop over every BN
        // Compute PC for 32 CNs at once
        for (j=0; j<8; j++)
        {
            // BN offset is units of 2*384/32 = 24
            ymm0 = p_cnProcBuf   [j*24 + i];
            ymm1 = p_cnProcBufRes[j*24 + i];

            // Add BN and input LLR, extract the sign bit
            // and add in GF(2) (xor)
            pcRes ^= _mm256_movemask_epi8(_mm256_adds_epi8(ymm0,ymm1));
        }

        // If no error pcRes should be 0
        // Only use valid CNs
        pcResSum |= (pcRes&(0xFFFFFFFF>>(32-Mrem)));

        // If PC failed we can stop here
        if (pcResSum > 0)
        {
            return pcResSum;
        }
    }

    // =====================================================================
    // Process group with 10 BNs

    if (lut_numCnInCnGroups[5] > 0)
    {
        // Reset results
        pcResSum = 0;

        // Number of CNs in group
        M = lut_numCnInCnGroups[5]*Z;
        // Remainder modulo 32
        Mrem = M&31;
        // Number of groups of 32 CNs for parallel processing
        // Ceil for values not divisible by 32
        M32 = (M + 31)>>5;

        // Set pointers to start of group 10
        p_cnProcBuf    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[5]];
        p_cnProcBufRes = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[5]];

        // Loop over CNs
        for (i=0; i<(M32-1); i++)
        {
            pcRes = 0;
            // Loop over every BN
            // Compute PC for 32 CNs at once
            for (j=0; j<10; j++)
            {
                // BN offset is units of 2*384/32 = 24
                ymm0 = p_cnProcBuf   [j*24 + i];
                ymm1 = p_cnProcBufRes[j*24 + i];

                // Add BN and input LLR, extract the sign bit
                // and add in GF(2) (xor)
                pcRes ^= _mm256_movemask_epi8(_mm256_adds_epi8(ymm0,ymm1));
            }

            // If no error pcRes should be 0
            pcResSum |= pcRes;
        }

        // Last 32 CNs might not be full valid 32 depending on Z
        pcRes = 0;
        // Loop over every BN
        // Compute PC for 32 CNs at once
        for (j=0; j<10; j++)
        {
            // BN offset is units of 2*384/32 = 24
            ymm0 = p_cnProcBuf   [j*24 + i];
            ymm1 = p_cnProcBufRes[j*24 + i];

            // Add BN and input LLR, extract the sign bit
            // and add in GF(2) (xor)
            pcRes ^= _mm256_movemask_epi8(_mm256_adds_epi8(ymm0,ymm1));
        }

        // If no error pcRes should be 0
        // Only use valid CNs
        pcResSum |= (pcRes&(0xFFFFFFFF>>(32-Mrem)));

        // If PC failed we can stop here
        if (pcResSum > 0)
        {
            return pcResSum;
        }
    }

    return pcResSum;
}

#endif



