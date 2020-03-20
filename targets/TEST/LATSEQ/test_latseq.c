#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

//#ifdef LATSEQ
#include "latseq.h"
//#endif

int debug_enabled = 0;
#ifdef DLATSEQ
  int debug_enabled = 1;
#endif

double cpuf;
const char * test_log = "test1.lseq";

volatile int  oai_exit = 1; //Emulate global variable used by oai to indicate that oai is running

void print_usage(void) 
{
  printf("help test_latseq\n");
  printf("h \t: Help\n");
  printf("a \t: test_full() \t: a full unit test\n");
  printf("i \t: test_init_and_close() \t: test a simple init/close case\n");
  printf("m \t: test_multi_thread() \t: test multi-producers in different thread case\n");
}

int test_init_and_close() 
{
  oai_exit = 0;
  printf("[TEST] %s\n",__func__);
  if(!init_latseq(test_log, debug_enabled)) {
    printf("[ERROR] : init_latseq()\n");
    exit(EXIT_FAILURE);
  }
  sleep(1);
  if(!close_latseq()) {
    printf("[ERROR] : close_latseq()\n");
    exit(EXIT_FAILURE);
  }

  return 0;
}

int test_full() 
{
  oai_exit = 0;
  printf("[TEST] %s\n",__func__);
  if(!init_latseq(test_log, debug_enabled)) {
    printf("[ERROR] : init_latseq()\n");
    exit(EXIT_FAILURE);
  }
  LATSEQ_P("full3 D", "ip%d", 0);
  sleep(1);
  LATSEQ_P("full2 D", "ip%d.mac%d", 0, 1);
  if(!close_latseq()) {
    printf("[ERROR] : close_latseq()\n");
    exit(EXIT_FAILURE);
  }
  return 0;
}

void thread_test1(void)
{
  pthread_t thId = pthread_self();
  printf("[TEST] [%d] thread started\n");
  int i = 0;
  while(!oai_exit) {
    if (!i) {
      LATSEQ_P("full3 D", "ip%d", 0);
      usleep(25000);
      LATSEQ_P("full2 D", "ip%d.mac%d", 0, 1);
      i = 1;
      continue;
    }
  }
  pthread_exit(NULL);
}

void thread_test2(void)
{
  pthread_t thId = pthread_self();
  printf("[TEST] [%d] thread started\n");
  int i = 0;
  while(!oai_exit) {
    if (!i) {
      LATSEQ_P("full3 D", "ip%d", 1);
      usleep(1000);
      LATSEQ_P("full2 D", "ip%d.mac%d", 1, 1);
      usleep(9000);
      LATSEQ_P("full1 D", "ip%d.mac%d.phy%d", 1, 1, 4);
      i = 1;
      continue;
    }
  }
  pthread_exit(NULL);
}

int test_multithread() 
{
  oai_exit = 0;
  printf("[TEST] %s\n",__func__);
  if(!init_latseq(test_log, debug_enabled)) {
    printf("[ERROR] : init_latseq()\n");
    exit(EXIT_FAILURE);
  }
  pthread_t th1;
  pthread_t th2;
  pthread_create(&th1, NULL, (void *) &thread_test1, NULL);
  pthread_create(&th2, NULL, (void *) &thread_test2, NULL);
  sleep(1);
  oai_exit = 1;
  pthread_join(th1, NULL);
  pthread_join(th2, NULL);

  if(!close_latseq()) {
    printf("[ERROR] : close_latseq()\n");
    exit(EXIT_FAILURE);
  }
  return 0;
}

int main (int argc, char **argv) 
{
  #ifdef LATSEQ
    printf("Call Latseq...\n");
  #endif
  if (argc != 2) {
    print_usage();
    exit(EXIT_FAILURE);
  }
  char opt = (char)argv[1][0];
  switch (opt)
  {
  case 'h':
    print_usage();
    break;

  case 'i':
    (void)test_init_and_close();
    break;
  
  case 'a':
    (void)test_full();
    break;

  case 'm':
    (void)test_multithread();
    break;
  
  default:
    print_usage();
    break;
  }
  
  //#endif
  oai_exit = 1;
  return 0;
}
