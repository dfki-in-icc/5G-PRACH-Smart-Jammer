#ifndef TASK_WORK_STEALING_THREAD_POOL_H
#define TASK_WORK_STEALING_THREAD_POOL_H 

#include <stdlib.h>

typedef struct{
  void* args;
  void (*func)(void* args);
} task_t;

typedef struct{
  task_t t[16];
  size_t sz;
} batch_task_t;

#endif

