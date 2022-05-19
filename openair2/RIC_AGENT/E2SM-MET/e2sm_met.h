

#ifndef _E2SM_COMMON_H_
#define _E2SM_COMMON_H_

extern int e2sm_met_init(void);

extern int 
e2sm_met_decode_and_handle_action_def(uint8_t *def_buf, 
                                          size_t def_size, 
                                          ric_ran_function_t *func,
                                          uint32_t      interval_ms,
                                          ric_subscription_t* rs, 
                                          ric_agent_info_t *ric);
#endif /* _E2SM_COMMON_H_ */
