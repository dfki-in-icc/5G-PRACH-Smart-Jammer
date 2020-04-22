TEST_DIR	= $(shell pwd)
OPENAIR_DIR 	= $(TEST_DIR)/../../..
OPENAIR2_COMMON = $(OPENAIR_DIR)/openair2/COMMON
UTILS_DIR 	= $(OPENAIR_DIR)/common/utils
T_TRACER_DIR 	= $(UTILS_DIR)/T
LATSEQ_DIR	= $(UTILS_DIR)/LATSEQ

CC = gcc

#CFLAGS += -m32 -DPHYSIM -DNB_ANTENNAS_RX=2 -DNB_ANTENNAS_TX=2 -I/usr/include/X11
#CFLAGS += -I/usr/include/libxml2 -L/usr/local/lib -I/usr/include/atlas -L/u99
CFLAGS += -g
CFLAGS += -Wall
CFLAGS += -O3
#CFLAGS += -I$(OPENAIR_DIR) -I$(OPENAIR2_COMMON) -I$(UTILS_DIR) -I$(T_TRACER_DIR) -I$(LATSEQ_DIR)  -I$(MEAS_DIR)

LDLIBS = -lpthread

VPATH += $(LATSEQ_DIR)
VPATH += $(MEAS_DIR)

EXE = latseq_checker
SRCS = latseq_checker.c
OBJS = $(SRCS:.c=.o)

all: $(EXE)

latseq_checker: $(OBJS)
	@echo "Compiling latseq_checker"
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDLIBS)

run: $(EXE)
	@echo "Run test_latseq"
	./latseq_checker

clean:
	rm -f core gmon.out $(EXE) *.o
