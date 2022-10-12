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

#include "nr_pdcp_integrity_nia2.h"

#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "common/utils/assertions.h"
#include "openair3/SECU/aes_128.h"
#include "openair3/SECU/aes_128_cbc_cmac.h"

void *nr_pdcp_integrity_nia2_init(unsigned char *integrity_key)
{
  // This is a hack. Reduce the 3 functions to just cipher?
  return integrity_key;
}

void nr_pdcp_integrity_nia2_integrity(void *integrity_context, unsigned char *out, unsigned char *buffer, int length, int bearer, int count, int direction)
{
  DevAssert(integrity_context != NULL);
  DevAssert(out != NULL);
  DevAssert(buffer != NULL);
  DevAssert(length > -1);
  DevAssert(bearer > -1 && bearer < 32);
  DevAssert(count > -1);

  uint8_t const *integrity_key = (uint8_t *)integrity_context;
  aes_128_t k_iv = {0};
  memcpy(&k_iv.key, integrity_key, sizeof(k_iv.key));
  k_iv.type = AES_INITIALIZATION_VECTOR_16;
  k_iv.iv16.d.bearer = bearer;
  k_iv.iv16.d.direction = direction;
  //  k_iv.iv_p.count = ntohl(count);
  k_iv.iv16.d.count = htonl(count);

  uint8_t result[16] = {0};
  aes_128_cbc_cmac(&k_iv, length, buffer, sizeof(result), result);

  // AES CMAC (RFC 4493) outputs 128 bits but NR PDCP PDUs have a MAC-I of
  // 32 bits (see 38.323 6.2). RFC 4493 2.1 says to truncate most significant
  // bit first (so seems to say 33.401 B.2.3)
  // Precondition: out should have enough space...
  memcpy(out, result, 4);
}

void nr_pdcp_integrity_nia2_free_integrity(void *integrity_context)
{
  (void)integrity_context;
}
