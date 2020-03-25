#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

//#ifdef LATSEQ
#include "latseq.h"
//#endif

int debug_enabled = 0;
#ifdef LATSEQ_DEBUG
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
  printf("t \t: test_multi_thread() \t: test multi-producers in different thread case\n");
  printf("m \t: measure_log_measure() \t: measure time took by log_measure\n");
  printf("n \t: measure_log_n() \t: measure time took by log_measure with n varargs\n");
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

int measure_log_measure()
{
  oai_exit = 0;
  printf("[TEST] %s\n",__func__);
  if(!init_latseq(test_log, debug_enabled)) {
    printf("[ERROR] : init_latseq()\n");
    exit(EXIT_FAILURE);
  }
#ifdef TEST_LATSEQ
  struct timeval begin, end;
  gettimeofday(&begin, NULL);
#endif
  const uint32_t num_call = 1000000;
  for (int i = 0; i < num_call; i++)
  {
    LATSEQ_P("meas", "call.%d", i);
  }

#ifdef TEST_LATSEQ
  gettimeofday(&end, NULL);
#endif
  sleep(1);
  
  if(!close_latseq()) {
    printf("[ERROR] : close_latseq()\n");
    exit(EXIT_FAILURE);
  }
#ifdef TEST_LATSEQ
  printf("[LATSEQ] %d log_measure took : %lu us\n", num_call, (end.tv_usec - begin.tv_usec)); //at 23-03, 0.0328usec
#endif

  return 0;
}

int measure_log_n() 
{
  oai_exit = 0;
  printf("[TEST] %s\n",__func__);
  if(!init_latseq(test_log, debug_enabled)) {
    printf("[ERROR] : init_latseq()\n");
    exit(EXIT_FAILURE);
  }
#ifdef TEST_LATSEQ
  struct timeval begin, end;
  gettimeofday(&begin, NULL);
  long t1, t2, t3, t5, t10;
#endif
  const uint32_t num_call = 1000;
  int i;
  //test n=1
  for (i = 0; i < num_call; i++)
  {
    LATSEQ_P("meas1", "call.%d", i);
  }
#ifdef TEST_LATSEQ
  gettimeofday(&end, NULL);
  t1 = end.tv_usec - begin.tv_usec;
  gettimeofday(&begin, NULL);
#endif
  //test n=2
  for (i = 0; i < num_call; i++)
  {
    LATSEQ_P("meas2", "call.%d.%d", i,0);
  }
#ifdef TEST_LATSEQ
  gettimeofday(&end, NULL);
  t2 = end.tv_usec - begin.tv_usec;
  gettimeofday(&begin, NULL);
#endif
  //test n=3
  for (i = 0; i < num_call; i++)
  {
    LATSEQ_P("meas3", "call.%d.%d.%d", i,0,1);
  }
#ifdef TEST_LATSEQ
  gettimeofday(&end, NULL);
  t3 = end.tv_usec - begin.tv_usec;
  gettimeofday(&begin, NULL);
#endif
  //test n=5
  for (i = 0; i < num_call; i++)
  {
    LATSEQ_P("meas3", "call.%d.%d.%d.%d.%d", i,0,1,2,3);
  }
#ifdef TEST_LATSEQ
  gettimeofday(&end, NULL);
  t5 = end.tv_usec - begin.tv_usec;
  gettimeofday(&begin, NULL);
#endif
  //test n=10 (max given by NB_DATA_IDENTIFIERS)
  for (i = 0; i < num_call; i++)
  {
    LATSEQ_P("meas4", "call.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d",i,0,1,2,3,4,5,7,8,9);
  }
#ifdef TEST_LATSEQ
  gettimeofday(&end, NULL);
  t10 = end.tv_usec - begin.tv_usec;
#endif
  sleep(1);
  
  if(!close_latseq()) {
    printf("[ERROR] : close_latseq()\n");
    exit(EXIT_FAILURE);
  }
#ifdef TEST_LATSEQ
  printf("[LATSEQ] log_measure took :\n"); //at 23-03, 32.8ns
  printf("\tvar_args=1 : %.1f ns/call\n", 1000*(double)t1/num_call); // mean : 19ns at 23-03
  printf("\tvar_args=2 : %.1f ns/call\n", 1000*(double)t2/num_call); // mean : 22ns at 23-03
  printf("\tvar_args=3 : %.1f ns/call\n", 1000*(double)t3/num_call); // mean : 25ns at 23-03
  printf("\tvar_args=5 : %.1f ns/call\n", 1000*(double)t5/num_call); // mean : 32ns at 23-03
  printf("\tvar_args=10 : %.1f ns/call\n", 1000*(double)t10/num_call); // mean : 61ns at 23-03
#endif
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

  case 't':
    (void)test_multithread();
    break;
  
  case 'm':
    (void)measure_log_measure();
    break;

  case 'n':
    (void)measure_log_n();
    break;
  
  default:
    print_usage();
    break;
  }
  
  //#endif
  oai_exit = 1;
  return 0;
}
