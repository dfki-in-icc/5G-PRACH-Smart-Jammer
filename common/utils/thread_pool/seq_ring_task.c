/*
MIT License

Copyright (c) 2021 Mikel Irazabal

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


#include "seq_ring_task.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// For working correctly, maintain the default elements to a multiple of 2
#define DEFAULT_ELM 32

inline static
uint32_t mask(uint32_t cap, uint32_t val)
{

  return val & (cap-1);
}

static 
bool full(seq_ring_task_t* r)
{
  return size_seq_ring_task(r) == r->cap -1;
}

static
void enlarge_buffer(seq_ring_task_t* r)
{
  assert(r != NULL);
  assert(full(r));

  const uint32_t factor = 2;
  task_t* tmp_buffer = calloc(r->cap * factor, sizeof(task_t) );
  assert(tmp_buffer != NULL);

  const uint32_t head_pos = mask(r->cap, r->head);
  const uint32_t tail_pos = mask(r->cap, r->tail);

  if(head_pos > tail_pos){
    memcpy(tmp_buffer, r->array + tail_pos , (head_pos-tail_pos)*sizeof(task_t) );
  } else {
    memcpy(tmp_buffer, r->array + tail_pos, (r->cap-tail_pos)*sizeof(task_t));
    memcpy(tmp_buffer + (r->cap-tail_pos), r->array, head_pos*sizeof(task_t));
  }
  r->cap *= factor;
  free(r->array);
  r->array = tmp_buffer;
  r->tail = 0;
  r->head = r->cap/2 - 1;
}

void init_seq_ring_task(seq_ring_task_t* r)
{
  assert(r != NULL);
  task_t* tmp_buffer = calloc(DEFAULT_ELM, sizeof(task_t)); 
  assert(tmp_buffer != NULL);
  seq_ring_task_t tmp = {.array = tmp_buffer, .head = 0, .tail = 0, .cap = DEFAULT_ELM};
  memcpy(r, &tmp, sizeof(seq_ring_task_t));
}

void free_seq_ring_task(seq_ring_task_t* r, seq_free_func fp)
{
  assert(r != NULL);
  assert(fp == NULL);
  free(r->array);
}


void push_back_seq_ring_task(seq_ring_task_t* r, task_t t)
{
  assert(r != NULL);

  if(full(r))
    enlarge_buffer(r);
  
  const uint32_t pos = mask(r->cap, r->head);
  r->array[pos] = t;
  r->head += 1;
}

task_t pop_seq_ring_task(seq_ring_task_t* r )
{
  assert(r != NULL);
  assert(size_seq_ring_task(r) > 0);

  const uint32_t pos = mask(r->cap, r->tail);
  task_t t = r->array[pos];
  r->tail += 1; 
  return t;
}

size_t size_seq_ring_task(seq_ring_task_t* r)
{
  assert(r != NULL);

  return r->head - r->tail;
}

