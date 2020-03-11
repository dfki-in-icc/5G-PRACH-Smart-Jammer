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

latseq_t * g_latseq = NULL;
pthread_t logger_thread;
//cpuf; //cpu frequency in GHz -> nsec

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

void log_measure(const char * point, const char *fmt, ...)
{
  //check primitives
  if (g_latseq == NULL) {
    fprintf(stderr, "[LATSEQ] not initialized\n");
    exit(EXIT_FAILURE);
  }
  if (g_latseq->is_running == 0) {
      fprintf(stderr, "[LATSEQ] is not running\n");
      exit(EXIT_FAILURE);
  }
  //check if occupancy ok
  if (OCCUPANCY(g_latseq->i_write_head, g_latseq->i_read_head) > MAX_LOG_OCCUPANCY) {
    g_latseq->is_running = 0;
    fprintf(stderr, "[LATSEQ] log buffer max occupancy reached \n");
    if (fclose(g_latseq->outstream)) {
      fprintf(stderr, "[LATSEQ] error on closing %s\n", g_latseq->filelog_name);
      exit(EXIT_FAILURE);
    }
    return;
  }
  //get list of argument
  va_list va;
  va_start(va, fmt);

  //Update head position
  g_latseq->i_write_head++;
  //get reference on new element
  latseq_element_t * e = &g_latseq->log_buffer[g_latseq->i_write_head%MAX_LOG_SIZE];
  //Log time
  e->ts = rdtsc();
  //Log point
  e->point = strdup(point); //check if error
  //Log list of identifiers
  e->len_id = 0;
  vsprintf(e->data_id, fmt, va); //should be moved to logger ?
  //e->data_id = strdup(identifier);
  //e->data_id = malloc(e->len_id * sizeof(char));
  //memcpy(e->data_id, identifier, e->data_id);
  va_end(va);
  //Update stats
  return;
}

int _write_latseq_entry(void)
{
  if (g_latseq == NULL) {
    fprintf(stderr, "[LATSEQ] not initialized\n");
    exit(EXIT_FAILURE);
  }
  latseq_element_t * e = &g_latseq->log_buffer[g_latseq->i_read_head%MAX_LOG_SIZE];
  char * entry;
  //Convert latseq_element to a string
  entry = calloc(MAX_SIZE_LINE_OF_LOG, sizeof(char));
  //Compute time
  uint64_t tdiff = (e->ts - g_latseq->rdtsc_zero)/(cpuf*1000);
  uint64_t tf = (g_latseq->time_zero.tv_sec*1000000L + g_latseq->time_zero.tv_usec) + tdiff;
  struct timeval etv = {
    (time_t) ((tf - (tf%1000000L))/1000000L),
    (suseconds_t) (tf%1000000L)
  };
  //Copy of ts, point name and data identifier
  if (sprintf(entry, "%ld.%06ld %s %s",
      etv.tv_sec,
      etv.tv_usec,
      e->point,
      e->data_id) == 0)
    fprintf(stderr, "[LATSEQ] empty entry\n"); 
  //offset += buffer[g_latseq->i_read_head%MAX_LOG_SIZE].len_id;
  /* The idea was to forge string data identifier here and not in the code
  int i;
  int offset;
  for(i = 0; i < NB_DATA_IDENTIFIERS; i++) {
    offset += sprintf(entry[offset], "%s%d.", latseq_identifiers_t[i], buffer[g_latseq->i_read_head%MAX_LOG_SIZE].data_id[i]);
  }*/
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

  // cleanup buffer element
  e->ts = 0;
  free(e->point);
  e->len_id = 0;
  free(e->data_id);

  return ret;
}

void latseq_log_to_file(void)
{
  pthread_t thId = pthread_self();
  pthread_setname_np(thId, "latseq_log_to_file");
  //set priority
  int prio_for_policy = 10;
  pthread_setschedprio(thId, prio_for_policy);

  
  while(g_latseq->is_running) {// run until flag running is at 0
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
    if (
      (g_latseq->i_read_head == g_latseq->i_write_head)
      )
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