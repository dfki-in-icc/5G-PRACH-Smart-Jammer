#ifndef NOTIFICATION_QUEUE_THREAD_POOL_H
#define NOTIFICATION_QUEUE_THREAD_POOL_H 

#include "task.h"
#include "seq_ring_task.h"

#include <pthread.h>
#include <stdbool.h>

typedef struct {
  pthread_mutex_t mtx;
  pthread_cond_t cv;
  seq_ring_task_t r;
  int done;

  _Atomic bool spin;
} not_q_t;

typedef struct{
  task_t t;
  bool success;
} ret_try_t;

void init_not_q(not_q_t* q);

void free_not_q(not_q_t* q, void (*clean)(task_t*) );

bool try_push_not_q(not_q_t* q, task_t t);

void push_not_q(not_q_t* q, task_t t);

ret_try_t try_pop_not_q(not_q_t* q);

bool pop_not_q(not_q_t* q, ret_try_t* out);

void done_not_q(not_q_t* q);

void wake_spin_not_q(not_q_t* q);

void stop_spin_not_q(not_q_t* q);


#endif

