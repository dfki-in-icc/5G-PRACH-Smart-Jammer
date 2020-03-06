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
#include <common/utils/T/T.h>
#include <common/utils/utils.h>
#include <common/utils/LOG/log.h>
#include <openair1/PHY/TOOLS/time_meas.h>

#include "latseq.h"
/*----------------------------------------------------------------------------*/

/*--- DEFINE -----------------------------------------------------------------*/

#define MAX_LOG_SIZE 1024
#define MAX_LOG_OCCUPANCY 768
#define MAX_POINT_NAME_SIZE 16
#define NB_DATA_IDENTIFIERS 7 // to update according to distinct data identifier used in point
#define MAX_SIZE_LINE_OF_LOG 128 // ts=8c + pointname=MAX_POINT_NAME_SIZEc + identifier=NB_DATA_IDENTIFIERx(4c name + 4c number) (Worst-case)
#define LATSEQ_P(p, i) log_measure(p, i); // LatSeq point, nb of id and ids...
#define OCCUPANCY(w, r) (w - r)

/*--- STRUCT -----------------------------------------------------------------*/
//link to the NB_DATA_IDENTIFIERS
static const char LATSEQ_IDENTIFIERS[NB_DATA_IDENTIFIERS][8] = {
  "ip", "drb", "pdcp", "rlcsn", "rlcso", "lcid", "tb"
};

typedef struct latseq_element_t {
  uint64_t            ts; // timestamp of the measure
  char *              point; // point name
  char                len_id; // length of data identifier
  char *              data_id;
  //unsigned short      data_id[NB_DATA_IDENTIFIERS]; // replace by a unique variable where mask is applied for each identifier.
} latseq_element_t;

typedef struct latseq_stats_t {
  unsigned int        entry_counter;
} latseq_stats_t;

typedef struct latseq_t {
  int                 is_running;
  int                 is_debug;
  const char *        filelog_name;
  FILE *              outstream;
  latseq_element_t    log_buffer[MAX_LOG_SIZE]; //log buffer, structure mutex-less
  int                 i_write_head; // position of writer in the log_buffer (main thread)
  int                 i_read_head;  // position of reader in the log buffer (logger thread)
  struct timeval      time_zero; // time zero
  uint64_t            rdtsc_zero; //rdtsc zero;
  latseq_stats_t      stats; // stats of latseq instance
} latseq_t;
/*----------------------------------------------------------------------------*/


/*--- FUNCTIONS --------------------------------------------------------------*/
/** \fn int init_latseq(char * filename);
 * \brief init latency sequences module 
 * \param filename output file name for latseq stats
 * \return 0 if error 1 otherwise
*/
int init_latseq(char * filename);

/** \fn init_logger_to_mem(void);
 * \brief init thread logger
*/
void init_logger_latseq(void);

/** \fn void log_measure(const char * point, const char *identifier);
 * \brief function to log a new measure into buffer
 * \param point name of the measurement point
 * \param id identifier for the data pointed
*/
void log_measure(const char * point, const char *identifier);

/** \fn int _write_latseq_entry(void);
 * \brief private function to write an entry in the log file
*/
int _write_latseq_entry(void);

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