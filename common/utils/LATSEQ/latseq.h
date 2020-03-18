#ifndef __LATSEQ_H__
#define __LATSEQ_H__

/*--- INCLUDES ---------------------------------------------------------------*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <stdarg.h>
#include <time.h>
#include <stdint.h>
#ifndef __STDC_FORMAT_MACROS
  #define __STDC_FORMAT_MACROS
#endif
#include <inttypes.h>
#ifndef _GNU_SOURCE
  #define _GNU_SOURCE
#endif
#include <pthread.h>
//#include <T.h>
#include <utils.h>
#include <LOG/log.h>
//#include <openair1/PHY/TOOLS/time_meas.h>

#include "latseq.h"
/*----------------------------------------------------------------------------*/

/*--- DEFINE -----------------------------------------------------------------*/

#define MAX_LOG_SIZE        1024
#define MAX_LOG_OCCUPANCY   768
#define MAX_POINT_NAME_SIZE 16
#define MAX_LEN_DATA_ID     16
#define MAX_NB_DATA_ID      16
#define NB_DATA_IDENTIFIERS 10 // to update according to distinct data identifier used in point
//link to the NB_DATA_IDENTIFIERS
static const char LATSEQ_IDENTIFIERS[NB_DATA_IDENTIFIERS][8] = {
  "enb",
  "ip",
  "drb",
  "pdcp",
  "rsn",
  "rso",
  "lcid",
  "tb",
  "harq",
  "phy"
};
#define MAX_DATA_ID_SIZE 128 // > (4+8)x7 
#define MAX_SIZE_LINE_OF_LOG 256 // ts=8c + pointname=MAX_POINT_NAME_SIZEc + identifier=NB_DATA_IDENTIFIERx(4c name + 4c number) (Worst-case)
//#define LATSEQ_P(p, i) log_measure(p, i); // LatSeq point, nb of id and ids...
#define LATSEQ_P(p, f, ...) do {log_measure(p, f, __VA_ARGS__, -1); } while(0) // -1 to stop iterate on va_arg. assumption : all values of ids are >= 0.
#define OCCUPANCY(w, r) (w - r)

#define MAX_NB_THREAD 256

/*--- STRUCT -----------------------------------------------------------------*/

// A latseq element of the buffer
typedef struct latseq_element_t {
  uint64_t            ts; // timestamp of the measure
  const char *              point;
  //char                point[MAX_POINT_NAME_SIZE]; // point name
  const char *              format;
  //char *              format; // format for the data identifier
  short                len_id; // Number data identifiers
  uint32_t             data_id[MAX_NB_DATA_ID]; // values for the data identifier. What is the best type ?
} latseq_element_t;

// Statistics structures for latseq
typedef struct latseq_stats_t {
  unsigned int        entry_counter;
} latseq_stats_t;

//thread specific data struct
typedef struct latseq_thread_data_t {
  uint8_t             th_latseq_id; //Identifier of pthread for registry
  latseq_element_t    log_buffer[MAX_LOG_SIZE]; //log buffer, structure mutex-less
  unsigned int        i_write_head; // position of writer in the log_buffer (main thread)
} latseq_thread_data_t;

//Registry of pointers to thread-specific struct latseq_data_thread
typedef struct latseq_registry_t {
  uint8_t                 read_ith_thread;
  uint8_t                 last_th;
  latseq_thread_data_t *  registry[MAX_NB_THREAD];
  unsigned int            i_read_heads[MAX_NB_THREAD]; // position of reader in the ith log buffer (logger thread)
} latseq_registry_t;

// Global structure of LatSeq module
typedef struct latseq_t {
  int                 is_running; //0 is running, -1 not running
  int                 is_debug; //1 debug, 0 prod
  const char *        filelog_name;
  FILE *              outstream; //Output descriptor
  struct timeval      time_zero; // time zero
  uint64_t            rdtsc_zero; //rdtsc zero
  latseq_registry_t   local_log_buffers; //Register of thread-specific buffers
  latseq_stats_t      stats; // stats of latseq instance
} latseq_t;

/*--- EXTERNS ----------------------------------------------------------------*/

extern latseq_t g_latseq; // global structure
extern __thread latseq_thread_data_t tls_latseq;

/*--- FUNCTIONS --------------------------------------------------------------*/
/** \fn int init_latseq(char * filename);
 * \brief init latency sequences module 
 * \param filename output file name for latseq stats
 * \param debuf level for this instance of latseq
 * \return 0 if error 1 otherwise
*/
int init_latseq(const char * filename, int debug);

/** \fn init_logger_to_mem(void);
 * \brief init thread logger
*/
void init_logger_latseq(void);

/** \fn init_thread_for_latseq(void);
 * \brief init tls_latseq for local oai thread
*/
void init_thread_for_latseq(void);

/** \fn void log_measure(const char * point, const char *identifier);
 * \brief function to log a new measure into buffer
 * \param point name of the measurement point
 * \param id identifier for the data pointed
 * \todo  measure latency introduced by this function
*/
static inline void log_measure(const char * point, const char *fmt, ...)
{
  // No check here because it will be check by the reader
  //get list of argument
  va_list va;
  va_start(va, fmt); //start all values after fmt
  //check if the oai thread is already registered
  if (tls_latseq.th_latseq_id == 0) {
    //is not initialized yet
    init_thread_for_latseq();
  }
  //Update head position
  tls_latseq.i_write_head++;
  //get reference on new element
  latseq_element_t * e = &tls_latseq.log_buffer[tls_latseq.i_write_head%MAX_LOG_SIZE];

  //Log time
  e->ts = rdtsc(); //Not used because rdtsc from log.h seems to be not static
  //e->ts = rdtsc_oai(); //use of rdtsc defined in time_meas.h
  //unsigned long long a, d;
  //__asm__ volatile ("rdtsc" : "=a" (a), "=d" (d));
  //e->ts = (d<<32) | a; //because of imcompatibility between inline and static of rdtsc. Assumption : usage of __x86_64__

  //Log point name
  //strcpy(e->point, point);
  e->point = point;

  //Log data identifier
  e->len_id = 0;
  while ( (e->data_id[e->len_id] = (uint32_t)va_arg(va, int) )!= -1)
    e->len_id++;
  
  //Clean up va_list
  va_end(va);
}

/** \fn static int write_latseq_entry(void);
 * \brief private function to write an entry in the log file
*/
static int write_latseq_entry(void);

/** \fn void log_to_file(void);
 * \brief function to save buffer of logs into a file
*/
void latseq_log_to_file(void);

/** \fn void latseq_print_stats(void);
 * \brief print some stats about latseq
*/
void latseq_print_stats(void);

/** \fn int close_latseq(void);
 * \brief finish latseq measurement if a latseq is running
 * \return 0 if error 1 otherwise
*/
int close_latseq(void);

/*----------------------------------------------------------------------------*/

#endif
