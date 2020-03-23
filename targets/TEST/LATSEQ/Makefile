TEST_DIR	= $(shell pwd)
OPENAIR_DIR 	= $(TEST_DIR)/../../..
OPENAIR2_COMMON = $(OPENAIR_DIR)/openair2/COMMON
UTILS_DIR 	= $(OPENAIR_DIR)/common/utils
T_TRACER_DIR 	= $(UTILS_DIR)/T
LATSEQ_DIR	= $(UTILS_DIR)/LATSEQ
MEAS_DIR        = $(OPENAIR_DIR)/openair1/PHY/TOOLS

CC = gcc

#CFLAGS += -m32 -DPHYSIM -DNB_ANTENNAS_RX=2 -DNB_ANTENNAS_TX=2 -I/usr/include/X11
#CFLAGS += -I/usr/include/libxml2 -L/usr/local/lib -I/usr/include/atlas -L/usr/X11R6/lib 
CFLAGS += -std=gnu99 #TODO : to be compiled in c99 like all oai
CFLAGS += -Wall -Wconversion
CFLAGS += -g
#CFLAGS += -O3
CFLAGS += -DLATSEQ
#CFLAGS += -DLATSEQ_DEBUG
CFLAGS += -DTEST_LATSEQ
CFLAGS += -I$(OPENAIR_DIR) -I$(OPENAIR2_COMMON) -I$(UTILS_DIR) -I$(T_TRACER_DIR) -I$(LATSEQ_DIR)  -I$(MEAS_DIR)

LDLIBS = -lpthread

VPATH += $(LATSEQ_DIR)
VPATH += $(MEAS_DIR)

EXE = test_latseq
SRCS = test_latseq.c latseq.c time_meas.c
OBJS = $(SRCS:.c=.o)

all: $(EXE)

$(EXE): $(OBJS)
	@echo "Compiling test_latseq"
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDLIBS)

run: $(EXE)
	@echo "Run test_latseq"
	./test_latseq a

run-parsing: $(LATSEQ_DIR)/lseq_stats/lseqlogs.py
	cd $(LATSEQ_DIR)/lseq_stats/;\
	      	./lseqlogs.py -l $(TEST_DIR)/test1.lseq

clean:
	rm -f core $(EXE) *.o
