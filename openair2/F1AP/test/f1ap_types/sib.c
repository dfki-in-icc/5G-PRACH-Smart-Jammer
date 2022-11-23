#include "sib.h"

#include <assert.h>

void free_sib(sib_t* src)
{
  assert(src != NULL);

  free_byte_array(src->msg);

  assert(src->area_scope == NULL && "Not implemented");
}

bool eq_sib(sib_t const* m0, sib_t const* m1)
{
  if(m0 == m1)
    return true;

  if(m0 == NULL)
    return false;

  if(m1 == NULL)
    return false;

  // mandatory
  if(m0->type != m1->type)
    return false;

    // mandatory
  if(eq_byte_array(&m0->msg, &m1->msg) == false) 
    return false;

  // mandatory // INTEGER (0..31, ...)
  if(m0->tag != m1->tag)
    return false;

  // optional
  assert(m0->area_scope == NULL); 
  assert(m1->area_scope == NULL); 

  return true;
} 

