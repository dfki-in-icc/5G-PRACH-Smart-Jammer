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

/*! \file latseq.c
* \brief latency sequence tool
* \author Flavien Ronteix--Jacquet
* \date 2020
* \version 0.1
* @ingroup util
*/
#define _GNU_SOURCE // required for pthread_setname_np()
#include "latseq.h"
#include "latseq_extern.h"

#include <pthread.h>

#include "assertions.h"

/*----------------------------------------------------------------------------*/

latseq_t * g_latseq = NULL; // Should it still a pointer ? extern to latseq.h
pthread_t logger_thread;
extern double cpuf; //cpu frequency in GHz -> nsec

/*----------------------------------------------------------------------------*/

int init_latseq(char * filename)
{
  // init global struct
  g_latseq = malloc(sizeof(latseq_t));
  if (g_latseq == NULL) {
    perror ("cannot allocated memory for log generation module \n");
    exit(EXIT_FAILURE);
  }
  
  // init members
  g_latseq->is_running = 0;
  g_latseq->is_debug = 1;
  g_latseq->filelog_name = strdup(filename);
  g_latseq->i_read_head = 0;
  g_latseq->i_write_head = 0;
  //synchronise time and rdtsc
  gettimeofday(&g_latseq->time_zero, NULL);
  g_latseq->rdtsc_zero = rdtsc(); //check at compile time that constant_tsc is enabled in /proc/cpuinfo
  if (cpuf == 0)
    cpuf = get_cpu_freq_GHz();

  g_latseq->stats.entry_counter = 0;
  g_latseq->outstream = fopen(g_latseq->filelog_name, "a"); //open log file in append mode, should we open it in the logger thread ?
  
  // init logger thread
  g_latseq->is_running = 1;
  init_logger_latseq();

  return g_latseq->is_running;
}

void init_logger_latseq(void)
{
  // init thread to write buffer to file
  pthread_create(&logger_thread, NULL, (void *) &latseq_log_to_file, NULL);
  if (g_latseq->is_debug)
    printf("[LATSEQ] Logger thread started\n");
}

int _write_latseq_entry(void)
{
  if (g_latseq == NULL) {
    fprintf(stderr, "[LATSEQ] not initialized\n");
    exit(EXIT_FAILURE);
  }
  latseq_element_t * e = &g_latseq->log_buffer[g_latseq->i_read_head%MAX_LOG_SIZE];
  char * entry;
  char * tmps;
  //Convert latseq_element to a string
  entry = calloc(MAX_SIZE_LINE_OF_LOG, sizeof(char));
  tmps = calloc(e->len_id * 6, sizeof(char)); // how to compute size needed ? 6 corresponds to value 999.999
  //Compute time
  uint64_t tdiff = (e->ts - g_latseq->rdtsc_zero)/(cpuf*1000);
  uint64_t tf = (g_latseq->time_zero.tv_sec*1000000L + g_latseq->time_zero.tv_usec) + tdiff;
  struct timeval etv = {
    (time_t) ((tf - (tf%1000000L))/1000000L),
    (suseconds_t) (tf%1000000L)
  };
  //Write the data identifier, e.g. do the vsprintf() here and not at measure()
  //We put the first MAX_NB_DATA_ID elements of array, even there are no MAX_NB_DATA_ID element to write. sprintf will get the firsts...
  sprintf(
    tmps,
    e->format, 
    e->data_id[0],
    e->data_id[1],
    e->data_id[2],
    e->data_id[3],
    e->data_id[4],
    e->data_id[5],
    e->data_id[6],
    e->data_id[7],
    e->data_id[8],
    e->data_id[9],
    e->data_id[10],
    e->data_id[11],
    e->data_id[12],
    e->data_id[13],
    e->data_id[14],
    e->data_id[15]);
  //Copy of ts, point name and data identifier
  if (sprintf(entry, "%ld.%06ld %s %s",
      etv.tv_sec,
      etv.tv_usec,
      e->point,
      tmps) == 0)
    fprintf(stderr, "[LATSEQ] empty entry\n"); 

  // Write into file
  int ret = fwrite(entry, 1, sizeof(entry), g_latseq->outstream);
  if (ret < 0) {
    g_latseq->is_running = 0;
    fclose(g_latseq->outstream);
    fprintf(stderr, "[LATSEQ] output log file cannot be written\n");
    exit(EXIT_FAILURE);
  }
  if (g_latseq->is_debug)
    printf("[LATSEQ] log an entry : %s (len %d)\n", &entry[0], ret);
  free(entry);
  free(tmps);

  // cleanup buffer element
  e->ts = 0;
  //free(e->point); // no free() for const char * not allocated by malloc()
  e->len_id = 0;
  free(e->data_id);

  return ret;
}

void latseq_log_to_file(void)
{
  pthread_t thId = pthread_self();
  //set name
  pthread_setname_np(thId, "latseq_log_to_file");
  //set priority
  int prio_for_policy = 10;
  pthread_setschedprio(thId, prio_for_policy);

  while(g_latseq->is_running) {// run until flag running is at 0
    //TODO for each buffer
    //If max occupancy reached
    if (OCCUPANCY(g_latseq->i_write_head, g_latseq->i_read_head) > MAX_LOG_OCCUPANCY) {
      g_latseq->is_running = 0;
      fclose(g_latseq->outstream);
      fprintf(stderr, "[LATSEQ] log buffer max occupancy reached \n");
      exit(EXIT_FAILURE);
    }
    //Update header position
    g_latseq->i_read_head++;
    //If no new element
    if (g_latseq->i_read_head == g_latseq->i_write_head)
      continue;
    //Write pointed entry into log file
    (void)_write_latseq_entry();

    //Update counter and stats
    g_latseq->stats.entry_counter++;
    //sleep ? if low priority, no.
  }
  //Write all remaining data
  while (g_latseq->i_read_head != g_latseq->i_write_head)
  {
    g_latseq->i_read_head++;
    (void)_write_latseq_entry();
  }
  //exit thread
  pthread_exit(NULL);
}

void latseq_print_stats(void)
{
  printf("[LATSEQ] stats :\n");
  printf("[LATSEQ] number of entry in log : %d\n", g_latseq->stats.entry_counter);
  printf("[LATSEQ] heads positions : %d (Write) : %d (Read)\n", g_latseq->i_write_head, g_latseq->i_read_head);
}

int close_latseq(void)
{
  g_latseq->is_running = 0;
  //Wait logger finish to write data
  pthread_join(logger_thread, NULL);
  //At this point, data_ids and points should be freed by the logger thread
  free((char*) g_latseq->filelog_name);
  if (fclose(g_latseq->outstream)){
    fprintf(stderr, "[LATSEQ] error on closing %s\n", g_latseq->filelog_name);
    exit(EXIT_FAILURE);
  }
  if (g_latseq->is_debug)
    latseq_print_stats();
  free(g_latseq);
  return 0;
}