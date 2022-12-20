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



#ifndef BYTE_ARRAY_H
#define BYTE_ARRAY_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

typedef struct {
  size_t len;
  uint8_t *buf;
} byte_array_t;

/* create on the stack a byte_array_t named 'name' implemented as array of length 'length'*/
#define BYTE_ARRAY_STACK(name, length)  \
  uint8_t (name##_BuF)[(length)];       \
  memset((name##_BuF), 0, sizeof((name##_BuF))); \
  byte_array_t (name) = {.buf = (name##_BuF), .len = (length)}

/* create on the heap a new 'byte_array_t' data structure named 'ba' from OCTET_STRING_t named 'octet' pointer*/
#define BYTE_ARRAY_HEAP_CP_FROM_OCTET_STRING_POINTERS(ba, octet)  \
  ba->buf = calloc(1, octet->size);\
  assert(ba->buf!=NULL && "Memory exhausted");\
  memcpy(ba->buf, octet->buf, octet->size);\
  ba->len = octet->size;

#define BYTE_ARRAY_HEAP_CP_FROM_OCTET_STRING(ba, octet)  \
  ba.buf = calloc(1, octet.size);\
  assert(ba.buf!=NULL && "Memory exhausted");\
  memcpy(ba.buf, octet.buf, octet.size);\
  ba.len = octet.size;


byte_array_t copy_byte_array(byte_array_t src);

void free_byte_array(byte_array_t ba);

bool eq_byte_array(const byte_array_t* m0, const byte_array_t* m1);

#endif
