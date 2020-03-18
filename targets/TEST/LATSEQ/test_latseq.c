#include <stdio.h>
#include <unistd.h>

//#ifdef LATSEQ
#include "common/utils/LATSEQ/latseq.h"
//#endif

const char * test_log = "test.lseq";

void print_usage(void) {

}

int main (int argc, char **argv) {
  #ifdef LATSEQ
    printf("Call Latseq...\n");
  #endif
    init_latseq(test_log, 0);
    sleep(1);
    close_latseq();
  //#endif
  
  return 0;
}