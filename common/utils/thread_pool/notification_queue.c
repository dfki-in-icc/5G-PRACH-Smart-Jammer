#include "notification_queue.h"

#include <assert.h>
#include <errno.h>
#include <string.h>

void init_not_q(not_q_t* q)
{
  assert(q != NULL);

  q->done = 0;
  init_seq_ring_task(&q->r);

  pthread_mutexattr_t attr = {0};
#ifdef _DEBUG
  int const rc_mtx = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);
  assert(rc_mtx == 0);
#endif
  int rc = pthread_mutex_init(&q->mtx, &attr);
  assert(rc == 0 && "Error while creating the mtx");

  pthread_condattr_t* c_attr = NULL; 
  rc = pthread_cond_init(&q->cv, c_attr);
  assert(rc == 0);
  q->spin = false;

  q->sl.lock = false;
}

void free_not_q(not_q_t* q, void (*clean)(task_t*) )
{
  assert(q != NULL);
  assert(q->done == 1);

  free_seq_ring_task(&q->r, clean);

  int rc = pthread_mutex_destroy(&q->mtx);
  assert(rc == 0);

  rc = pthread_cond_destroy(&q->cv);
  assert(rc == 0);
}

bool try_push_not_q(not_q_t* q, task_t t)
{
  assert(q != NULL);
  assert(q->done == 0 || q->done ==1);
  assert(t.func != NULL);
  assert(t.args != NULL);

  if(pthread_mutex_trylock(&q->mtx ) != 0)
    return false;

  push_back_seq_ring_task(&q->r, t);

  int rc = pthread_mutex_unlock(&q->mtx);
  assert(rc == 0);

  pthread_cond_signal(&q->cv);

  return true;
}

void push_not_q(not_q_t* q, task_t t)
{
  assert(q != NULL);
  assert(q->done == 0 || q->done ==1);
  assert(t.func != NULL);
  //assert(t.args != NULL);

  int rc = pthread_mutex_lock(&q->mtx);
  assert(rc == 0);

  push_back_seq_ring_task(&q->r, t);

  pthread_mutex_unlock(&q->mtx);

  pthread_cond_signal(&q->cv);
}

ret_try_t try_pop_not_q(not_q_t* q)
{
  assert(q != NULL);

  ret_try_t ret = {.success = false}; 

  int rc = pthread_mutex_trylock(&q->mtx);
  assert(rc == 0 || rc == EBUSY);

  if(rc == EBUSY)
    return ret;

  assert(q->done == 0 || q->done ==1);

  size_t sz = size_seq_ring_task(&q->r); 
  if(sz == 0){
    rc = pthread_mutex_unlock(&q->mtx);
    assert(rc == 0);

    return ret;
  }

  ret.t = pop_seq_ring_task(&q->r);

  rc = pthread_mutex_unlock(&q->mtx);
  assert(rc == 0);
  ret.success = true; 

  return ret;
}

bool pop_not_q(not_q_t* q, ret_try_t* out)
{
  assert(q != NULL);
  assert(out != NULL);

  int rc = pthread_mutex_lock(&q->mtx);
  assert(rc == 0);
  assert(q->done == 0 || q->done ==1);

label:
  while(size_seq_ring_task(&q->r) == 0 && q->done == 0 && q->spin == false)
    pthread_cond_wait(&q->cv , &q->mtx);

  if(q->done == 1){
    int const rc = pthread_mutex_unlock(&q->mtx);
    assert(rc == 0);
    return false;
  }

  if(q->spin){
    int rc = pthread_mutex_unlock(&q->mtx);
    assert(rc == 0);

    lock_spinlock(&q->sl);
    unlock_spinlock(&q->sl);

    rc = pthread_mutex_lock(&q->mtx);
    assert(rc == 0);
  }

  if(size_seq_ring_task(&q->r) == 0)
    goto label;

  out->t = pop_seq_ring_task(&q->r);

  rc = pthread_mutex_unlock(&q->mtx);
  assert(rc == 0);

  return true;
}

void done_not_q(not_q_t* q)
{
  assert(q != NULL);

  int rc = pthread_mutex_lock(&q->mtx);
  assert(rc == 0);
  
  q->done = 1;

  rc = pthread_mutex_unlock(&q->mtx);
  assert(rc == 0);

  rc = pthread_cond_signal(&q->cv);
  assert(rc == 0);
}

void wake_spin_not_q(not_q_t* q)
{
  assert(q != NULL);


  lock_spinlock(&q->sl);

  int rc = pthread_mutex_lock(&q->mtx);
  assert(rc == 0);
 
  assert(q->spin == false);
  q->spin = true;

  rc = pthread_mutex_unlock(&q->mtx);
  assert(rc == 0);


  pthread_cond_signal(&q->cv);
}

void stop_spin_not_q(not_q_t* q)
{
  assert(q != NULL);

  int rc = pthread_mutex_lock(&q->mtx);
  assert(rc == 0);
 
  assert(q->spin == true);
  q->spin = false;

  rc = pthread_mutex_unlock(&q->mtx);
  assert(rc == 0);

  unlock_spinlock(&q->sl);
}

