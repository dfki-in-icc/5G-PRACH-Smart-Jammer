#include "activate_cell.h"

#include <assert.h>

void free_activate_cell(activate_cell_t* cell)
{
  assert(cell != NULL);

  // mandatory
  // NR CGI 9.3.1.12

  // optional
  // NR PCI
  free(cell->nr_pci);

  if(cell->sys_info != NULL){
    free_gnb_cu_sys_info(cell->sys_info);
    free(cell->sys_info);
  }

  assert(cell->sz_avail_plmn_lst  == 0 && "Not implemented");

  // optional
  // Extended Available PLMN List 9.3.1.76 
  assert(cell->sz_ext_avail_plmn_lst == 0 && "Not implemented");

  // optional AB Info IAB-donor-CU 9.3.1.105
  assert(cell->ab_info == NULL && "Not implemented");

  // Available SNPN ID List 9.3.1.163
  assert(cell->sz_av_snpn_id_lst == 0 && "Not implemented");

}

bool eq_activate_cell(activate_cell_t const* m0,  activate_cell_t const* m1)
{
  assert(m0 != NULL);
  assert(m1 != NULL);

  // mandatory
  // NR CGI 9.3.1.12
  if(eq_nr_cgi(&m0->nr_cgi, &m1->nr_cgi) == false)
    return false;

  // optional
  // NR PCI
  if(m0->nr_pci != m1->nr_pci){
    if(m0->nr_pci == NULL)
      return false;
    if(m1->nr_pci == NULL)
      return false;
    if(*m0->nr_pci != *m1->nr_pci)
      return false;
  }

  // optional
  // gNB-CU System Information 9.3.1.42
  if(eq_gnb_cu_sys_info(m0->sys_info, m1->sys_info) == false)
    return false;

  // optional
  // Available PLMN List 9.3.1.65 
  if(m0->sz_avail_plmn_lst != m1->sz_avail_plmn_lst)
    return false;

  for(size_t i = 0; i < m0->sz_avail_plmn_lst; ++i){
    assert( 0!= 0 && "Not implemented");
  }

  // optional
  // Extended Available PLMN List 9.3.1.76 
  if(m0->sz_ext_avail_plmn_lst != m1->sz_ext_avail_plmn_lst)
    return false;

  for(size_t i = 0; i < m0->sz_ext_avail_plmn_lst; ++i){
    assert(0!=0 && "Not implemented");
  }

  // optional AB Info IAB-donor-CU 9.3.1.105
  assert(m0->ab_info == NULL && "Not implemented");
  assert(m1->ab_info == NULL && "Not implemented");

  // Available SNPN ID List 9.3.1.163
  if(m0->sz_av_snpn_id_lst != m1->sz_av_snpn_id_lst)
    return false;

  for(size_t i =0; i < m0->sz_av_snpn_id_lst; ++i){
    assert(0!=0 && "Not implemented");
  }

  return true;
}

