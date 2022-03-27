#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_MEM_ALIGN
#endif
#ifndef LV_ATTRIBUTE_IMG_D24
#define LV_ATTRIBUTE_IMG_D24
#endif
const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_IMG_D24 uint8_t d24_map[] = {
  0x00, 0x00, 0x00, 0x00, 	/*Color of index 0*/
  0xe3, 0xe6, 0xe6, 0x17, 	/*Color of index 1*/
  0xae, 0x6d, 0x14, 0x06, 	/*Color of index 2*/
  0xf9, 0xc9, 0x85, 0x14, 	/*Color of index 3*/
  0xbe, 0x6c, 0x1a, 0x1f, 	/*Color of index 4*/
  0xf6, 0xc3, 0x7f, 0x56, 	/*Color of index 5*/
  0xe0, 0xdf, 0xde, 0xc1, 	/*Color of index 6*/
  0xe1, 0xdd, 0xd9, 0xfc, 	/*Color of index 7*/
  0xf6, 0xc4, 0x82, 0xaf, 	/*Color of index 8*/
  0xf6, 0xc4, 0x81, 0xe5, 	/*Color of index 9*/
  0xbe, 0x6d, 0x18, 0x68, 	/*Color of index 10*/
  0xf4, 0xc7, 0x8b, 0xfd, 	/*Color of index 11*/
  0xbe, 0x6e, 0x18, 0xb6, 	/*Color of index 12*/
  0xbe, 0x6e, 0x18, 0xf4, 	/*Color of index 13*/
  0xf0, 0x00, 0x00, 0x01, 	/*Color of index 14*/
  0xdf, 0xde, 0xde, 0x59, 	/*Color of index 15*/

  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x33, 0x33, 0x11, 0x11, 0x1f, 0xff, 0xff, 0xf1, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0x33, 0x33, 0x33, 0x33, 0x31, 0xff, 0x66, 0x67, 0x66, 0x66, 0xf1, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x55, 0x85, 0x88, 0x88, 0x88, 0x86, 0x67, 0x77, 0x77, 0x76, 0x66, 0xf1, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x55, 0x88, 0x99, 0x99, 0x99, 0x9b, 0xbb, 0x77, 0x77, 0x77, 0x77, 0x77, 0x66, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x55, 0x88, 0x99, 0x99, 0x99, 0xbb, 0xbb, 0xbb, 0xbb, 0xb7, 0x77, 0x77, 0x77, 0x76, 0x61, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x35, 0x59, 0x9b, 0x99, 0x99, 0x99, 0xbb, 0xbb, 0xbb, 0xbb, 0xb7, 0x77, 0x77, 0x77, 0x77, 0x66, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x58, 0x99, 0x99, 0x99, 0x99, 0x9b, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xb7, 0x77, 0x77, 0x77, 0x76, 0xf1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x35, 0x88, 0x99, 0x99, 0x99, 0x99, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0x77, 0x77, 0x77, 0x77, 0x61, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x03, 0x55, 0x99, 0x99, 0x99, 0x99, 0x99, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xb7, 0x77, 0x77, 0x77, 0x6f, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x03, 0x58, 0x99, 0x99, 0x99, 0x99, 0x9b, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xb7, 0x77, 0x77, 0x77, 0x7f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x05, 0x89, 0x99, 0x99, 0x99, 0x99, 0x99, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0x77, 0x77, 0x77, 0x76, 0xf1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x35, 0x89, 0x99, 0x99, 0x99, 0x99, 0x9b, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0x77, 0x77, 0x77, 0x76, 0x6f, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x03, 0x35, 0x99, 0x99, 0x99, 0x99, 0x99, 0x9b, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xb7, 0x77, 0x77, 0x77, 0x76, 0x61, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x03, 0x58, 0x99, 0x99, 0x99, 0x99, 0x99, 0x9b, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0x77, 0x77, 0x77, 0x77, 0x77, 0x66, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x03, 0x58, 0x99, 0x99, 0x99, 0x99, 0x99, 0xb9, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0x77, 0x77, 0x77, 0x77, 0x76, 0xf1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x03, 0x58, 0x99, 0x99, 0x99, 0x99, 0x99, 0x9b, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0x7b, 0x77, 0x77, 0x61, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x33, 0x53, 0x88, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xb7, 0xb7, 0x77, 0x6f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x33, 0x35, 0x58, 0x89, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0x77, 0x6f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x03, 0x35, 0x88, 0x89, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x9b, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xb7, 0x6f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x33, 0x58, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x9b, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0x61, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x35, 0x89, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xb9, 0x85, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x55, 0xb9, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x9b, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xb9, 0x83, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x88, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x9b, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xb9, 0x85, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x88, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x9b, 0x9b, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xb9, 0xb9, 0x85, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x9b, 0x9b, 0xbb, 0xbb, 0xbb, 0xbb, 0xb9, 0xb9, 0xb9, 0x9b, 0x85, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x85, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x83, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x89, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x98, 0x83, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x88, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x98, 0x53, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x55, 0xb9, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x95, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x35, 0x89, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x98, 0x85, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x33, 0x58, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x88, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x03, 0x35, 0x88, 0x89, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x88, 0x53, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x01, 0x33, 0x35, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x85, 0x55, 0x33, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x33, 0x35, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x04, 0xa4, 0x20, 0x00, 0x02, 0x2e, 0x00, 0x00, 0x24, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x0a, 0xca, 0x20, 0x00, 0x04, 0x44, 0x20, 0x02, 0x4a, 0x44, 0x00, 0x00, 0x22, 0x2e, 0x00, 0x04, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x02, 0x4a, 0xda, 0x22, 0x00, 0x04, 0xa4, 0x2e, 0x04, 0xaa, 0xa4, 0x00, 0x02, 0x44, 0x42, 0x00, 0xe4, 0xaa, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x04, 0xac, 0xda, 0x42, 0x02, 0x4a, 0xca, 0x22, 0x44, 0xcc, 0xa4, 0x00, 0x02, 0xca, 0x42, 0x0e, 0x2a, 0xda, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x0e, 0x24, 0xcd, 0xda, 0x42, 0x02, 0xac, 0xda, 0x22, 0x4c, 0xcd, 0xa4, 0x0e, 0x2a, 0xcc, 0x44, 0x02, 0xac, 0xcc, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x02, 0x4a, 0xdd, 0xda, 0x42, 0x04, 0xcc, 0xda, 0x22, 0xac, 0xdc, 0xa4, 0x02, 0x4a, 0xdc, 0x42, 0x24, 0xad, 0xda, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x02, 0xac, 0xdd, 0xda, 0x42, 0x4a, 0xcd, 0xda, 0x42, 0xad, 0xdd, 0xa4, 0x02, 0xad, 0xdc, 0xa2, 0x44, 0xcd, 0xdc, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x02, 0xad, 0xdd, 0xda, 0x44, 0x4c, 0xdd, 0xda, 0x24, 0xac, 0xdc, 0xa2, 0x44, 0xcd, 0xdc, 0x42, 0x4a, 0xcd, 0xda, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x02, 0xac, 0xdd, 0xda, 0x44, 0xad, 0xdd, 0xda, 0x42, 0x4c, 0xca, 0x42, 0x4c, 0xcd, 0xdc, 0xa2, 0x44, 0xcd, 0xca, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x02, 0x4a, 0xcc, 0xc4, 0x2a, 0xcd, 0xdd, 0xda, 0x42, 0x42, 0x44, 0x22, 0xac, 0xdd, 0xdc, 0xa4, 0xe4, 0x4a, 0xa4, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x0e, 0x44, 0xaa, 0x44, 0x4c, 0xcd, 0xdd, 0xda, 0x42, 0x00, 0x00, 0x24, 0xcd, 0xdd, 0xdc, 0xa2, 0x00, 0x44, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xe2, 0x22, 0x22, 0xad, 0xdd, 0xdd, 0xdc, 0x42, 0x00, 0x00, 0x2a, 0xcd, 0xdd, 0xdc, 0xa4, 0x00, 0xe2, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0xad, 0xdd, 0xdd, 0xda, 0x44, 0x00, 0x00, 0x2a, 0xcd, 0xdd, 0xdc, 0xa2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0xad, 0xdd, 0xdd, 0xdc, 0x42, 0x00, 0x00, 0x24, 0xad, 0xdd, 0xda, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0xad, 0xdd, 0xdd, 0xda, 0x22, 0x00, 0x00, 0xe2, 0x4c, 0xcc, 0xaa, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0xe4, 0x4c, 0xdd, 0xdc, 0xc4, 0x2e, 0x00, 0x00, 0x00, 0x44, 0xaa, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4a, 0xac, 0xcc, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x44, 0x44, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

const lv_img_dsc_t d24 = {
  .header.cf = LV_IMG_CF_INDEXED_4BIT,
  .header.always_zero = 0,
  .header.reserved = 0,
  .header.w = 64,
  .header.h = 64,
  .data_size = 2113,
  .data = d24_map,
};
