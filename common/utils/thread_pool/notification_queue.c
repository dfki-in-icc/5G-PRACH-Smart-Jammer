#include "notification_queue.h"

#include <assert.h>
#include <errno.h>
#include <string.h>

#include "spinlock.h"

#include <stdio.h>

static
int64_t time_now_us(void)
{
  struct timespec tms;

  /* The C11 way */
  /* if (! timespec_get(&tms, TIME_UTC))  */

  /* POSIX.1-2008 way */
  if (clock_gettime(CLOCK_REALTIME,&tms)) {
    return -1;
  }
  /* seconds, multiplied with 1 million */
  int64_t micros = tms.tv_sec * 1000000;
  /* Add full microseconds */
  micros += tms.tv_nsec/1000;
  /* round up if necessary */
  if (tms.tv_nsec % 1000 >= 500) {
    ++micros;
  }
  return micros;
}






void init_not_q(not_q_t* q)
{
  assert(q != NULL);

  q->done = 0;
  seq_ring_init(&q->r, sizeof(task_t));

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
}

void free_not_q(not_q_t* q, void (*clean)(void*) )
{
  assert(q != NULL);
  assert(q->done == 1);

  seq_ring_free(&q->r, clean);

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

  seq_ring_push_back(&q->r, (uint8_t*)&t, sizeof(task_t));

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

  seq_ring_push_back(&q->r, (void*)&t, sizeof(task_t));

  pthread_mutex_unlock(&q->mtx);

  pthread_cond_signal(&q->cv);
}

void push_batch_not_q(not_q_t* q, size_t len, task_t const t[len])
{
  assert(q != NULL);
  assert(q->done == 0 || q->done ==1);
  assert(len != 0);
  //assert(t.args != NULL);

  int rc = pthread_mutex_lock(&q->mtx);
  assert(rc == 0);

  for(uint32_t i = 0; i < len; ++i){
    seq_ring_push_back(&q->r, (void*)(&t[i]), sizeof(task_t));
  }

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

  size_t sz = seq_ring_size(&q->r); 
  if(sz == 0){
    rc = pthread_mutex_unlock(&q->mtx);
    assert(rc == 0);

    return ret;
  }

  void* it = seq_ring_at(&q->r, 0);
  assert(it != seq_ring_end(&q->r));
  memcpy(&ret.t, it, sizeof(task_t)); 
  void* next = seq_ring_next(&q->r, it);

  seq_ring_erase(&q->r, it, next);

  assert(sz == 1 + seq_ring_size(&q->r)); 

  rc = pthread_mutex_unlock(&q->mtx);
  assert(rc == 0);
  ret.success = true; 

  return ret;
}

bool pop_not_q(not_q_t* q, ret_try_t* out)
{
  assert(q != NULL);
  assert(out != NULL);

  pthread_mutex_lock(&q->mtx);
  assert(q->done == 0 || q->done ==1);

label:
  while(seq_ring_size(&q->r) == 0 && q->done == 0 && q->spin == false)
    pthread_cond_wait(&q->cv, &q->mtx);

  if(q->done == 1){
    int rc = pthread_mutex_unlock(&q->mtx);
    assert(rc == 0);
    return false;
  }

  if(q->spin){
    int rc = pthread_mutex_unlock(&q->mtx);
    assert(rc == 0);

    lock_spinlock(&q->sl);
    
    pthread_mutex_lock(&q->mtx);
    if(seq_ring_size(&q->r) == 0){
      goto label;
    } 
  }

//  printf("Thread id %ld waked up at %ld\n", pthread_self(), time_now_us());

  void* it = seq_ring_at(&q->r, 0);
  assert(it != seq_ring_end(&q->r));
  memcpy(out, it, sizeof(task_t)); 
  void* next = seq_ring_next(&q->r, it);
  seq_ring_erase(&q->r, it, next);

  int rc = pthread_mutex_unlock(&q->mtx);
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
  q->spin = true;
  pthread_cond_signal(&q->cv);
}

void stop_spin_not_q(not_q_t* q)
{
  assert(q != NULL);
  q->spin = false;
  unlock_spinlock(&q->sl);
}


