#include <assert.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <omp.h>

  static inline
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
  int64_t const tv_nsec = tms.tv_nsec;
  micros += tv_nsec/1000;
  /* round up if necessary */
  if (tv_nsec % 1000 >= 500) {
    ++micros;
  }
  return micros;
}

static inline
int64_t naive_fibonnacci(int64_t a)
{
  if(a < 2)
    return a;
  
  return naive_fibonnacci(a-1) + naive_fibonnacci(a-2);
}

typedef struct{
  int64_t a;
  int64_t time;
} pair_t;

static __thread
int acc = 0;

static
void do_work(void* arg)
{
  pair_t* a = (pair_t*)arg;
  assert(a->a < 10);
  //naive_fibonnacci(13 + a->a);
  naive_fibonnacci(19 + a->a);

  if((acc % 4096) == 0)
    printf("%ld \n", time_now_us() - a->time);
  acc += 1;

  free(a);
}

int main()
{

  omp_set_num_threads(8);

  const int64_t last = time_now_us() ;
  #pragma omp parallel
  {
#pragma omp single
    {
  for(int i =0; i < 8*1024*1024; ++i){
    pair_t* pa = malloc(sizeof(pair_t));
    pa->a = 0; //i%10;
    pa->time = last;
 
    #pragma omp task
    {
    do_work(pa); 
    }
  }
    }
  }
  sleep(15);

  return 0;
}

