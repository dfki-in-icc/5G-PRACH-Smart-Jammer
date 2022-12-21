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

#include "find.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

void* find_if_arr(seq_arr_t* arr, void* start_it, void* end_it, void* value, bool (*f)(const void*, const void*))
{
  assert(arr != NULL);

  while (start_it != end_it) {
    if (f(value, start_it))
      return start_it;
    start_it = seq_next(arr, start_it);
  }
  return start_it;
}

void* find_if_rb_tree(assoc_rb_tree_t* tree, void* start_it, void* end_it, void const* value, bool (*f)(const void*, const void*))
{
  assert(tree != NULL);

  while (start_it != end_it) {
    if (f(value, assoc_key(tree, start_it)))
      return start_it;
    start_it = assoc_next(tree, start_it);
  }

  return start_it;
}

bml_iter_t find_if_bi_map_left(bi_map_t* map, bml_iter_t start_it, bml_iter_t end_it, void const* value, bool (*f)(const void*, const void*))
{
  assert(map != NULL);
  assert(start_it.it != NULL);

  while (start_it.it != end_it.it) {
    if (f(value, assoc_key(&map->left, start_it.it)))
      return start_it;
    start_it = bi_map_next_left(map, start_it);
  }

  return start_it;
}

bmr_iter_t find_if_bi_map_right(bi_map_t* map, bmr_iter_t start_it, bmr_iter_t end_it, void const* value, bool (*f)(const void*, const void*))
{
  assert(map != NULL);
  assert(start_it.it != NULL);

  while (start_it.it != end_it.it) {
    if (f(value, start_it.it) == true)
      return start_it;

    start_it = bi_map_next_right(map, start_it);
  }

  return start_it;
}
