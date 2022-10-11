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

#include "../../../openair3/SECU/aes_128_ctr.h"
#include "../../../common/utils/assertions.h"

#include "nr_pdcp_security_nea2.h"

#include <arpa/inet.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

void *nr_pdcp_security_nea2_init(unsigned char *ciphering_key)
{
  // This is a hack, IMO init, cipher and free functions should be reduced to cipher.
  return ciphering_key;
}

void nr_pdcp_security_nea2_cipher(void *security_context, unsigned char *buffer, int length, int bearer, int count, int direction)
{
  DevAssert(security_context != NULL);
  DevAssert(buffer != NULL);
  DevAssert(length > 0);
  DevAssert(bearer > -1 && bearer < 32);
  DevAssert(direction > -1 && direction < 2);
  DevAssert(count > -1);

  aes_128_ctr_t p = {0};
  const uint8_t *ciphering_key = (uint8_t const *)security_context;
  memcpy(p.key, ciphering_key, 16);
  p.iv_p.count = ntohl(count);
  p.iv_p.bearer = bearer;
  p.iv_p.direction = direction;

  uint8_t out[length];
  memset(out, 0, length);
  const size_t len_out = length;
  aes_128_ctr(&p, length, buffer, len_out, out);

  memcpy(buffer, out, length);
}

void nr_pdcp_security_nea2_free_security(void *security_context)
{
  (void)security_context;
}
