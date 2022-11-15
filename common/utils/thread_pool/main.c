/*
MIT License

Copyright (c) 2022 Mikel Irazabal

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "task_manager.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>

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
  int64_t* sol;
} pair_t;

static __thread
int acc = 0;

static
void do_work(void* arg)
{
  pair_t* a = (pair_t*)arg;
  assert(a->a < 10);
  naive_fibonnacci(19 + a->a);

  int64_t elapsed = time_now_us() - a->time;
  if((acc % 4096) == 0)
    printf("elpsed time = %ld \n", elapsed );
  acc += 1;
  *a->sol = elapsed;
  free(a);
}

void clean_func(void* arg)
{
  task_t* t = (task_t*)arg;
  free((pair_t*)t->args);
}

int main()
{
  task_manager_t man;
  init_task_manager(&man, 1);

  const int64_t last = time_now_us() ;

  int64_t* elapsed = calloc(8*1024*1024, sizeof(int64_t));
  assert(elapsed != NULL);

  for(int i = 0; i < 8*1024*1024; ++i){
    pair_t* pa = malloc(sizeof(pair_t));
    pa->a = 0; //i%10;
    pa->time = last;
    pa->sol = &elapsed[i];
    task_t t = {.args = pa, t.func = do_work};
    async_task_manager(&man, t);
  }

//  sleep(15);
  wait_all_task_manager(&man); // sleep(30); 

  free_task_manager(&man, clean_func);

  return EXIT_SUCCESS;
}

