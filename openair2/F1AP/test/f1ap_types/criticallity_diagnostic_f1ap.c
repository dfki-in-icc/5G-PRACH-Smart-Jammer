#include "criticallity_diagnostic_f1ap.h"

#include <assert.h>
#include <stdbool.h>

void free_criticallity_diagnose(criticallity_diagnostic_f1ap_t* c)
{
  assert(c != NULL);

  if(c->proc_code != NULL)
    free(c->proc_code);

  if(c->trig_msg != NULL)
    free(c->trig_msg);

  if(c->proc_crit != NULL)
    free(c->proc_crit);

  if(c->trans_id != NULL)
    free(c->trans_id);

  assert(c->sz_crit_diagn_ie < 257);
  if(c->sz_crit_diagn_ie > 0)
    free(c->crit_diagn_ie);
}

bool eq_criticallity_diagnose(criticallity_diagnostic_f1ap_t const* m0, criticallity_diagnostic_f1ap_t const* m1)
{
  if(m0 == m1)
    return true;

  if(m0 == NULL)
    return false;

  if(m1 == NULL)
    return false;


  //optional
  //Procedure Code
  if(m0->proc_code != m1->proc_code){
    if(m0->proc_code == NULL)
      return false;
    if(m1->proc_code == NULL)
      return false;
    if(*m0->proc_code != *m1->proc_code)
      return false;
  }

  //optional
  //Triggering Message
  if(m0->trig_msg!= m1->trig_msg){
    if(m0->trig_msg == NULL)
      return false;
    if(m1-> trig_msg == NULL)
      return false;
    if(*m0->trig_msg != *m1-> trig_msg)
      return false;
  }

  //optional
  //Procedure Criticality
  if(m0->proc_crit!= m1->proc_crit){
    if(m0->proc_crit == NULL)
      return false;
    if(m1->proc_crit == NULL)
      return false;
    if(*m0->proc_crit != *m1->proc_crit)
      return false;
  }

  //optional
  //Transaction ID
  if(m0->trans_id!= m1->trans_id){
    if(m0->trans_id == NULL)
      return false;
    if(m1->trans_id == NULL)
      return false;
    if(*m0->trans_id != *m1->trans_id)
      return false;
  }

  if(m0->sz_crit_diagn_ie != m1->sz_crit_diagn_ie)
    return false;

  for(size_t i = 0; i < m0->sz_crit_diagn_ie; ++i){
    if(eq_crit_diagn_ie(&m0-> crit_diagn_ie[i], &m1->crit_diagn_ie[i]) == false)
      return false;
  }

  return true;
}

