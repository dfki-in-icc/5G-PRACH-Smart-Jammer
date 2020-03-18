#include <stdio.h>
#include <unistd.h>

//#ifdef LATSEQ
#include "latseq.h"
//#endif

double cpuf;
const char * test_log = "test.lseq";

void oai_exit(int code) {
  exit(code);
}

void print_usage(void) {
  printf("help test_latseq\n");
  printf("h \t: Help\n");
  printf("a \t: test_full() \t: a full unit test\n");
  printf("i \t: test_init_and_close() \t: test a simple init/close case\n");
}

int test_init_and_close() {
  printf("[TEST] %s\n",__func__);
  if(!init_latseq(test_log, 0)) {
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

int test_full() {
  printf("[TEST] %s\n",__func__);
  if(!init_latseq(test_log, 0)) {
    printf("[ERROR] : init_latseq()\n");
    exit(EXIT_FAILURE);
  }
  //Do stuff here
  if(!close_latseq()) {
    printf("[ERROR] : close_latseq()\n");
    exit(EXIT_FAILURE);
  }
  return 0;
}

int main (int argc, char **argv) {
  #ifdef LATSEQ
    printf("Call Latseq...\n");
  #endif
  if (argc < 2) {
    print_usage();
    exit(EXIT_FAILURE);
  }
  char opt = (char)argv[2];
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
  
  default:
    print_usage();
    break;
  }
  
  //#endif
  
  return 0;
}
