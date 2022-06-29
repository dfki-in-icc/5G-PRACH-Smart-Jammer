#ifndef E2_AGENT_API_MOSAIC_H
#define E2_AGENT_API_MOSAIC_H

#include "sm_io.h"


void init_agent_api(const char* server_ip_str, 
                    int mcc, 
                    int mnc, 
                    int mnc_digit_len,
                    int nb_id,
                    sm_io_ag_t io);

void stop_agent_api(void);

#endif

