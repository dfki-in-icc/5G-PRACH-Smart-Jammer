#ifndef F1AP_NEIGHBOUR_CELL_INFO_H
#define F1AP_NEIGHBOUR_CELL_INFO_H

#include "nr_cgi.h"
#include "intented_tdd_dl_ul_conf.h"

typedef struct{
  // NR CGI 9.3.1.12
  // Mandatory
  nr_cgi_t nr_cgi;

  // Intended TDD DL-UL Configuration 9.3.1.89
  // Optional
  intended_tdd_dl_ul_conf_t*  intended_tdd_dl_ul_conf;

} neighbour_cell_info_t;

void free_neighbour_cell_info( neighbour_cell_info_t* src);

#endif
