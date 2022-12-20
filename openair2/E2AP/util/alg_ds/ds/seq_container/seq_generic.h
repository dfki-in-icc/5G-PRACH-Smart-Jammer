#ifndef SEQUENCE_CONTAINERS_GENERIC
#define SEQUENCE_CONTAINERS_GENERIC

#include "seq_arr.h"

/*
 *Generic sequence container file. Nomenclature from the C++ sequence containers
 */


// Initialization
#define seq_init(T,U) _Generic ((T), seq_arr_t*: seq_arr_init, \
                                       default: seq_arr_init) (T,U)


#define seq_free(T,U) _Generic ((T), seq_arr_t*: seq_arr_free, \
                                       default: seq_arr_free)(T,U)


// Modifiers
#define seq_push_back(T,U,W) _Generic ((T), seq_arr_t*:   seq_arr_push_back,  \
                                            default:      seq_arr_push_back)(T,U,W)

#define seq_erase(T,U,W) _Generic ((T), seq_arr_t*: seq_arr_erase, \
                                          default: seq_arr_erase)(T,U,W)

#define seq_erase_free(T, U, W, X)                                             \
  _Generic((T), seq_arr_t *                                                    \
           : seq_arr_erase_free, default                                       \
           : seq_arr_erase_free)(T, U, W, X)

// Capacity
#define seq_size(T) _Generic ((T), seq_arr_t*: seq_arr_size, \
                                       default: seq_arr_size)(T)


// Forward Iterator Concept
#define seq_front(T) _Generic ((T), seq_arr_t*: seq_arr_front, \
                                        default: seq_arr_front)(T)

#define seq_next(T,U) _Generic((T), seq_arr_t*: seq_arr_next, \
                                        default: seq_arr_next)(T,U)

#define seq_end(T) _Generic((T), seq_arr_t*: seq_arr_end, \
                                        default: seq_arr_end)(T)

// at position starting from zero
#define seq_at(T,U) _Generic((T), seq_arr_t*: seq_arr_at, \
                                        default: seq_arr_at)(T,U)

//
#define seq_distance(T,U,V) _Generic((T), seq_arr_t*: seq_arr_dist,\
                                          default: seq_arr_dist)(T,U,V)

// Equality

#define seq_equal(T,U,V) _Generic((T), seq_arr_t*: seq_arr_equal, \
                                        default: seq_arr_equal)(U,V)


#define seq_value(T,U) _Generic((T), seq_arr_t*: seq_arr_value, \
                                        default: seq_arr_value)(U)


#endif
