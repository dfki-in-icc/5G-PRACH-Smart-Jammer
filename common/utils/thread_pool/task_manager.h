#ifndef TASK_MANAGER_WORKING_STEALING_H
#define TASK_MANAGER_WORKING_STEALING_H 

#define TASK_MANAGER
//#define FIBONACCI
//#define OMP_TP 1

#include "task.h"
#include "notification_queue.h"

#include <pthread.h>
#include <stdatomic.h>
#include <stdint.h>

typedef struct {

  pthread_t* t_arr;
  size_t len_thr;
  
  atomic_uint_fast64_t index;

  not_q_t* q_arr;

  atomic_int_fast64_t num_task;

  pthread_cond_t wait_cv; 
  pthread_mutex_t wait_mtx;

//  spinlock_t spin;
  _Atomic int waiting; // 1 cv, 2 spin
                       //
  //_Atomic bool waiting;
} task_manager_t;

void init_task_manager(task_manager_t* man, uint32_t num_threads);

void free_task_manager(task_manager_t* man, void (*clean)(task_t* args) );

void async_task_manager(task_manager_t* man, task_t t);

void wait_all_task_manager(task_manager_t* man);

void wait_all_spin_task_manager(task_manager_t* man);

void wake_and_spin_task_manager(task_manager_t* man);

void stop_spin_task_manager(task_manager_t* man);

#endif

