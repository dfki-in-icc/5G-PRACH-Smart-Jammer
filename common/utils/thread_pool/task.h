#ifndef TASK_WORK_STEALING_THREAD_POOL_H
#define TASK_WORK_STEALING_THREAD_POOL_H 

#include <stdlib.h>


typedef enum{

  NR_PROCESS_UL_SEGMENT, // nr_processULSegment
  NR_PUSCH_SYMBOL_PROCESSING_NOPRECODING, //  nr_pusch_symbol_processing_noprecoding

} func_e;

typedef struct{
  void* args;
//  func_e func;
  void (*func)(void* args);
} task_t;

typedef struct{
  task_t t[16];
  size_t sz;
} batch_task_t;

#endif

