#ifndef F1AP_SERV_EUTRA_CELL_INFO_H
#define F1AP_SERV_EUTRA_CELL_INFO_H

#include "../byte_array.h"

#include <stdint.h>

typedef enum {
  FDD_INFO_SERV_EUTRA_CELL_INFO,
  TDD_INFO_SERV_EUTRA_CELL_INFO,
  END_SERV_EUTRA_CELL_INFO
} serv_eutra_cell_info_e;

typedef struct{
 uint16_t ul_offset_to_point_a; // [0,2199]
 uint16_t dl_offset_to_point_a;  // [0,2199]
} fdd_info_serv_eutra_cell_info_t; 

typedef struct{
  uint16_t offset_to_point_a; // [0,2199]
} tdd_info_serv_eutra_cell_info_t; 

// 9.3.1.64
typedef struct {
 serv_eutra_cell_info_e type;
 union{
   fdd_info_serv_eutra_cell_info_t fdd;
   tdd_info_serv_eutra_cell_info_t tdd;
 }; 

 //Optional
 byte_array_t* prot_eutra_resource_ind; 

} serv_eutra_cell_info_t;

void free_serv_eutra_cell_info(serv_eutra_cell_info_t* src);


#endif
