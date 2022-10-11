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

#ifndef AES_128_CTR_OAI_H
#define AES_128_CTR_OAI_H

#include <endian.h>
#include <stdint.h>
#include <stdlib.h>

#if __BYTE_ORDER == __BIG_ENDIAN
static_assert(0 != 0, "The struct iv_t needs little endianness");
#endif

// TS 133 401 pag. 88

typedef struct {
  union {
    uint32_t count;
    uint8_t count_8[4];
  };
  uint8_t padding: 2;
  uint8_t direction: 1;
  uint8_t bearer: 5;
  uint8_t pad_8;
  uint16_t pad_16;
  uint64_t pad_64;
} iv_t;

// static_assert((16 == sizeof(iv_t)) , "Initialization Vector needs to have an AES_BLOCK_SIZE" );

typedef struct {
  uint8_t key[16];
  union {
    uint8_t iv[16];
    iv_t iv_p;
  };
} aes_128_ctr_t;

void aes_128_ctr(const aes_128_ctr_t* p, size_t len, uint8_t const plain[len], size_t len_out, uint8_t out[len_out]);

#endif
