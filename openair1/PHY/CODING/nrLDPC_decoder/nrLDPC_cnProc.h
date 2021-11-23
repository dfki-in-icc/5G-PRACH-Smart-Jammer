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

#ifndef __NR_LDPC_CNPROC__H__
#define __NR_LDPC_CNPROC__H__

/**
   \brief Performs CN processing for BG2 on the CN processing buffer and stores the results in the CN processing results buffer.
   \param p_lut Pointer to decoder LUTs
   \param p_procBuf Pointer to processing buffers
   \param Z Lifting size
*/
static inline void nrLDPC_cnProc_BG2(t_nrLDPC_lut* p_lut, t_nrLDPC_procBuf* p_procBuf, uint16_t Z)
{
    const uint8_t*  lut_numCnInCnGroups   = p_lut->numCnInCnGroups;
    const uint32_t* lut_startAddrCnGroups = p_lut->startAddrCnGroups;

    int8_t* cnProcBuf    = p_procBuf->cnProcBuf;
    int8_t* cnProcBufRes = p_procBuf->cnProcBufRes;
    
    __m256i* p_cnProcBuf;
    __m256i* p_cnProcBufRes;

    // Number of CNs in Groups
    uint32_t M;
    uint32_t i;
    uint32_t j;
    uint32_t k;
    // Offset to each bit within a group in terms of 32 Byte
    uint32_t bitOffsetInGroup;

    __m256i ymm0, min, sgn;
    __m256i* p_cnProcBufResBit;

    const __m256i* p_ones   = (__m256i*) ones256_epi8;
    const __m256i* p_maxLLR = (__m256i*) maxLLR256_epi8;

    // LUT with offsets for bits that need to be processed
    // 1. bit proc requires LLRs of 2. and 3. bit, 2.bits of 1. and 3. etc.
    // Offsets are in units of bitOffsetInGroup
    const uint8_t lut_idxCnProcG3[3][2] = {{72,144}, {0,144}, {0,72}};

    // =====================================================================
    // Process group with 3 BNs

    if (lut_numCnInCnGroups[0] > 0)
    {
        // Number of groups of 32 CNs for parallel processing
        // Ceil for values not divisible by 32
        M = (lut_numCnInCnGroups[0]*Z + 31)>>5;
        // Set the offset to each bit within a group in terms of 32 Byte
        bitOffsetInGroup = (lut_numCnInCnGroups_BG2_R15[0]*NR_LDPC_ZMAX)>>5;

        // Set pointers to start of group 3
        p_cnProcBuf    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[0]];
        p_cnProcBufRes = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[0]];

        // Loop over every BN
        for (j=0; j<3; j++)
        {
            // Set of results pointer to correct BN address
            p_cnProcBufResBit = p_cnProcBufRes + (j*bitOffsetInGroup);

            __m256i *pj0 = &p_cnProcBuf[lut_idxCnProcG3[j][0]];
            __m256i *pj1 = &p_cnProcBuf[lut_idxCnProcG3[j][1]];

            // Loop over CNs
            for (i=0; i<M; i++)
            {
                // Abs and sign of 32 CNs (first BN)
	      //                ymm0 = p_cnProcBuf[lut_idxCnProcG3[j][0] + i];
	        ymm0 = pj0[i];
                sgn  = _mm256_sign_epi8(*p_ones, ymm0);
                min  = _mm256_abs_epi8(ymm0);

                // 32 CNs of second BN
		//  ymm0 = p_cnProcBuf[lut_idxCnProcG3[j][1] + i];
		ymm0 = pj1[i];
                min  = _mm256_min_epu8(min, _mm256_abs_epi8(ymm0));
                sgn  = _mm256_sign_epi8(sgn, ymm0);

                // Store result
                min = _mm256_min_epu8(min, *p_maxLLR); // 128 in epi8 is -127
                //*p_cnProcBufResBit = _mm256_sign_epi8(min, sgn);
                //p_cnProcBufResBit++;
		p_cnProcBufResBit[i]=_mm256_sign_epi8(min, sgn);
            }
        }
    }

    // =====================================================================
    // Process group with 4 BNs

    // Offset is 20*384/32 = 240
    const uint16_t lut_idxCnProcG4[4][3] = {{240,480,720}, {0,480,720}, {0,240,720}, {0,240,480}};

    if (lut_numCnInCnGroups[1] > 0)
    {
        // Number of groups of 32 CNs for parallel processing
        // Ceil for values not divisible by 32
        M = (lut_numCnInCnGroups[1]*Z + 31)>>5;
        // Set the offset to each bit within a group in terms of 32 Byte
        bitOffsetInGroup = (lut_numCnInCnGroups_BG2_R15[1]*NR_LDPC_ZMAX)>>5;

        // Set pointers to start of group 4
        p_cnProcBuf    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[1]];
        p_cnProcBufRes = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[1]];

        // Loop over every BN
        for (j=0; j<4; j++)
        {
            // Set of results pointer to correct BN address
            p_cnProcBufResBit = p_cnProcBufRes + (j*bitOffsetInGroup);

            // Loop over CNs
            for (i=0; i<M; i++)
            {
                // Abs and sign of 32 CNs (first BN)
                ymm0 = p_cnProcBuf[lut_idxCnProcG4[j][0] + i];
                sgn  = _mm256_sign_epi8(*p_ones, ymm0);
                min  = _mm256_abs_epi8(ymm0);

                // Loop over BNs
                for (k=1; k<3; k++)
                {
                    ymm0 = p_cnProcBuf[lut_idxCnProcG4[j][k] + i];
                    min  = _mm256_min_epu8(min, _mm256_abs_epi8(ymm0));
                    sgn  = _mm256_sign_epi8(sgn, ymm0);
                }

                // Store result
                min = _mm256_min_epu8(min, *p_maxLLR); // 128 in epi8 is -127
                *p_cnProcBufResBit = _mm256_sign_epi8(min, sgn);
                p_cnProcBufResBit++;
            }
        }
    }

    // =====================================================================
    // Process group with 5 BNs

    // Offset is 9*384/32 = 108
    const uint16_t lut_idxCnProcG5[5][4] = {{108,216,324,432}, {0,216,324,432},
                                            {0,108,324,432}, {0,108,216,432}, {0,108,216,324}};

    if (lut_numCnInCnGroups[2] > 0)
    {
        // Number of groups of 32 CNs for parallel processing
        // Ceil for values not divisible by 32
        M = (lut_numCnInCnGroups[2]*Z + 31)>>5;
        // Set the offset to each bit within a group in terms of 32 Byte
        bitOffsetInGroup = (lut_numCnInCnGroups_BG2_R15[2]*NR_LDPC_ZMAX)>>5;

        // Set pointers to start of group 5
        p_cnProcBuf    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[2]];
        p_cnProcBufRes = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[2]];

        // Loop over every BN
        for (j=0; j<5; j++)
        {
            // Set of results pointer to correct BN address
            p_cnProcBufResBit = p_cnProcBufRes + (j*bitOffsetInGroup);

            // Loop over CNs
            for (i=0; i<M; i++)
            {
                // Abs and sign of 32 CNs (first BN)
                ymm0 = p_cnProcBuf[lut_idxCnProcG5[j][0] + i];
                sgn  = _mm256_sign_epi8(*p_ones, ymm0);
                min  = _mm256_abs_epi8(ymm0);

                // Loop over BNs
                for (k=1; k<4; k++)
                {
                    ymm0 = p_cnProcBuf[lut_idxCnProcG5[j][k] + i];
                    min  = _mm256_min_epu8(min, _mm256_abs_epi8(ymm0));
                    sgn  = _mm256_sign_epi8(sgn, ymm0);
                }

                // Store result
                min = _mm256_min_epu8(min, *p_maxLLR); // 128 in epi8 is -127
                *p_cnProcBufResBit = _mm256_sign_epi8(min, sgn);
                p_cnProcBufResBit++;
            }
        }
    }

    // =====================================================================
    // Process group with 6 BNs

    // Offset is 3*384/32 = 36
    const uint16_t lut_idxCnProcG6[6][5] = {{36,72,108,144,180}, {0,72,108,144,180},
                                            {0,36,108,144,180}, {0,36,72,144,180},
                                            {0,36,72,108,180}, {0,36,72,108,144}};

    if (lut_numCnInCnGroups[3] > 0)
    {
        // Number of groups of 32 CNs for parallel processing
        // Ceil for values not divisible by 32
        M = (lut_numCnInCnGroups[3]*Z + 31)>>5;
        // Set the offset to each bit within a group in terms of 32 Byte
        bitOffsetInGroup = (lut_numCnInCnGroups_BG2_R15[3]*NR_LDPC_ZMAX)>>5;

        // Set pointers to start of group 6
        p_cnProcBuf    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[3]];
        p_cnProcBufRes = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[3]];

        // Loop over every BN
        for (j=0; j<6; j++)
        {
            // Set of results pointer to correct BN address
            p_cnProcBufResBit = p_cnProcBufRes + (j*bitOffsetInGroup);

            // Loop over CNs
            for (i=0; i<M; i++)
            {
                // Abs and sign of 32 CNs (first BN)
                ymm0 = p_cnProcBuf[lut_idxCnProcG6[j][0] + i];
                sgn  = _mm256_sign_epi8(*p_ones, ymm0);
                min  = _mm256_abs_epi8(ymm0);

                // Loop over BNs
                for (k=1; k<5; k++)
                {
                    ymm0 = p_cnProcBuf[lut_idxCnProcG6[j][k] + i];
                    min  = _mm256_min_epu8(min, _mm256_abs_epi8(ymm0));
                    sgn  = _mm256_sign_epi8(sgn, ymm0);
                }

                // Store result
                min = _mm256_min_epu8(min, *p_maxLLR); // 128 in epi8 is -127
                *p_cnProcBufResBit = _mm256_sign_epi8(min, sgn);
                p_cnProcBufResBit++;
            }
        }
    }

    // =====================================================================
    // Process group with 8 BNs

    // Offset is 2*384/32 = 24
    const uint8_t lut_idxCnProcG8[8][7] = {{24,48,72,96,120,144,168}, {0,48,72,96,120,144,168},
                                           {0,24,72,96,120,144,168}, {0,24,48,96,120,144,168},
                                           {0,24,48,72,120,144,168}, {0,24,48,72,96,144,168},
                                           {0,24,48,72,96,120,168}, {0,24,48,72,96,120,144}};

    if (lut_numCnInCnGroups[4] > 0)
    {
        // Number of groups of 32 CNs for parallel processing
        // Ceil for values not divisible by 32
        M = (lut_numCnInCnGroups[4]*Z + 31)>>5;
        // Set the offset to each bit within a group in terms of 32 Byte
        bitOffsetInGroup = (lut_numCnInCnGroups_BG2_R15[4]*NR_LDPC_ZMAX)>>5;

        // Set pointers to start of group 8
        p_cnProcBuf    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[4]];
        p_cnProcBufRes = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[4]];

        // Loop over every BN
        for (j=0; j<8; j++)
        {
            // Set of results pointer to correct BN address
            p_cnProcBufResBit = p_cnProcBufRes + (j*bitOffsetInGroup);

            // Loop over CNs
            for (i=0; i<M; i++)
            {
                // Abs and sign of 32 CNs (first BN)
                ymm0 = p_cnProcBuf[lut_idxCnProcG8[j][0] + i];
                sgn  = _mm256_sign_epi8(*p_ones, ymm0);
                min  = _mm256_abs_epi8(ymm0);

                // Loop over BNs
                for (k=1; k<7; k++)
                {
                    ymm0 = p_cnProcBuf[lut_idxCnProcG8[j][k] + i];
                    min  = _mm256_min_epu8(min, _mm256_abs_epi8(ymm0));
                    sgn  = _mm256_sign_epi8(sgn, ymm0);
                }

                // Store result
                min = _mm256_min_epu8(min, *p_maxLLR); // 128 in epi8 is -127
                *p_cnProcBufResBit = _mm256_sign_epi8(min, sgn);
                p_cnProcBufResBit++;
            }
        }
    }

    // =====================================================================
    // Process group with 10 BNs

    // Offset is 2*384/32 = 24
    const uint8_t lut_idxCnProcG10[10][9] = {{24,48,72,96,120,144,168,192,216}, {0,48,72,96,120,144,168,192,216},
                                             {0,24,72,96,120,144,168,192,216}, {0,24,48,96,120,144,168,192,216},
                                             {0,24,48,72,120,144,168,192,216}, {0,24,48,72,96,144,168,192,216},
                                             {0,24,48,72,96,120,168,192,216}, {0,24,48,72,96,120,144,192,216},
                                             {0,24,48,72,96,120,144,168,216}, {0,24,48,72,96,120,144,168,192}};

    if (lut_numCnInCnGroups[5] > 0)
    {
        // Number of groups of 32 CNs for parallel processing
        // Ceil for values not divisible by 32
        M = (lut_numCnInCnGroups[5]*Z + 31)>>5;
        // Set the offset to each bit within a group in terms of 32 Byte
        bitOffsetInGroup = (lut_numCnInCnGroups_BG2_R15[5]*NR_LDPC_ZMAX)>>5;

        // Set pointers to start of group 10
        p_cnProcBuf    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[5]];
        p_cnProcBufRes = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[5]];

        // Loop over every BN
        for (j=0; j<10; j++)
        {
            // Set of results pointer to correct BN address
            p_cnProcBufResBit = p_cnProcBufRes + (j*bitOffsetInGroup);

            // Loop over CNs
            for (i=0; i<M; i++)
            {
                // Abs and sign of 32 CNs (first BN)
                ymm0 = p_cnProcBuf[lut_idxCnProcG10[j][0] + i];
                sgn  = _mm256_sign_epi8(*p_ones, ymm0);
                min  = _mm256_abs_epi8(ymm0);

                // Loop over BNs
                for (k=1; k<9; k++)
                {
                    ymm0 = p_cnProcBuf[lut_idxCnProcG10[j][k] + i];
                    min  = _mm256_min_epu8(min, _mm256_abs_epi8(ymm0));
                    sgn  = _mm256_sign_epi8(sgn, ymm0);
                }

                // Store result
                min = _mm256_min_epu8(min, *p_maxLLR); // 128 in epi8 is -127
                *p_cnProcBufResBit = _mm256_sign_epi8(min, sgn);
                p_cnProcBufResBit++;
            }
        }
    }

}

static inline void nrLDPC_cnProc_BG1_first(t_nrLDPC_lut* p_lut, int8_t* llr, t_nrLDPC_procBuf* p_procBuf, uint16_t Z)
{
    const uint16_t (*lut_circShift_CNG3) [lut_numCnInCnGroups_BG1_R13[0]] = (const uint16_t(*)[lut_numCnInCnGroups_BG1_R13[0]]) p_lut->circShift[0];
    const uint16_t (*lut_circShift_CNG4) [lut_numCnInCnGroups_BG1_R13[1]] = (const uint16_t(*)[lut_numCnInCnGroups_BG1_R13[1]]) p_lut->circShift[1];
    const uint16_t (*lut_circShift_CNG5) [lut_numCnInCnGroups_BG1_R13[2]] = (const uint16_t(*)[lut_numCnInCnGroups_BG1_R13[2]]) p_lut->circShift[2];
    const uint16_t (*lut_circShift_CNG6) [lut_numCnInCnGroups_BG1_R13[3]] = (const uint16_t(*)[lut_numCnInCnGroups_BG1_R13[3]]) p_lut->circShift[3];
    const uint16_t (*lut_circShift_CNG7) [lut_numCnInCnGroups_BG1_R13[4]] = (const uint16_t(*)[lut_numCnInCnGroups_BG1_R13[4]]) p_lut->circShift[4];
    const uint16_t (*lut_circShift_CNG8) [lut_numCnInCnGroups_BG1_R13[5]] = (const uint16_t(*)[lut_numCnInCnGroups_BG1_R13[5]]) p_lut->circShift[5];
    const uint16_t (*lut_circShift_CNG9) [lut_numCnInCnGroups_BG1_R13[6]] = (const uint16_t(*)[lut_numCnInCnGroups_BG1_R13[6]]) p_lut->circShift[6];
    const uint16_t (*lut_circShift_CNG10)[lut_numCnInCnGroups_BG1_R13[7]] = (const uint16_t(*)[lut_numCnInCnGroups_BG1_R13[7]]) p_lut->circShift[7];
    const uint16_t (*lut_circShift_CNG19)[lut_numCnInCnGroups_BG1_R13[8]] = (const uint16_t(*)[lut_numCnInCnGroups_BG1_R13[8]]) p_lut->circShift[8];

    const uint8_t (*lut_posBnInCnProcBuf_CNG3) [lut_numCnInCnGroups_BG1_R13[0]] = (const uint8_t(*)[lut_numCnInCnGroups_BG1_R13[0]]) p_lut->posBnInCnProcBuf[0];
    const uint8_t (*lut_posBnInCnProcBuf_CNG4) [lut_numCnInCnGroups_BG1_R13[1]] = (const uint8_t(*)[lut_numCnInCnGroups_BG1_R13[1]]) p_lut->posBnInCnProcBuf[1];
    const uint8_t (*lut_posBnInCnProcBuf_CNG5) [lut_numCnInCnGroups_BG1_R13[2]] = (const uint8_t(*)[lut_numCnInCnGroups_BG1_R13[2]]) p_lut->posBnInCnProcBuf[2];
    const uint8_t (*lut_posBnInCnProcBuf_CNG6) [lut_numCnInCnGroups_BG1_R13[3]] = (const uint8_t(*)[lut_numCnInCnGroups_BG1_R13[3]]) p_lut->posBnInCnProcBuf[3];
    const uint8_t (*lut_posBnInCnProcBuf_CNG7) [lut_numCnInCnGroups_BG1_R13[4]] = (const uint8_t(*)[lut_numCnInCnGroups_BG1_R13[4]]) p_lut->posBnInCnProcBuf[4];
    const uint8_t (*lut_posBnInCnProcBuf_CNG8) [lut_numCnInCnGroups_BG1_R13[5]] = (const uint8_t(*)[lut_numCnInCnGroups_BG1_R13[5]]) p_lut->posBnInCnProcBuf[5];
    const uint8_t (*lut_posBnInCnProcBuf_CNG9) [lut_numCnInCnGroups_BG1_R13[6]] = (const uint8_t(*)[lut_numCnInCnGroups_BG1_R13[6]]) p_lut->posBnInCnProcBuf[6];
    const uint8_t (*lut_posBnInCnProcBuf_CNG10)[lut_numCnInCnGroups_BG1_R13[7]] = (const uint8_t(*)[lut_numCnInCnGroups_BG1_R13[7]]) p_lut->posBnInCnProcBuf[7];
    const uint8_t (*lut_posBnInCnProcBuf_CNG19)[lut_numCnInCnGroups_BG1_R13[8]] = (const uint8_t(*)[lut_numCnInCnGroups_BG1_R13[8]]) p_lut->posBnInCnProcBuf[8];

    const uint16_t (*lut_addrOffset_CNG7_SG1) [13] = (const uint16_t(*)[13]) p_lut->addrOffset_CNG7_SG1;
    const uint16_t (*lut_addrOffset_CNG7_SG2) [7] = (const uint16_t(*)[7]) p_lut->addrOffset_CNG7_SG2;
    const uint16_t (*lut_addrOffset_CNG7_SG3) [1] = (const uint16_t(*)[1]) p_lut->addrOffset_CNG7_SG3;
    const uint16_t (*lut_addrOffset_CNG7_SG4) [2] = (const uint16_t(*)[2]) p_lut->addrOffset_CNG7_SG4;

    const uint8_t*  lut_numCnInCnGroups = p_lut->numCnInCnGroups;
    const uint32_t* lut_startAddrCnGroups = p_lut->startAddrCnGroups;

    int8_t* cnProcBuf    = p_procBuf->cnProcBuf;
    int8_t* cnProcBufRes = p_procBuf->cnProcBufRes;
    int8_t* bnProcBuf    = p_procBuf->bnProcBuf;
    int8_t* bnProcBufRes = p_procBuf->bnProcBufRes;
    int8_t* llrRes       = p_procBuf->llrRes;
    int8_t* llrProcBuf   = p_procBuf->llrProcBuf;
    int8_t* bnProcBufTemp = p_procBuf->bnProcBufTemp;
    uint32_t idxBn = 0;
    int8_t* p_llrRes;
    int8_t* p_cnProcBuf;
    int8_t* p_cnProcBufRes;
    int8_t* p_bnProcBuf;
    int8_t* p_bnProcBufRes;
    int8_t* p_bnProcBufTemp;
    
    __m128i* p_llrProcBuf128;
    __m128i* p_bnProcBuf128;
    __m128i* p_bnProcBufRes128;
    __m256i* p_cnProcBuf256;
    __m256i* p_cnProcBufRes256;
    __m256i* p_bnProcBufTemp256;

    // Number of CNs in Groups
    uint32_t M;
    uint32_t i;
    uint32_t j;
    uint32_t k;
    // Offset to each bit within a group in terms of 32 Byte
    uint32_t bitOffsetInGroup;
    uint32_t cnOffsetInGroup;

    __m256i ymm0, ymm1, ymmRes0, ymmRes1, min, sgn;
    __m256i* p_cnProcBufResBit;

    const __m256i* p_ones   = (__m256i*) ones256_epi8;
    const __m256i* p_maxLLR = (__m256i*) maxLLR256_epi8;

    // =====================================================================
    // Copy LLRs to cnProcBuf for CNG3
#ifdef NR_LDPC_PROFILER_DETAIL
    start_meas(&p_profiler->llr2CnProcBuf);
#endif
    bitOffsetInGroup = lut_numCnInCnGroups_BG1_R13[0]*NR_LDPC_ZMAX;

    for (j=0; j<3; j++)
    {
        p_cnProcBuf = &cnProcBuf[lut_startAddrCnGroups[0] + j*bitOffsetInGroup];

        idxBn = lut_posBnInCnProcBuf_CNG3[j][0]*Z;

        nrLDPC_circ_memcpy(p_cnProcBuf, &llr[idxBn], Z, lut_circShift_CNG3[j][0]);
    }
#ifdef NR_LDPC_PROFILER_DETAIL
    stop_meas(&p_profiler->llr2CnProcBuf);
#endif

    // LUT with offsets for bits that need to be processed
    // 1. bit proc requires LLRs of 2. and 3. bit, 2.bits of 1. and 3. etc.
    // Offsets are in units of bitOffsetInGroup (1*384/32)
    const uint8_t lut_idxCnProcG3[3][2] = {{12,24}, {0,24}, {0,12}};
    // Process group with 3 BNs

    if (lut_numCnInCnGroups[0] > 0)
    {
        // Number of groups of 32 CNs for parallel processing
        // Ceil for values not divisible by 32
        M = (lut_numCnInCnGroups[0]*Z + 31)>>5;

        // Set the offset to each bit within a group in terms of 32 Byte
        bitOffsetInGroup = (lut_numCnInCnGroups_BG1_R13[0]*NR_LDPC_ZMAX)>>5;

        // Set pointers to start of group 3
        p_cnProcBuf256    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[0]];
        p_cnProcBufRes256 = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[0]];

        // Loop over every BN
        for (j=0; j<3; j++)
        {
            // Set of results pointer to correct BN address
            p_cnProcBufResBit = p_cnProcBufRes256 + (j*bitOffsetInGroup);

            // Loop over CNs
            for (i=0; i<M; i++)
            {
                // Abs and sign of 32 CNs (first BN)
                ymm0 = p_cnProcBuf256[lut_idxCnProcG3[j][0] + i];
                sgn  = _mm256_sign_epi8(*p_ones, ymm0);
                min  = _mm256_abs_epi8(ymm0);

                // 32 CNs of second BN
                ymm0 = p_cnProcBuf256[lut_idxCnProcG3[j][1] + i];
                min  = _mm256_min_epu8(min, _mm256_abs_epi8(ymm0));
                sgn  = _mm256_sign_epi8(sgn, ymm0);

                // Store result
                min = _mm256_min_epu8(min, *p_maxLLR); // 128 in epi8 is -127
                *p_cnProcBufResBit = _mm256_sign_epi8(min, sgn);
                p_cnProcBufResBit++;
            }
        }

      // Copy CN to BN messages in CNG3 to bnProcBuff
      bitOffsetInGroup = lut_numCnInCnGroups_BG1_R13[0]*NR_LDPC_ZMAX;
      p_bnProcBuf = &bnProcBuf[0];
      p_bnProcBufRes = &bnProcBufRes[0];

      for (j=0; j<3; j++)
      {
        p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[0] + j*bitOffsetInGroup];
        nrLDPC_inv_circ_memcpy(p_bnProcBuf, p_cnProcBufRes, Z, lut_circShift_CNG3[j][0]);
        p_bnProcBuf += Z;

        idxBn = lut_posBnInCnProcBuf_CNG3[j][0]*Z;
        memcpy(p_bnProcBufRes, &llrProcBuf[idxBn], Z);
        p_bnProcBufRes += Z;
      }

      // Update the BN connected to CNG3
      M = (3*Z + 31)>>5;
      p_bnProcBuf128 = (__m128i*) &bnProcBuf[0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes[0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      for (i=0,j=0; i<M; i++,j+=2)
      {
        // First 16 LLRs of first CN
        ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
        ymm1 = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);

        ymmRes0 = _mm256_adds_epi16(ymm0, ymm1);

        // Second 16 LLRs of first CN
        ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);
        ymm1 = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);

        ymmRes1 = _mm256_adds_epi16(ymm0, ymm1);

        // Pack results back to epi8
        ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
        // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
        // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
        *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

        // Next result
        p_bnProcBufTemp256++;
      }

      // Copy the updated LLRs to llrProcBuf for next CNG processing
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (j=0; j<3; j++)
      {
        idxBn = lut_posBnInCnProcBuf_CNG3[j][0]*Z;
        memcpy(&llrRes[idxBn], p_bnProcBufTemp, Z);
        p_bnProcBufTemp += Z;
      }
    }

    // =====================================================================
    // Copy LLRs to cnProcBuf for CNG4
#ifdef NR_LDPC_PROFILER_DETAIL
    start_meas(&p_profiler->llr2CnProcBuf);
#endif
    bitOffsetInGroup = lut_numCnInCnGroups_BG1_R13[1]*NR_LDPC_ZMAX;

    for (j=0; j<4; j++)
    {
        p_cnProcBuf = &cnProcBuf[lut_startAddrCnGroups[1] + j*bitOffsetInGroup];

        for (i=0; i<lut_numCnInCnGroups[1]; i++)
        {
            idxBn = lut_posBnInCnProcBuf_CNG4[j][i]*Z;

            nrLDPC_circ_memcpy(p_cnProcBuf, &llrRes[idxBn], Z, lut_circShift_CNG4[j][i]);

            p_cnProcBuf += Z;
        }
    }
#ifdef NR_LDPC_PROFILER_DETAIL
    stop_meas(&p_profiler->llr2CnProcBuf);
#endif
    // Process group with 4 BNs

    // Offset is 5*384/32 = 60
    const uint8_t lut_idxCnProcG4[4][3] = {{60,120,180}, {0,120,180}, {0,60,180}, {0,60,120}};

    if (lut_numCnInCnGroups[1] > 0)
    {
        // Number of groups of 32 CNs for parallel processing
        // Ceil for values not divisible by 32
        M = (lut_numCnInCnGroups[1]*Z + 31)>>5;

        // Set the offset to each bit within a group in terms of 32 Byte
        bitOffsetInGroup = (lut_numCnInCnGroups_BG1_R13[1]*NR_LDPC_ZMAX)>>5;

        // Set pointers to start of group 4
        p_cnProcBuf256    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[1]];
        p_cnProcBufRes256 = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[1]];

        // Loop over every BN
        for (j=0; j<4; j++)
        {
            // Set of results pointer to correct BN address
            p_cnProcBufResBit = p_cnProcBufRes256 + (j*bitOffsetInGroup);

            // Loop over CNs
            for (i=0; i<M; i++)
            {
                // Abs and sign of 32 CNs (first BN)
                ymm0 = p_cnProcBuf256[lut_idxCnProcG4[j][0] + i];
                sgn  = _mm256_sign_epi8(*p_ones, ymm0);
                min  = _mm256_abs_epi8(ymm0);

                // Loop over BNs
                for (k=1; k<3; k++)
                {
                    ymm0 = p_cnProcBuf256[lut_idxCnProcG4[j][k] + i];
                    min  = _mm256_min_epu8(min, _mm256_abs_epi8(ymm0));
                    sgn  = _mm256_sign_epi8(sgn, ymm0);
                }

                // Store result
                min = _mm256_min_epu8(min, *p_maxLLR); // 128 in epi8 is -127
                *p_cnProcBufResBit = _mm256_sign_epi8(min, sgn);
                p_cnProcBufResBit++;
            }
        }

      // Process BNs with degree 1 in CNG4
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_bnProcBufTemp = &bnProcBufTemp[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[1]];
      for (i=0; i<11; i++)
      {
        nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[addrOffset_BG1_CNG4_SG1_R13[i][0]],
          Z, p_lut->circShift_CNG4_SG1[i]);
        p_bnProcBuf += Z;

        memcpy(p_bnProcBufRes, &llrProcBuf[bnIdx_BG1_CNG4_SG1_R13[i]*Z], Z);
        p_bnProcBufRes += Z;
      }

      M = (11*Z + 31)>>5;
      p_bnProcBuf128 = (__m128i*) &bnProcBuf[0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes[0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      for (i=0,j=0; i<M; i++,j+=2)
      {
        // First 16 LLRs of first CN
        ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
        ymm1 = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);

        ymmRes0 = _mm256_adds_epi16(ymm0, ymm1);

        // Second 16 LLRs of first CN
        ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);
        ymm1 = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);

        ymmRes1 = _mm256_adds_epi16(ymm0, ymm1);

        // Pack results back to epi8
        ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
        // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
        // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
        *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

        // Next result
        p_bnProcBufTemp256++;
      }
      
      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (i=0; i<11; i++)
      {
        memcpy(&llrRes[bnIdx_BG1_CNG4_SG1_R13[i]*Z], p_bnProcBufTemp, Z);
        p_bnProcBufTemp += Z;
      }
      
      // Process BNs with degree 2 in CNG4
      // Arrange CN message values for addition
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[1]];
      bitOffsetInGroup = 3*NR_LDPC_ZMAX;
      for (i=0; i<2; i++)
      {
        p_bnProcBuf = &bnProcBuf[i*bitOffsetInGroup];
        for (j=0; j<3; j++)
        {
          nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[addrOffset_BG1_CNG4_SG2_R13[j][i]],
            Z, p_lut->circShift_CNG4_SG2[j]);
          p_bnProcBuf += Z;
        }
      }

      // Arrange input LLRs
      for (i=0; i<3; i++)
      {
        memcpy(p_bnProcBufRes, &p_llrRes[bnIdx_BG1_CNG4_SG2_R13[i]*Z], Z);
        p_bnProcBufRes += Z;
      }
      
      // Perform addition
      p_bnProcBuf128    = (__m128i*) &bnProcBuf    [0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes [0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      cnOffsetInGroup = (3*NR_LDPC_ZMAX)>>4;
      M = (3*Z + 31)>>5;
      for (i=0,j=0; i<M; i++,j+=2)
      {
          // First 16 LLRs of first CN
          ymmRes0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
          ymmRes1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);

          // Loop over CNs
          for (k=1; k<2; k++)
          {
              ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j]);
              ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

              ymm1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j+1]);
              ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);
          }

          // Add LLR from receiver input
          ymm0    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);
          ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

          ymm1    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);
          ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);

          // Pack results back to epi8
          ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
          // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
          // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
          *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

          // Next result
          p_bnProcBufTemp256++;
      }

      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (i=0; i<3; i++)
      {
        memcpy(&llrRes[bnIdx_BG1_CNG4_SG2_R13[i]*Z], p_bnProcBufTemp, Z);
        p_bnProcBufTemp += Z;
      }

      // Process BNs with degree 3 in CNG4
      // Arrange CN message values for addition
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[1]];
      bitOffsetInGroup = 1*NR_LDPC_ZMAX;
      for (i=0; i<3; i++)
      {
        p_bnProcBuf = &bnProcBuf[i*bitOffsetInGroup];
        nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[addrOffset_BG1_CNG4_SG3_R13[0][i]],
          Z, p_lut->circShift_CNG4_SG3[i]);
        p_bnProcBuf += Z;
      }

      // Perform addition
      p_bnProcBuf128    = (__m128i*) &bnProcBuf    [0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes [0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      cnOffsetInGroup = (1*NR_LDPC_ZMAX)>>4;
      M = (1*Z + 31)>>5;
      for (i=0,j=0; i<M; i++,j+=2)
      {
          // First 16 LLRs of first CN
          ymmRes0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
          ymmRes1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);

          // Loop over CNs
          for (k=1; k<3; k++)
          {
              ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j]);
              ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

              ymm1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j+1]);
              ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);
          }

          // Add LLR from receiver input
          p_llrProcBuf128   = (__m128i*) &llrProcBuf[bnIdx_BG1_CNG4_SG3_R13[0]*Z];
          ymm0    = _mm256_cvtepi8_epi16(p_llrProcBuf128[j]);
          ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

          ymm1    = _mm256_cvtepi8_epi16(p_llrProcBuf128[j+1]);
          ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);

          // Pack results back to epi8
          ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
          // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
          // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
          *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

          // Next result
          p_bnProcBufTemp256++;
      }

      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      memcpy(&llrRes[bnIdx_BG1_CNG4_SG3_R13[0]*Z], p_bnProcBufTemp, Z);
    }

    // =====================================================================
#ifdef NR_LDPC_PROFILER_DETAIL
    start_meas(&p_profiler->llr2CnProcBuf);
#endif
    bitOffsetInGroup = lut_numCnInCnGroups_BG1_R13[2]*NR_LDPC_ZMAX;

    for (j=0; j<5; j++)
    {
        p_cnProcBuf = &cnProcBuf[lut_startAddrCnGroups[2] + j*bitOffsetInGroup];

        for (i=0; i<lut_numCnInCnGroups[2]; i++)
        {
            idxBn = lut_posBnInCnProcBuf_CNG5[j][i]*Z;
            nrLDPC_circ_memcpy(p_cnProcBuf, &llrRes[idxBn], Z, lut_circShift_CNG5[j][i]);
            p_cnProcBuf += Z;
        }
    }
#ifdef NR_LDPC_PROFILER_DETAIL
    stop_meas(&p_profiler->llr2CnProcBuf);
#endif
    // Process group with 5 BNs

    // Offset is 18*384/32 = 216
    const uint16_t lut_idxCnProcG5[5][4] = {{216,432,648,864}, {0,432,648,864},
                                            {0,216,648,864}, {0,216,432,864}, {0,216,432,648}};

    if (lut_numCnInCnGroups[2] > 0)
    {
        // Number of groups of 32 CNs for parallel processing
        // Ceil for values not divisible by 32
        M = (lut_numCnInCnGroups[2]*Z + 31)>>5;

        // Set the offset to each bit within a group in terms of 32 Byte
        bitOffsetInGroup = (lut_numCnInCnGroups_BG1_R13[2]*NR_LDPC_ZMAX)>>5;

        // Set pointers to start of group 5
        p_cnProcBuf256    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[2]];
        p_cnProcBufRes256 = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[2]];

        // Loop over every BN
        for (j=0; j<5; j++)
        {
            // Set of results pointer to correct BN address
            p_cnProcBufResBit = p_cnProcBufRes256 + (j*bitOffsetInGroup);

            // Loop over CNs
            for (i=0; i<M; i++)
            {
                // Abs and sign of 32 CNs (first BN)
                ymm0 = p_cnProcBuf256[lut_idxCnProcG5[j][0] + i];
                sgn  = _mm256_sign_epi8(*p_ones, ymm0);
                min  = _mm256_abs_epi8(ymm0);

                // Loop over BNs
                for (k=1; k<4; k++)
                {
                    ymm0 = p_cnProcBuf256[lut_idxCnProcG5[j][k] + i];
                    min  = _mm256_min_epu8(min, _mm256_abs_epi8(ymm0));
                    sgn  = _mm256_sign_epi8(sgn, ymm0);
                }

                // Store result
                min = _mm256_min_epu8(min, *p_maxLLR); // 128 in epi8 is -127
                *p_cnProcBufResBit = _mm256_sign_epi8(min, sgn);
                p_cnProcBufResBit++;
            }
        }

      // Process BNs with degree 1 in CNG5
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_bnProcBufTemp = &bnProcBufTemp[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[2]];
      for (i=0; i<20; i++)
      {
        nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[addrOffset_BG1_CNG5_SG1_R13[i][0]],
          Z, p_lut->circShift_CNG5_SG1[i]);
        p_bnProcBuf += Z;

        memcpy(p_bnProcBufRes, &llrProcBuf[bnIdx_BG1_CNG5_SG1_R13[i]*Z], Z);
        p_bnProcBufTemp += Z;
      }

      M = (20*Z + 31)>>5;
      p_bnProcBuf128 = (__m128i*) &bnProcBuf[0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes[0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      for (i=0,j=0; i<M; i++,j+=2)
      {
        // First 16 LLRs of first CN
        ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
        ymm1 = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);

        ymmRes0 = _mm256_adds_epi16(ymm0, ymm1);

        // Second 16 LLRs of first CN
        ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);
        ymm1 = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);

        ymmRes1 = _mm256_adds_epi16(ymm0, ymm1);

        // Pack results back to epi8
        ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
        // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
        // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
        *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

        // Next result
        p_bnProcBufTemp256++;
      }
      
      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (i=0; i<20; i++)
      {
        memcpy(&llrRes[bnIdx_BG1_CNG5_SG1_R13[i]*Z], p_bnProcBufTemp, Z);
        p_bnProcBufTemp += Z;
      }

      // Process BNs with degree 2 in CNG5
      // Arrange CN message values for addition
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[2]];
      bitOffsetInGroup = 8*NR_LDPC_ZMAX;
      for (i=0; i<2; i++)
      {
        p_bnProcBuf = &bnProcBuf[i*bitOffsetInGroup];
        for (j=0; j<8; j++)
        {
          nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[addrOffset_BG1_CNG5_SG2_R13[j][i]],
            Z, p_lut->circShift_CNG5_SG2[j]);
          p_bnProcBuf += Z;
        }
      }

      // Arrange input LLRs
      for (i=0; i<8; i++)
      {
        memcpy(p_bnProcBufRes, &p_llrRes[bnIdx_BG1_CNG5_SG2_R13[i]*Z], Z);
        p_bnProcBufRes += Z;
      }
      
      // Perform addition
      p_bnProcBuf128    = (__m128i*) &bnProcBuf    [0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes [0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      cnOffsetInGroup = (8*NR_LDPC_ZMAX)>>4;
      M = (8*Z + 31)>>5;
      for (i=0,j=0; i<M; i++,j+=2)
      {
          // First 16 LLRs of first CN
          ymmRes0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
          ymmRes1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);

          // Loop over CNs
          for (k=1; k<2; k++)
          {
              ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j]);
              ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

              ymm1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j+1]);
              ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);
          }

          // Add LLR from receiver input
          ymm0    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);
          ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

          ymm1    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);
          ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);

          // Pack results back to epi8
          ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
          // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
          // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
          *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

          // Next result
          p_bnProcBufTemp256++;
      }

      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (i=0; i<8; i++)
      {
        memcpy(&llrRes[bnIdx_BG1_CNG5_SG2_R13[i]*Z], p_bnProcBufTemp, Z);
        p_bnProcBufTemp += Z;
      }

      // Process BNs with degree 3 in CNG5
      // Arrange CN message values for addition
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[2]];
      bitOffsetInGroup = 6*NR_LDPC_ZMAX;
      for (i=0; i<3; i++)
      {
        p_bnProcBuf = &bnProcBuf[i*bitOffsetInGroup];
        for (j=0; j<6; j++)
        {
          nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[addrOffset_BG1_CNG5_SG3_R13[j][i]],
            Z, p_lut->circShift_CNG5_SG3[j]);
          p_bnProcBuf += Z;
        }
      }

      // Arrange input LLRs
      for (i=0; i<6; i++)
      {
        memcpy(p_bnProcBufRes, &p_llrRes[bnIdx_BG1_CNG5_SG3_R13[i]*Z], Z);
        p_bnProcBufRes += Z;
      }
      
      // Perform addition
      p_bnProcBuf128    = (__m128i*) &bnProcBuf    [0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes [0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      cnOffsetInGroup = (6*NR_LDPC_ZMAX)>>4;
      M = (6*Z + 31)>>5;
      for (i=0,j=0; i<M; i++,j+=2)
      {
          // First 16 LLRs of first CN
          ymmRes0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
          ymmRes1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);

          // Loop over CNs
          for (k=1; k<3; k++)
          {
              ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j]);
              ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

              ymm1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j+1]);
              ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);
          }

          // Add LLR from receiver input
          ymm0    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);
          ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

          ymm1    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);
          ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);

          // Pack results back to epi8
          ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
          // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
          // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
          *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

          // Next result
          p_bnProcBufTemp256++;
      }

      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (i=0; i<6; i++)
      {
        memcpy(&llrRes[bnIdx_BG1_CNG5_SG3_R13[i]*Z], p_bnProcBufTemp, Z);
        p_bnProcBufTemp += Z;
      }

      // Process BNs with degree 4 in CNG5
      // Arrange CN message values for addition
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[2]];
      bitOffsetInGroup = 2*NR_LDPC_ZMAX;
      for (i=0; i<4; i++)
      {
        p_bnProcBuf = &bnProcBuf[i*bitOffsetInGroup];
        for (j=0; j<2; j++)
        {
          nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[addrOffset_BG1_CNG5_SG4_R13[j][i]],
            Z, p_lut->circShift_CNG5_SG4[j]);
          p_bnProcBuf += Z;
        }
      }

      // Arrange input LLRs
      for (i=0; i<2; i++)
      {
        memcpy(p_bnProcBufRes, &p_llrRes[bnIdx_BG1_CNG5_SG4_R13[i]*Z], Z);
        p_bnProcBufRes += Z;
      }
      
      // Perform addition
      p_bnProcBuf128    = (__m128i*) &bnProcBuf    [0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes [0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      cnOffsetInGroup = (2*NR_LDPC_ZMAX)>>4;
      M = (2*Z + 31)>>5;
      for (i=0,j=0; i<M; i++,j+=2)
      {
          // First 16 LLRs of first CN
          ymmRes0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
          ymmRes1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);

          // Loop over CNs
          for (k=1; k<4; k++)
          {
              ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j]);
              ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

              ymm1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j+1]);
              ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);
          }

          // Add LLR from receiver input
          ymm0    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);
          ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

          ymm1    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);
          ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);

          // Pack results back to epi8
          ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
          // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
          // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
          *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

          // Next result
          p_bnProcBufTemp256++;
      }

      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (i=0; i<2; i++)
      {
        memcpy(&llrRes[bnIdx_BG1_CNG5_SG4_R13[i]*Z], p_bnProcBufTemp, Z);
        p_bnProcBufTemp += Z;
      }

      // Process BNs with degree 5 in CNG5
      // Arrange CN message values for addition
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[2]];
      bitOffsetInGroup = 2*NR_LDPC_ZMAX;
      for (i=0; i<5; i++)
      {
        p_bnProcBuf = &bnProcBuf[i*bitOffsetInGroup];
        for (j=0; j<2; j++)
        {
          nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[addrOffset_BG1_CNG5_SG5_R13[j][i]],
            Z, p_lut->circShift_CNG5_SG5[j]);
          p_bnProcBuf += Z;
        }
      }

      // Arrange input LLRs
      for (i=0; i<2; i++)
      {
        memcpy(p_bnProcBufRes, &p_llrRes[bnIdx_BG1_CNG5_SG5_R13[i]*Z], Z);
        p_bnProcBufRes += Z;
      }
      
      // Perform addition
      p_bnProcBuf128    = (__m128i*) &bnProcBuf    [0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes [0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      cnOffsetInGroup = (2*NR_LDPC_ZMAX)>>4;
      M = (2*Z + 31)>>5;
      for (i=0,j=0; i<M; i++,j+=2)
      {
          // First 16 LLRs of first CN
          ymmRes0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
          ymmRes1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);

          // Loop over CNs
          for (k=1; k<5; k++)
          {
              ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j]);
              ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

              ymm1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j+1]);
              ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);
          }

          // Add LLR from receiver input
          ymm0    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);
          ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

          ymm1    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);
          ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);

          // Pack results back to epi8
          ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
          // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
          // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
          *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

          // Next result
          p_bnProcBufTemp256++;
      }

      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (i=0; i<2; i++)
      {
        memcpy(&llrRes[bnIdx_BG1_CNG5_SG5_R13[i]*Z], p_bnProcBufTemp, Z);
        p_bnProcBufTemp += Z;
      }

      // Process BNs with degree 9 in CNG5
      // Arrange CN message values for addition
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[2]];
      bitOffsetInGroup = 2*NR_LDPC_ZMAX;
      for (i=0; i<9; i++)
      {
        p_bnProcBuf = &bnProcBuf[i*bitOffsetInGroup];
        for (j=0; j<2; j++)
        {
          nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[addrOffset_BG1_CNG5_SG9_R13[j][i]],
            Z, p_lut->circShift_CNG5_SG9[j]);
          p_bnProcBuf += Z;
        }
      }

      // Arrange input LLRs
      for (i=0; i<2; i++)
      {
        memcpy(p_bnProcBufRes, &p_llrRes[bnIdx_BG1_CNG5_SG9_R13[i]*Z], Z);
        p_bnProcBufRes += Z;
      }
      
      // Perform addition
      p_bnProcBuf128    = (__m128i*) &bnProcBuf    [0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes [0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      cnOffsetInGroup = (2*NR_LDPC_ZMAX)>>4;
      M = (2*Z + 31)>>5;
      for (i=0,j=0; i<M; i++,j+=2)
      {
          // First 16 LLRs of first CN
          ymmRes0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
          ymmRes1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);

          // Loop over CNs
          for (k=1; k<9; k++)
          {
              ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j]);
              ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

              ymm1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j+1]);
              ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);
          }

          // Add LLR from receiver input
          ymm0    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);
          ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

          ymm1    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);
          ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);

          // Pack results back to epi8
          ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
          // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
          // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
          *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

          // Next result
          p_bnProcBufTemp256++;
      }

      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (i=0; i<2; i++)
      {
        memcpy(&llrRes[bnIdx_BG1_CNG5_SG9_R13[i]*Z], p_bnProcBufTemp, Z);
        p_bnProcBufTemp += Z;
      }
    }

    // =====================================================================
#ifdef NR_LDPC_PROFILER_DETAIL
    start_meas(&p_profiler->llr2CnProcBuf);
#endif
    bitOffsetInGroup = lut_numCnInCnGroups_BG1_R13[3]*NR_LDPC_ZMAX;

    for (j=0; j<6; j++)
    {
        p_cnProcBuf = &cnProcBuf[lut_startAddrCnGroups[3] + j*bitOffsetInGroup];

        for (i=0; i<lut_numCnInCnGroups[3]; i++)
        {
            idxBn = lut_posBnInCnProcBuf_CNG6[j][i]*Z;
            nrLDPC_circ_memcpy(p_cnProcBuf, &llrRes[idxBn], Z, lut_circShift_CNG6[j][i]);
            p_cnProcBuf += Z;
        }
    }
#ifdef NR_LDPC_PROFILER_DETAIL
    stop_meas(&p_profiler->llr2CnProcBuf);
#endif
    // Process group with 6 BNs

    // Offset is 8*384/32 = 96
    const uint16_t lut_idxCnProcG6[6][5] = {{96,192,288,384,480}, {0,192,288,384,480},
                                            {0,96,288,384,480}, {0,96,192,384,480},
                                            {0,96,192,288,480}, {0,96,192,288,384}};

    if (lut_numCnInCnGroups[3] > 0)
    {
        // Number of groups of 32 CNs for parallel processing
        // Ceil for values not divisible by 32
        M = (lut_numCnInCnGroups[3]*Z + 31)>>5;

        // Set the offset to each bit within a group in terms of 32 Byte
        bitOffsetInGroup = (lut_numCnInCnGroups_BG1_R13[3]*NR_LDPC_ZMAX)>>5;

        // Set pointers to start of group 6
        p_cnProcBuf256    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[3]];
        p_cnProcBufRes256 = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[3]];

        // Loop over every BN
        for (j=0; j<6; j++)
        {
            // Set of results pointer to correct BN address
            p_cnProcBufResBit = (__m256i*) p_cnProcBufRes + (j*bitOffsetInGroup);

            // Loop over CNs
            for (i=0; i<M; i++)
            {
                // Abs and sign of 32 CNs (first BN)
                ymm0 = p_cnProcBuf256[lut_idxCnProcG6[j][0] + i];
                sgn  = _mm256_sign_epi8(*p_ones, ymm0);
                min  = _mm256_abs_epi8(ymm0);

                // Loop over BNs
                for (k=1; k<5; k++)
                {
                    ymm0 = p_cnProcBuf256[lut_idxCnProcG6[j][k] + i];
                    min  = _mm256_min_epu8(min, _mm256_abs_epi8(ymm0));
                    sgn  = _mm256_sign_epi8(sgn, ymm0);
                }

                // Store result
                min = _mm256_min_epu8(min, *p_maxLLR); // 128 in epi8 is -127
                *p_cnProcBufResBit = _mm256_sign_epi8(min, sgn);
                p_cnProcBufResBit++;
            }
        }

      // Process BNs with degree 1 in CNG6
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_bnProcBufTemp = &bnProcBufTemp[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[3]];
      for (i=0; i<20; i++)
      {
        nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[addrOffset_BG1_CNG6_SG1_R13[i][0]],
          Z, p_lut->circShift_CNG6_SG1[i]);
        p_bnProcBuf += Z;

        memcpy(p_bnProcBufRes, &llrProcBuf[bnIdx_BG1_CNG6_SG1_R13[i]*Z], Z);
        p_bnProcBufTemp += Z;
      }

      M = (20*Z + 31)>>5;
      p_bnProcBuf128 = (__m128i*) &bnProcBuf[0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes[0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      for (i=0,j=0; i<M; i++,j+=2)
      {
        // First 16 LLRs of first CN
        ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
        ymm1 = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);

        ymmRes0 = _mm256_adds_epi16(ymm0, ymm1);

        // Second 16 LLRs of first CN
        ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);
        ymm1 = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);

        ymmRes1 = _mm256_adds_epi16(ymm0, ymm1);

        // Pack results back to epi8
        ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
        // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
        // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
        *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

        // Next result
        p_bnProcBufTemp256++;
      }
      
      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (i=0; i<20; i++)
      {
        memcpy(&llrRes[bnIdx_BG1_CNG6_SG1_R13[i]*Z], p_bnProcBufTemp, Z);
        p_bnProcBufTemp += Z;
      }

      // Process BNs with degree 2 in CNG6
      // Arrange CN message values for addition
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[3]];
      bitOffsetInGroup = 3*NR_LDPC_ZMAX;
      for (i=0; i<2; i++)
      {
        p_bnProcBuf = &bnProcBuf[i*bitOffsetInGroup];
        for (j=0; j<3; j++)
        {
          nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[addrOffset_BG1_CNG6_SG2_R13[j][i]],
            Z, p_lut->circShift_CNG6_SG2[j]);
          p_bnProcBuf += Z;
        }
      }

      // Arrange input LLRs
      for (i=0; i<3; i++)
      {
        memcpy(p_bnProcBufRes, &p_llrRes[bnIdx_BG1_CNG6_SG2_R13[i]*Z], Z);
        p_bnProcBufRes += Z;
      }
      
      // Perform addition
      p_bnProcBuf128    = (__m128i*) &bnProcBuf    [0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes [0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      cnOffsetInGroup = (3*NR_LDPC_ZMAX)>>4;
      M = (3*Z + 31)>>5;
      for (i=0,j=0; i<M; i++,j+=2)
      {
          // First 16 LLRs of first CN
          ymmRes0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
          ymmRes1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);

          // Loop over CNs
          for (k=1; k<2; k++)
          {
              ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j]);
              ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

              ymm1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j+1]);
              ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);
          }

          // Add LLR from receiver input
          ymm0    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);
          ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

          ymm1    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);
          ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);

          // Pack results back to epi8
          ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
          // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
          // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
          *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

          // Next result
          p_bnProcBufTemp256++;
      }

      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (i=0; i<3; i++)
      {
        memcpy(&llrRes[bnIdx_BG1_CNG6_SG2_R13[i]*Z], p_bnProcBufTemp, Z);
        p_bnProcBufTemp += Z;
      }

      // Process BNs with degree 3 in CNG6
      // Arrange CN message values for addition
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[3]];
      bitOffsetInGroup = 3*NR_LDPC_ZMAX;
      for (i=0; i<3; i++)
      {
        p_bnProcBuf = &bnProcBuf[i*bitOffsetInGroup];
        for (j=0; j<3; j++)
        {
          nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[addrOffset_BG1_CNG6_SG3_R13[j][i]],
            Z, p_lut->circShift_CNG6_SG3[j]);
          p_bnProcBuf += Z;
        }
      }

      // Arrange input LLRs
      for (i=0; i<3; i++)
      {
        memcpy(p_bnProcBufRes, &p_llrRes[bnIdx_BG1_CNG6_SG3_R13[i]*Z], Z);
        p_bnProcBufRes += Z;
      }
      
      // Perform addition
      p_bnProcBuf128    = (__m128i*) &bnProcBuf    [0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes [0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      cnOffsetInGroup = (3*NR_LDPC_ZMAX)>>4;
      M = (3*Z + 31)>>5;
      for (i=0,j=0; i<M; i++,j+=2)
      {
          // First 16 LLRs of first CN
          ymmRes0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
          ymmRes1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);

          // Loop over CNs
          for (k=1; k<3; k++)
          {
              ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j]);
              ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

              ymm1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j+1]);
              ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);
          }

          // Add LLR from receiver input
          ymm0    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);
          ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

          ymm1    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);
          ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);

          // Pack results back to epi8
          ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
          // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
          // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
          *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

          // Next result
          p_bnProcBufTemp256++;
      }

      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (i=0; i<3; i++)
      {
        memcpy(&llrRes[bnIdx_BG1_CNG6_SG3_R13[i]*Z], p_bnProcBufTemp, Z);
        p_bnProcBufTemp += Z;
      }

      // Process BNs with degree 4 in CNG6
      // Arrange CN message values for addition
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[2]];
      bitOffsetInGroup = 2*NR_LDPC_ZMAX;
      for (i=0; i<4; i++)
      {
        p_bnProcBuf = &bnProcBuf[i*bitOffsetInGroup];
        for (j=0; j<2; j++)
        {
          nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[addrOffset_BG1_CNG6_SG4_R13[j][i]],
            Z, p_lut->circShift_CNG6_SG4[j]);
          p_bnProcBuf += Z;
        }
      }

      // Arrange input LLRs
      for (i=0; i<2; i++)
      {
        memcpy(p_bnProcBufRes, &p_llrRes[bnIdx_BG1_CNG6_SG4_R13[i]*Z], Z);
        p_bnProcBufRes += Z;
      }
      
      // Perform addition
      p_bnProcBuf128    = (__m128i*) &bnProcBuf    [0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes [0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      cnOffsetInGroup = (2*NR_LDPC_ZMAX)>>4;
      M = (2*Z + 31)>>5;
      for (i=0,j=0; i<M; i++,j+=2)
      {
          // First 16 LLRs of first CN
          ymmRes0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
          ymmRes1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);

          // Loop over CNs
          for (k=1; k<4; k++)
          {
              ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j]);
              ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

              ymm1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j+1]);
              ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);
          }

          // Add LLR from receiver input
          ymm0    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);
          ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

          ymm1    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);
          ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);

          // Pack results back to epi8
          ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
          // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
          // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
          *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

          // Next result
          p_bnProcBufTemp256++;
      }

      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (i=0; i<2; i++)
      {
        memcpy(&llrRes[bnIdx_BG1_CNG6_SG4_R13[i]*Z], p_bnProcBufTemp, Z);
        p_bnProcBufTemp += Z;
      }

      // Process BNs with degree 5 in CNG6
      // Arrange CN message values for addition
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[3]];
      bitOffsetInGroup = 1*NR_LDPC_ZMAX;
      for (i=0; i<5; i++)
      {
        p_bnProcBuf = &bnProcBuf[i*bitOffsetInGroup];
        nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[addrOffset_BG1_CNG6_SG5_R13[0][i]],
          Z, p_lut->circShift_CNG6_SG5[i]);
        p_bnProcBuf += Z;
      }

      // Perform addition
      p_bnProcBuf128    = (__m128i*) &bnProcBuf    [0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes [0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      cnOffsetInGroup = (1*NR_LDPC_ZMAX)>>4;
      M = (1*Z + 31)>>5;
      for (i=0,j=0; i<M; i++,j+=2)
      {
          // First 16 LLRs of first CN
          ymmRes0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
          ymmRes1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);

          // Loop over CNs
          for (k=1; k<5; k++)
          {
              ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j]);
              ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

              ymm1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j+1]);
              ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);
          }

          // Add LLR from receiver input
          p_llrProcBuf128   = (__m128i*) &llrProcBuf[bnIdx_BG1_CNG6_SG5_R13[0]*Z];
          ymm0    = _mm256_cvtepi8_epi16(p_llrProcBuf128[j]);
          ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

          ymm1    = _mm256_cvtepi8_epi16(p_llrProcBuf128[j+1]);
          ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);

          // Pack results back to epi8
          ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
          // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
          // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
          *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

          // Next result
          p_bnProcBufTemp256++;
      }

      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      memcpy(&llrRes[bnIdx_BG1_CNG6_SG5_R13[0]*Z], p_bnProcBufTemp, Z);
    }

    // =====================================================================
#ifdef NR_LDPC_PROFILER_DETAIL
    start_meas(&p_profiler->llr2CnProcBuf);
#endif
    bitOffsetInGroup = lut_numCnInCnGroups_BG1_R13[4]*NR_LDPC_ZMAX;

    for (j=0; j<7; j++)
    {
        p_cnProcBuf = &cnProcBuf[lut_startAddrCnGroups[4] + j*bitOffsetInGroup];

        for (i=0; i<lut_numCnInCnGroups[4]; i++)
        {
            idxBn = lut_posBnInCnProcBuf_CNG7[j][i]*Z;
            nrLDPC_circ_memcpy(p_cnProcBuf, &llrRes[idxBn], Z, lut_circShift_CNG7[j][i]);
            p_cnProcBuf += Z;
        }
    }
#ifdef NR_LDPC_PROFILER_DETAIL
    stop_meas(&p_profiler->llr2CnProcBuf);
#endif
    // Process group with 7 BNs

    // Offset is 5*384/32 = 60
    const uint16_t lut_idxCnProcG7[7][6] = {{60,120,180,240,300,360}, {0,120,180,240,300,360},
                                            {0,60,180,240,300,360},   {0,60,120,240,300,360},
                                            {0,60,120,180,300,360},   {0,60,120,180,240,360},
                                            {0,60,120,180,240,300}};

    if (lut_numCnInCnGroups[4] > 0)
    {
        // Number of groups of 32 CNs for parallel processing
        // Ceil for values not divisible by 32
        M = (lut_numCnInCnGroups[4]*Z + 31)>>5;

        // Set the offset to each bit within a group in terms of 32 Byte
        bitOffsetInGroup = (lut_numCnInCnGroups_BG1_R13[4]*NR_LDPC_ZMAX)>>5;

        // Set pointers to start of group 7
        p_cnProcBuf256    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[4]];
        p_cnProcBufRes256 = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[4]];

        // Loop over every BN
        for (j=0; j<7; j++)
        {
            // Set of results pointer to correct BN address
            p_cnProcBufResBit = p_cnProcBufRes256 + (j*bitOffsetInGroup);

            // Loop over CNs
            for (i=0; i<M; i++)
            {
                // Abs and sign of 32 CNs (first BN)
                ymm0 = p_cnProcBuf256[lut_idxCnProcG7[j][0] + i];
                sgn  = _mm256_sign_epi8(*p_ones, ymm0);
                min  = _mm256_abs_epi8(ymm0);

                // Loop over BNs
                for (k=1; k<6; k++)
                {
                    ymm0 = p_cnProcBuf256[lut_idxCnProcG7[j][k] + i];
                    min  = _mm256_min_epu8(min, _mm256_abs_epi8(ymm0));
                    sgn  = _mm256_sign_epi8(sgn, ymm0);
                }

                // Store result
                min = _mm256_min_epu8(min, *p_maxLLR); // 128 in epi8 is -127
                *p_cnProcBufResBit = _mm256_sign_epi8(min, sgn);
                p_cnProcBufResBit++;
            }
        }

      // Process BNs with degree 1 in CNG7
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_bnProcBufTemp = &bnProcBufTemp[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[4]];
      for (i=0; i<p_lut->numBnDegCNG7[0]; i++)
      {
        nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[lut_addrOffset_CNG7_SG1[i][0]],
          Z, p_lut->circShift_CNG7_SG1[i]);
        p_bnProcBuf += Z;

        memcpy(p_bnProcBufRes, &llrProcBuf[p_lut->bnIdx_CNG7_SG1[i]*Z], Z);
        p_bnProcBufTemp += Z;
      }

      M = (p_lut->numBnDegCNG7[0]*Z + 31)>>5;
      p_bnProcBuf128 = (__m128i*) &bnProcBuf[0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes[0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      for (i=0,j=0; i<M; i++,j+=2)
      {
        // First 16 LLRs of first CN
        ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
        ymm1 = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);

        ymmRes0 = _mm256_adds_epi16(ymm0, ymm1);

        // Second 16 LLRs of first CN
        ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);
        ymm1 = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);

        ymmRes1 = _mm256_adds_epi16(ymm0, ymm1);

        // Pack results back to epi8
        ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
        // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
        // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
        *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

        // Next result
        p_bnProcBufTemp256++;
      }
      
      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (i=0; i<p_lut->numBnDegCNG7[0]; i++)
      {
        memcpy(&llrRes[p_lut->bnIdx_CNG7_SG1[i]*Z], p_bnProcBufTemp, Z);
        p_bnProcBufTemp += Z;
      }

      // Process BNs with degree 2 in CNG7
      // Arrange CN message values for addition
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[4]];
      bitOffsetInGroup = p_lut->numBnDegCNG7[1]*NR_LDPC_ZMAX;
      for (i=0; i<2; i++)
      {
        p_bnProcBuf = &bnProcBuf[i*bitOffsetInGroup];
        for (j=0; j<p_lut->numBnDegCNG7[1]; j++)
        {
          nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[lut_addrOffset_CNG7_SG2[j][i]],
            Z, p_lut->circShift_CNG7_SG2[j]);
          p_bnProcBuf += Z;
        }
      }

      // Arrange input LLRs
      for (i=0; i<p_lut->numBnDegCNG7[1]; i++)
      {
        memcpy(p_bnProcBufRes, &p_llrRes[p_lut->bnIdx_CNG7_SG2[i]*Z], Z);
        p_bnProcBufRes += Z;
      }
      
      // Perform addition
      p_bnProcBuf128    = (__m128i*) &bnProcBuf    [0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes [0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      cnOffsetInGroup = (p_lut->numBnDegCNG7[1]*NR_LDPC_ZMAX)>>4;
      M = (p_lut->numBnDegCNG7[1]*Z + 31)>>5;
      for (i=0,j=0; i<M; i++,j+=2)
      {
          // First 16 LLRs of first CN
          ymmRes0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
          ymmRes1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);

          // Loop over CNs
          for (k=1; k<2; k++)
          {
              ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j]);
              ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

              ymm1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j+1]);
              ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);
          }

          // Add LLR from receiver input
          ymm0    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);
          ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

          ymm1    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);
          ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);

          // Pack results back to epi8
          ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
          // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
          // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
          *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

          // Next result
          p_bnProcBufTemp256++;
      }

      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (i=0; i<p_lut->numBnDegCNG7[1]; i++)
      {
        memcpy(&llrRes[p_lut->bnIdx_CNG7_SG2[i]*Z], p_bnProcBufTemp, Z);
        p_bnProcBufTemp += Z;
      }

      // Process BNs with degree 3 in CNG7
      // Arrange CN message values for addition
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[4]];
      bitOffsetInGroup = p_lut->numBnDegCNG7[2]*NR_LDPC_ZMAX;
      for (i=0; i<4; i++)
      {
        p_bnProcBuf = &bnProcBuf[i*bitOffsetInGroup];
        for (j=0; j<p_lut->numBnDegCNG7[2]; j++)
        {
          nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[lut_addrOffset_CNG7_SG3[j][i]],
            Z, p_lut->circShift_CNG7_SG3[j]);
          p_bnProcBuf += Z;
        }
      }

      // Arrange input LLRs
      for (i=0; i<p_lut->numBnDegCNG7[2]; i++)
      {
        memcpy(p_bnProcBufRes, &p_llrRes[p_lut->bnIdx_CNG7_SG4[i]*Z], Z);
        p_bnProcBufRes += Z;
      }
      
      // Perform addition
      p_bnProcBuf128    = (__m128i*) &bnProcBuf    [0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes [0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      cnOffsetInGroup = (p_lut->numBnDegCNG7[2]*NR_LDPC_ZMAX)>>4;
      M = (2*Z + 31)>>5;
      for (i=0,j=0; i<M; i++,j+=2)
      {
          // First 16 LLRs of first CN
          ymmRes0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
          ymmRes1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);

          // Loop over CNs
          for (k=1; k<3; k++)
          {
              ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j]);
              ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

              ymm1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j+1]);
              ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);
          }

          // Add LLR from receiver input
          ymm0    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);
          ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

          ymm1    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);
          ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);

          // Pack results back to epi8
          ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
          // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
          // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
          *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

          // Next result
          p_bnProcBufTemp256++;
      }

      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (i=0; i<p_lut->numBnDegCNG7[2]; i++)
      {
        memcpy(&llrRes[p_lut->bnIdx_CNG7_SG4[i]*Z], p_bnProcBufTemp, Z);
        p_bnProcBufTemp += Z;
      }

      // Process BNs with degree 4 in CNG7
      // Arrange CN message values for addition
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[4]];
      bitOffsetInGroup = p_lut->numBnDegCNG7[3]*NR_LDPC_ZMAX;
      for (i=0; i<4; i++)
      {
        p_bnProcBuf = &bnProcBuf[i*bitOffsetInGroup];
        for (j=0; j<p_lut->numBnDegCNG7[3]; j++)
        {
          nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[lut_addrOffset_CNG7_SG4[j][i]],
            Z, p_lut->circShift_CNG7_SG4[j]);
          p_bnProcBuf += Z;
        }
      }

      // Arrange input LLRs
      for (i=0; i<p_lut->numBnDegCNG7[3]; i++)
      {
        memcpy(p_bnProcBufRes, &p_llrRes[p_lut->bnIdx_CNG7_SG4[i]*Z], Z);
        p_bnProcBufRes += Z;
      }
      
      // Perform addition
      p_bnProcBuf128    = (__m128i*) &bnProcBuf    [0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes [0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      cnOffsetInGroup = (p_lut->numBnDegCNG7[3]*NR_LDPC_ZMAX)>>4;
      M = (p_lut->numBnDegCNG7[3]*Z + 31)>>5;
      for (i=0,j=0; i<M; i++,j+=2)
      {
          // First 16 LLRs of first CN
          ymmRes0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
          ymmRes1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);

          // Loop over CNs
          for (k=1; k<4; k++)
          {
              ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j]);
              ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

              ymm1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j+1]);
              ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);
          }

          // Add LLR from receiver input
          ymm0    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);
          ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

          ymm1    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);
          ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);

          // Pack results back to epi8
          ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
          // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
          // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
          *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

          // Next result
          p_bnProcBufTemp256++;
      }

      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (i=0; i<p_lut->numBnDegCNG7[3]; i++)
      {
        memcpy(&llrRes[p_lut->bnIdx_CNG7_SG4[i]*Z], p_bnProcBufTemp, Z);
        p_bnProcBufTemp += Z;
      }
    }

    // =====================================================================
#ifdef NR_LDPC_PROFILER_DETAIL
    start_meas(&p_profiler->llr2CnProcBuf);
#endif
    bitOffsetInGroup = lut_numCnInCnGroups_BG1_R13[5]*NR_LDPC_ZMAX;

    for (j=0; j<8; j++)
    {
        p_cnProcBuf = &cnProcBuf[lut_startAddrCnGroups[5] + j*bitOffsetInGroup];

        for (i=0; i<lut_numCnInCnGroups[5]; i++)
        {
            idxBn = lut_posBnInCnProcBuf_CNG8[j][i]*Z;
            nrLDPC_circ_memcpy(p_cnProcBuf, &llrRes[idxBn], Z, lut_circShift_CNG8[j][i]);
            p_cnProcBuf += Z;
        }
    }
#ifdef NR_LDPC_PROFILER_DETAIL
    stop_meas(&p_profiler->llr2CnProcBuf);
#endif
    // Process group with 8 BNs

    // Offset is 2*384/32 = 24
    const uint8_t lut_idxCnProcG8[8][7] = {{24,48,72,96,120,144,168}, {0,48,72,96,120,144,168},
                                           {0,24,72,96,120,144,168}, {0,24,48,96,120,144,168},
                                           {0,24,48,72,120,144,168}, {0,24,48,72,96,144,168},
                                           {0,24,48,72,96,120,168}, {0,24,48,72,96,120,144}};

    if (lut_numCnInCnGroups[5] > 0)
    {
        // Number of groups of 32 CNs for parallel processing
        // Ceil for values not divisible by 32
        M = (lut_numCnInCnGroups[5]*Z + 31)>>5;

        // Set the offset to each bit within a group in terms of 32 Byte
        bitOffsetInGroup = (lut_numCnInCnGroups_BG1_R13[5]*NR_LDPC_ZMAX)>>5;

        // Set pointers to start of group 8
        p_cnProcBuf256    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[5]];
        p_cnProcBufRes256 = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[5]];

        // Loop over every BN
        for (j=0; j<8; j++)
        {
            // Set of results pointer to correct BN address
            p_cnProcBufResBit = p_cnProcBufRes256 + (j*bitOffsetInGroup);

            // Loop over CNs
            for (i=0; i<M; i++)
            {
                // Abs and sign of 32 CNs (first BN)
                ymm0 = p_cnProcBuf256[lut_idxCnProcG8[j][0] + i];
                sgn  = _mm256_sign_epi8(*p_ones, ymm0);
                min  = _mm256_abs_epi8(ymm0);

                // Loop over BNs
                for (k=1; k<7; k++)
                {
                    ymm0 = p_cnProcBuf256[lut_idxCnProcG8[j][k] + i];
                    min  = _mm256_min_epu8(min, _mm256_abs_epi8(ymm0));
                    sgn  = _mm256_sign_epi8(sgn, ymm0);
                }

                // Store result
                min = _mm256_min_epu8(min, *p_maxLLR); // 128 in epi8 is -127
                *p_cnProcBufResBit = _mm256_sign_epi8(min, sgn);
                p_cnProcBufResBit++;
            }
        }

      // Process BNs with degree 1 in CNG8
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_bnProcBufTemp = &bnProcBufTemp[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[5]];
      for (i=0; i<4; i++)
      {
        nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[addrOffset_BG1_CNG8_SG1_R13[i][0]],
          Z, p_lut->circShift_CNG8_SG1[i]);
        p_bnProcBuf += Z;

        memcpy(p_bnProcBufRes, &llrProcBuf[bnIdx_BG1_CNG8_SG1_R13[i]*Z], Z);
        p_bnProcBufTemp += Z;
      }

      M = (4*Z + 31)>>5;
      p_bnProcBuf128 = (__m128i*) &bnProcBuf[0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes[0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      for (i=0,j=0; i<M; i++,j+=2)
      {
        // First 16 LLRs of first CN
        ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
        ymm1 = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);

        ymmRes0 = _mm256_adds_epi16(ymm0, ymm1);

        // Second 16 LLRs of first CN
        ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);
        ymm1 = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);

        ymmRes1 = _mm256_adds_epi16(ymm0, ymm1);

        // Pack results back to epi8
        ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
        // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
        // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
        *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

        // Next result
        p_bnProcBufTemp256++;
      }
      
      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (i=0; i<4; i++)
      {
        memcpy(&llrRes[bnIdx_BG1_CNG8_SG1_R13[i]*Z], p_bnProcBufTemp, Z);
        p_bnProcBufTemp += Z;
      }

      // Process BNs with degree 2 in CNG8
      // Arrange CN message values for addition
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[5]];
      bitOffsetInGroup = 6*NR_LDPC_ZMAX;
      for (i=0; i<2; i++)
      {
        p_bnProcBuf = &bnProcBuf[i*bitOffsetInGroup];
        for (j=0; j<6; j++)
        {
          nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[addrOffset_BG1_CNG8_SG2_R13[j][i]],
            Z, p_lut->circShift_CNG8_SG2[j]);
          p_bnProcBuf += Z;
        }
      }

      // Arrange input LLRs
      for (i=0; i<6; i++)
      {
        memcpy(p_bnProcBufRes, &p_llrRes[bnIdx_BG1_CNG8_SG2_R13[i]*Z], Z);
        p_bnProcBufRes += Z;
      }
      
      // Perform addition
      p_bnProcBuf128    = (__m128i*) &bnProcBuf    [0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes [0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      cnOffsetInGroup = (6*NR_LDPC_ZMAX)>>4;
      M = (6*Z + 31)>>5;
      for (i=0,j=0; i<M; i++,j+=2)
      {
          // First 16 LLRs of first CN
          ymmRes0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
          ymmRes1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);

          // Loop over CNs
          for (k=1; k<2; k++)
          {
              ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j]);
              ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

              ymm1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j+1]);
              ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);
          }

          // Add LLR from receiver input
          ymm0    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);
          ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

          ymm1    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);
          ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);

          // Pack results back to epi8
          ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
          // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
          // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
          *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

          // Next result
          p_bnProcBufTemp256++;
      }

      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (i=0; i<6; i++)
      {
        memcpy(&llrRes[bnIdx_BG1_CNG8_SG2_R13[i]*Z], p_bnProcBufTemp, Z);
        p_bnProcBufTemp += Z;
      }
    }

    // =====================================================================
#ifdef NR_LDPC_PROFILER_DETAIL
    start_meas(&p_profiler->llr2CnProcBuf);
#endif
    bitOffsetInGroup = lut_numCnInCnGroups_BG1_R13[6]*NR_LDPC_ZMAX;

    for (j=0; j<9; j++)
    {
        p_cnProcBuf = &cnProcBuf[lut_startAddrCnGroups[6] + j*bitOffsetInGroup];

        for (i=0; i<lut_numCnInCnGroups[6]; i++)
        {
            idxBn = lut_posBnInCnProcBuf_CNG9[j][i]*Z;
            nrLDPC_circ_memcpy(p_cnProcBuf, &llrRes[idxBn], Z, lut_circShift_CNG9[j][i]);
            p_cnProcBuf += Z;
        }
    }
#ifdef NR_LDPC_PROFILER_DETAIL
    stop_meas(&p_profiler->llr2CnProcBuf);
#endif
    // Process group with 9 BNs

    // Offset is 2*384/32 = 24
    const uint8_t lut_idxCnProcG9[9][8] = {{24,48,72,96,120,144,168,192}, {0,48,72,96,120,144,168,192},
                                           {0,24,72,96,120,144,168,192}, {0,24,48,96,120,144,168,192},
                                           {0,24,48,72,120,144,168,192}, {0,24,48,72,96,144,168,192},
                                           {0,24,48,72,96,120,168,192}, {0,24,48,72,96,120,144,192},
                                           {0,24,48,72,96,120,144,168}};

    if (lut_numCnInCnGroups[6] > 0)
    {
        // Number of groups of 32 CNs for parallel processing
        // Ceil for values not divisible by 32
        M = (lut_numCnInCnGroups[6]*Z + 31)>>5;

        // Set the offset to each bit within a group in terms of 32 Byte
        bitOffsetInGroup = (lut_numCnInCnGroups_BG1_R13[6]*NR_LDPC_ZMAX)>>5;

        // Set pointers to start of group 9
        p_cnProcBuf256    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[6]];
        p_cnProcBufRes256 = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[6]];

        // Loop over every BN
        for (j=0; j<9; j++)
        {
            // Set of results pointer to correct BN address
            p_cnProcBufResBit = p_cnProcBufRes256 + (j*bitOffsetInGroup);

            // Loop over CNs
            for (i=0; i<M; i++)
            {
                // Abs and sign of 32 CNs (first BN)
                ymm0 = p_cnProcBuf256[lut_idxCnProcG9[j][0] + i];
                sgn  = _mm256_sign_epi8(*p_ones, ymm0);
                min  = _mm256_abs_epi8(ymm0);

                // Loop over BNs
                for (k=1; k<8; k++)
                {
                    ymm0 = p_cnProcBuf256[lut_idxCnProcG9[j][k] + i];
                    min  = _mm256_min_epu8(min, _mm256_abs_epi8(ymm0));
                    sgn  = _mm256_sign_epi8(sgn, ymm0);
                }

                // Store result
                min = _mm256_min_epu8(min, *p_maxLLR); // 128 in epi8 is -127
                *p_cnProcBufResBit = _mm256_sign_epi8(min, sgn);
                p_cnProcBufResBit++;
            }
        }

      // Process BNs with degree 1 in CNG9
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_bnProcBufTemp = &bnProcBufTemp[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[6]];
      for (i=0; i<4; i++)
      {
        nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[addrOffset_BG1_CNG9_SG1_R13[i][0]],
          Z, p_lut->circShift_CNG9_SG1[i]);
        p_bnProcBuf += Z;

        memcpy(p_bnProcBufRes, &llrProcBuf[bnIdx_BG1_CNG9_SG1_R13[i]*Z], Z);
        p_bnProcBufTemp += Z;
      }

      M = (4*Z + 31)>>5;
      p_bnProcBuf128 = (__m128i*) &bnProcBuf[0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes[0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      for (i=0,j=0; i<M; i++,j+=2)
      {
        // First 16 LLRs of first CN
        ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
        ymm1 = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);

        ymmRes0 = _mm256_adds_epi16(ymm0, ymm1);

        // Second 16 LLRs of first CN
        ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);
        ymm1 = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);

        ymmRes1 = _mm256_adds_epi16(ymm0, ymm1);

        // Pack results back to epi8
        ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
        // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
        // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
        *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

        // Next result
        p_bnProcBufTemp256++;
      }
      
      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (i=0; i<4; i++)
      {
        memcpy(&llrRes[bnIdx_BG1_CNG9_SG1_R13[i]*Z], p_bnProcBufTemp, Z);
        p_bnProcBufTemp += Z;
      }

      // Process BNs with degree 2 in CNG9
      // Arrange CN message values for addition
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[6]];
      bitOffsetInGroup = 7*NR_LDPC_ZMAX;
      for (i=0; i<2; i++)
      {
        p_bnProcBuf = &bnProcBuf[i*bitOffsetInGroup];
        for (j=0; j<7; j++)
        {
          nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[addrOffset_BG1_CNG9_SG2_R13[j][i]],
            Z, p_lut->circShift_CNG9_SG2[j]);
          p_bnProcBuf += Z;
        }
      }

      // Arrange input LLRs
      for (i=0; i<7; i++)
      {
        memcpy(p_bnProcBufRes, &p_llrRes[bnIdx_BG1_CNG9_SG2_R13[i]*Z], Z);
        p_bnProcBufRes += Z;
      }
      
      // Perform addition
      p_bnProcBuf128    = (__m128i*) &bnProcBuf    [0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes [0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      cnOffsetInGroup = (7*NR_LDPC_ZMAX)>>4;
      M = (7*Z + 31)>>5;
      for (i=0,j=0; i<M; i++,j+=2)
      {
          // First 16 LLRs of first CN
          ymmRes0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
          ymmRes1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);

          // Loop over CNs
          for (k=1; k<2; k++)
          {
              ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j]);
              ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

              ymm1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j+1]);
              ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);
          }

          // Add LLR from receiver input
          ymm0    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);
          ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

          ymm1    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);
          ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);

          // Pack results back to epi8
          ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
          // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
          // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
          *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

          // Next result
          p_bnProcBufTemp256++;
      }

      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (i=0; i<7; i++)
      {
        memcpy(&llrRes[bnIdx_BG1_CNG9_SG2_R13[i]*Z], p_bnProcBufTemp, Z);
        p_bnProcBufTemp += Z;
      }
    }

    // =====================================================================
#ifdef NR_LDPC_PROFILER_DETAIL
    start_meas(&p_profiler->llr2CnProcBuf);
#endif
    bitOffsetInGroup = lut_numCnInCnGroups_BG1_R13[7]*NR_LDPC_ZMAX;

    for (j=0; j<10; j++)
    {
        p_cnProcBuf = &cnProcBuf[lut_startAddrCnGroups[7] + j*bitOffsetInGroup];

        for (i=0; i<lut_numCnInCnGroups[7]; i++)
        {
            idxBn = lut_posBnInCnProcBuf_CNG10[j][i]*Z;
            nrLDPC_circ_memcpy(p_cnProcBuf, &llrRes[idxBn], Z, lut_circShift_CNG10[j][i]);
            p_cnProcBuf += Z;
        }
    }
#ifdef NR_LDPC_PROFILER_DETAIL
    stop_meas(&p_profiler->llr2CnProcBuf);
#endif
    // Process group with 10 BNs

    // Offset is 1*384/32 = 12
    const uint8_t lut_idxCnProcG10[10][9] = {{12,24,36,48,60,72,84,96,108}, {0,24,36,48,60,72,84,96,108},
                                             {0,12,36,48,60,72,84,96,108}, {0,12,24,48,60,72,84,96,108},
                                             {0,12,24,36,60,72,84,96,108}, {0,12,24,36,48,72,84,96,108},
                                             {0,12,24,36,48,60,84,96,108}, {0,12,24,36,48,60,72,96,108},
                                             {0,12,24,36,48,60,72,84,108}, {0,12,24,36,48,60,72,84,96}};

    if (lut_numCnInCnGroups[7] > 0)
    {
        // Number of groups of 32 CNs for parallel processing
        // Ceil for values not divisible by 32
        M = (lut_numCnInCnGroups[7]*Z + 31)>>5;

        // Set the offset to each bit within a group in terms of 32 Byte
        bitOffsetInGroup = (lut_numCnInCnGroups_BG1_R13[7]*NR_LDPC_ZMAX)>>5;

        // Set pointers to start of group 10
        p_cnProcBuf256    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[7]];
        p_cnProcBufRes256 = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[7]];

        // Loop over every BN
        for (j=0; j<10; j++)
        {
            // Set of results pointer to correct BN address
            p_cnProcBufResBit = p_cnProcBufRes256 + (j*bitOffsetInGroup);

            // Loop over CNs
            for (i=0; i<M; i++)
            {
                // Abs and sign of 32 CNs (first BN)
                ymm0 = p_cnProcBuf256[lut_idxCnProcG10[j][0] + i];
                sgn  = _mm256_sign_epi8(*p_ones, ymm0);
                min  = _mm256_abs_epi8(ymm0);

                // Loop over BNs
                for (k=1; k<9; k++)
                {
                    ymm0 = p_cnProcBuf256[lut_idxCnProcG10[j][k] + i];
                    min  = _mm256_min_epu8(min, _mm256_abs_epi8(ymm0));
                    sgn  = _mm256_sign_epi8(sgn, ymm0);
                }

                // Store result
                min = _mm256_min_epu8(min, *p_maxLLR); // 128 in epi8 is -127
                *p_cnProcBufResBit = _mm256_sign_epi8(min, sgn);
                p_cnProcBufResBit++;
            }
        }

      // Copy CN to BN messages in CNG10 to bnProcBuff
      bitOffsetInGroup = lut_numCnInCnGroups_BG1_R13[7]*NR_LDPC_ZMAX;
      p_bnProcBuf = &bnProcBuf[0];
      p_bnProcBufRes = &bnProcBufRes[0];

      for (j=0; j<10; j++)
      {
        p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[7] + j*bitOffsetInGroup];
        nrLDPC_inv_circ_memcpy(p_bnProcBuf, p_cnProcBufRes, Z, lut_circShift_CNG10[j][0]);
        p_bnProcBuf += Z;

        idxBn = lut_posBnInCnProcBuf_CNG10[j][0]*Z;
        memcpy(p_bnProcBufRes, &llrProcBuf[idxBn], Z);
        p_bnProcBufRes += Z;
      }

      // Update the BN connected to CNG10
      M = (10*Z + 31)>>5;
      p_bnProcBuf128 = (__m128i*) &bnProcBuf[0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes[0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      for (i=0,j=0; i<M; i++,j+=2)
      {
        // First 16 LLRs of first CN
        ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
        ymm1 = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);

        ymmRes0 = _mm256_adds_epi16(ymm0, ymm1);

        // Second 16 LLRs of first CN
        ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);
        ymm1 = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);

        ymmRes1 = _mm256_adds_epi16(ymm0, ymm1);

        // Pack results back to epi8
        ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
        // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
        // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
        *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

        // Next result
        p_bnProcBufTemp256++;
      }

      // Copy the updated LLRs to llrProcBuf for next CNG processing
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (j=0; j<10; j++)
      {
        idxBn = lut_posBnInCnProcBuf_CNG10[j][0]*Z;
        memcpy(&llrRes[idxBn], p_bnProcBufTemp, Z);
        p_llrRes += Z;
      }
    }

    // =====================================================================
#ifdef NR_LDPC_PROFILER_DETAIL
    start_meas(&p_profiler->llr2CnProcBuf);
#endif
    bitOffsetInGroup = lut_numCnInCnGroups_BG1_R13[8]*NR_LDPC_ZMAX;

    for (j=0; j<19; j++)
    {
        p_cnProcBuf = &cnProcBuf[lut_startAddrCnGroups[8] + j*bitOffsetInGroup];

        for (i=0; i<lut_numCnInCnGroups[8]; i++)
        {
            idxBn = lut_posBnInCnProcBuf_CNG19[j][i]*Z;
            nrLDPC_circ_memcpy(p_cnProcBuf, &llrRes[idxBn], Z, lut_circShift_CNG19[j][i]);
            p_cnProcBuf += Z;
        }
    }
#ifdef NR_LDPC_PROFILER_DETAIL
    stop_meas(&p_profiler->llr2CnProcBuf);
#endif
    // Process group with 19 BNs

    // Offset is 4*384/32 = 12
    const uint16_t lut_idxCnProcG19[19][18] = {{48,96,144,192,240,288,336,384,432,480,528,576,624,672,720,768,816,864}, {0,96,144,192,240,288,336,384,432,480,528,576,624,672,720,768,816,864},
                                               {0,48,144,192,240,288,336,384,432,480,528,576,624,672,720,768,816,864}, {0,48,96,192,240,288,336,384,432,480,528,576,624,672,720,768,816,864},
                                               {0,48,96,144,240,288,336,384,432,480,528,576,624,672,720,768,816,864}, {0,48,96,144,192,288,336,384,432,480,528,576,624,672,720,768,816,864},
                                               {0,48,96,144,192,240,336,384,432,480,528,576,624,672,720,768,816,864}, {0,48,96,144,192,240,288,384,432,480,528,576,624,672,720,768,816,864},
                                               {0,48,96,144,192,240,288,336,432,480,528,576,624,672,720,768,816,864}, {0,48,96,144,192,240,288,336,384,480,528,576,624,672,720,768,816,864},
                                               {0,48,96,144,192,240,288,336,384,432,528,576,624,672,720,768,816,864}, {0,48,96,144,192,240,288,336,384,432,480,576,624,672,720,768,816,864},
                                               {0,48,96,144,192,240,288,336,384,432,480,528,624,672,720,768,816,864}, {0,48,96,144,192,240,288,336,384,432,480,528,576,672,720,768,816,864},
                                               {0,48,96,144,192,240,288,336,384,432,480,528,576,624,720,768,816,864}, {0,48,96,144,192,240,288,336,384,432,480,528,576,624,672,768,816,864},
                                               {0,48,96,144,192,240,288,336,384,432,480,528,576,624,672,720,816,864}, {0,48,96,144,192,240,288,336,384,432,480,528,576,624,672,720,768,864},
                                               {0,48,96,144,192,240,288,336,384,432,480,528,576,624,672,720,768,816}};

    if (lut_numCnInCnGroups[8] > 0)
    {
        // Number of groups of 32 CNs for parallel processing
        // Ceil for values not divisible by 32
        M = (lut_numCnInCnGroups[8]*Z + 31)>>5;

        // Set the offset to each bit within a group in terms of 32 Byte
        bitOffsetInGroup = (lut_numCnInCnGroups_BG1_R13[8]*NR_LDPC_ZMAX)>>5;

        // Set pointers to start of group 19
        p_cnProcBuf256    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[8]];
        p_cnProcBufRes256 = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[8]];

        // Loop over every BN
        for (j=0; j<19; j++)
        {
            // Set of results pointer to correct BN address
            p_cnProcBufResBit = p_cnProcBufRes256 + (j*bitOffsetInGroup);

            // Loop over CNs
            for (i=0; i<M; i++)
            {
                // Abs and sign of 32 CNs (first BN)
                ymm0 = p_cnProcBuf256[lut_idxCnProcG19[j][0] + i];
                sgn  = _mm256_sign_epi8(*p_ones, ymm0);
                min  = _mm256_abs_epi8(ymm0);

                // Loop over BNs
                for (k=1; k<18; k++)
                {
                    ymm0 = p_cnProcBuf256[lut_idxCnProcG19[j][k] + i];
                    min  = _mm256_min_epu8(min, _mm256_abs_epi8(ymm0));
                    sgn  = _mm256_sign_epi8(sgn, ymm0);
                }

                // Store result
                min = _mm256_min_epu8(min, *p_maxLLR); // 128 in epi8 is -127
                *p_cnProcBufResBit = _mm256_sign_epi8(min, sgn);
                p_cnProcBufResBit++;
            }
        }

      // Process BNs with degree 2 in CNG19
      // Arrange CN message values for addition
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[8]];
      bitOffsetInGroup = 3*NR_LDPC_ZMAX;
      for (i=0; i<2; i++)
      {
        p_bnProcBuf = &bnProcBuf[i*bitOffsetInGroup];
        for (j=0; j<3; j++)
        {
          nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[addrOffset_BG1_CNG19_SG2_R13[j][i]],
            Z, p_lut->circShift_CNG19_SG2[j]);
          p_bnProcBuf += Z;
        }
      }

      // Arrange input LLRs
      for (i=0; i<3; i++)
      {
        memcpy(p_bnProcBufRes, &p_llrRes[bnIdx_BG1_CNG19_SG2_R13[i]*Z], Z);
        p_bnProcBufRes += Z;
      }
      
      // Perform addition
      p_bnProcBuf128    = (__m128i*) &bnProcBuf    [0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes [0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      cnOffsetInGroup = (3*NR_LDPC_ZMAX)>>4;
      M = (3*Z + 31)>>5;
      for (i=0,j=0; i<M; i++,j+=2)
      {
          // First 16 LLRs of first CN
          ymmRes0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
          ymmRes1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);

          // Loop over CNs
          for (k=1; k<2; k++)
          {
              ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j]);
              ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

              ymm1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j+1]);
              ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);
          }

          // Add LLR from receiver input
          ymm0    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);
          ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

          ymm1    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);
          ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);

          // Pack results back to epi8
          ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
          // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
          // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
          *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

          // Next result
          p_bnProcBufTemp256++;
      }

      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (i=0; i<3; i++)
      {
        memcpy(&llrRes[bnIdx_BG1_CNG6_SG2_R13[i]*Z], p_bnProcBufTemp, Z);
        p_bnProcBufTemp += Z;
      }

      // Process BNs with degree 3 in CNG19
      // Arrange CN message values for addition
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[8]];
      bitOffsetInGroup = 22*NR_LDPC_ZMAX;
      for (i=0; i<3; i++)
      {
        p_bnProcBuf = &bnProcBuf[i*bitOffsetInGroup];
        for (j=0; j<22; j++)
        {
          nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[addrOffset_BG1_CNG19_SG3_R13[j][i]],
            Z, p_lut->circShift_CNG19_SG3[j]);
          p_bnProcBuf += Z;
        }
      }

      // Arrange input LLRs
      for (i=0; i<22; i++)
      {
        memcpy(p_bnProcBufRes, &p_llrRes[bnIdx_BG1_CNG19_SG3_R13[i]*Z], Z);
        p_bnProcBufRes += Z;
      }
      
      // Perform addition
      p_bnProcBuf128    = (__m128i*) &bnProcBuf    [0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes [0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      cnOffsetInGroup = (22*NR_LDPC_ZMAX)>>4;
      M = (22*Z + 31)>>5;
      for (i=0,j=0; i<M; i++,j+=2)
      {
          // First 16 LLRs of first CN
          ymmRes0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
          ymmRes1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);

          // Loop over CNs
          for (k=1; k<3; k++)
          {
              ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j]);
              ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

              ymm1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j+1]);
              ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);
          }

          // Add LLR from receiver input
          ymm0    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);
          ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

          ymm1    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);
          ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);

          // Pack results back to epi8
          ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
          // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
          // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
          *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

          // Next result
          p_bnProcBufTemp256++;
      }

      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (i=0; i<22; i++)
      {
        memcpy(&llrRes[bnIdx_BG1_CNG19_SG3_R13[i]*Z], p_bnProcBufTemp, Z);
        p_bnProcBufTemp += Z;
      }

      // Process BNs with degree 4 in CNG6
      // Arrange CN message values for addition
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[2]];
      bitOffsetInGroup = 2*NR_LDPC_ZMAX;
      for (i=0; i<4; i++)
      {
        p_bnProcBuf = &bnProcBuf[i*bitOffsetInGroup];
        for (j=0; j<2; j++)
        {
          nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[addrOffset_BG1_CNG6_SG4_R13[j][i]],
            Z, p_lut->circShift_CNG6_SG4[j]);
          p_bnProcBuf += Z;
        }
      }

      // Arrange input LLRs
      for (i=0; i<2; i++)
      {
        memcpy(p_bnProcBufRes, &p_llrRes[bnIdx_BG1_CNG6_SG4_R13[i]*Z], Z);
        p_bnProcBufRes += Z;
      }
      
      // Perform addition
      p_bnProcBuf128    = (__m128i*) &bnProcBuf    [0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes [0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      cnOffsetInGroup = (2*NR_LDPC_ZMAX)>>4;
      M = (2*Z + 31)>>5;
      for (i=0,j=0; i<M; i++,j+=2)
      {
          // First 16 LLRs of first CN
          ymmRes0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
          ymmRes1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);

          // Loop over CNs
          for (k=1; k<4; k++)
          {
              ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j]);
              ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

              ymm1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j+1]);
              ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);
          }

          // Add LLR from receiver input
          ymm0    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);
          ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

          ymm1    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);
          ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);

          // Pack results back to epi8
          ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
          // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
          // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
          *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

          // Next result
          p_bnProcBufTemp256++;
      }

      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (i=0; i<2; i++)
      {
        memcpy(&llrRes[bnIdx_BG1_CNG6_SG4_R13[i]*Z], p_bnProcBufTemp, Z);
        p_bnProcBufTemp += Z;
      }

      // Process BNs with degree 4 in CNG19
      // Arrange CN message values for addition
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[8]];
      bitOffsetInGroup = 1*NR_LDPC_ZMAX;
      for (i=0; i<4; i++)
      {
        p_bnProcBuf = &bnProcBuf[i*bitOffsetInGroup];
        nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[addrOffset_BG1_CNG19_SG4_R13[0][i]],
          Z, p_lut->circShift_CNG19_SG4[i]);
        p_bnProcBuf += Z;
      }

      // Perform addition
      p_bnProcBuf128    = (__m128i*) &bnProcBuf    [0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes [0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      cnOffsetInGroup = (1*NR_LDPC_ZMAX)>>4;
      M = (1*Z + 31)>>5;
      for (i=0,j=0; i<M; i++,j+=2)
      {
          // First 16 LLRs of first CN
          ymmRes0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
          ymmRes1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);

          // Loop over CNs
          for (k=1; k<3; k++)
          {
              ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j]);
              ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

              ymm1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j+1]);
              ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);
          }

          // Add LLR from receiver input
          p_llrProcBuf128   = (__m128i*) &llrProcBuf[bnIdx_BG1_CNG19_SG4_R13[0]*Z];
          ymm0    = _mm256_cvtepi8_epi16(p_llrProcBuf128[j]);
          ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

          ymm1    = _mm256_cvtepi8_epi16(p_llrProcBuf128[j+1]);
          ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);

          // Pack results back to epi8
          ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
          // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
          // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
          *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

          // Next result
          p_bnProcBufTemp256++;
      }

      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      memcpy(&llrRes[bnIdx_BG1_CNG19_SG4_R13[0]*Z], p_bnProcBufTemp, Z);
    }

}


static inline void nrLDPC_cnProc_BG1_second(t_nrLDPC_lut* p_lut, int8_t* llr, t_nrLDPC_procBuf* p_procBuf, uint16_t Z)
{
    const uint16_t (*lut_circShift_CNG3) [lut_numCnInCnGroups_BG1_R13[0]] = (const uint16_t(*)[lut_numCnInCnGroups_BG1_R13[0]]) p_lut->circShift[0];
    const uint16_t (*lut_circShift_CNG4) [lut_numCnInCnGroups_BG1_R13[1]] = (const uint16_t(*)[lut_numCnInCnGroups_BG1_R13[1]]) p_lut->circShift[1];
    const uint16_t (*lut_circShift_CNG5) [lut_numCnInCnGroups_BG1_R13[2]] = (const uint16_t(*)[lut_numCnInCnGroups_BG1_R13[2]]) p_lut->circShift[2];
    const uint16_t (*lut_circShift_CNG6) [lut_numCnInCnGroups_BG1_R13[3]] = (const uint16_t(*)[lut_numCnInCnGroups_BG1_R13[3]]) p_lut->circShift[3];
    const uint16_t (*lut_circShift_CNG7) [lut_numCnInCnGroups_BG1_R13[4]] = (const uint16_t(*)[lut_numCnInCnGroups_BG1_R13[4]]) p_lut->circShift[4];
    const uint16_t (*lut_circShift_CNG8) [lut_numCnInCnGroups_BG1_R13[5]] = (const uint16_t(*)[lut_numCnInCnGroups_BG1_R13[5]]) p_lut->circShift[5];
    const uint16_t (*lut_circShift_CNG9) [lut_numCnInCnGroups_BG1_R13[6]] = (const uint16_t(*)[lut_numCnInCnGroups_BG1_R13[6]]) p_lut->circShift[6];
    const uint16_t (*lut_circShift_CNG10)[lut_numCnInCnGroups_BG1_R13[7]] = (const uint16_t(*)[lut_numCnInCnGroups_BG1_R13[7]]) p_lut->circShift[7];
    const uint16_t (*lut_circShift_CNG19)[lut_numCnInCnGroups_BG1_R13[8]] = (const uint16_t(*)[lut_numCnInCnGroups_BG1_R13[8]]) p_lut->circShift[8];

    const uint8_t (*lut_posBnInCnProcBuf_CNG3) [lut_numCnInCnGroups_BG1_R13[0]] = (const uint8_t(*)[lut_numCnInCnGroups_BG1_R13[0]]) p_lut->posBnInCnProcBuf[0];
    const uint8_t (*lut_posBnInCnProcBuf_CNG4) [lut_numCnInCnGroups_BG1_R13[1]] = (const uint8_t(*)[lut_numCnInCnGroups_BG1_R13[1]]) p_lut->posBnInCnProcBuf[1];
    const uint8_t (*lut_posBnInCnProcBuf_CNG5) [lut_numCnInCnGroups_BG1_R13[2]] = (const uint8_t(*)[lut_numCnInCnGroups_BG1_R13[2]]) p_lut->posBnInCnProcBuf[2];
    const uint8_t (*lut_posBnInCnProcBuf_CNG6) [lut_numCnInCnGroups_BG1_R13[3]] = (const uint8_t(*)[lut_numCnInCnGroups_BG1_R13[3]]) p_lut->posBnInCnProcBuf[3];
    const uint8_t (*lut_posBnInCnProcBuf_CNG7) [lut_numCnInCnGroups_BG1_R13[4]] = (const uint8_t(*)[lut_numCnInCnGroups_BG1_R13[4]]) p_lut->posBnInCnProcBuf[4];
    const uint8_t (*lut_posBnInCnProcBuf_CNG8) [lut_numCnInCnGroups_BG1_R13[5]] = (const uint8_t(*)[lut_numCnInCnGroups_BG1_R13[5]]) p_lut->posBnInCnProcBuf[5];
    const uint8_t (*lut_posBnInCnProcBuf_CNG9) [lut_numCnInCnGroups_BG1_R13[6]] = (const uint8_t(*)[lut_numCnInCnGroups_BG1_R13[6]]) p_lut->posBnInCnProcBuf[6];
    const uint8_t (*lut_posBnInCnProcBuf_CNG10)[lut_numCnInCnGroups_BG1_R13[7]] = (const uint8_t(*)[lut_numCnInCnGroups_BG1_R13[7]]) p_lut->posBnInCnProcBuf[7];
    const uint8_t (*lut_posBnInCnProcBuf_CNG19)[lut_numCnInCnGroups_BG1_R13[8]] = (const uint8_t(*)[lut_numCnInCnGroups_BG1_R13[8]]) p_lut->posBnInCnProcBuf[8];

    const uint16_t (*lut_addrOffset_CNG7_SG1) [13] = (const uint16_t(*)[13]) p_lut->addrOffset_CNG7_SG1;
    const uint16_t (*lut_addrOffset_CNG7_SG2) [7] = (const uint16_t(*)[7]) p_lut->addrOffset_CNG7_SG2;
    const uint16_t (*lut_addrOffset_CNG7_SG3) [1] = (const uint16_t(*)[1]) p_lut->addrOffset_CNG7_SG3;
    const uint16_t (*lut_addrOffset_CNG7_SG4) [2] = (const uint16_t(*)[2]) p_lut->addrOffset_CNG7_SG4;

    const uint8_t*  lut_numCnInCnGroups = p_lut->numCnInCnGroups;
    const uint32_t* lut_startAddrCnGroups = p_lut->startAddrCnGroups;

    int8_t* cnProcBuf    = p_procBuf->cnProcBuf;
    int8_t* cnProcBufRes = p_procBuf->cnProcBufRes;
    int8_t* bnProcBuf    = p_procBuf->bnProcBuf;
    int8_t* bnProcBufRes = p_procBuf->bnProcBufRes;
    int8_t* llrRes       = p_procBuf->llrRes;
    int8_t* llrProcBuf   = p_procBuf->llrProcBuf;
    int8_t* bnProcBufTemp = p_procBuf->bnProcBufTemp;
    uint32_t idxBn = 0;
    int8_t* p_llrRes;
    int8_t* p_cnProcBuf;
    int8_t* p_cnProcBufRes;
    int8_t* p_bnProcBuf;
    int8_t* p_bnProcBufRes;
    int8_t* p_bnProcBufTemp;
    uint32_t bitOffsetInGroup;
    
    __m128i* p_llrProcBuf128;
    __m128i* p_bnProcBuf128;
    __m128i* p_bnProcBufRes128;
    __m256i* p_cnProcBuf256;
    __m256i* p_cnProcBufRes256;
    __m256i* p_bnProcBufTemp256;

    // Number of CNs in Groups
    uint32_t M;
    uint32_t i;
    uint32_t j;
    uint32_t k;
    // Offset to each bit within a group in terms of 32 Byte
    uint32_t cnOffsetInGroup;

    __m256i ymm0, ymm1, ymmRes0, ymmRes1, min, sgn;
    __m256i* p_cnProcBufResBit;

    const __m256i* p_ones   = (__m256i*) ones256_epi8;
    const __m256i* p_maxLLR = (__m256i*) maxLLR256_epi8;

    // =====================================================================
    // Copy LLRs to cnProcBuf for CNG3
#ifdef NR_LDPC_PROFILER_DETAIL
    start_meas(&p_profiler->llr2CnProcBuf);
#endif
    bitOffsetInGroup = lut_numCnInCnGroups_BG1_R13[0]*NR_LDPC_ZMAX;

    for (j=0; j<3; j++)
    {
        p_cnProcBuf = &cnProcBuf[lut_startAddrCnGroups[0] + j*bitOffsetInGroup];

        idxBn = lut_posBnInCnProcBuf_CNG3[j][0]*Z;

        nrLDPC_circ_memcpy(p_cnProcBuf, &llr[idxBn], Z, lut_circShift_CNG3[j][0]);
    }

    // Subtract the edge message of previous iteration from the new LLR
    M = (lut_numCnInCnGroups[0]*Z*3 + 31)>>5;
    p_cnProcBuf256    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[0]];
    p_cnProcBufRes256 = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[0]];
    for (k=0; k<M; k++)
      p_cnProcBuf256[k] = _mm256_subs_epi8(p_cnProcBuf256[k], p_cnProcBufRes256[k]);

#ifdef NR_LDPC_PROFILER_DETAIL
    stop_meas(&p_profiler->llr2CnProcBuf);
#endif

    // LUT with offsets for bits that need to be processed
    // 1. bit proc requires LLRs of 2. and 3. bit, 2.bits of 1. and 3. etc.
    // Offsets are in units of bitOffsetInGroup (1*384/32)
    const uint8_t lut_idxCnProcG3[3][2] = {{12,24}, {0,24}, {0,12}};
    // Process group with 3 BNs

    if (lut_numCnInCnGroups[0] > 0)
    {
        // Number of groups of 32 CNs for parallel processing
        // Ceil for values not divisible by 32
        M = (lut_numCnInCnGroups[0]*Z + 31)>>5;

        // Set the offset to each bit within a group in terms of 32 Byte
        bitOffsetInGroup = (lut_numCnInCnGroups_BG1_R13[0]*NR_LDPC_ZMAX)>>5;

        // Set pointers to start of group 3
        p_cnProcBuf256    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[0]];
        p_cnProcBufRes256 = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[0]];

        // Loop over every BN
        for (j=0; j<3; j++)
        {
            // Set of results pointer to correct BN address
            p_cnProcBufResBit = p_cnProcBufRes256 + (j*bitOffsetInGroup);

            // Loop over CNs
            for (i=0; i<M; i++)
            {
                // Abs and sign of 32 CNs (first BN)
                ymm0 = p_cnProcBuf256[lut_idxCnProcG3[j][0] + i];
                sgn  = _mm256_sign_epi8(*p_ones, ymm0);
                min  = _mm256_abs_epi8(ymm0);

                // 32 CNs of second BN
                ymm0 = p_cnProcBuf256[lut_idxCnProcG3[j][1] + i];
                min  = _mm256_min_epu8(min, _mm256_abs_epi8(ymm0));
                sgn  = _mm256_sign_epi8(sgn, ymm0);

                // Store result
                min = _mm256_min_epu8(min, *p_maxLLR); // 128 in epi8 is -127
                *p_cnProcBufResBit = _mm256_sign_epi8(min, sgn);
                p_cnProcBufResBit++;
            }
        }

      // Copy CN to BN messages in CNG3 to bnProcBuff
      bitOffsetInGroup = lut_numCnInCnGroups_BG1_R13[0]*NR_LDPC_ZMAX;
      p_bnProcBuf = &bnProcBuf[0];
      p_bnProcBufRes = &bnProcBufRes[0];

      for (j=0; j<3; j++)
      {
        p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[0] + j*bitOffsetInGroup];
        nrLDPC_inv_circ_memcpy(p_bnProcBuf, p_cnProcBufRes, Z, lut_circShift_CNG3[j][0]);
        p_bnProcBuf += Z;

        idxBn = lut_posBnInCnProcBuf_CNG3[j][0]*Z;
        memcpy(p_bnProcBufRes, &llrProcBuf[idxBn], Z);
        p_bnProcBufRes += Z;
      }

      // Update the BN connected to CNG3
      M = (3*Z + 31)>>5;
      p_bnProcBuf128 = (__m128i*) &bnProcBuf[0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes[0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      for (i=0,j=0; i<M; i++,j+=2)
      {
        // First 16 LLRs of first CN
        ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
        ymm1 = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);

        ymmRes0 = _mm256_adds_epi16(ymm0, ymm1);

        // Second 16 LLRs of first CN
        ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);
        ymm1 = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);

        ymmRes1 = _mm256_adds_epi16(ymm0, ymm1);

        // Pack results back to epi8
        ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
        // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
        // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
        *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

        // Next result
        p_bnProcBufTemp256++;
      }

      // Copy the updated LLRs to llrProcBuf for next CNG processing
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (j=0; j<3; j++)
      {
        idxBn = lut_posBnInCnProcBuf_CNG3[j][0]*Z;
        memcpy(&llrRes[idxBn], p_bnProcBufTemp, Z);
        p_bnProcBufTemp += Z;
      }
    }

    // =====================================================================
    // Copy LLRs to cnProcBuf for CNG4
#ifdef NR_LDPC_PROFILER_DETAIL
    start_meas(&p_profiler->llr2CnProcBuf);
#endif
    bitOffsetInGroup = lut_numCnInCnGroups_BG1_R13[1]*NR_LDPC_ZMAX;

    for (j=0; j<4; j++)
    {
        p_cnProcBuf = &cnProcBuf[lut_startAddrCnGroups[1] + j*bitOffsetInGroup];

        for (i=0; i<lut_numCnInCnGroups[1]; i++)
        {
            idxBn = lut_posBnInCnProcBuf_CNG4[j][i]*Z;

            nrLDPC_circ_memcpy(p_cnProcBuf, &llrRes[idxBn], Z, lut_circShift_CNG4[j][i]);

            p_cnProcBuf += Z;
        }
    }

    M = (lut_numCnInCnGroups[1]*Z*4 + 31)>>5;
    p_cnProcBuf256    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[1]];
    p_cnProcBufRes256 = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[1]];
    for (k=0; k<M; k++)
      p_cnProcBuf256[k] = _mm256_subs_epi8(p_cnProcBuf256[k], p_cnProcBufRes256[k]);

#ifdef NR_LDPC_PROFILER_DETAIL
    stop_meas(&p_profiler->llr2CnProcBuf);
#endif
    // Process group with 4 BNs

    // Offset is 5*384/32 = 60
    const uint8_t lut_idxCnProcG4[4][3] = {{60,120,180}, {0,120,180}, {0,60,180}, {0,60,120}};

    if (lut_numCnInCnGroups[1] > 0)
    {
        // Number of groups of 32 CNs for parallel processing
        // Ceil for values not divisible by 32
        M = (lut_numCnInCnGroups[1]*Z + 31)>>5;

        // Set the offset to each bit within a group in terms of 32 Byte
        bitOffsetInGroup = (lut_numCnInCnGroups_BG1_R13[1]*NR_LDPC_ZMAX)>>5;

        // Set pointers to start of group 4
        p_cnProcBuf256    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[1]];
        p_cnProcBufRes256 = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[1]];

        // Loop over every BN
        for (j=0; j<4; j++)
        {
            // Set of results pointer to correct BN address
            p_cnProcBufResBit = p_cnProcBufRes256 + (j*bitOffsetInGroup);

            // Loop over CNs
            for (i=0; i<M; i++)
            {
                // Abs and sign of 32 CNs (first BN)
                ymm0 = p_cnProcBuf256[lut_idxCnProcG4[j][0] + i];
                sgn  = _mm256_sign_epi8(*p_ones, ymm0);
                min  = _mm256_abs_epi8(ymm0);

                // Loop over BNs
                for (k=1; k<3; k++)
                {
                    ymm0 = p_cnProcBuf256[lut_idxCnProcG4[j][k] + i];
                    min  = _mm256_min_epu8(min, _mm256_abs_epi8(ymm0));
                    sgn  = _mm256_sign_epi8(sgn, ymm0);
                }

                // Store result
                min = _mm256_min_epu8(min, *p_maxLLR); // 128 in epi8 is -127
                *p_cnProcBufResBit = _mm256_sign_epi8(min, sgn);
                p_cnProcBufResBit++;
            }
        }

      // Process BNs with degree 1 in CNG4
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_bnProcBufTemp = &bnProcBufTemp[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[1]];
      for (i=0; i<11; i++)
      {
        nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[addrOffset_BG1_CNG4_SG1_R13[i][0]],
          Z, p_lut->circShift_CNG4_SG1[i]);
        p_bnProcBuf += Z;

        memcpy(p_bnProcBufRes, &llrProcBuf[bnIdx_BG1_CNG4_SG1_R13[i]*Z], Z);
        p_bnProcBufRes += Z;
      }

      M = (11*Z + 31)>>5;
      p_bnProcBuf128 = (__m128i*) &bnProcBuf[0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes[0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      for (i=0,j=0; i<M; i++,j+=2)
      {
        // First 16 LLRs of first CN
        ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
        ymm1 = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);

        ymmRes0 = _mm256_adds_epi16(ymm0, ymm1);

        // Second 16 LLRs of first CN
        ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);
        ymm1 = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);

        ymmRes1 = _mm256_adds_epi16(ymm0, ymm1);

        // Pack results back to epi8
        ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
        // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
        // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
        *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

        // Next result
        p_bnProcBufTemp256++;
      }
      
      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (i=0; i<11; i++)
      {
        memcpy(&llrRes[bnIdx_BG1_CNG4_SG1_R13[i]*Z], p_bnProcBufTemp, Z);
        p_bnProcBufTemp += Z;
      }
      
      // Process BNs with degree 2 in CNG4
      // Arrange CN message values for addition
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[1]];
      bitOffsetInGroup = 3*NR_LDPC_ZMAX;
      for (i=0; i<2; i++)
      {
        p_bnProcBuf = &bnProcBuf[i*bitOffsetInGroup];
        for (j=0; j<3; j++)
        {
          nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[addrOffset_BG1_CNG4_SG2_R13[j][i]],
            Z, p_lut->circShift_CNG4_SG2[j]);
          p_bnProcBuf += Z;
        }
      }

      // Arrange input LLRs
      for (i=0; i<3; i++)
      {
        memcpy(p_bnProcBufRes, &p_llrRes[bnIdx_BG1_CNG4_SG2_R13[i]*Z], Z);
        p_bnProcBufRes += Z;
      }
      
      // Perform addition
      p_bnProcBuf128    = (__m128i*) &bnProcBuf    [0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes [0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      cnOffsetInGroup = (3*NR_LDPC_ZMAX)>>4;
      M = (3*Z + 31)>>5;
      for (i=0,j=0; i<M; i++,j+=2)
      {
          // First 16 LLRs of first CN
          ymmRes0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
          ymmRes1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);

          // Loop over CNs
          for (k=1; k<2; k++)
          {
              ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j]);
              ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

              ymm1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j+1]);
              ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);
          }

          // Add LLR from receiver input
          ymm0    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);
          ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

          ymm1    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);
          ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);

          // Pack results back to epi8
          ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
          // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
          // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
          *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

          // Next result
          p_bnProcBufTemp256++;
      }

      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (i=0; i<3; i++)
      {
        memcpy(&llrRes[bnIdx_BG1_CNG4_SG2_R13[i]*Z], p_bnProcBufTemp, Z);
        p_bnProcBufTemp += Z;
      }

      // Process BNs with degree 3 in CNG4
      // Arrange CN message values for addition
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[1]];
      bitOffsetInGroup = 1*NR_LDPC_ZMAX;
      for (i=0; i<3; i++)
      {
        p_bnProcBuf = &bnProcBuf[i*bitOffsetInGroup];
        nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[addrOffset_BG1_CNG4_SG3_R13[0][i]],
          Z, p_lut->circShift_CNG4_SG3[i]);
        p_bnProcBuf += Z;
      }

      // Perform addition
      p_bnProcBuf128    = (__m128i*) &bnProcBuf    [0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes [0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      cnOffsetInGroup = (1*NR_LDPC_ZMAX)>>4;
      M = (1*Z + 31)>>5;
      for (i=0,j=0; i<M; i++,j+=2)
      {
          // First 16 LLRs of first CN
          ymmRes0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
          ymmRes1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);

          // Loop over CNs
          for (k=1; k<3; k++)
          {
              ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j]);
              ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

              ymm1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j+1]);
              ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);
          }

          // Add LLR from receiver input
          p_llrProcBuf128   = (__m128i*) &llrProcBuf[bnIdx_BG1_CNG4_SG3_R13[0]*Z];
          ymm0    = _mm256_cvtepi8_epi16(p_llrProcBuf128[j]);
          ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

          ymm1    = _mm256_cvtepi8_epi16(p_llrProcBuf128[j+1]);
          ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);

          // Pack results back to epi8
          ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
          // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
          // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
          *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

          // Next result
          p_bnProcBufTemp256++;
      }

      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      memcpy(&llrRes[bnIdx_BG1_CNG4_SG3_R13[0]*Z], p_bnProcBufTemp, Z);
    }

    // =====================================================================
#ifdef NR_LDPC_PROFILER_DETAIL
    start_meas(&p_profiler->llr2CnProcBuf);
#endif
    bitOffsetInGroup = lut_numCnInCnGroups_BG1_R13[2]*NR_LDPC_ZMAX;

    for (j=0; j<5; j++)
    {
        p_cnProcBuf = &cnProcBuf[lut_startAddrCnGroups[2] + j*bitOffsetInGroup];

        for (i=0; i<lut_numCnInCnGroups[2]; i++)
        {
            idxBn = lut_posBnInCnProcBuf_CNG5[j][i]*Z;
            nrLDPC_circ_memcpy(p_cnProcBuf, &llrRes[idxBn], Z, lut_circShift_CNG5[j][i]);
            p_cnProcBuf += Z;
        }
    }

    M = (lut_numCnInCnGroups[2]*Z*5 + 31)>>5;
    p_cnProcBuf256    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[2]];
    p_cnProcBufRes256 = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[2]];
    for (k=0; k<M; k++)
      p_cnProcBuf256[k] = _mm256_subs_epi8(p_cnProcBuf256[k], p_cnProcBufRes256[k]);

#ifdef NR_LDPC_PROFILER_DETAIL
    stop_meas(&p_profiler->llr2CnProcBuf);
#endif
    // Process group with 5 BNs

    // Offset is 18*384/32 = 216
    const uint16_t lut_idxCnProcG5[5][4] = {{216,432,648,864}, {0,432,648,864},
                                            {0,216,648,864}, {0,216,432,864}, {0,216,432,648}};

    if (lut_numCnInCnGroups[2] > 0)
    {
        // Number of groups of 32 CNs for parallel processing
        // Ceil for values not divisible by 32
        M = (lut_numCnInCnGroups[2]*Z + 31)>>5;

        // Set the offset to each bit within a group in terms of 32 Byte
        bitOffsetInGroup = (lut_numCnInCnGroups_BG1_R13[2]*NR_LDPC_ZMAX)>>5;

        // Set pointers to start of group 5
        p_cnProcBuf256    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[2]];
        p_cnProcBufRes256 = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[2]];

        // Loop over every BN
        for (j=0; j<5; j++)
        {
            // Set of results pointer to correct BN address
            p_cnProcBufResBit = p_cnProcBufRes256 + (j*bitOffsetInGroup);

            // Loop over CNs
            for (i=0; i<M; i++)
            {
                // Abs and sign of 32 CNs (first BN)
                ymm0 = p_cnProcBuf256[lut_idxCnProcG5[j][0] + i];
                sgn  = _mm256_sign_epi8(*p_ones, ymm0);
                min  = _mm256_abs_epi8(ymm0);

                // Loop over BNs
                for (k=1; k<4; k++)
                {
                    ymm0 = p_cnProcBuf256[lut_idxCnProcG5[j][k] + i];
                    min  = _mm256_min_epu8(min, _mm256_abs_epi8(ymm0));
                    sgn  = _mm256_sign_epi8(sgn, ymm0);
                }

                // Store result
                min = _mm256_min_epu8(min, *p_maxLLR); // 128 in epi8 is -127
                *p_cnProcBufResBit = _mm256_sign_epi8(min, sgn);
                p_cnProcBufResBit++;
            }
        }

      // Process BNs with degree 1 in CNG5
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_bnProcBufTemp = &bnProcBufTemp[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[2]];
      for (i=0; i<20; i++)
      {
        nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[addrOffset_BG1_CNG5_SG1_R13[i][0]],
          Z, p_lut->circShift_CNG5_SG1[i]);
        p_bnProcBuf += Z;

        memcpy(p_bnProcBufRes, &llrProcBuf[bnIdx_BG1_CNG5_SG1_R13[i]*Z], Z);
        p_bnProcBufTemp += Z;
      }

      M = (20*Z + 31)>>5;
      p_bnProcBuf128 = (__m128i*) &bnProcBuf[0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes[0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      for (i=0,j=0; i<M; i++,j+=2)
      {
        // First 16 LLRs of first CN
        ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
        ymm1 = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);

        ymmRes0 = _mm256_adds_epi16(ymm0, ymm1);

        // Second 16 LLRs of first CN
        ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);
        ymm1 = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);

        ymmRes1 = _mm256_adds_epi16(ymm0, ymm1);

        // Pack results back to epi8
        ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
        // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
        // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
        *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

        // Next result
        p_bnProcBufTemp256++;
      }
      
      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (i=0; i<20; i++)
      {
        memcpy(&llrRes[bnIdx_BG1_CNG5_SG1_R13[i]*Z], p_bnProcBufTemp, Z);
        p_bnProcBufTemp += Z;
      }

      // Process BNs with degree 2 in CNG5
      // Arrange CN message values for addition
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[2]];
      bitOffsetInGroup = 8*NR_LDPC_ZMAX;
      for (i=0; i<2; i++)
      {
        p_bnProcBuf = &bnProcBuf[i*bitOffsetInGroup];
        for (j=0; j<8; j++)
        {
          nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[addrOffset_BG1_CNG5_SG2_R13[j][i]],
            Z, p_lut->circShift_CNG5_SG2[j]);
          p_bnProcBuf += Z;
        }
      }

      // Arrange input LLRs
      for (i=0; i<8; i++)
      {
        memcpy(p_bnProcBufRes, &p_llrRes[bnIdx_BG1_CNG5_SG2_R13[i]*Z], Z);
        p_bnProcBufRes += Z;
      }
      
      // Perform addition
      p_bnProcBuf128    = (__m128i*) &bnProcBuf    [0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes [0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      cnOffsetInGroup = (8*NR_LDPC_ZMAX)>>4;
      M = (8*Z + 31)>>5;
      for (i=0,j=0; i<M; i++,j+=2)
      {
          // First 16 LLRs of first CN
          ymmRes0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
          ymmRes1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);

          // Loop over CNs
          for (k=1; k<2; k++)
          {
              ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j]);
              ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

              ymm1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j+1]);
              ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);
          }

          // Add LLR from receiver input
          ymm0    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);
          ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

          ymm1    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);
          ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);

          // Pack results back to epi8
          ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
          // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
          // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
          *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

          // Next result
          p_bnProcBufTemp256++;
      }

      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (i=0; i<8; i++)
      {
        memcpy(&llrRes[bnIdx_BG1_CNG5_SG2_R13[i]*Z], p_bnProcBufTemp, Z);
        p_bnProcBufTemp += Z;
      }

      // Process BNs with degree 3 in CNG5
      // Arrange CN message values for addition
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[2]];
      bitOffsetInGroup = 6*NR_LDPC_ZMAX;
      for (i=0; i<3; i++)
      {
        p_bnProcBuf = &bnProcBuf[i*bitOffsetInGroup];
        for (j=0; j<6; j++)
        {
          nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[addrOffset_BG1_CNG5_SG3_R13[j][i]],
            Z, p_lut->circShift_CNG5_SG3[j]);
          p_bnProcBuf += Z;
        }
      }

      // Arrange input LLRs
      for (i=0; i<6; i++)
      {
        memcpy(p_bnProcBufRes, &p_llrRes[bnIdx_BG1_CNG5_SG3_R13[i]*Z], Z);
        p_bnProcBufRes += Z;
      }
      
      // Perform addition
      p_bnProcBuf128    = (__m128i*) &bnProcBuf    [0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes [0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      cnOffsetInGroup = (6*NR_LDPC_ZMAX)>>4;
      M = (6*Z + 31)>>5;
      for (i=0,j=0; i<M; i++,j+=2)
      {
          // First 16 LLRs of first CN
          ymmRes0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
          ymmRes1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);

          // Loop over CNs
          for (k=1; k<3; k++)
          {
              ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j]);
              ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

              ymm1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j+1]);
              ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);
          }

          // Add LLR from receiver input
          ymm0    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);
          ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

          ymm1    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);
          ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);

          // Pack results back to epi8
          ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
          // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
          // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
          *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

          // Next result
          p_bnProcBufTemp256++;
      }

      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (i=0; i<6; i++)
      {
        memcpy(&llrRes[bnIdx_BG1_CNG5_SG3_R13[i]*Z], p_bnProcBufTemp, Z);
        p_bnProcBufTemp += Z;
      }

      // Process BNs with degree 4 in CNG5
      // Arrange CN message values for addition
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[2]];
      bitOffsetInGroup = 2*NR_LDPC_ZMAX;
      for (i=0; i<4; i++)
      {
        p_bnProcBuf = &bnProcBuf[i*bitOffsetInGroup];
        for (j=0; j<2; j++)
        {
          nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[addrOffset_BG1_CNG5_SG4_R13[j][i]],
            Z, p_lut->circShift_CNG5_SG4[j]);
          p_bnProcBuf += Z;
        }
      }

      // Arrange input LLRs
      for (i=0; i<2; i++)
      {
        memcpy(p_bnProcBufRes, &p_llrRes[bnIdx_BG1_CNG5_SG4_R13[i]*Z], Z);
        p_bnProcBufRes += Z;
      }
      
      // Perform addition
      p_bnProcBuf128    = (__m128i*) &bnProcBuf    [0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes [0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      cnOffsetInGroup = (2*NR_LDPC_ZMAX)>>4;
      M = (2*Z + 31)>>5;
      for (i=0,j=0; i<M; i++,j+=2)
      {
          // First 16 LLRs of first CN
          ymmRes0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
          ymmRes1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);

          // Loop over CNs
          for (k=1; k<4; k++)
          {
              ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j]);
              ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

              ymm1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j+1]);
              ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);
          }

          // Add LLR from receiver input
          ymm0    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);
          ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

          ymm1    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);
          ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);

          // Pack results back to epi8
          ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
          // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
          // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
          *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

          // Next result
          p_bnProcBufTemp256++;
      }

      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (i=0; i<2; i++)
      {
        memcpy(&llrRes[bnIdx_BG1_CNG5_SG4_R13[i]*Z], p_bnProcBufTemp, Z);
        p_bnProcBufTemp += Z;
      }

      // Process BNs with degree 5 in CNG5
      // Arrange CN message values for addition
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[2]];
      bitOffsetInGroup = 2*NR_LDPC_ZMAX;
      for (i=0; i<5; i++)
      {
        p_bnProcBuf = &bnProcBuf[i*bitOffsetInGroup];
        for (j=0; j<2; j++)
        {
          nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[addrOffset_BG1_CNG5_SG5_R13[j][i]],
            Z, p_lut->circShift_CNG5_SG5[j]);
          p_bnProcBuf += Z;
        }
      }

      // Arrange input LLRs
      for (i=0; i<2; i++)
      {
        memcpy(p_bnProcBufRes, &p_llrRes[bnIdx_BG1_CNG5_SG5_R13[i]*Z], Z);
        p_bnProcBufRes += Z;
      }
      
      // Perform addition
      p_bnProcBuf128    = (__m128i*) &bnProcBuf    [0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes [0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      cnOffsetInGroup = (2*NR_LDPC_ZMAX)>>4;
      M = (2*Z + 31)>>5;
      for (i=0,j=0; i<M; i++,j+=2)
      {
          // First 16 LLRs of first CN
          ymmRes0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
          ymmRes1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);

          // Loop over CNs
          for (k=1; k<5; k++)
          {
              ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j]);
              ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

              ymm1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j+1]);
              ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);
          }

          // Add LLR from receiver input
          ymm0    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);
          ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

          ymm1    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);
          ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);

          // Pack results back to epi8
          ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
          // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
          // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
          *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

          // Next result
          p_bnProcBufTemp256++;
      }

      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (i=0; i<2; i++)
      {
        memcpy(&llrRes[bnIdx_BG1_CNG5_SG5_R13[i]*Z], p_bnProcBufTemp, Z);
        p_bnProcBufTemp += Z;
      }

      // Process BNs with degree 9 in CNG5
      // Arrange CN message values for addition
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[2]];
      bitOffsetInGroup = 2*NR_LDPC_ZMAX;
      for (i=0; i<9; i++)
      {
        p_bnProcBuf = &bnProcBuf[i*bitOffsetInGroup];
        for (j=0; j<2; j++)
        {
          nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[addrOffset_BG1_CNG5_SG9_R13[j][i]],
            Z, p_lut->circShift_CNG5_SG9[j]);
          p_bnProcBuf += Z;
        }
      }

      // Arrange input LLRs
      for (i=0; i<2; i++)
      {
        memcpy(p_bnProcBufRes, &p_llrRes[bnIdx_BG1_CNG5_SG9_R13[i]*Z], Z);
        p_bnProcBufRes += Z;
      }
      
      // Perform addition
      p_bnProcBuf128    = (__m128i*) &bnProcBuf    [0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes [0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      cnOffsetInGroup = (2*NR_LDPC_ZMAX)>>4;
      M = (2*Z + 31)>>5;
      for (i=0,j=0; i<M; i++,j+=2)
      {
          // First 16 LLRs of first CN
          ymmRes0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
          ymmRes1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);

          // Loop over CNs
          for (k=1; k<9; k++)
          {
              ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j]);
              ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

              ymm1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j+1]);
              ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);
          }

          // Add LLR from receiver input
          ymm0    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);
          ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

          ymm1    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);
          ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);

          // Pack results back to epi8
          ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
          // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
          // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
          *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

          // Next result
          p_bnProcBufTemp256++;
      }

      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (i=0; i<2; i++)
      {
        memcpy(&llrRes[bnIdx_BG1_CNG5_SG9_R13[i]*Z], p_bnProcBufTemp, Z);
        p_bnProcBufTemp += Z;
      }
    }

    // =====================================================================
#ifdef NR_LDPC_PROFILER_DETAIL
    start_meas(&p_profiler->llr2CnProcBuf);
#endif
    bitOffsetInGroup = lut_numCnInCnGroups_BG1_R13[3]*NR_LDPC_ZMAX;

    for (j=0; j<6; j++)
    {
        p_cnProcBuf = &cnProcBuf[lut_startAddrCnGroups[3] + j*bitOffsetInGroup];

        for (i=0; i<lut_numCnInCnGroups[3]; i++)
        {
            idxBn = lut_posBnInCnProcBuf_CNG6[j][i]*Z;
            nrLDPC_circ_memcpy(p_cnProcBuf, &llrRes[idxBn], Z, lut_circShift_CNG6[j][i]);
            p_cnProcBuf += Z;
        }
    }

    M = (lut_numCnInCnGroups[3]*Z*6 + 31)>>5;
    p_cnProcBuf256    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[3]];
    p_cnProcBufRes256 = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[3]];
    for (k=0; k<M; k++)
      p_cnProcBuf256[k] = _mm256_subs_epi8(p_cnProcBuf256[k], p_cnProcBufRes256[k]);

#ifdef NR_LDPC_PROFILER_DETAIL
    stop_meas(&p_profiler->llr2CnProcBuf);
#endif
    // Process group with 6 BNs

    // Offset is 8*384/32 = 96
    const uint16_t lut_idxCnProcG6[6][5] = {{96,192,288,384,480}, {0,192,288,384,480},
                                            {0,96,288,384,480}, {0,96,192,384,480},
                                            {0,96,192,288,480}, {0,96,192,288,384}};

    if (lut_numCnInCnGroups[3] > 0)
    {
        // Number of groups of 32 CNs for parallel processing
        // Ceil for values not divisible by 32
        M = (lut_numCnInCnGroups[3]*Z + 31)>>5;

        // Set the offset to each bit within a group in terms of 32 Byte
        bitOffsetInGroup = (lut_numCnInCnGroups_BG1_R13[3]*NR_LDPC_ZMAX)>>5;

        // Set pointers to start of group 6
        p_cnProcBuf256    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[3]];
        p_cnProcBufRes256 = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[3]];

        // Loop over every BN
        for (j=0; j<6; j++)
        {
            // Set of results pointer to correct BN address
            p_cnProcBufResBit = (__m256i*) p_cnProcBufRes + (j*bitOffsetInGroup);

            // Loop over CNs
            for (i=0; i<M; i++)
            {
                // Abs and sign of 32 CNs (first BN)
                ymm0 = p_cnProcBuf256[lut_idxCnProcG6[j][0] + i];
                sgn  = _mm256_sign_epi8(*p_ones, ymm0);
                min  = _mm256_abs_epi8(ymm0);

                // Loop over BNs
                for (k=1; k<5; k++)
                {
                    ymm0 = p_cnProcBuf256[lut_idxCnProcG6[j][k] + i];
                    min  = _mm256_min_epu8(min, _mm256_abs_epi8(ymm0));
                    sgn  = _mm256_sign_epi8(sgn, ymm0);
                }

                // Store result
                min = _mm256_min_epu8(min, *p_maxLLR); // 128 in epi8 is -127
                *p_cnProcBufResBit = _mm256_sign_epi8(min, sgn);
                p_cnProcBufResBit++;
            }
        }

      // Process BNs with degree 1 in CNG6
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_bnProcBufTemp = &bnProcBufTemp[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[3]];
      for (i=0; i<20; i++)
      {
        nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[addrOffset_BG1_CNG6_SG1_R13[i][0]],
          Z, p_lut->circShift_CNG6_SG1[i]);
        p_bnProcBuf += Z;

        memcpy(p_bnProcBufRes, &llrProcBuf[bnIdx_BG1_CNG6_SG1_R13[i]*Z], Z);
        p_bnProcBufTemp += Z;
      }

      M = (20*Z + 31)>>5;
      p_bnProcBuf128 = (__m128i*) &bnProcBuf[0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes[0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      for (i=0,j=0; i<M; i++,j+=2)
      {
        // First 16 LLRs of first CN
        ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
        ymm1 = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);

        ymmRes0 = _mm256_adds_epi16(ymm0, ymm1);

        // Second 16 LLRs of first CN
        ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);
        ymm1 = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);

        ymmRes1 = _mm256_adds_epi16(ymm0, ymm1);

        // Pack results back to epi8
        ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
        // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
        // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
        *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

        // Next result
        p_bnProcBufTemp256++;
      }
      
      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (i=0; i<20; i++)
      {
        memcpy(&llrRes[bnIdx_BG1_CNG6_SG1_R13[i]*Z], p_bnProcBufTemp, Z);
        p_bnProcBufTemp += Z;
      }

      // Process BNs with degree 2 in CNG6
      // Arrange CN message values for addition
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[3]];
      bitOffsetInGroup = 3*NR_LDPC_ZMAX;
      for (i=0; i<2; i++)
      {
        p_bnProcBuf = &bnProcBuf[i*bitOffsetInGroup];
        for (j=0; j<3; j++)
        {
          nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[addrOffset_BG1_CNG6_SG2_R13[j][i]],
            Z, p_lut->circShift_CNG6_SG2[j]);
          p_bnProcBuf += Z;
        }
      }

      // Arrange input LLRs
      for (i=0; i<3; i++)
      {
        memcpy(p_bnProcBufRes, &p_llrRes[bnIdx_BG1_CNG6_SG2_R13[i]*Z], Z);
        p_bnProcBufRes += Z;
      }
      
      // Perform addition
      p_bnProcBuf128    = (__m128i*) &bnProcBuf    [0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes [0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      cnOffsetInGroup = (3*NR_LDPC_ZMAX)>>4;
      M = (3*Z + 31)>>5;
      for (i=0,j=0; i<M; i++,j+=2)
      {
          // First 16 LLRs of first CN
          ymmRes0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
          ymmRes1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);

          // Loop over CNs
          for (k=1; k<2; k++)
          {
              ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j]);
              ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

              ymm1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j+1]);
              ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);
          }

          // Add LLR from receiver input
          ymm0    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);
          ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

          ymm1    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);
          ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);

          // Pack results back to epi8
          ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
          // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
          // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
          *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

          // Next result
          p_bnProcBufTemp256++;
      }

      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (i=0; i<3; i++)
      {
        memcpy(&llrRes[bnIdx_BG1_CNG6_SG2_R13[i]*Z], p_bnProcBufTemp, Z);
        p_bnProcBufTemp += Z;
      }

      // Process BNs with degree 3 in CNG6
      // Arrange CN message values for addition
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[3]];
      bitOffsetInGroup = 3*NR_LDPC_ZMAX;
      for (i=0; i<3; i++)
      {
        p_bnProcBuf = &bnProcBuf[i*bitOffsetInGroup];
        for (j=0; j<3; j++)
        {
          nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[addrOffset_BG1_CNG6_SG3_R13[j][i]],
            Z, p_lut->circShift_CNG6_SG3[j]);
          p_bnProcBuf += Z;
        }
      }

      // Arrange input LLRs
      for (i=0; i<3; i++)
      {
        memcpy(p_bnProcBufRes, &p_llrRes[bnIdx_BG1_CNG6_SG3_R13[i]*Z], Z);
        p_bnProcBufRes += Z;
      }
      
      // Perform addition
      p_bnProcBuf128    = (__m128i*) &bnProcBuf    [0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes [0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      cnOffsetInGroup = (3*NR_LDPC_ZMAX)>>4;
      M = (3*Z + 31)>>5;
      for (i=0,j=0; i<M; i++,j+=2)
      {
          // First 16 LLRs of first CN
          ymmRes0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
          ymmRes1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);

          // Loop over CNs
          for (k=1; k<3; k++)
          {
              ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j]);
              ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

              ymm1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j+1]);
              ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);
          }

          // Add LLR from receiver input
          ymm0    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);
          ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

          ymm1    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);
          ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);

          // Pack results back to epi8
          ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
          // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
          // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
          *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

          // Next result
          p_bnProcBufTemp256++;
      }

      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (i=0; i<3; i++)
      {
        memcpy(&llrRes[bnIdx_BG1_CNG6_SG3_R13[i]*Z], p_bnProcBufTemp, Z);
        p_bnProcBufTemp += Z;
      }

      // Process BNs with degree 4 in CNG6
      // Arrange CN message values for addition
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[2]];
      bitOffsetInGroup = 2*NR_LDPC_ZMAX;
      for (i=0; i<4; i++)
      {
        p_bnProcBuf = &bnProcBuf[i*bitOffsetInGroup];
        for (j=0; j<2; j++)
        {
          nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[addrOffset_BG1_CNG6_SG4_R13[j][i]],
            Z, p_lut->circShift_CNG6_SG4[j]);
          p_bnProcBuf += Z;
        }
      }

      // Arrange input LLRs
      for (i=0; i<2; i++)
      {
        memcpy(p_bnProcBufRes, &p_llrRes[bnIdx_BG1_CNG6_SG4_R13[i]*Z], Z);
        p_bnProcBufRes += Z;
      }
      
      // Perform addition
      p_bnProcBuf128    = (__m128i*) &bnProcBuf    [0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes [0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      cnOffsetInGroup = (2*NR_LDPC_ZMAX)>>4;
      M = (2*Z + 31)>>5;
      for (i=0,j=0; i<M; i++,j+=2)
      {
          // First 16 LLRs of first CN
          ymmRes0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
          ymmRes1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);

          // Loop over CNs
          for (k=1; k<4; k++)
          {
              ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j]);
              ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

              ymm1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j+1]);
              ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);
          }

          // Add LLR from receiver input
          ymm0    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);
          ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

          ymm1    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);
          ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);

          // Pack results back to epi8
          ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
          // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
          // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
          *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

          // Next result
          p_bnProcBufTemp256++;
      }

      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (i=0; i<2; i++)
      {
        memcpy(&llrRes[bnIdx_BG1_CNG6_SG4_R13[i]*Z], p_bnProcBufTemp, Z);
        p_bnProcBufTemp += Z;
      }

      // Process BNs with degree 5 in CNG6
      // Arrange CN message values for addition
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[3]];
      bitOffsetInGroup = 1*NR_LDPC_ZMAX;
      for (i=0; i<5; i++)
      {
        p_bnProcBuf = &bnProcBuf[i*bitOffsetInGroup];
        nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[addrOffset_BG1_CNG6_SG5_R13[0][i]],
          Z, p_lut->circShift_CNG6_SG5[i]);
        p_bnProcBuf += Z;
      }

      // Perform addition
      p_bnProcBuf128    = (__m128i*) &bnProcBuf    [0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes [0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      cnOffsetInGroup = (1*NR_LDPC_ZMAX)>>4;
      M = (1*Z + 31)>>5;
      for (i=0,j=0; i<M; i++,j+=2)
      {
          // First 16 LLRs of first CN
          ymmRes0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
          ymmRes1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);

          // Loop over CNs
          for (k=1; k<5; k++)
          {
              ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j]);
              ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

              ymm1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j+1]);
              ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);
          }

          // Add LLR from receiver input
          p_llrProcBuf128   = (__m128i*) &llrProcBuf[bnIdx_BG1_CNG6_SG5_R13[0]*Z];
          ymm0    = _mm256_cvtepi8_epi16(p_llrProcBuf128[j]);
          ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

          ymm1    = _mm256_cvtepi8_epi16(p_llrProcBuf128[j+1]);
          ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);

          // Pack results back to epi8
          ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
          // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
          // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
          *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

          // Next result
          p_bnProcBufTemp256++;
      }

      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      memcpy(&llrRes[bnIdx_BG1_CNG6_SG5_R13[0]*Z], p_bnProcBufTemp, Z);
    }

    // =====================================================================
#ifdef NR_LDPC_PROFILER_DETAIL
    start_meas(&p_profiler->llr2CnProcBuf);
#endif
    bitOffsetInGroup = lut_numCnInCnGroups_BG1_R13[4]*NR_LDPC_ZMAX;

    for (j=0; j<7; j++)
    {
        p_cnProcBuf = &cnProcBuf[lut_startAddrCnGroups[4] + j*bitOffsetInGroup];

        for (i=0; i<lut_numCnInCnGroups[4]; i++)
        {
            idxBn = lut_posBnInCnProcBuf_CNG7[j][i]*Z;
            nrLDPC_circ_memcpy(p_cnProcBuf, &llrRes[idxBn], Z, lut_circShift_CNG7[j][i]);
            p_cnProcBuf += Z;
        }
    }

    M = (lut_numCnInCnGroups[4]*Z*7 + 31)>>5;
    p_cnProcBuf256    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[4]];
    p_cnProcBufRes256 = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[4]];
    for (k=0; k<M; k++)
      p_cnProcBuf256[k] = _mm256_subs_epi8(p_cnProcBuf256[k], p_cnProcBufRes256[k]);

#ifdef NR_LDPC_PROFILER_DETAIL
    stop_meas(&p_profiler->llr2CnProcBuf);
#endif
    // Process group with 7 BNs

    // Offset is 5*384/32 = 60
    const uint16_t lut_idxCnProcG7[7][6] = {{60,120,180,240,300,360}, {0,120,180,240,300,360},
                                            {0,60,180,240,300,360},   {0,60,120,240,300,360},
                                            {0,60,120,180,300,360},   {0,60,120,180,240,360},
                                            {0,60,120,180,240,300}};

    if (lut_numCnInCnGroups[4] > 0)
    {
        // Number of groups of 32 CNs for parallel processing
        // Ceil for values not divisible by 32
        M = (lut_numCnInCnGroups[4]*Z + 31)>>5;

        // Set the offset to each bit within a group in terms of 32 Byte
        bitOffsetInGroup = (lut_numCnInCnGroups_BG1_R13[4]*NR_LDPC_ZMAX)>>5;

        // Set pointers to start of group 7
        p_cnProcBuf256    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[4]];
        p_cnProcBufRes256 = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[4]];

        // Loop over every BN
        for (j=0; j<7; j++)
        {
            // Set of results pointer to correct BN address
            p_cnProcBufResBit = p_cnProcBufRes256 + (j*bitOffsetInGroup);

            // Loop over CNs
            for (i=0; i<M; i++)
            {
                // Abs and sign of 32 CNs (first BN)
                ymm0 = p_cnProcBuf256[lut_idxCnProcG7[j][0] + i];
                sgn  = _mm256_sign_epi8(*p_ones, ymm0);
                min  = _mm256_abs_epi8(ymm0);

                // Loop over BNs
                for (k=1; k<6; k++)
                {
                    ymm0 = p_cnProcBuf256[lut_idxCnProcG7[j][k] + i];
                    min  = _mm256_min_epu8(min, _mm256_abs_epi8(ymm0));
                    sgn  = _mm256_sign_epi8(sgn, ymm0);
                }

                // Store result
                min = _mm256_min_epu8(min, *p_maxLLR); // 128 in epi8 is -127
                *p_cnProcBufResBit = _mm256_sign_epi8(min, sgn);
                p_cnProcBufResBit++;
            }
        }

      // Process BNs with degree 1 in CNG7
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_bnProcBufTemp = &bnProcBufTemp[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[4]];
      for (i=0; i<p_lut->numBnDegCNG7[0]; i++)
      {
        nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[lut_addrOffset_CNG7_SG1[i][0]],
          Z, p_lut->circShift_CNG7_SG1[i]);
        p_bnProcBuf += Z;

        memcpy(p_bnProcBufRes, &llrProcBuf[p_lut->bnIdx_CNG7_SG1[i]*Z], Z);
        p_bnProcBufTemp += Z;
      }

      M = (p_lut->numBnDegCNG7[0]*Z + 31)>>5;
      p_bnProcBuf128 = (__m128i*) &bnProcBuf[0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes[0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      for (i=0,j=0; i<M; i++,j+=2)
      {
        // First 16 LLRs of first CN
        ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
        ymm1 = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);

        ymmRes0 = _mm256_adds_epi16(ymm0, ymm1);

        // Second 16 LLRs of first CN
        ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);
        ymm1 = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);

        ymmRes1 = _mm256_adds_epi16(ymm0, ymm1);

        // Pack results back to epi8
        ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
        // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
        // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
        *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

        // Next result
        p_bnProcBufTemp256++;
      }
      
      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (i=0; i<p_lut->numBnDegCNG7[0]; i++)
      {
        memcpy(&llrRes[p_lut->bnIdx_CNG7_SG1[i]*Z], p_bnProcBufTemp, Z);
        p_bnProcBufTemp += Z;
      }

      // Process BNs with degree 2 in CNG7
      // Arrange CN message values for addition
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[4]];
      bitOffsetInGroup = p_lut->numBnDegCNG7[1]*NR_LDPC_ZMAX;
      for (i=0; i<2; i++)
      {
        p_bnProcBuf = &bnProcBuf[i*bitOffsetInGroup];
        for (j=0; j<p_lut->numBnDegCNG7[1]; j++)
        {
          nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[lut_addrOffset_CNG7_SG2[j][i]],
            Z, p_lut->circShift_CNG7_SG2[j]);
          p_bnProcBuf += Z;
        }
      }

      // Arrange input LLRs
      for (i=0; i<p_lut->numBnDegCNG7[1]; i++)
      {
        memcpy(p_bnProcBufRes, &p_llrRes[p_lut->bnIdx_CNG7_SG2[i]*Z], Z);
        p_bnProcBufRes += Z;
      }
      
      // Perform addition
      p_bnProcBuf128    = (__m128i*) &bnProcBuf    [0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes [0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      cnOffsetInGroup = (p_lut->numBnDegCNG7[1]*NR_LDPC_ZMAX)>>4;
      M = (p_lut->numBnDegCNG7[1]*Z + 31)>>5;
      for (i=0,j=0; i<M; i++,j+=2)
      {
          // First 16 LLRs of first CN
          ymmRes0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
          ymmRes1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);

          // Loop over CNs
          for (k=1; k<2; k++)
          {
              ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j]);
              ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

              ymm1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j+1]);
              ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);
          }

          // Add LLR from receiver input
          ymm0    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);
          ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

          ymm1    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);
          ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);

          // Pack results back to epi8
          ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
          // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
          // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
          *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

          // Next result
          p_bnProcBufTemp256++;
      }

      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (i=0; i<p_lut->numBnDegCNG7[1]; i++)
      {
        memcpy(&llrRes[p_lut->bnIdx_CNG7_SG2[i]*Z], p_bnProcBufTemp, Z);
        p_bnProcBufTemp += Z;
      }

      // Process BNs with degree 3 in CNG7
      // Arrange CN message values for addition
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[4]];
      bitOffsetInGroup = p_lut->numBnDegCNG7[2]*NR_LDPC_ZMAX;
      for (i=0; i<4; i++)
      {
        p_bnProcBuf = &bnProcBuf[i*bitOffsetInGroup];
        for (j=0; j<p_lut->numBnDegCNG7[2]; j++)
        {
          nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[lut_addrOffset_CNG7_SG3[j][i]],
            Z, p_lut->circShift_CNG7_SG3[j]);
          p_bnProcBuf += Z;
        }
      }

      // Arrange input LLRs
      for (i=0; i<p_lut->numBnDegCNG7[2]; i++)
      {
        memcpy(p_bnProcBufRes, &p_llrRes[p_lut->bnIdx_CNG7_SG4[i]*Z], Z);
        p_bnProcBufRes += Z;
      }
      
      // Perform addition
      p_bnProcBuf128    = (__m128i*) &bnProcBuf    [0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes [0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      cnOffsetInGroup = (p_lut->numBnDegCNG7[2]*NR_LDPC_ZMAX)>>4;
      M = (2*Z + 31)>>5;
      for (i=0,j=0; i<M; i++,j+=2)
      {
          // First 16 LLRs of first CN
          ymmRes0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
          ymmRes1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);

          // Loop over CNs
          for (k=1; k<3; k++)
          {
              ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j]);
              ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

              ymm1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j+1]);
              ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);
          }

          // Add LLR from receiver input
          ymm0    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);
          ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

          ymm1    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);
          ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);

          // Pack results back to epi8
          ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
          // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
          // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
          *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

          // Next result
          p_bnProcBufTemp256++;
      }

      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (i=0; i<p_lut->numBnDegCNG7[2]; i++)
      {
        memcpy(&llrRes[p_lut->bnIdx_CNG7_SG4[i]*Z], p_bnProcBufTemp, Z);
        p_bnProcBufTemp += Z;
      }

      // Process BNs with degree 4 in CNG7
      // Arrange CN message values for addition
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[4]];
      bitOffsetInGroup = p_lut->numBnDegCNG7[3]*NR_LDPC_ZMAX;
      for (i=0; i<4; i++)
      {
        p_bnProcBuf = &bnProcBuf[i*bitOffsetInGroup];
        for (j=0; j<p_lut->numBnDegCNG7[3]; j++)
        {
          nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[lut_addrOffset_CNG7_SG4[j][i]],
            Z, p_lut->circShift_CNG7_SG4[j]);
          p_bnProcBuf += Z;
        }
      }

      // Arrange input LLRs
      for (i=0; i<p_lut->numBnDegCNG7[3]; i++)
      {
        memcpy(p_bnProcBufRes, &p_llrRes[p_lut->bnIdx_CNG7_SG4[i]*Z], Z);
        p_bnProcBufRes += Z;
      }
      
      // Perform addition
      p_bnProcBuf128    = (__m128i*) &bnProcBuf    [0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes [0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      cnOffsetInGroup = (p_lut->numBnDegCNG7[3]*NR_LDPC_ZMAX)>>4;
      M = (p_lut->numBnDegCNG7[3]*Z + 31)>>5;
      for (i=0,j=0; i<M; i++,j+=2)
      {
          // First 16 LLRs of first CN
          ymmRes0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
          ymmRes1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);

          // Loop over CNs
          for (k=1; k<4; k++)
          {
              ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j]);
              ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

              ymm1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j+1]);
              ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);
          }

          // Add LLR from receiver input
          ymm0    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);
          ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

          ymm1    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);
          ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);

          // Pack results back to epi8
          ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
          // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
          // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
          *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

          // Next result
          p_bnProcBufTemp256++;
      }

      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (i=0; i<p_lut->numBnDegCNG7[3]; i++)
      {
        memcpy(&llrRes[p_lut->bnIdx_CNG7_SG4[i]*Z], p_bnProcBufTemp, Z);
        p_bnProcBufTemp += Z;
      }
    }

    // =====================================================================
#ifdef NR_LDPC_PROFILER_DETAIL
    start_meas(&p_profiler->llr2CnProcBuf);
#endif
    bitOffsetInGroup = lut_numCnInCnGroups_BG1_R13[5]*NR_LDPC_ZMAX;

    for (j=0; j<8; j++)
    {
        p_cnProcBuf = &cnProcBuf[lut_startAddrCnGroups[5] + j*bitOffsetInGroup];

        for (i=0; i<lut_numCnInCnGroups[5]; i++)
        {
            idxBn = lut_posBnInCnProcBuf_CNG8[j][i]*Z;
            nrLDPC_circ_memcpy(p_cnProcBuf, &llrRes[idxBn], Z, lut_circShift_CNG8[j][i]);
            p_cnProcBuf += Z;
        }
    }

    M = (lut_numCnInCnGroups[5]*Z*8 + 31)>>5;
    p_cnProcBuf256    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[5]];
    p_cnProcBufRes256 = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[5]];
    for (k=0; k<M; k++)
      p_cnProcBuf256[k] = _mm256_subs_epi8(p_cnProcBuf256[k], p_cnProcBufRes256[k]);

#ifdef NR_LDPC_PROFILER_DETAIL
    stop_meas(&p_profiler->llr2CnProcBuf);
#endif
    // Process group with 8 BNs

    // Offset is 2*384/32 = 24
    const uint8_t lut_idxCnProcG8[8][7] = {{24,48,72,96,120,144,168}, {0,48,72,96,120,144,168},
                                           {0,24,72,96,120,144,168}, {0,24,48,96,120,144,168},
                                           {0,24,48,72,120,144,168}, {0,24,48,72,96,144,168},
                                           {0,24,48,72,96,120,168}, {0,24,48,72,96,120,144}};

    if (lut_numCnInCnGroups[5] > 0)
    {
        // Number of groups of 32 CNs for parallel processing
        // Ceil for values not divisible by 32
        M = (lut_numCnInCnGroups[5]*Z + 31)>>5;

        // Set the offset to each bit within a group in terms of 32 Byte
        bitOffsetInGroup = (lut_numCnInCnGroups_BG1_R13[5]*NR_LDPC_ZMAX)>>5;

        // Set pointers to start of group 8
        p_cnProcBuf256    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[5]];
        p_cnProcBufRes256 = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[5]];

        // Loop over every BN
        for (j=0; j<8; j++)
        {
            // Set of results pointer to correct BN address
            p_cnProcBufResBit = p_cnProcBufRes256 + (j*bitOffsetInGroup);

            // Loop over CNs
            for (i=0; i<M; i++)
            {
                // Abs and sign of 32 CNs (first BN)
                ymm0 = p_cnProcBuf256[lut_idxCnProcG8[j][0] + i];
                sgn  = _mm256_sign_epi8(*p_ones, ymm0);
                min  = _mm256_abs_epi8(ymm0);

                // Loop over BNs
                for (k=1; k<7; k++)
                {
                    ymm0 = p_cnProcBuf256[lut_idxCnProcG8[j][k] + i];
                    min  = _mm256_min_epu8(min, _mm256_abs_epi8(ymm0));
                    sgn  = _mm256_sign_epi8(sgn, ymm0);
                }

                // Store result
                min = _mm256_min_epu8(min, *p_maxLLR); // 128 in epi8 is -127
                *p_cnProcBufResBit = _mm256_sign_epi8(min, sgn);
                p_cnProcBufResBit++;
            }
        }

      // Process BNs with degree 1 in CNG8
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_bnProcBufTemp = &bnProcBufTemp[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[5]];
      for (i=0; i<4; i++)
      {
        nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[addrOffset_BG1_CNG8_SG1_R13[i][0]],
          Z, p_lut->circShift_CNG8_SG1[i]);
        p_bnProcBuf += Z;

        memcpy(p_bnProcBufRes, &llrProcBuf[bnIdx_BG1_CNG8_SG1_R13[i]*Z], Z);
        p_bnProcBufTemp += Z;
      }

      M = (4*Z + 31)>>5;
      p_bnProcBuf128 = (__m128i*) &bnProcBuf[0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes[0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      for (i=0,j=0; i<M; i++,j+=2)
      {
        // First 16 LLRs of first CN
        ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
        ymm1 = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);

        ymmRes0 = _mm256_adds_epi16(ymm0, ymm1);

        // Second 16 LLRs of first CN
        ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);
        ymm1 = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);

        ymmRes1 = _mm256_adds_epi16(ymm0, ymm1);

        // Pack results back to epi8
        ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
        // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
        // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
        *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

        // Next result
        p_bnProcBufTemp256++;
      }
      
      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (i=0; i<4; i++)
      {
        memcpy(&llrRes[bnIdx_BG1_CNG8_SG1_R13[i]*Z], p_bnProcBufTemp, Z);
        p_bnProcBufTemp += Z;
      }

      // Process BNs with degree 2 in CNG8
      // Arrange CN message values for addition
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[5]];
      bitOffsetInGroup = 6*NR_LDPC_ZMAX;
      for (i=0; i<2; i++)
      {
        p_bnProcBuf = &bnProcBuf[i*bitOffsetInGroup];
        for (j=0; j<6; j++)
        {
          nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[addrOffset_BG1_CNG8_SG2_R13[j][i]],
            Z, p_lut->circShift_CNG8_SG2[j]);
          p_bnProcBuf += Z;
        }
      }

      // Arrange input LLRs
      for (i=0; i<6; i++)
      {
        memcpy(p_bnProcBufRes, &p_llrRes[bnIdx_BG1_CNG8_SG2_R13[i]*Z], Z);
        p_bnProcBufRes += Z;
      }
      
      // Perform addition
      p_bnProcBuf128    = (__m128i*) &bnProcBuf    [0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes [0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      cnOffsetInGroup = (6*NR_LDPC_ZMAX)>>4;
      M = (6*Z + 31)>>5;
      for (i=0,j=0; i<M; i++,j+=2)
      {
          // First 16 LLRs of first CN
          ymmRes0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
          ymmRes1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);

          // Loop over CNs
          for (k=1; k<2; k++)
          {
              ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j]);
              ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

              ymm1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j+1]);
              ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);
          }

          // Add LLR from receiver input
          ymm0    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);
          ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

          ymm1    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);
          ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);

          // Pack results back to epi8
          ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
          // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
          // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
          *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

          // Next result
          p_bnProcBufTemp256++;
      }

      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (i=0; i<6; i++)
      {
        memcpy(&llrRes[bnIdx_BG1_CNG8_SG2_R13[i]*Z], p_bnProcBufTemp, Z);
        p_bnProcBufTemp += Z;
      }
    }

    // =====================================================================
#ifdef NR_LDPC_PROFILER_DETAIL
    start_meas(&p_profiler->llr2CnProcBuf);
#endif
    bitOffsetInGroup = lut_numCnInCnGroups_BG1_R13[6]*NR_LDPC_ZMAX;

    for (j=0; j<9; j++)
    {
        p_cnProcBuf = &cnProcBuf[lut_startAddrCnGroups[6] + j*bitOffsetInGroup];

        for (i=0; i<lut_numCnInCnGroups[6]; i++)
        {
            idxBn = lut_posBnInCnProcBuf_CNG9[j][i]*Z;
            nrLDPC_circ_memcpy(p_cnProcBuf, &llrRes[idxBn], Z, lut_circShift_CNG9[j][i]);
            p_cnProcBuf += Z;
        }
    }

    M = (lut_numCnInCnGroups[6]*Z*9 + 31)>>5;
    p_cnProcBuf256    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[6]];
    p_cnProcBufRes256 = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[6]];
    for (k=0; k<M; k++)
      p_cnProcBuf256[k] = _mm256_subs_epi8(p_cnProcBuf256[k], p_cnProcBufRes256[k]);

#ifdef NR_LDPC_PROFILER_DETAIL
    stop_meas(&p_profiler->llr2CnProcBuf);
#endif
    // Process group with 9 BNs

    // Offset is 2*384/32 = 24
    const uint8_t lut_idxCnProcG9[9][8] = {{24,48,72,96,120,144,168,192}, {0,48,72,96,120,144,168,192},
                                           {0,24,72,96,120,144,168,192}, {0,24,48,96,120,144,168,192},
                                           {0,24,48,72,120,144,168,192}, {0,24,48,72,96,144,168,192},
                                           {0,24,48,72,96,120,168,192}, {0,24,48,72,96,120,144,192},
                                           {0,24,48,72,96,120,144,168}};

    if (lut_numCnInCnGroups[6] > 0)
    {
        // Number of groups of 32 CNs for parallel processing
        // Ceil for values not divisible by 32
        M = (lut_numCnInCnGroups[6]*Z + 31)>>5;

        // Set the offset to each bit within a group in terms of 32 Byte
        bitOffsetInGroup = (lut_numCnInCnGroups_BG1_R13[6]*NR_LDPC_ZMAX)>>5;

        // Set pointers to start of group 9
        p_cnProcBuf256    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[6]];
        p_cnProcBufRes256 = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[6]];

        // Loop over every BN
        for (j=0; j<9; j++)
        {
            // Set of results pointer to correct BN address
            p_cnProcBufResBit = p_cnProcBufRes256 + (j*bitOffsetInGroup);

            // Loop over CNs
            for (i=0; i<M; i++)
            {
                // Abs and sign of 32 CNs (first BN)
                ymm0 = p_cnProcBuf256[lut_idxCnProcG9[j][0] + i];
                sgn  = _mm256_sign_epi8(*p_ones, ymm0);
                min  = _mm256_abs_epi8(ymm0);

                // Loop over BNs
                for (k=1; k<8; k++)
                {
                    ymm0 = p_cnProcBuf256[lut_idxCnProcG9[j][k] + i];
                    min  = _mm256_min_epu8(min, _mm256_abs_epi8(ymm0));
                    sgn  = _mm256_sign_epi8(sgn, ymm0);
                }

                // Store result
                min = _mm256_min_epu8(min, *p_maxLLR); // 128 in epi8 is -127
                *p_cnProcBufResBit = _mm256_sign_epi8(min, sgn);
                p_cnProcBufResBit++;
            }
        }

      // Process BNs with degree 1 in CNG9
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_bnProcBufTemp = &bnProcBufTemp[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[6]];
      for (i=0; i<4; i++)
      {
        nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[addrOffset_BG1_CNG9_SG1_R13[i][0]],
          Z, p_lut->circShift_CNG9_SG1[i]);
        p_bnProcBuf += Z;

        memcpy(p_bnProcBufRes, &llrProcBuf[bnIdx_BG1_CNG9_SG1_R13[i]*Z], Z);
        p_bnProcBufTemp += Z;
      }

      M = (4*Z + 31)>>5;
      p_bnProcBuf128 = (__m128i*) &bnProcBuf[0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes[0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      for (i=0,j=0; i<M; i++,j+=2)
      {
        // First 16 LLRs of first CN
        ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
        ymm1 = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);

        ymmRes0 = _mm256_adds_epi16(ymm0, ymm1);

        // Second 16 LLRs of first CN
        ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);
        ymm1 = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);

        ymmRes1 = _mm256_adds_epi16(ymm0, ymm1);

        // Pack results back to epi8
        ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
        // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
        // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
        *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

        // Next result
        p_bnProcBufTemp256++;
      }
      
      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (i=0; i<4; i++)
      {
        memcpy(&llrRes[bnIdx_BG1_CNG9_SG1_R13[i]*Z], p_bnProcBufTemp, Z);
        p_bnProcBufTemp += Z;
      }

      // Process BNs with degree 2 in CNG9
      // Arrange CN message values for addition
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[6]];
      bitOffsetInGroup = 7*NR_LDPC_ZMAX;
      for (i=0; i<2; i++)
      {
        p_bnProcBuf = &bnProcBuf[i*bitOffsetInGroup];
        for (j=0; j<7; j++)
        {
          nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[addrOffset_BG1_CNG9_SG2_R13[j][i]],
            Z, p_lut->circShift_CNG9_SG2[j]);
          p_bnProcBuf += Z;
        }
      }

      // Arrange input LLRs
      for (i=0; i<7; i++)
      {
        memcpy(p_bnProcBufRes, &p_llrRes[bnIdx_BG1_CNG9_SG2_R13[i]*Z], Z);
        p_bnProcBufRes += Z;
      }
      
      // Perform addition
      p_bnProcBuf128    = (__m128i*) &bnProcBuf    [0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes [0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      cnOffsetInGroup = (7*NR_LDPC_ZMAX)>>4;
      M = (7*Z + 31)>>5;
      for (i=0,j=0; i<M; i++,j+=2)
      {
          // First 16 LLRs of first CN
          ymmRes0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
          ymmRes1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);

          // Loop over CNs
          for (k=1; k<2; k++)
          {
              ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j]);
              ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

              ymm1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j+1]);
              ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);
          }

          // Add LLR from receiver input
          ymm0    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);
          ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

          ymm1    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);
          ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);

          // Pack results back to epi8
          ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
          // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
          // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
          *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

          // Next result
          p_bnProcBufTemp256++;
      }

      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (i=0; i<7; i++)
      {
        memcpy(&llrRes[bnIdx_BG1_CNG9_SG2_R13[i]*Z], p_bnProcBufTemp, Z);
        p_bnProcBufTemp += Z;
      }
    }

    // =====================================================================
#ifdef NR_LDPC_PROFILER_DETAIL
    start_meas(&p_profiler->llr2CnProcBuf);
#endif
    bitOffsetInGroup = lut_numCnInCnGroups_BG1_R13[7]*NR_LDPC_ZMAX;

    for (j=0; j<10; j++)
    {
        p_cnProcBuf = &cnProcBuf[lut_startAddrCnGroups[7] + j*bitOffsetInGroup];

        for (i=0; i<lut_numCnInCnGroups[7]; i++)
        {
            idxBn = lut_posBnInCnProcBuf_CNG10[j][i]*Z;
            nrLDPC_circ_memcpy(p_cnProcBuf, &llrRes[idxBn], Z, lut_circShift_CNG10[j][i]);
            p_cnProcBuf += Z;
        }
    }

    M = (lut_numCnInCnGroups[7]*Z*10 + 31)>>5;
    p_cnProcBuf256    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[7]];
    p_cnProcBufRes256 = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[7]];
    for (k=0; k<M; k++)
      p_cnProcBuf256[k] = _mm256_subs_epi8(p_cnProcBuf256[k], p_cnProcBufRes256[k]);

#ifdef NR_LDPC_PROFILER_DETAIL
    stop_meas(&p_profiler->llr2CnProcBuf);
#endif
    // Process group with 10 BNs

    // Offset is 1*384/32 = 12
    const uint8_t lut_idxCnProcG10[10][9] = {{12,24,36,48,60,72,84,96,108}, {0,24,36,48,60,72,84,96,108},
                                             {0,12,36,48,60,72,84,96,108}, {0,12,24,48,60,72,84,96,108},
                                             {0,12,24,36,60,72,84,96,108}, {0,12,24,36,48,72,84,96,108},
                                             {0,12,24,36,48,60,84,96,108}, {0,12,24,36,48,60,72,96,108},
                                             {0,12,24,36,48,60,72,84,108}, {0,12,24,36,48,60,72,84,96}};

    if (lut_numCnInCnGroups[7] > 0)
    {
        // Number of groups of 32 CNs for parallel processing
        // Ceil for values not divisible by 32
        M = (lut_numCnInCnGroups[7]*Z + 31)>>5;

        // Set the offset to each bit within a group in terms of 32 Byte
        bitOffsetInGroup = (lut_numCnInCnGroups_BG1_R13[7]*NR_LDPC_ZMAX)>>5;

        // Set pointers to start of group 10
        p_cnProcBuf256    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[7]];
        p_cnProcBufRes256 = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[7]];

        // Loop over every BN
        for (j=0; j<10; j++)
        {
            // Set of results pointer to correct BN address
            p_cnProcBufResBit = p_cnProcBufRes256 + (j*bitOffsetInGroup);

            // Loop over CNs
            for (i=0; i<M; i++)
            {
                // Abs and sign of 32 CNs (first BN)
                ymm0 = p_cnProcBuf256[lut_idxCnProcG10[j][0] + i];
                sgn  = _mm256_sign_epi8(*p_ones, ymm0);
                min  = _mm256_abs_epi8(ymm0);

                // Loop over BNs
                for (k=1; k<9; k++)
                {
                    ymm0 = p_cnProcBuf256[lut_idxCnProcG10[j][k] + i];
                    min  = _mm256_min_epu8(min, _mm256_abs_epi8(ymm0));
                    sgn  = _mm256_sign_epi8(sgn, ymm0);
                }

                // Store result
                min = _mm256_min_epu8(min, *p_maxLLR); // 128 in epi8 is -127
                *p_cnProcBufResBit = _mm256_sign_epi8(min, sgn);
                p_cnProcBufResBit++;
            }
        }

      // Copy CN to BN messages in CNG10 to bnProcBuff
      bitOffsetInGroup = lut_numCnInCnGroups_BG1_R13[7]*NR_LDPC_ZMAX;
      p_bnProcBuf = &bnProcBuf[0];
      p_bnProcBufRes = &bnProcBufRes[0];

      for (j=0; j<10; j++)
      {
        p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[7] + j*bitOffsetInGroup];
        nrLDPC_inv_circ_memcpy(p_bnProcBuf, p_cnProcBufRes, Z, lut_circShift_CNG10[j][0]);
        p_bnProcBuf += Z;

        idxBn = lut_posBnInCnProcBuf_CNG10[j][0]*Z;
        memcpy(p_bnProcBufRes, &llrProcBuf[idxBn], Z);
        p_bnProcBufRes += Z;
      }

      // Update the BN connected to CNG10
      M = (10*Z + 31)>>5;
      p_bnProcBuf128 = (__m128i*) &bnProcBuf[0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes[0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      for (i=0,j=0; i<M; i++,j+=2)
      {
        // First 16 LLRs of first CN
        ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
        ymm1 = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);

        ymmRes0 = _mm256_adds_epi16(ymm0, ymm1);

        // Second 16 LLRs of first CN
        ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);
        ymm1 = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);

        ymmRes1 = _mm256_adds_epi16(ymm0, ymm1);

        // Pack results back to epi8
        ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
        // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
        // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
        *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

        // Next result
        p_bnProcBufTemp256++;
      }

      // Copy the updated LLRs to llrProcBuf for next CNG processing
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (j=0; j<10; j++)
      {
        idxBn = lut_posBnInCnProcBuf_CNG10[j][0]*Z;
        memcpy(&llrRes[idxBn], p_bnProcBufTemp, Z);
        p_llrRes += Z;
      }
    }

    // =====================================================================
#ifdef NR_LDPC_PROFILER_DETAIL
    start_meas(&p_profiler->llr2CnProcBuf);
#endif
    bitOffsetInGroup = lut_numCnInCnGroups_BG1_R13[8]*NR_LDPC_ZMAX;

    for (j=0; j<19; j++)
    {
        p_cnProcBuf = &cnProcBuf[lut_startAddrCnGroups[8] + j*bitOffsetInGroup];

        for (i=0; i<lut_numCnInCnGroups[8]; i++)
        {
            idxBn = lut_posBnInCnProcBuf_CNG19[j][i]*Z;
            nrLDPC_circ_memcpy(p_cnProcBuf, &llrRes[idxBn], Z, lut_circShift_CNG19[j][i]);
            p_cnProcBuf += Z;
        }
    }

    M = (lut_numCnInCnGroups[8]*Z*19 + 31)>>5;
    p_cnProcBuf256    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[8]];
    p_cnProcBufRes256 = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[8]];
    for (k=0; k<M; k++)
      p_cnProcBuf256[k] = _mm256_subs_epi8(p_cnProcBuf256[k], p_cnProcBufRes256[k]);

#ifdef NR_LDPC_PROFILER_DETAIL
    stop_meas(&p_profiler->llr2CnProcBuf);
#endif
    // Process group with 19 BNs

    // Offset is 4*384/32 = 12
    const uint16_t lut_idxCnProcG19[19][18] = {{48,96,144,192,240,288,336,384,432,480,528,576,624,672,720,768,816,864}, {0,96,144,192,240,288,336,384,432,480,528,576,624,672,720,768,816,864},
                                               {0,48,144,192,240,288,336,384,432,480,528,576,624,672,720,768,816,864}, {0,48,96,192,240,288,336,384,432,480,528,576,624,672,720,768,816,864},
                                               {0,48,96,144,240,288,336,384,432,480,528,576,624,672,720,768,816,864}, {0,48,96,144,192,288,336,384,432,480,528,576,624,672,720,768,816,864},
                                               {0,48,96,144,192,240,336,384,432,480,528,576,624,672,720,768,816,864}, {0,48,96,144,192,240,288,384,432,480,528,576,624,672,720,768,816,864},
                                               {0,48,96,144,192,240,288,336,432,480,528,576,624,672,720,768,816,864}, {0,48,96,144,192,240,288,336,384,480,528,576,624,672,720,768,816,864},
                                               {0,48,96,144,192,240,288,336,384,432,528,576,624,672,720,768,816,864}, {0,48,96,144,192,240,288,336,384,432,480,576,624,672,720,768,816,864},
                                               {0,48,96,144,192,240,288,336,384,432,480,528,624,672,720,768,816,864}, {0,48,96,144,192,240,288,336,384,432,480,528,576,672,720,768,816,864},
                                               {0,48,96,144,192,240,288,336,384,432,480,528,576,624,720,768,816,864}, {0,48,96,144,192,240,288,336,384,432,480,528,576,624,672,768,816,864},
                                               {0,48,96,144,192,240,288,336,384,432,480,528,576,624,672,720,816,864}, {0,48,96,144,192,240,288,336,384,432,480,528,576,624,672,720,768,864},
                                               {0,48,96,144,192,240,288,336,384,432,480,528,576,624,672,720,768,816}};

    if (lut_numCnInCnGroups[8] > 0)
    {
        // Number of groups of 32 CNs for parallel processing
        // Ceil for values not divisible by 32
        M = (lut_numCnInCnGroups[8]*Z + 31)>>5;

        // Set the offset to each bit within a group in terms of 32 Byte
        bitOffsetInGroup = (lut_numCnInCnGroups_BG1_R13[8]*NR_LDPC_ZMAX)>>5;

        // Set pointers to start of group 19
        p_cnProcBuf256    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[8]];
        p_cnProcBufRes256 = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[8]];

        // Loop over every BN
        for (j=0; j<19; j++)
        {
            // Set of results pointer to correct BN address
            p_cnProcBufResBit = p_cnProcBufRes256 + (j*bitOffsetInGroup);

            // Loop over CNs
            for (i=0; i<M; i++)
            {
                // Abs and sign of 32 CNs (first BN)
                ymm0 = p_cnProcBuf256[lut_idxCnProcG19[j][0] + i];
                sgn  = _mm256_sign_epi8(*p_ones, ymm0);
                min  = _mm256_abs_epi8(ymm0);

                // Loop over BNs
                for (k=1; k<18; k++)
                {
                    ymm0 = p_cnProcBuf256[lut_idxCnProcG19[j][k] + i];
                    min  = _mm256_min_epu8(min, _mm256_abs_epi8(ymm0));
                    sgn  = _mm256_sign_epi8(sgn, ymm0);
                }

                // Store result
                min = _mm256_min_epu8(min, *p_maxLLR); // 128 in epi8 is -127
                *p_cnProcBufResBit = _mm256_sign_epi8(min, sgn);
                p_cnProcBufResBit++;
            }
        }

      // Process BNs with degree 2 in CNG19
      // Arrange CN message values for addition
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[8]];
      bitOffsetInGroup = 3*NR_LDPC_ZMAX;
      for (i=0; i<2; i++)
      {
        p_bnProcBuf = &bnProcBuf[i*bitOffsetInGroup];
        for (j=0; j<3; j++)
        {
          nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[addrOffset_BG1_CNG19_SG2_R13[j][i]],
            Z, p_lut->circShift_CNG19_SG2[j]);
          p_bnProcBuf += Z;
        }
      }

      // Arrange input LLRs
      for (i=0; i<3; i++)
      {
        memcpy(p_bnProcBufRes, &p_llrRes[bnIdx_BG1_CNG19_SG2_R13[i]*Z], Z);
        p_bnProcBufRes += Z;
      }
      
      // Perform addition
      p_bnProcBuf128    = (__m128i*) &bnProcBuf    [0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes [0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      cnOffsetInGroup = (3*NR_LDPC_ZMAX)>>4;
      M = (3*Z + 31)>>5;
      for (i=0,j=0; i<M; i++,j+=2)
      {
          // First 16 LLRs of first CN
          ymmRes0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
          ymmRes1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);

          // Loop over CNs
          for (k=1; k<2; k++)
          {
              ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j]);
              ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

              ymm1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j+1]);
              ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);
          }

          // Add LLR from receiver input
          ymm0    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);
          ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

          ymm1    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);
          ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);

          // Pack results back to epi8
          ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
          // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
          // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
          *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

          // Next result
          p_bnProcBufTemp256++;
      }

      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (i=0; i<3; i++)
      {
        memcpy(&llrRes[bnIdx_BG1_CNG6_SG2_R13[i]*Z], p_bnProcBufTemp, Z);
        p_bnProcBufTemp += Z;
      }

      // Process BNs with degree 3 in CNG19
      // Arrange CN message values for addition
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[8]];
      bitOffsetInGroup = 22*NR_LDPC_ZMAX;
      for (i=0; i<3; i++)
      {
        p_bnProcBuf = &bnProcBuf[i*bitOffsetInGroup];
        for (j=0; j<22; j++)
        {
          nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[addrOffset_BG1_CNG19_SG3_R13[j][i]],
            Z, p_lut->circShift_CNG19_SG3[j]);
          p_bnProcBuf += Z;
        }
      }

      // Arrange input LLRs
      for (i=0; i<22; i++)
      {
        memcpy(p_bnProcBufRes, &p_llrRes[bnIdx_BG1_CNG19_SG3_R13[i]*Z], Z);
        p_bnProcBufRes += Z;
      }
      
      // Perform addition
      p_bnProcBuf128    = (__m128i*) &bnProcBuf    [0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes [0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      cnOffsetInGroup = (22*NR_LDPC_ZMAX)>>4;
      M = (22*Z + 31)>>5;
      for (i=0,j=0; i<M; i++,j+=2)
      {
          // First 16 LLRs of first CN
          ymmRes0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
          ymmRes1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);

          // Loop over CNs
          for (k=1; k<3; k++)
          {
              ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j]);
              ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

              ymm1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j+1]);
              ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);
          }

          // Add LLR from receiver input
          ymm0    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);
          ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

          ymm1    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);
          ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);

          // Pack results back to epi8
          ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
          // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
          // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
          *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

          // Next result
          p_bnProcBufTemp256++;
      }

      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (i=0; i<22; i++)
      {
        memcpy(&llrRes[bnIdx_BG1_CNG19_SG3_R13[i]*Z], p_bnProcBufTemp, Z);
        p_bnProcBufTemp += Z;
      }

      // Process BNs with degree 4 in CNG6
      // Arrange CN message values for addition
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[2]];
      bitOffsetInGroup = 2*NR_LDPC_ZMAX;
      for (i=0; i<4; i++)
      {
        p_bnProcBuf = &bnProcBuf[i*bitOffsetInGroup];
        for (j=0; j<2; j++)
        {
          nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[addrOffset_BG1_CNG6_SG4_R13[j][i]],
            Z, p_lut->circShift_CNG6_SG4[j]);
          p_bnProcBuf += Z;
        }
      }

      // Arrange input LLRs
      for (i=0; i<2; i++)
      {
        memcpy(p_bnProcBufRes, &p_llrRes[bnIdx_BG1_CNG6_SG4_R13[i]*Z], Z);
        p_bnProcBufRes += Z;
      }
      
      // Perform addition
      p_bnProcBuf128    = (__m128i*) &bnProcBuf    [0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes [0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      cnOffsetInGroup = (2*NR_LDPC_ZMAX)>>4;
      M = (2*Z + 31)>>5;
      for (i=0,j=0; i<M; i++,j+=2)
      {
          // First 16 LLRs of first CN
          ymmRes0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
          ymmRes1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);

          // Loop over CNs
          for (k=1; k<4; k++)
          {
              ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j]);
              ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

              ymm1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j+1]);
              ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);
          }

          // Add LLR from receiver input
          ymm0    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j]);
          ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

          ymm1    = _mm256_cvtepi8_epi16(p_bnProcBufRes128[j+1]);
          ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);

          // Pack results back to epi8
          ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
          // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
          // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
          *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

          // Next result
          p_bnProcBufTemp256++;
      }

      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      for (i=0; i<2; i++)
      {
        memcpy(&llrRes[bnIdx_BG1_CNG6_SG4_R13[i]*Z], p_bnProcBufTemp, Z);
        p_bnProcBufTemp += Z;
      }

      // Process BNs with degree 4 in CNG19
      // Arrange CN message values for addition
      p_bnProcBuf = &bnProcBuf[0];
      p_llrRes = &llrRes[0];
      p_cnProcBufRes = &cnProcBufRes[lut_startAddrCnGroups[8]];
      bitOffsetInGroup = 1*NR_LDPC_ZMAX;
      for (i=0; i<4; i++)
      {
        p_bnProcBuf = &bnProcBuf[i*bitOffsetInGroup];
        nrLDPC_inv_circ_memcpy(p_bnProcBuf, &p_cnProcBufRes[addrOffset_BG1_CNG19_SG4_R13[0][i]],
          Z, p_lut->circShift_CNG19_SG4[i]);
        p_bnProcBuf += Z;
      }

      // Perform addition
      p_bnProcBuf128    = (__m128i*) &bnProcBuf    [0];
      p_bnProcBufRes128 = (__m128i*) &bnProcBufRes [0];
      p_bnProcBufTemp256 = (__m256i*) &bnProcBufTemp[0];
      cnOffsetInGroup = (1*NR_LDPC_ZMAX)>>4;
      M = (1*Z + 31)>>5;
      for (i=0,j=0; i<M; i++,j+=2)
      {
          // First 16 LLRs of first CN
          ymmRes0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j]);
          ymmRes1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[j+1]);

          // Loop over CNs
          for (k=1; k<3; k++)
          {
              ymm0 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j]);
              ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

              ymm1 = _mm256_cvtepi8_epi16(p_bnProcBuf128[k*cnOffsetInGroup + j+1]);
              ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);
          }

          // Add LLR from receiver input
          p_llrProcBuf128   = (__m128i*) &llrProcBuf[bnIdx_BG1_CNG19_SG4_R13[0]*Z];
          ymm0    = _mm256_cvtepi8_epi16(p_llrProcBuf128[j]);
          ymmRes0 = _mm256_adds_epi16(ymmRes0, ymm0);

          ymm1    = _mm256_cvtepi8_epi16(p_llrProcBuf128[j+1]);
          ymmRes1 = _mm256_adds_epi16(ymmRes1, ymm1);

          // Pack results back to epi8
          ymm0 = _mm256_packs_epi16(ymmRes0, ymmRes1);
          // ymm0     = [ymmRes1[255:128] ymmRes0[255:128] ymmRes1[127:0] ymmRes0[127:0]]
          // p_llrRes = [ymmRes1[255:128] ymmRes1[127:0] ymmRes0[255:128] ymmRes0[127:0]]
          *p_bnProcBufTemp256 = _mm256_permute4x64_epi64(ymm0, 0xD8);

          // Next result
          p_bnProcBufTemp256++;
      }

      // Copy the results to LLR results buffer
      p_bnProcBufTemp = &bnProcBufTemp[0];
      memcpy(&llrRes[bnIdx_BG1_CNG19_SG4_R13[0]*Z], p_bnProcBufTemp, Z);
    }

}

/**
   \brief Performs CN processing for BG1 on the CN processing buffer and stores the results in the CN processing results buffer.
   \param p_lut Pointer to decoder LUTs
   \param Z Lifting size
*/
static inline void nrLDPC_cnProc_BG1(t_nrLDPC_lut* p_lut, t_nrLDPC_procBuf* p_procBuf, uint16_t Z)
{
    const uint8_t*  lut_numCnInCnGroups   = p_lut->numCnInCnGroups;
    const uint32_t* lut_startAddrCnGroups = p_lut->startAddrCnGroups;

    int8_t* cnProcBuf    = p_procBuf->cnProcBuf;
    int8_t* cnProcBufRes = p_procBuf->cnProcBufRes;
    
    __m256i* p_cnProcBuf;
    __m256i* p_cnProcBufRes;

    // Number of CNs in Groups
    uint32_t M;
    uint32_t i;
    uint32_t j;
    uint32_t k;
    // Offset to each bit within a group in terms of 32 Byte
    uint32_t bitOffsetInGroup;

    __m256i ymm0, min, sgn;
    __m256i* p_cnProcBufResBit;

    const __m256i* p_ones   = (__m256i*) ones256_epi8;
    const __m256i* p_maxLLR = (__m256i*) maxLLR256_epi8;

    // LUT with offsets for bits that need to be processed
    // 1. bit proc requires LLRs of 2. and 3. bit, 2.bits of 1. and 3. etc.
    // Offsets are in units of bitOffsetInGroup (1*384/32)
    const uint8_t lut_idxCnProcG3[3][2] = {{12,24}, {0,24}, {0,12}};

    // =====================================================================
    // Process group with 3 BNs

    if (lut_numCnInCnGroups[0] > 0)
    {
        // Number of groups of 32 CNs for parallel processing
        // Ceil for values not divisible by 32
        M = (lut_numCnInCnGroups[0]*Z + 31)>>5;

        // Set the offset to each bit within a group in terms of 32 Byte
        bitOffsetInGroup = (lut_numCnInCnGroups_BG1_R13[0]*NR_LDPC_ZMAX)>>5;

        // Set pointers to start of group 3
        p_cnProcBuf    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[0]];
        p_cnProcBufRes = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[0]];

        // Loop over every BN
        for (j=0; j<3; j++)
        {
            // Set of results pointer to correct BN address
            p_cnProcBufResBit = p_cnProcBufRes + (j*bitOffsetInGroup);

            // Loop over CNs
            for (i=0; i<M; i++)
            {
                // Abs and sign of 32 CNs (first BN)
                ymm0 = p_cnProcBuf[lut_idxCnProcG3[j][0] + i];
                sgn  = _mm256_sign_epi8(*p_ones, ymm0);
                min  = _mm256_abs_epi8(ymm0);

                // 32 CNs of second BN
                ymm0 = p_cnProcBuf[lut_idxCnProcG3[j][1] + i];
                min  = _mm256_min_epu8(min, _mm256_abs_epi8(ymm0));
                sgn  = _mm256_sign_epi8(sgn, ymm0);

                // Store result
                min = _mm256_min_epu8(min, *p_maxLLR); // 128 in epi8 is -127
                *p_cnProcBufResBit = _mm256_sign_epi8(min, sgn);
                p_cnProcBufResBit++;
            }
        }
    }

    // =====================================================================
    // Process group with 4 BNs

    // Offset is 5*384/32 = 60
    const uint8_t lut_idxCnProcG4[4][3] = {{60,120,180}, {0,120,180}, {0,60,180}, {0,60,120}};

    if (lut_numCnInCnGroups[1] > 0)
    {
        // Number of groups of 32 CNs for parallel processing
        // Ceil for values not divisible by 32
        M = (lut_numCnInCnGroups[1]*Z + 31)>>5;

        // Set the offset to each bit within a group in terms of 32 Byte
        bitOffsetInGroup = (lut_numCnInCnGroups_BG1_R13[1]*NR_LDPC_ZMAX)>>5;

        // Set pointers to start of group 4
        p_cnProcBuf    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[1]];
        p_cnProcBufRes = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[1]];

        // Loop over every BN
        for (j=0; j<4; j++)
        {
            // Set of results pointer to correct BN address
            p_cnProcBufResBit = p_cnProcBufRes + (j*bitOffsetInGroup);

            // Loop over CNs
            for (i=0; i<M; i++)
            {
                // Abs and sign of 32 CNs (first BN)
                ymm0 = p_cnProcBuf[lut_idxCnProcG4[j][0] + i];
                sgn  = _mm256_sign_epi8(*p_ones, ymm0);
                min  = _mm256_abs_epi8(ymm0);

                // Loop over BNs
                for (k=1; k<3; k++)
                {
                    ymm0 = p_cnProcBuf[lut_idxCnProcG4[j][k] + i];
                    min  = _mm256_min_epu8(min, _mm256_abs_epi8(ymm0));
                    sgn  = _mm256_sign_epi8(sgn, ymm0);
                }

                // Store result
                min = _mm256_min_epu8(min, *p_maxLLR); // 128 in epi8 is -127
                *p_cnProcBufResBit = _mm256_sign_epi8(min, sgn);
                p_cnProcBufResBit++;
            }
        }
    }

    // =====================================================================
    // Process group with 5 BNs

    // Offset is 18*384/32 = 216
    const uint16_t lut_idxCnProcG5[5][4] = {{216,432,648,864}, {0,432,648,864},
                                            {0,216,648,864}, {0,216,432,864}, {0,216,432,648}};

    if (lut_numCnInCnGroups[2] > 0)
    {
        // Number of groups of 32 CNs for parallel processing
        // Ceil for values not divisible by 32
        M = (lut_numCnInCnGroups[2]*Z + 31)>>5;

        // Set the offset to each bit within a group in terms of 32 Byte
        bitOffsetInGroup = (lut_numCnInCnGroups_BG1_R13[2]*NR_LDPC_ZMAX)>>5;

        // Set pointers to start of group 5
        p_cnProcBuf    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[2]];
        p_cnProcBufRes = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[2]];

        // Loop over every BN
        for (j=0; j<5; j++)
        {
            // Set of results pointer to correct BN address
            p_cnProcBufResBit = p_cnProcBufRes + (j*bitOffsetInGroup);

            // Loop over CNs
            for (i=0; i<M; i++)
            {
                // Abs and sign of 32 CNs (first BN)
                ymm0 = p_cnProcBuf[lut_idxCnProcG5[j][0] + i];
                sgn  = _mm256_sign_epi8(*p_ones, ymm0);
                min  = _mm256_abs_epi8(ymm0);

                // Loop over BNs
                for (k=1; k<4; k++)
                {
                    ymm0 = p_cnProcBuf[lut_idxCnProcG5[j][k] + i];
                    min  = _mm256_min_epu8(min, _mm256_abs_epi8(ymm0));
                    sgn  = _mm256_sign_epi8(sgn, ymm0);
                }

                // Store result
                min = _mm256_min_epu8(min, *p_maxLLR); // 128 in epi8 is -127
                *p_cnProcBufResBit = _mm256_sign_epi8(min, sgn);
                p_cnProcBufResBit++;
            }
        }
    }

    // =====================================================================
    // Process group with 6 BNs

    // Offset is 8*384/32 = 96
    const uint16_t lut_idxCnProcG6[6][5] = {{96,192,288,384,480}, {0,192,288,384,480},
                                            {0,96,288,384,480}, {0,96,192,384,480},
                                            {0,96,192,288,480}, {0,96,192,288,384}};

    if (lut_numCnInCnGroups[3] > 0)
    {
        // Number of groups of 32 CNs for parallel processing
        // Ceil for values not divisible by 32
        M = (lut_numCnInCnGroups[3]*Z + 31)>>5;

        // Set the offset to each bit within a group in terms of 32 Byte
        bitOffsetInGroup = (lut_numCnInCnGroups_BG1_R13[3]*NR_LDPC_ZMAX)>>5;

        // Set pointers to start of group 6
        p_cnProcBuf    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[3]];
        p_cnProcBufRes = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[3]];

        // Loop over every BN
        for (j=0; j<6; j++)
        {
            // Set of results pointer to correct BN address
            p_cnProcBufResBit = p_cnProcBufRes + (j*bitOffsetInGroup);

            // Loop over CNs
            for (i=0; i<M; i++)
            {
                // Abs and sign of 32 CNs (first BN)
                ymm0 = p_cnProcBuf[lut_idxCnProcG6[j][0] + i];
                sgn  = _mm256_sign_epi8(*p_ones, ymm0);
                min  = _mm256_abs_epi8(ymm0);

                // Loop over BNs
                for (k=1; k<5; k++)
                {
                    ymm0 = p_cnProcBuf[lut_idxCnProcG6[j][k] + i];
                    min  = _mm256_min_epu8(min, _mm256_abs_epi8(ymm0));
                    sgn  = _mm256_sign_epi8(sgn, ymm0);
                }

                // Store result
                min = _mm256_min_epu8(min, *p_maxLLR); // 128 in epi8 is -127
                *p_cnProcBufResBit = _mm256_sign_epi8(min, sgn);
                p_cnProcBufResBit++;
            }
        }
    }

    // =====================================================================
    // Process group with 7 BNs

    // Offset is 5*384/32 = 60
    const uint16_t lut_idxCnProcG7[7][6] = {{60,120,180,240,300,360}, {0,120,180,240,300,360},
                                            {0,60,180,240,300,360},   {0,60,120,240,300,360},
                                            {0,60,120,180,300,360},   {0,60,120,180,240,360},
                                            {0,60,120,180,240,300}};

    if (lut_numCnInCnGroups[4] > 0)
    {
        // Number of groups of 32 CNs for parallel processing
        // Ceil for values not divisible by 32
        M = (lut_numCnInCnGroups[4]*Z + 31)>>5;

        // Set the offset to each bit within a group in terms of 32 Byte
        bitOffsetInGroup = (lut_numCnInCnGroups_BG1_R13[4]*NR_LDPC_ZMAX)>>5;

        // Set pointers to start of group 7
        p_cnProcBuf    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[4]];
        p_cnProcBufRes = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[4]];

        // Loop over every BN
        for (j=0; j<7; j++)
        {
            // Set of results pointer to correct BN address
            p_cnProcBufResBit = p_cnProcBufRes + (j*bitOffsetInGroup);

            // Loop over CNs
            for (i=0; i<M; i++)
            {
                // Abs and sign of 32 CNs (first BN)
                ymm0 = p_cnProcBuf[lut_idxCnProcG7[j][0] + i];
                sgn  = _mm256_sign_epi8(*p_ones, ymm0);
                min  = _mm256_abs_epi8(ymm0);

                // Loop over BNs
                for (k=1; k<6; k++)
                {
                    ymm0 = p_cnProcBuf[lut_idxCnProcG7[j][k] + i];
                    min  = _mm256_min_epu8(min, _mm256_abs_epi8(ymm0));
                    sgn  = _mm256_sign_epi8(sgn, ymm0);
                }

                // Store result
                min = _mm256_min_epu8(min, *p_maxLLR); // 128 in epi8 is -127
                *p_cnProcBufResBit = _mm256_sign_epi8(min, sgn);
                p_cnProcBufResBit++;
            }
        }
    }

    // =====================================================================
    // Process group with 8 BNs

    // Offset is 2*384/32 = 24
    const uint8_t lut_idxCnProcG8[8][7] = {{24,48,72,96,120,144,168}, {0,48,72,96,120,144,168},
                                           {0,24,72,96,120,144,168}, {0,24,48,96,120,144,168},
                                           {0,24,48,72,120,144,168}, {0,24,48,72,96,144,168},
                                           {0,24,48,72,96,120,168}, {0,24,48,72,96,120,144}};

    if (lut_numCnInCnGroups[5] > 0)
    {
        // Number of groups of 32 CNs for parallel processing
        // Ceil for values not divisible by 32
        M = (lut_numCnInCnGroups[5]*Z + 31)>>5;

        // Set the offset to each bit within a group in terms of 32 Byte
        bitOffsetInGroup = (lut_numCnInCnGroups_BG1_R13[5]*NR_LDPC_ZMAX)>>5;

        // Set pointers to start of group 8
        p_cnProcBuf    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[5]];
        p_cnProcBufRes = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[5]];

        // Loop over every BN
        for (j=0; j<8; j++)
        {
            // Set of results pointer to correct BN address
            p_cnProcBufResBit = p_cnProcBufRes + (j*bitOffsetInGroup);

            // Loop over CNs
            for (i=0; i<M; i++)
            {
                // Abs and sign of 32 CNs (first BN)
                ymm0 = p_cnProcBuf[lut_idxCnProcG8[j][0] + i];
                sgn  = _mm256_sign_epi8(*p_ones, ymm0);
                min  = _mm256_abs_epi8(ymm0);

                // Loop over BNs
                for (k=1; k<7; k++)
                {
                    ymm0 = p_cnProcBuf[lut_idxCnProcG8[j][k] + i];
                    min  = _mm256_min_epu8(min, _mm256_abs_epi8(ymm0));
                    sgn  = _mm256_sign_epi8(sgn, ymm0);
                }

                // Store result
                min = _mm256_min_epu8(min, *p_maxLLR); // 128 in epi8 is -127
                *p_cnProcBufResBit = _mm256_sign_epi8(min, sgn);
                p_cnProcBufResBit++;
            }
        }
    }

    // =====================================================================
    // Process group with 9 BNs

    // Offset is 2*384/32 = 24
    const uint8_t lut_idxCnProcG9[9][8] = {{24,48,72,96,120,144,168,192}, {0,48,72,96,120,144,168,192},
                                           {0,24,72,96,120,144,168,192}, {0,24,48,96,120,144,168,192},
                                           {0,24,48,72,120,144,168,192}, {0,24,48,72,96,144,168,192},
                                           {0,24,48,72,96,120,168,192}, {0,24,48,72,96,120,144,192},
                                           {0,24,48,72,96,120,144,168}};

    if (lut_numCnInCnGroups[6] > 0)
    {
        // Number of groups of 32 CNs for parallel processing
        // Ceil for values not divisible by 32
        M = (lut_numCnInCnGroups[6]*Z + 31)>>5;

        // Set the offset to each bit within a group in terms of 32 Byte
        bitOffsetInGroup = (lut_numCnInCnGroups_BG1_R13[6]*NR_LDPC_ZMAX)>>5;

        // Set pointers to start of group 9
        p_cnProcBuf    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[6]];
        p_cnProcBufRes = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[6]];

        // Loop over every BN
        for (j=0; j<9; j++)
        {
            // Set of results pointer to correct BN address
            p_cnProcBufResBit = p_cnProcBufRes + (j*bitOffsetInGroup);

            // Loop over CNs
            for (i=0; i<M; i++)
            {
                // Abs and sign of 32 CNs (first BN)
                ymm0 = p_cnProcBuf[lut_idxCnProcG9[j][0] + i];
                sgn  = _mm256_sign_epi8(*p_ones, ymm0);
                min  = _mm256_abs_epi8(ymm0);

                // Loop over BNs
                for (k=1; k<8; k++)
                {
                    ymm0 = p_cnProcBuf[lut_idxCnProcG9[j][k] + i];
                    min  = _mm256_min_epu8(min, _mm256_abs_epi8(ymm0));
                    sgn  = _mm256_sign_epi8(sgn, ymm0);
                }

                // Store result
                min = _mm256_min_epu8(min, *p_maxLLR); // 128 in epi8 is -127
                *p_cnProcBufResBit = _mm256_sign_epi8(min, sgn);
                p_cnProcBufResBit++;
            }
        }
    }

    // =====================================================================
    // Process group with 10 BNs

    // Offset is 1*384/32 = 12
    const uint8_t lut_idxCnProcG10[10][9] = {{12,24,36,48,60,72,84,96,108}, {0,24,36,48,60,72,84,96,108},
                                             {0,12,36,48,60,72,84,96,108}, {0,12,24,48,60,72,84,96,108},
                                             {0,12,24,36,60,72,84,96,108}, {0,12,24,36,48,72,84,96,108},
                                             {0,12,24,36,48,60,84,96,108}, {0,12,24,36,48,60,72,96,108},
                                             {0,12,24,36,48,60,72,84,108}, {0,12,24,36,48,60,72,84,96}};

    if (lut_numCnInCnGroups[7] > 0)
    {
        // Number of groups of 32 CNs for parallel processing
        // Ceil for values not divisible by 32
        M = (lut_numCnInCnGroups[7]*Z + 31)>>5;

        // Set the offset to each bit within a group in terms of 32 Byte
        bitOffsetInGroup = (lut_numCnInCnGroups_BG1_R13[7]*NR_LDPC_ZMAX)>>5;

        // Set pointers to start of group 10
        p_cnProcBuf    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[7]];
        p_cnProcBufRes = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[7]];

        // Loop over every BN
        for (j=0; j<10; j++)
        {
            // Set of results pointer to correct BN address
            p_cnProcBufResBit = p_cnProcBufRes + (j*bitOffsetInGroup);

            // Loop over CNs
            for (i=0; i<M; i++)
            {
                // Abs and sign of 32 CNs (first BN)
                ymm0 = p_cnProcBuf[lut_idxCnProcG10[j][0] + i];
                sgn  = _mm256_sign_epi8(*p_ones, ymm0);
                min  = _mm256_abs_epi8(ymm0);

                // Loop over BNs
                for (k=1; k<9; k++)
                {
                    ymm0 = p_cnProcBuf[lut_idxCnProcG10[j][k] + i];
                    min  = _mm256_min_epu8(min, _mm256_abs_epi8(ymm0));
                    sgn  = _mm256_sign_epi8(sgn, ymm0);
                }

                // Store result
                min = _mm256_min_epu8(min, *p_maxLLR); // 128 in epi8 is -127
                *p_cnProcBufResBit = _mm256_sign_epi8(min, sgn);
                p_cnProcBufResBit++;
            }
        }
    }

    // =====================================================================
    // Process group with 19 BNs

    // Offset is 4*384/32 = 12
    const uint16_t lut_idxCnProcG19[19][18] = {{48,96,144,192,240,288,336,384,432,480,528,576,624,672,720,768,816,864}, {0,96,144,192,240,288,336,384,432,480,528,576,624,672,720,768,816,864},
                                               {0,48,144,192,240,288,336,384,432,480,528,576,624,672,720,768,816,864}, {0,48,96,192,240,288,336,384,432,480,528,576,624,672,720,768,816,864},
                                               {0,48,96,144,240,288,336,384,432,480,528,576,624,672,720,768,816,864}, {0,48,96,144,192,288,336,384,432,480,528,576,624,672,720,768,816,864},
                                               {0,48,96,144,192,240,336,384,432,480,528,576,624,672,720,768,816,864}, {0,48,96,144,192,240,288,384,432,480,528,576,624,672,720,768,816,864},
                                               {0,48,96,144,192,240,288,336,432,480,528,576,624,672,720,768,816,864}, {0,48,96,144,192,240,288,336,384,480,528,576,624,672,720,768,816,864},
                                               {0,48,96,144,192,240,288,336,384,432,528,576,624,672,720,768,816,864}, {0,48,96,144,192,240,288,336,384,432,480,576,624,672,720,768,816,864},
                                               {0,48,96,144,192,240,288,336,384,432,480,528,624,672,720,768,816,864}, {0,48,96,144,192,240,288,336,384,432,480,528,576,672,720,768,816,864},
                                               {0,48,96,144,192,240,288,336,384,432,480,528,576,624,720,768,816,864}, {0,48,96,144,192,240,288,336,384,432,480,528,576,624,672,768,816,864},
                                               {0,48,96,144,192,240,288,336,384,432,480,528,576,624,672,720,816,864}, {0,48,96,144,192,240,288,336,384,432,480,528,576,624,672,720,768,864},
                                               {0,48,96,144,192,240,288,336,384,432,480,528,576,624,672,720,768,816}};

    if (lut_numCnInCnGroups[8] > 0)
    {
        // Number of groups of 32 CNs for parallel processing
        // Ceil for values not divisible by 32
        M = (lut_numCnInCnGroups[8]*Z + 31)>>5;

        // Set the offset to each bit within a group in terms of 32 Byte
        bitOffsetInGroup = (lut_numCnInCnGroups_BG1_R13[8]*NR_LDPC_ZMAX)>>5;

        // Set pointers to start of group 19
        p_cnProcBuf    = (__m256i*) &cnProcBuf   [lut_startAddrCnGroups[8]];
        p_cnProcBufRes = (__m256i*) &cnProcBufRes[lut_startAddrCnGroups[8]];

        // Loop over every BN
        for (j=0; j<19; j++)
        {
            // Set of results pointer to correct BN address
            p_cnProcBufResBit = p_cnProcBufRes + (j*bitOffsetInGroup);

            // Loop over CNs
            for (i=0; i<M; i++)
            {
                // Abs and sign of 32 CNs (first BN)
                ymm0 = p_cnProcBuf[lut_idxCnProcG19[j][0] + i];
                sgn  = _mm256_sign_epi8(*p_ones, ymm0);
                min  = _mm256_abs_epi8(ymm0);

                // Loop over BNs
                for (k=1; k<18; k++)
                {
                    ymm0 = p_cnProcBuf[lut_idxCnProcG19[j][k] + i];
                    min  = _mm256_min_epu8(min, _mm256_abs_epi8(ymm0));
                    sgn  = _mm256_sign_epi8(sgn, ymm0);
                }

                // Store result
                min = _mm256_min_epu8(min, *p_maxLLR); // 128 in epi8 is -127
                *p_cnProcBufResBit = _mm256_sign_epi8(min, sgn);
                p_cnProcBufResBit++;
            }
        }
    }

}

/**
   \brief Performs parity check for BG1 on the CN processing buffer. Stops as soon as error is detected.
   \param p_lut Pointer to decoder LUTs
   \param Z Lifting size
   \return 32-bit parity check indicator
*/
static inline uint32_t nrLDPC_cnProcPc_BG1(t_nrLDPC_lut* p_lut, t_nrLDPC_procBuf* p_procBuf, uint16_t Z)
{
    const uint8_t*  lut_numCnInCnGroups   = p_lut->numCnInCnGroups;
    const uint32_t* lut_startAddrCnGroups = p_lut->startAddrCnGroups;

    int8_t* cnProcBuf    = p_procBuf->cnProcBuf;
    int8_t* cnProcBufRes = p_procBuf->cnProcBufRes;
    
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
static inline uint32_t nrLDPC_cnProcPc_BG2(t_nrLDPC_lut* p_lut, t_nrLDPC_procBuf* p_procBuf, uint16_t Z)
{
    const uint8_t*  lut_numCnInCnGroups   = p_lut->numCnInCnGroups;
    const uint32_t* lut_startAddrCnGroups = p_lut->startAddrCnGroups;

    int8_t* cnProcBuf    = p_procBuf->cnProcBuf;
    int8_t* cnProcBufRes = p_procBuf->cnProcBufRes;
    
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
