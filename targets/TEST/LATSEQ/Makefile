OPENAIR_DIR 	= $(shell pwd)/../../..
OPENAIR2_COMMON = $(OPENAIR_DIR)/openair2/COMMON
UTILS_DIR 	= $(OPENAIR_DIR)/common/utils
T_TRACER_DIR 	= $(UTILS_DIR)/T
LATSEQ_DIR	= $(UTILS_DIR)/LATSEQ
MEAS_DIR        = $(OPENAIR_DIR)/openair1/PHY/TOOLS
#CFLAGS += -m32 -DPHYSIM -DNB_ANTENNAS_RX=2 -DNB_ANTENNAS_TX=2 -I/usr/include/X11
#CFLAGS += -I/usr/include/libxml2 -L/usr/local/lib -I/usr/include/atlas -L/usr/X11R6/lib 
CFLAGS += -O3
CFLAGS += -DLATSEQ
CFLAGS += -DLATSEQ_DEBUG
CFLAGS += -I$(OPENAIR_DIR) -I$(OPENAIR2_COMMON) -I$(UTILS_DIR) -I$(T_TRACER_DIR) -I$(LATSEQ_DIR)  -I$(MEAS_DIR)

VPATH += $(LATSEQ_DIR)
VPATH += $(MEAS_DIR)

all: test_latseq

test_latseq: LDLIBS += -lpthread
test_latseq: test_latseq.o latseq.o time_meas.o

run: test_latseq
	@echo "Run test_latseq"
	./test_latseq

clean:
	rm -f test_latseq
	rm -f *.o
	rm -f *.lseq
