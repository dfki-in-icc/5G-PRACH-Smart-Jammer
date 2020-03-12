LATency SEQuence framework is used to extract statistics about latency (with jitter,...) in 5G-NR RAN.*

For now, latseq is designed to be the more indenpendant as possible : Means that it does not use oai LOG system (not register by logInit()) and the flag "LATSEQ" disable all lines related to latseq in the code (using #ifdef). In a second time, it could be conceivable to integrate more deeply latseq into oai code.

latseq_t, global structure for latseq embodied the latseq logging info. log_buffer is a circular buffer with 2 head index, i_write_head and i_read_head. this buffer of latseq_element_t is designed to bo mutex-less.

LATSEQ_P macro calls log_measure(). The idea is to have a low-footprint at logging explains why log_measure() should do a minimal amount of operations.

latseq_log_to_file() is the function run in the logger thread. It writes log_elements in the log file.

ASSUMPTIONS
- All the point and latseq module run on the same machine (to don't have to synchronize clock of different machines)
- Clock give by asm rdtsc is same for all the CPU cores (constant_tsc)

USAGE
0) Add a new measure point in the code with
#include "common/utils/LATSEQ/latseq.h"
#if LATSEQ
LATSEQ_P("pdcp....", "pdcp0.rlc1....")  
#endif
where first argument is the name of the point and the seconde argument is a string of data_identifier
1) Compile OAI code with option --enable-latseq (LATSEQ)
2) Run scanario for Uplink and Downlink
3) Process logs (as defined ) to yield data
4) Do statistics

TODO
- do the list of point
- do the list of useful idenfier
- harmonize string output with the rest of code (printf,...)
- check size of MAX_LOG_SIZE. 1024 might be not enough space for buffer
- check problem of massive multi-threaded OAI
- Change order to increment head because we skip the first cell (0)
- handle the end of local oai thread
