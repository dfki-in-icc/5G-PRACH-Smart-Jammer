#include $(OPENAIR_DIR)/common/utils/Makefile.inc
LATSEQ_TEST_DIR = $(shell pwd)
OPENAIR_DIR 	= $(LATSEQ_TEST_DIR)/../../..
TOP_DIR      	= $(OPENAIR_DIR)
OPENAIR1_TOP 	= $(OPENAIR1_DIR)
OPENAIR2_TOP 	= $(OPENAIR2_DIR)
OPENAIR3_TOP 	= $(OPENAIR3_DIR)
OPENAIR3     	= $(OPENAIR3_DIR)
COMMON_DIR 		= $(OPENAIR2_DIR)/common
UTILS_DIR		= $(COMMON_DIR)/utils
T_TRACER_DIR 	= $(UTILS_DIR)/T
LATSEQ_DIR		= $(UTILS_DIR)/LATSEQ


CFLAGS += -m32 -DPHYSIM -DNB_ANTENNAS_RX=2 -DNB_ANTENNAS_TX=2 -I/usr/include/X11
CFLAGS += -I/usr/include/libxml2 -L/usr/local/lib -I/usr/include/atlas -L/usr/X11R6/lib 
CFLAGS += -DLATSEQ
CFLAGS += -DLATSEQ_DEBUG

TEST_LATSEQ_OBJS = test_latseq.o

OBJ=$(LATSEQ_DIR)/latseq.o

all: test_latseq

# La compilation de latseq.o ne fonctionne pas ! "no rule to make target"
$(OBJ): %.o : %.c
	@echo Compiling $<
	$(CC) -c -I$(LATSEQ_DIR) -I$(OPENAIR_DIR) -I$(UTILS_DIR) -I$(T_TRACER_DIR) -o $@ $<

test_latseq: $(OBJ) test_latseq.c  
	@echo "Compiling test_latseq.c ..."
	@$(CC)  test_latseq.c -I$(LATSEQ_DIR) -I$(OPENAIR_DIR) -I$(T_TRACER_DIR) -o test_latseq $(OBJ) -lpthread

run: test_latseq
	./test_latseq

clean:
	rm -f test_latseq
	rm -f $(OBJ)
	rm -f *.o
	rm -f *.lseq

# $(CFLAGS) $(OBJ) -lm  -lblas -lpthread -lxml2 -lX11 -lXpm -lrt  -lforms -llapack_atlas