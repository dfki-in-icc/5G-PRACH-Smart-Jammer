#include "task_manager.h"

#include <assert.h> 
#include <stdlib.h>

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





typedef struct{
  task_manager_t* man;
  int idx;
} task_thread_args_t;

static
void* worker_thread(void* arg)
{
  assert(arg != NULL);

  task_thread_args_t* args = (task_thread_args_t*)arg; 
  int const idx = args->idx;
  task_manager_t* man = args->man;

  uint32_t const len = man->len_thr;
  uint32_t const num_it = 2*(man->len_thr + idx); 

  for(;;){

    ret_try_t ret = {.success = false}; 

    for(uint32_t i = idx; i < num_it; ++i){
      ret = try_pop_not_q(&man->q_arr[i%len]);
      if(ret.success == true){
        break;
      } 
    }

    if(ret.success == false && pop_not_q(&man->q_arr[idx], &ret) == false)
      break;

    int64_t now = time_now_us();
    printf("Before func %ld id %lu \n", now , pthread_self() );
    ret.t.func(ret.t.args); 
    man->num_task -= 1;
    printf("After func %ld id %lu elapsed %ld \n", time_now_us(), pthread_self(), time_now_us()-now );

    if(man->num_task == 0 && man->waiting == true ){
      printf("time now task manager %ld \n ", time_now_us()); 
      pthread_cond_signal(&man->wait_cv);
    }

  }

  free(args);
  return NULL;
}

static
pthread_attr_t get_priotiy_attr(void)
{
  pthread_attr_t attr = {0};

  int rc = pthread_attr_init(&attr);
  assert(rc == 0);

  rc = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
  assert(rc == 0);

  rc = pthread_attr_setschedpolicy(&attr,  SCHED_RR);
  assert(rc == 0);

  int max_prio = sched_get_priority_max(SCHED_RR);
  int min_prio = sched_get_priority_max(SCHED_RR);
  int prio = min_prio + (max_prio - min_prio)/2;

  struct sched_param sparam={0};
  sparam.sched_priority = prio;
  rc = pthread_attr_setschedparam(&attr, &sparam);
  assert(rc == 0);

  return attr;
}


void init_task_manager(task_manager_t* man, uint32_t num_threads)
{
  assert(man != NULL);
  assert(num_threads > 0 && num_threads < 33 && "Do you have zero or more than 32 processors??");

  man->q_arr = calloc(num_threads, sizeof(not_q_t));
  assert(man->q_arr != NULL && "Memory exhausted");
    
  for(uint32_t i = 0; i < num_threads; ++i){
    init_not_q(&man->q_arr[i]);   
  }

  man->t_arr = calloc(num_threads, sizeof(pthread_t));
  assert(man->t_arr != NULL && "Memory exhausted" );
  man->len_thr = num_threads;

  for(uint32_t i = 0; i < num_threads; ++i){
    task_thread_args_t* args = malloc(sizeof(task_thread_args_t) ); 
    args->idx = i;
    args->man = man;

    pthread_attr_t attr = {0}; // get_priotiy_attr();

    int rc = pthread_create(&man->t_arr[i], &attr, worker_thread, args);
    assert(rc == 0);

    rc = pthread_attr_destroy(&attr);
    assert(rc == 0);
  }

  man->index = 0;

  // Waiting all

  man->waiting = false;

  pthread_mutexattr_t attr = {0};
#ifdef _DEBUG
  int const rc_mtx = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);
  assert(rc_mtx == 0);
#endif
  int rc = pthread_mutex_init(&man->wait_mtx, &attr);
  assert(rc == 0 && "Error while creating the mtx");

  pthread_condattr_t* c_attr = NULL; 
  rc = pthread_cond_init(&man->wait_cv, c_attr);
  assert(rc == 0);
}

void free_task_manager(task_manager_t* man, void (*clean)(void*))
{
  for(uint32_t i = 0; i < man->len_thr; ++i){
    done_not_q(&man->q_arr[i]);
  }

  for(uint32_t i = 0; i < man->len_thr; ++i){
    pthread_join(man->t_arr[i], NULL); 
  }

  for(uint32_t i = 0; i < man->len_thr; ++i){
    free_not_q(&man->q_arr[i], clean); 
  }

  free(man->q_arr);

  free(man->t_arr);

  int rc = pthread_mutex_destroy(&man->wait_mtx);
  assert(rc == 0);

  rc = pthread_cond_destroy(&man->wait_cv);
  assert(rc == 0);
}

void async_task_manager(task_manager_t* man, task_t t)
{
  assert(man != NULL);
  assert(t.func != NULL);
  //assert(t.args != NULL);

  uint64_t const index = man->index++;
  uint64_t const len_thr = man->len_thr;
  for(uint32_t i = 0; i < len_thr; ++i){
    if(try_push_not_q(&man->q_arr[(i+index) % len_thr], t)){
      man->num_task +=1;
      return;
    }
  }

  push_not_q(&man->q_arr[index%man->len_thr], t);
  man->num_task +=1;
}

void async_batch_task_manager(task_manager_t* man, batch_task_t const* b)
{
  assert(man != NULL);
  assert(b != NULL);
  assert(b->sz < 17 && "size larger than capacity?");
  assert(b->sz >=man->len_thr);

  uint32_t const len_thr = man->len_thr;
  uint32_t const quot = b->sz / len_thr;
  int64_t rem = b->sz % len_thr; 

  uint32_t idx = 0; 
  for(uint32_t i = 0; i < len_thr; ++i, --rem){
    push_batch_not_q(&man->q_arr[(man->index++)%len_thr], quot + (rem > 0), &b->t[idx]);
    idx += quot + (rem > 0);
  }

  man->num_task += b->sz;
}



void wait_all_task_manager(task_manager_t* man)
{
  assert(man != NULL);

  pthread_mutex_lock(&man->wait_mtx);
  man->waiting = true;

  while(man->num_task != 0) 
    pthread_cond_wait(&man->wait_cv , &man->wait_mtx);

  man->waiting = false;
  pthread_mutex_unlock(&man->wait_mtx);
}

void wake_spin_task_manager(task_manager_t* man)
{
  assert(man != NULL);
  for(int i = 0; i < man->len_thr; ++i){
     wake_spin_not_q(&man->q_arr[i]);
  }
}

void stop_spin_manager(task_manager_t* man)
{
  assert(man != NULL);
  
  for(int i =0; i < man->len_thr; ++i){
    stop_spin_not_q(&man->q_arr[i]);
  }
}

