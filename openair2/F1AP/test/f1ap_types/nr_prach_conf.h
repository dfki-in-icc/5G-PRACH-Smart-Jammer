#ifndef F1AP_NR_PRACH_CONF_MIR_H
#define F1AP_NR_PRACH_CONF_MIR_H

#include <stdbool.h>

typedef struct{

} nr_prach_conf_t ;

void free_nr_prach_conf(nr_prach_conf_t* src);

bool eq_nr_prach_conf(nr_prach_conf_t const* m0, nr_prach_conf_t const* m1);

#endif

