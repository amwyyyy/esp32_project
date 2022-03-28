#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_MEM_ALIGN
#endif
#ifndef LV_ATTRIBUTE_IMG_D31
#define LV_ATTRIBUTE_IMG_D31
#endif
const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_IMG_D31 uint8_t d31_map[] = {
  0x00, 0x00, 0x00, 0x00, 	/*Color of index 0*/
  0x00, 0x80, 0xff, 0x02, 	/*Color of index 1*/
  0x00, 0xc6, 0xff, 0x05, 	/*Color of index 2*/
  0x00, 0xad, 0xff, 0x05, 	/*Color of index 3*/
  0x00, 0xbc, 0xff, 0x0f, 	/*Color of index 4*/
  0x00, 0xbe, 0xff, 0x1d, 	/*Color of index 5*/
  0x00, 0xbe, 0xff, 0x2d, 	/*Color of index 6*/
  0x00, 0xbe, 0xff, 0x3f, 	/*Color of index 7*/
  0x00, 0xbf, 0xff, 0x54, 	/*Color of index 8*/
  0x00, 0xc1, 0xff, 0x72, 	/*Color of index 9*/
  0x00, 0xc0, 0xff, 0x90, 	/*Color of index 10*/
  0x00, 0xc0, 0xff, 0xb2, 	/*Color of index 11*/
  0x00, 0xc0, 0xff, 0xd4, 	/*Color of index 12*/
  0x00, 0xc0, 0xff, 0xed, 	/*Color of index 13*/
  0x00, 0xc0, 0xff, 0xfd, 	/*Color of index 14*/
  0x00, 0xff, 0xff, 0x01, 	/*Color of index 15*/

  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x35, 0x56, 0x41, 0x00, 0x00, 0x00, 0x00, 0x04, 0x56, 0x75, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x9a, 0x75, 0x30, 0x00, 0x00, 0x00, 0x05, 0x9b, 0xb9, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4a, 0xcc, 0xb9, 0x62, 0xf0, 0x00, 0x00, 0x08, 0xad, 0xdb, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x32, 0x40, 0x00, 0x4a, 0xde, 0xec, 0x96, 0x10, 0x00, 0x00, 0x08, 0xbd, 0xdc, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x67, 0x52, 0xf0, 0x27, 0xad, 0xed, 0xc9, 0x30, 0x00, 0x00, 0x06, 0x9b, 0xb9, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x9b, 0x97, 0x40, 0x34, 0x8a, 0xde, 0xda, 0x52, 0x00, 0x00, 0x04, 0x68, 0x96, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4a, 0xdd, 0xc9, 0x50, 0x00, 0x48, 0xbe, 0xeb, 0x74, 0x00, 0x00, 0x0f, 0x12, 0x23, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5a, 0xee, 0xca, 0x50, 0x00, 0x25, 0xbe, 0xeb, 0x85, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x02, 0x56, 0x87, 0x52, 0x00, 0x00, 0x49, 0xbc, 0xa8, 0x40, 0x00, 0x05, 0xae, 0xec, 0x85, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x45, 0x54, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x04, 0x8b, 0xcb, 0x94, 0x00, 0x00, 0x34, 0x68, 0x64, 0x30, 0x00, 0x25, 0xae, 0xeb, 0x85, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x68, 0x97, 0x43, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x05, 0x9c, 0xee, 0xa4, 0x00, 0x00, 0x0f, 0x25, 0x20, 0x00, 0x00, 0x47, 0xbe, 0xeb, 0x74, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x9c, 0xba, 0x83, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x05, 0x9c, 0xcd, 0xa5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x59, 0xce, 0xea, 0x52, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0xbe, 0xec, 0x94, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x04, 0x69, 0xaa, 0x72, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8b, 0xdd, 0xd9, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0xbd, 0xdb, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x01, 0x46, 0x86, 0x53, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x26, 0xac, 0xed, 0xb7, 0x30, 0x57, 0x87, 0x53, 0x00, 0x00, 0x04, 0x8a, 0xa9, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xf1, 0x23, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf2, 0x5a, 0xce, 0xec, 0x95, 0x32, 0x7b, 0xcb, 0x74, 0xf0, 0x00, 0x0f, 0x34, 0x44, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x27, 0xac, 0xde, 0xca, 0x63, 0x25, 0x9c, 0xec, 0x96, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x04, 0x65, 0x53, 0x00, 0x00, 0x00, 0x00, 0x02, 0x49, 0xce, 0xed, 0xa7, 0x40, 0x25, 0x9c, 0xec, 0x96, 0x10, 0x00, 0x13, 0x43, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x27, 0x9a, 0x84, 0x00, 0x12, 0x21, 0xf0, 0x02, 0x6a, 0xde, 0xca, 0x74, 0xf0, 0xf3, 0x7a, 0xba, 0x74, 0xf0, 0x01, 0x46, 0x66, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x02, 0x69, 0xcc, 0xa6, 0x34, 0x58, 0x86, 0x41, 0x02, 0x58, 0xbb, 0x97, 0x40, 0x00, 0x00, 0x47, 0x87, 0x4f, 0x00, 0x04, 0x8a, 0xba, 0x74, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x04, 0x7b, 0xde, 0xc8, 0x45, 0x9a, 0xb9, 0x53, 0x00, 0xf6, 0x87, 0x53, 0x02, 0x44, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x46, 0xbe, 0xec, 0x96, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x04, 0x8b, 0xee, 0xb8, 0x45, 0xbd, 0xdb, 0x84, 0x00, 0x02, 0x44, 0x30, 0x25, 0x76, 0x42, 0x00, 0x00, 0x00, 0x00, 0x69, 0xce, 0xec, 0xa5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x05, 0x8b, 0xee, 0xc7, 0x26, 0xbd, 0xda, 0x83, 0x00, 0x00, 0x00, 0x03, 0x79, 0xaa, 0x85, 0x00, 0x00, 0x00, 0xf2, 0x8b, 0xee, 0xeb, 0x85, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x04, 0x9b, 0xed, 0xc7, 0x42, 0x9a, 0xb9, 0x52, 0x00, 0x00, 0x00, 0x35, 0xac, 0xed, 0xb7, 0x00, 0x00, 0x00, 0x46, 0xad, 0xee, 0xda, 0x53, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x04, 0x8b, 0xee, 0xc8, 0x62, 0x56, 0x74, 0x4f, 0x00, 0x00, 0x0f, 0x6a, 0xce, 0xed, 0xb6, 0x00, 0x00, 0x00, 0x59, 0xcd, 0xed, 0xb8, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x04, 0x7a, 0xee, 0xda, 0x70, 0x00, 0x00, 0x00, 0x00, 0x02, 0x57, 0x9c, 0xee, 0xec, 0x96, 0x00, 0x00, 0x05, 0x8b, 0xee, 0xec, 0x96, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x02, 0x4a, 0xde, 0xdc, 0x94, 0x2f, 0xf0, 0xff, 0x13, 0x25, 0x8b, 0xdd, 0xee, 0xca, 0x53, 0x00, 0x00, 0x28, 0xbd, 0xee, 0xdb, 0x73, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x38, 0xce, 0xee, 0xb9, 0x75, 0x42, 0x44, 0x67, 0x9a, 0xbd, 0xee, 0xec, 0x97, 0x20, 0x00, 0x03, 0x7a, 0xde, 0xee, 0xb8, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x06, 0xac, 0xee, 0xec, 0xa9, 0x77, 0x78, 0x9a, 0xcd, 0xee, 0xed, 0xca, 0x63, 0x00, 0x00, 0x47, 0xac, 0xee, 0xec, 0x95, 0x45, 0x65, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x04, 0x7a, 0xde, 0xee, 0xdc, 0xcc, 0xcc, 0xcd, 0xde, 0xee, 0xdb, 0x85, 0x20, 0x00, 0x02, 0x7a, 0xce, 0xee, 0xca, 0x64, 0x79, 0xba, 0x83, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0xac, 0xde, 0xee, 0xee, 0xee, 0xee, 0xee, 0xcb, 0xa8, 0x50, 0x00, 0x00, 0x58, 0xad, 0xde, 0xec, 0xa7, 0x25, 0x9b, 0xec, 0xa2, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0xf3, 0x69, 0xbd, 0xdd, 0xed, 0xed, 0xdd, 0xdb, 0xa7, 0x53, 0xf0, 0x01, 0x24, 0x8b, 0xde, 0xee, 0xda, 0x74, 0x05, 0xac, 0xde, 0xa5, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x78, 0xaa, 0xbb, 0xba, 0xa9, 0x87, 0x64, 0x30, 0x00, 0x04, 0x6a, 0xbd, 0xee, 0xec, 0xa7, 0x5f, 0x04, 0x8a, 0xcb, 0x92, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x0f, 0x25, 0x65, 0x40, 0x04, 0x67, 0x88, 0x87, 0x75, 0x20, 0x00, 0x00, 0x04, 0x68, 0xac, 0xee, 0xee, 0xca, 0x84, 0x00, 0x03, 0x58, 0x98, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x02, 0x58, 0xa9, 0x72, 0x01, 0x32, 0x44, 0x43, 0x22, 0xf0, 0x0f, 0xf2, 0x47, 0xab, 0xde, 0xee, 0xec, 0x97, 0x2f, 0x00, 0x0f, 0x24, 0x44, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x04, 0x8a, 0xcc, 0xa5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x58, 0x9b, 0xce, 0xee, 0xed, 0xb9, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x05, 0x9c, 0xee, 0xb5, 0x00, 0x00, 0x00, 0x00, 0x02, 0x45, 0x57, 0x9b, 0xce, 0xee, 0xed, 0xcb, 0x84, 0x10, 0x00, 0x01, 0x45, 0x54, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x04, 0x8a, 0xec, 0xa5, 0x00, 0x00, 0x00, 0x00, 0x25, 0x79, 0xac, 0xcd, 0xee, 0xee, 0xdb, 0x96, 0x43, 0x00, 0x00, 0x04, 0x7a, 0x98, 0x4f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x04, 0x58, 0xa9, 0x72, 0x00, 0x34, 0x66, 0x78, 0x99, 0xbb, 0xdd, 0xee, 0xee, 0xdb, 0xa8, 0x63, 0x00, 0x00, 0x00, 0x58, 0xbc, 0xca, 0x74, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x12, 0x33, 0x3f, 0x35, 0x68, 0x9a, 0xbb, 0xde, 0xee, 0xee, 0xed, 0xdc, 0xa8, 0x54, 0x10, 0x00, 0x0f, 0x44, 0x9b, 0xde, 0xeb, 0x85, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x79, 0xbc, 0xcc, 0xdd, 0xee, 0xee, 0xee, 0xcb, 0x98, 0x75, 0x20, 0x00, 0x00, 0x03, 0x69, 0xbd, 0xee, 0xeb, 0x74, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x9c, 0xee, 0xee, 0xee, 0xed, 0xdc, 0xbb, 0xa8, 0x62, 0x00, 0x00, 0x00, 0x02, 0x57, 0x9c, 0xee, 0xed, 0xb9, 0x53, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x9c, 0xee, 0xee, 0xee, 0xdc, 0xa9, 0x85, 0x44, 0x3f, 0x00, 0x00, 0xf1, 0x25, 0x9b, 0xde, 0xee, 0xdb, 0x85, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x8a, 0xbc, 0xbb, 0xa9, 0x98, 0x76, 0x4f, 0x00, 0x00, 0x00, 0x00, 0x36, 0x8a, 0xbd, 0xee, 0xec, 0xb8, 0x51, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x57, 0x99, 0x87, 0x64, 0x20, 0x00, 0x00, 0x00, 0x00, 0x13, 0x55, 0x79, 0xbd, 0xee, 0xed, 0xca, 0x83, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x55, 0x54, 0x23, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x46, 0x9a, 0xbc, 0xde, 0xee, 0xdb, 0x96, 0x4f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x35, 0x78, 0x9a, 0xbd, 0xee, 0xee, 0xdb, 0xa8, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0xac, 0xde, 0xee, 0xee, 0xdd, 0xa9, 0x52, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7b, 0xce, 0xee, 0xee, 0xdb, 0xa8, 0x75, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x76, 0x30, 0x00, 0x7b, 0xdd, 0xdc, 0xcb, 0xa9, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x69, 0xba, 0x50, 0x00, 0x59, 0xbc, 0xb9, 0x86, 0x53, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6b, 0xcb, 0x60, 0x00, 0x46, 0x88, 0x76, 0x52, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x69, 0xba, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x35, 0x65, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

const lv_img_dsc_t d31 = {
  .header.cf = LV_IMG_CF_INDEXED_4BIT,
  .header.always_zero = 0,
  .header.reserved = 0,
  .header.w = 64,
  .header.h = 64,
  .data_size = 2113,
  .data = d31_map,
};