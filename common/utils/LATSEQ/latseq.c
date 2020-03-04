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

#include "latseq.h"
#include "latseq_extern.h"

#include "assertions.h"
#include <pthread.h>

/*----------------------------------------------------------------------------*/

latseq_t * g_latseq;
pthread_t logger_thread;

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
  memcpy(g_latseq->filelog_name, filename, sizeof(filename));
  g_latseq->i_read_head = 0;
  g_latseq->i_write_head = 0;
  g_latseq->stats.entry_counter = 0;
  g_latseq->outstream = fopen(g_latseq->filelog_name, "a"); //open log file in append mode, should we open it in the logger thread ?
  
  // init logger thread
  g_latseq->is_running = 1;
  init_logger_latseq();

}

void init_logger_latseq(void)
{
  // create thread write buffer to files
  pthread_create(&logger_thread, NULL, &latseq_log_to_file, NULL);
  if (g_latseq->is_debug)
    printf("[LATSEQ] Logger thread started\n");
}

void log_measure(const char * point, const char * identifier)
{
  //check if occupancy ok
  if (OCCUPANCY(g_latseq->i_write_head, g_latseq->i_read_head) > MAX_LOG_OCCUPANCY) {
    g_latseq->is_running = 0;
    close(g_latseq->outstream);
    fprintf(stderr, "[LATSEQ] log buffer max occupancy reached \n");
    return;
  }
  //Update head position
  g_latseq->i_write_head++;
  //get reference on new element
  latseq_element_t * e = &g_latseq->log_buffer[g_latseq->i_write_head%MAX_LOG_SIZE];
  //Log time
  e->ts = rdtsc_oai();
  //Log point
  e->point = malloc(sizeof(point));
  memcpy(e->point, point, sizeof(point));
  //Log list of identifiers
  e->len_id = sizeof(identifier);
  e->data_id = malloc(e->len_id * sizeof(char));
  memcpy(e->data_id, identifier, e->data_id);
  //Update stats
}

int _write_latseq_entry(void)
{
  latseq_element_t * e = &g_latseq->log_buffer[g_latseq->i_read_head%MAX_LOG_SIZE];
  char * entry;
  //Convert latseq_element to a string
  entry = calloc(MAX_SIZE_LINE_OF_LOG, sizeof(char));
  //Copy of ts, point name and data identifier
  sprintf(
    entry[0], 
    "%llu %s %s",
    e->ts,
    e->point,
    e->data_id
    ); //check offset
  //offset += buffer[g_latseq->i_read_head%MAX_LOG_SIZE].len_id;
  /* The idea was to forge string data identifier here and not in the code
  for(i = 0; i < NB_DATA_IDENTIFIERS; i++) {
    offset += sprintf(entry[offset], "%s%d.", latseq_identifiers_t[i], buffer[g_latseq->i_read_head%MAX_LOG_SIZE].data_id[i]);
  }*/
  // Write into file
  int ret = write(g_latseq->outstream, entry, sizeof(entry));
  if (ret < 0) {
    g_latseq->is_running = 0;
    close(g_latseq->outstream);
    fprintf(stderr, "[LATSEQ] output log file cannot be written \n");
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
  pthread_setname_np(pthread_self(), "latseq_log_to_file");
  int i;
  int offset;
  
  while(g_latseq->is_running) {// run until flag running is at 0
    //If max occupancy reached
    if (OCCUPANCY(g_latseq->i_write_head, g_latseq->i_read_head) > MAX_LOG_OCCUPANCY) {
      g_latseq->is_running = 0;
      close(g_latseq->outstream);
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
    //sleep ?
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
}

int close_latseq(void)
{
  g_latseq->is_running = 0;
  //Wait logger finish to write data
  pthread_join(&logger_thread, NULL);
  //At this point, data_ids and points should be freed by the logger thread
  close(g_latseq->outstream);
  if (g_latseq->is_debug)
    latseq_print_stats();
  free(g_latseq);
}