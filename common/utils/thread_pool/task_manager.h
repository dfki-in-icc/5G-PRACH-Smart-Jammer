#ifndef TASK_MANAGER_WORKING_STEALING_H
#define TASK_MANAGER_WORKING_STEALING_H 

#define TASK_MANAGER
//#define FIBONACCI


#include "task.h"
#include "notification_queue.h"
#include "wait.h"

#include <pthread.h>
#include <stdatomic.h>
#include <stdint.h>

typedef struct{

  pthread_t* t_arr;
  size_t len_thr;
  
  atomic_uint_fast64_t index;

  not_q_t* q_arr;

  atomic_uint_fast64_t num_task;

  pthread_cond_t  wait_cv; 
  pthread_mutex_t wait_mtx;
  _Atomic bool waiting;

} task_manager_t;

void init_task_manager(task_manager_t* man, uint32_t num_threads);

void free_task_manager(task_manager_t* man, void (*clean)(void* args) );

void async_task_manager(task_manager_t* man, task_t t);

void async_batch_task_manager(task_manager_t* man, batch_task_t const* b);

void wait_all_task_manager(task_manager_t* man);

void wake_spin_task_manager(task_manager_t* man);

void stop_spin_manager(task_manager_t* man);

#endif

