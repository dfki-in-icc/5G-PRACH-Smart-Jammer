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

#include "latseq.h"
/*----------------------------------------------------------------------------*/

/*--- DEFINE -----------------------------------------------------------------*/

#define MAX_LOG_SIZE 1000
#define LATSEQ(p, i) do {log_measure(p, i);} while(0) 

/*--- STRUCT -----------------------------------------------------------------*/
typedef struct latseq {
    int     is_running;
    int     is_debug;
    char *  filelog_name;
    FILE *  outstream;
    char    log_buffer[MAX_LOG_SIZE];
} latseq;
/*----------------------------------------------------------------------------*/


/*--- FUNCTIONS ---------------------------------------------------------------*/
/** \fn int init_latseq(char * filename);
 * \brief init latency sequences module 
 * \param filename output file name for latseq stats
 * \return 0 if error 1 otherwise
*/
int init_latseq(char * filename);

/** \fn void log_measure(const char * point, const char * id);
 * \brief function to log a new measure into buffer
 * \param point name of the measurement point
 * \param id identifier for the data pointed
*/
void log_measure(const char * point, const char * id);

/** \fn int log_to_mem(void);
 * \brief function to save buffer of logs into a file
 * \return 0 if error 1 otherwise
*/
int log_to_mem(void);

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