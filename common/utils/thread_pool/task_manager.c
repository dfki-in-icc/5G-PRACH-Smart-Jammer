#include "task_manager.h"

#include <assert.h> 
#include <stdlib.h>

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

    //int64_t now = time_now_us();
    //printf("Before func %ld id %lu \n", now , pthread_self() );
    ret.t.func(ret.t.args); 
    //printf("After func %ld id %lu elapsed %ld \n", time_now_us(), pthread_self(), time_now_us()-now );

    atomic_fetch_sub(&man->num_task, 1); 
    if(man->num_task < 1 && man->waiting != 0){

        pthread_mutex_lock(&man->wait_mtx);
       
        man->waiting = 0;
        pthread_cond_signal(&man->wait_cv); 
        
        pthread_mutex_unlock(&man->wait_mtx);
    }

//    if(man->num_task < 1 && man->waiting != 0){
//      man->waiting == 1 ? pthread_cond_signal(&man->wait_cv) : unlock_spinlock(&man->spin);
//    }

  }

  free(args);
  return NULL;
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

  man->index = 0;

  // Waiting all
  man->waiting = 0;

  pthread_mutexattr_t attr = {0};
#ifdef _DEBUG
  int const rc_mtx = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);
  assert(rc_mtx == 0);
#endif
  int rc = pthread_mutex_init(&man->wait_mtx, &attr);
  assert(rc == 0 && "Error while creating the mtx");

  man->spin.lock = false;

  pthread_condattr_t* c_attr = NULL; 
  rc = pthread_cond_init(&man->wait_cv, c_attr);
  assert(rc == 0);

  man->t_arr = calloc(num_threads, sizeof(pthread_t));
  assert(man->t_arr != NULL && "Memory exhausted" );
  man->len_thr = num_threads;

  for(uint32_t i = 0; i < num_threads; ++i){
    task_thread_args_t* args = malloc(sizeof(task_thread_args_t) ); 
    args->idx = i;
    args->man = man;

    int rc = pthread_create(&man->t_arr[i], NULL, worker_thread, args);
    assert(rc == 0);
  }

}

void free_task_manager(task_manager_t* man, void (*clean)(task_t*))
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
  const int32_t len_thr = man->len_thr;
  for(int32_t i = 0; i < len_thr ; ++i){
    if(try_push_not_q(&man->q_arr[(i+index) % len_thr], t)){
      atomic_fetch_add(&man->num_task, 1); 
      return;
    }
  }

  push_not_q(&man->q_arr[index%len_thr], t);
  atomic_fetch_add(&man->num_task, 1); 
}

void wait_all_task_manager(task_manager_t* man)
{
  assert(man != NULL);

  pthread_mutex_lock(&man->wait_mtx);
  man->waiting = 1;

  while(man->num_task > 0 && man->waiting != 0) 
    pthread_cond_wait(&man->wait_cv, &man->wait_mtx);

  pthread_mutex_unlock(&man->wait_mtx);
}

void wait_all_spin_task_manager(task_manager_t* man)
{
  assert(man != NULL);


  while(man->num_task > 0)
    sched_yield();

  /*
  if(man->num_task < 1)
    return;

  man->waiting = 2;
  lock_spinlock(&man->spin);
  lock_spinlock(&man->spin);
  man->waiting = 0;
  unlock_spinlock(&man->spin);
*/

}

void wake_and_spin_task_manager(task_manager_t* man)
{
  assert(man != NULL);

  const int32_t len_thr = man->len_thr;
  for(int i = 0; i < len_thr; ++i){
     wake_spin_not_q(&man->q_arr[i]);
  }
}

void stop_spin_task_manager(task_manager_t* man)
{
  assert(man != NULL);

  const int32_t len_thr = man->len_thr;
  for(int i =0; i < len_thr; ++i){
    stop_spin_not_q(&man->q_arr[i]);
  }
}

