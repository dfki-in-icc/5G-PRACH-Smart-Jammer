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



#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
 
#include "low_oran.h"

int oran_main(int argc, char **argv, oran_t *);

void *dpdk_thread(void *bs)
{
  char *v[] = { "softmodem", "config_file_o_du.dat", "0000:81:0e.0", "0000:81:0e.1"};
  oran_main(4, v, bs);
  exit(1);
  return 0;
}

void *oran_start_dpdk(char *ifname, shared_buffers *buffers)
{
  oran_t *bs;

  bs = calloc(1, sizeof(oran_t));
  if (bs == NULL) {
    printf("%s: out of memory\n", __FUNCTION__);
    exit(1);
  }

  bs->buffers = buffers;

  pthread_attr_t attr;

  if (pthread_attr_init(&attr) != 0) {
    printf("pthread_attr_init failed\n");
    exit(1);
  }

  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(10,&cpuset);
  if (pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpuset) != 0) {
    printf("pthread_attr_setaffinity_np failed\n");
    exit(1);
  }

  if (pthread_attr_setschedpolicy(&attr, SCHED_FIFO) != 0) {
    printf("pthread_attr_setschedpolicy failed\n");
    exit(1);
  }
  
 struct sched_param params;
  params.sched_priority = sched_get_priority_max(SCHED_FIFO);
  if (sched_get_priority_max(SCHED_FIFO) == -1) {
    printf("sched_get_priority_max failed\n");
    exit(1);
  }
  if (pthread_attr_setschedparam(&attr, &params) != 0) {
    printf("pthread_setschedparam failed\n");
    exit(1);
  }
  
  pthread_t t;
  if (pthread_create(&t, &attr, dpdk_thread, bs) != 0) {
    printf("%s: thread creation failed\n", __FUNCTION__);
    exit(1);
  }

  if (pthread_attr_destroy(&attr) != 0) {
    printf("pthread_attr_init failed\n");
    exit(1);
  }

  return bs;
}
                    

