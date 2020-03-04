LATency SEQuence framework is used to extract statistics about latency (with jitter,...) in 5G-NR RAN.

Usage
0) Add a new measure point with
#include "common/utils/LATSEQ/latseq.h"
#if LATSEQ
LATSEQ_P("pdcp....", "pdcp0.rlc1....")  
#endif
1) Run OAI code with option --enable-latseq (LATSEQ)
2) Run scanario for Uplink and Downlink
3) Process logs to yield data
4) Do statistics

TODO List
- harmonize string output qwith the rest of code (printf,...)
- change measurement points to do not have to prepare string identifier in the main code, should be produce in the log_measure() instead
- core logger
- measure points
- process data
