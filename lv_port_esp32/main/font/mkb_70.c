/*******************************************************************************
 * Size: 70 px
 * Bpp: 1
 * Opts: 
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef MKB_70
#define MKB_70 1
#endif

#if MKB_70

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap_mkb_70[] = {
    /* U+0020 " " */
    0x0,

    /* U+0030 "0" */
    0x0, 0x0, 0x7f, 0xc0, 0x0, 0x0, 0x0, 0x7f,
    0xff, 0x0, 0x0, 0x0, 0x1f, 0xff, 0xf0, 0x0,
    0x0, 0x7, 0xff, 0xff, 0x80, 0x0, 0x3, 0xff,
    0xff, 0xf8, 0x0, 0x0, 0x1f, 0xff, 0xff, 0xe0,
    0x0, 0x3f, 0xff, 0xff, 0xf0, 0x0, 0x7, 0xff,
    0xff, 0xff, 0x0, 0x1, 0xff, 0xff, 0xff, 0xe0,
    0x0, 0x7f, 0xfc, 0x1f, 0xfe, 0x0, 0xf, 0xff,
    0x1, 0xff, 0xe0, 0x3, 0xff, 0xc0, 0x1f, 0xfc,
    0x0, 0x7f, 0xf8, 0x3, 0xff, 0x80, 0xf, 0xfe,
    0x0, 0x3f, 0xf8, 0x1, 0xff, 0xe0, 0x7, 0xff,
    0x80, 0x7f, 0xf8, 0x0, 0xff, 0xe0, 0xff, 0xf0,
    0x0, 0xff, 0xe0, 0x1, 0xff, 0xe0, 0x3, 0xff,
    0xc0, 0x3f, 0xf8, 0x0, 0x3f, 0xf8, 0x3, 0xff,
    0xc0, 0x1, 0xff, 0xe1, 0xff, 0xe0, 0x0, 0xff,
    0xe0, 0x3f, 0xfc, 0x0, 0x1f, 0xfc, 0x7, 0xff,
    0x80, 0x3, 0xff, 0x80, 0xff, 0xf0, 0x0, 0x7f,
    0xf8, 0x1f, 0xfe, 0x0, 0xf, 0xff, 0x3, 0xff,
    0xc0, 0x1, 0xff, 0xe0, 0xff, 0xf0, 0x0, 0x7f,
    0xf8, 0x7, 0xff, 0x80, 0x3, 0xff, 0xc1, 0xff,
    0xe0, 0x0, 0xff, 0xf0, 0x3, 0xff, 0xc0, 0x3,
    0xff, 0xcf, 0xff, 0x0, 0x7, 0xff, 0x0, 0xff,
    0xf0, 0x0, 0x7f, 0xf8, 0x1f, 0xfe, 0x0, 0xf,
    0xff, 0x3, 0xff, 0xc0, 0x1, 0xff, 0xe0, 0x7f,
    0xf8, 0x0, 0x3f, 0xfc, 0x7, 0xff, 0x0, 0x7,
    0xff, 0x0, 0xff, 0xf0, 0x1, 0xff, 0xe0, 0xff,
    0xf0, 0x1, 0xff, 0xe0, 0x3, 0xff, 0xc0, 0x7,
    0xff, 0x0, 0x3f, 0xfc, 0x0, 0xff, 0xe0, 0x1f,
    0xfe, 0x0, 0x7f, 0xf0, 0x0, 0xff, 0xf0, 0x7,
    0xff, 0x0, 0x1f, 0xff, 0x1, 0xff, 0xe0, 0x1,
    0xff, 0xe0, 0x3f, 0xfc, 0x0, 0x3f, 0xff, 0xf,
    0xff, 0x0, 0x3, 0xff, 0xff, 0xff, 0xe0, 0x0,
    0x3f, 0xff, 0xff, 0xf8, 0x0, 0x7, 0xff, 0xff,
    0xfe, 0x0, 0x0, 0x7f, 0xff, 0xff, 0xc0, 0x0,
    0x3, 0xff, 0xff, 0xf8, 0x0, 0x0, 0x8f, 0xff,
    0xff, 0x80, 0x0, 0x7, 0xff, 0xf8, 0x0, 0x0,
    0x1, 0x3f, 0xf0, 0x0, 0x0, 0x0, 0x1f, 0xff,
    0x0, 0x0, 0x0,

    /* U+0031 "1" */
    0x0, 0x0, 0x3f, 0xf0, 0x0, 0x0, 0x0, 0xff,
    0xf0, 0x0, 0x0, 0x3, 0xff, 0xf8, 0x0, 0x0,
    0x3f, 0xff, 0xf0, 0x0, 0x0, 0x3f, 0xff, 0xfe,
    0x0, 0x3, 0xff, 0xff, 0xf0, 0x0, 0x3, 0xff,
    0xff, 0xf0, 0x0, 0x3, 0xff, 0xff, 0xf0, 0x0,
    0x3, 0xff, 0xff, 0xf0, 0x0, 0x3, 0xff, 0xff,
    0xf0, 0x0, 0x3, 0xff, 0xff, 0xf0, 0x0, 0x3,
    0xff, 0xff, 0xf0, 0x0, 0x0, 0x1, 0xff, 0xf0,
    0x0, 0x0, 0x0, 0xff, 0xf8, 0x0, 0x0, 0x1,
    0xff, 0xf0, 0x0, 0x0, 0xf, 0xff, 0x0, 0x0,
    0x0, 0x1, 0xff, 0xf0, 0x0, 0x0, 0x1, 0xff,
    0xf0, 0x0, 0x0, 0x0, 0x7f, 0xfc, 0x0, 0x0,
    0x1, 0xff, 0xf0, 0x0, 0x0, 0x1, 0xff, 0xf0,
    0x0, 0x0, 0x1, 0xff, 0xf0, 0x0, 0x0, 0x1,
    0xff, 0xf0, 0x0, 0x0, 0x1, 0xff, 0xf0, 0x0,
    0x0, 0x1, 0xff, 0xf0, 0x0, 0x0, 0x3, 0xff,
    0xe0, 0x0, 0x0, 0x1, 0xff, 0xf0, 0x0, 0x0,
    0x1, 0xff, 0xf8, 0x0, 0x0, 0x1, 0xff, 0xfc,
    0x0, 0x0, 0x3, 0xff, 0xe0, 0x0, 0x0, 0x1,
    0xff, 0xf0, 0x0, 0x0, 0x1, 0xff, 0xf0, 0x0,
    0x0, 0x1, 0xff, 0xf0, 0x0, 0x0, 0x1, 0xff,
    0xf0, 0x0, 0x0, 0x1, 0xff, 0xf0, 0x0, 0x0,
    0x1, 0xff, 0xf0, 0x0, 0x0, 0xf, 0xff, 0x80,
    0x0, 0x0, 0x0, 0xff, 0xf8, 0x0, 0x0, 0x1,
    0xff, 0xf0, 0x0, 0x0, 0x3, 0xff, 0xe0, 0x0,
    0x0, 0x1, 0xff, 0xf0, 0x0, 0x0, 0x1, 0xff,
    0xf0, 0x0, 0xf, 0xff, 0xff, 0xff, 0xf8, 0xf,
    0xff, 0xff, 0xff, 0xf8, 0xf, 0xff, 0xff, 0xff,
    0xf8, 0xf, 0xff, 0xff, 0xff, 0xf8, 0xf, 0xff,
    0xff, 0xff, 0xf8, 0x3, 0xff, 0xff, 0xff, 0xfc,
    0xf, 0xff, 0xff, 0xff, 0xf8, 0x1, 0xff, 0xff,
    0xff, 0xff, 0x3f, 0xff, 0xff, 0xff, 0xe0, 0x7f,
    0xff, 0xff, 0xff, 0xf8,

    /* U+0032 "2" */
    0x0, 0x1, 0xff, 0x0, 0x0, 0x0, 0x7, 0xff,
    0xf8, 0x0, 0x0, 0x7, 0xff, 0xff, 0x80, 0x0,
    0x3, 0xff, 0xff, 0xfc, 0x0, 0x3, 0xff, 0xff,
    0xff, 0x0, 0x0, 0x3f, 0xff, 0xff, 0xfc, 0x0,
    0xff, 0xff, 0xff, 0xfc, 0x0, 0x7f, 0xff, 0xff,
    0xff, 0x80, 0x1f, 0xff, 0xff, 0xff, 0xe0, 0x3,
    0xff, 0xff, 0xff, 0xfc, 0x0, 0x7f, 0xe0, 0x7f,
    0xff, 0x0, 0xf, 0xe0, 0x7, 0xff, 0xc0, 0x1,
    0xf0, 0x0, 0xff, 0xf8, 0x0, 0x30, 0x0, 0x3f,
    0xfe, 0x0, 0x0, 0x0, 0x7, 0xff, 0xc0, 0x0,
    0x0, 0x1, 0xff, 0xe0, 0x0, 0x0, 0x7, 0xff,
    0x80, 0x0, 0x0, 0x0, 0x1f, 0xfe, 0x0, 0x0,
    0x0, 0x7, 0xff, 0x80, 0x0, 0x0, 0x0, 0x7f,
    0xf0, 0x0, 0x0, 0x0, 0xff, 0xf0, 0x0, 0x0,
    0x0, 0x3f, 0xfc, 0x0, 0x0, 0x0, 0xf, 0xff,
    0x0, 0x0, 0x0, 0x7, 0xff, 0x80, 0x0, 0x0,
    0x1, 0xff, 0xe0, 0x0, 0x0, 0x0, 0xff, 0xf8,
    0x0, 0x0, 0x0, 0x7f, 0xfc, 0x0, 0x0, 0x0,
    0x1f, 0xff, 0x0, 0x0, 0x0, 0xf, 0xff, 0xa0,
    0x0, 0x0, 0x0, 0x7f, 0xfc, 0x0, 0x0, 0x1,
    0xff, 0xf0, 0x0, 0x0, 0x1, 0xff, 0xfc, 0x0,
    0x0, 0x0, 0x7f, 0xfe, 0x0, 0x0, 0x0, 0x3f,
    0xff, 0x0, 0x0, 0x0, 0x1f, 0xff, 0x80, 0x0,
    0x0, 0xf, 0xff, 0xc0, 0x0, 0x0, 0x7, 0xff,
    0xf0, 0x0, 0x0, 0x3, 0xff, 0xf8, 0x0, 0x0,
    0x7, 0xff, 0xfc, 0x0, 0x0, 0x0, 0x7f, 0xff,
    0x0, 0x0, 0x0, 0x7f, 0xff, 0x0, 0x0, 0x0,
    0x3f, 0xff, 0x80, 0x0, 0x0, 0x1f, 0xff, 0xc0,
    0x0, 0x0, 0xf, 0xff, 0xe7, 0xff, 0xf8, 0x7,
    0xff, 0xff, 0xff, 0xfe, 0x7, 0xff, 0xff, 0xff,
    0xff, 0x81, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x7f,
    0xff, 0xff, 0xff, 0xf8, 0xf, 0xff, 0xff, 0xff,
    0xff, 0x7, 0xff, 0xff, 0xff, 0xff, 0x80, 0x3f,
    0xff, 0xff, 0xff, 0xfd, 0xff, 0xff, 0xff, 0xff,
    0xe0, 0x7f, 0xff, 0xff, 0xff, 0xfe, 0x0,

    /* U+0033 "3" */
    0x0, 0xf, 0xfe, 0x0, 0x0, 0x3, 0xff, 0xff,
    0x0, 0x0, 0x7f, 0xff, 0xfe, 0x0, 0x7, 0xff,
    0xff, 0xfc, 0x0, 0xff, 0xff, 0xff, 0xe0, 0x3,
    0xff, 0xff, 0xff, 0xf1, 0xff, 0xff, 0xff, 0xfe,
    0x7, 0xff, 0xff, 0xff, 0xf0, 0x1f, 0xff, 0xff,
    0xff, 0xc0, 0x7f, 0xff, 0xff, 0xfe, 0x3, 0xfe,
    0x3, 0xff, 0xf8, 0xf, 0xc0, 0xf, 0xff, 0xc0,
    0x38, 0x0, 0x3f, 0xfe, 0x0, 0x80, 0x1, 0xff,
    0xf0, 0x0, 0x0, 0x7, 0xff, 0xc0, 0x0, 0x0,
    0x3f, 0xfc, 0x0, 0x0, 0x1f, 0xfe, 0x0, 0x0,
    0x0, 0x1f, 0xff, 0x0, 0x0, 0x0, 0xff, 0xf0,
    0x0, 0x0, 0x3, 0xff, 0xe0, 0x0, 0x0, 0xff,
    0xf8, 0x0, 0x0, 0x3f, 0xff, 0xc0, 0x0, 0x7f,
    0xff, 0xfc, 0x0, 0x3, 0xff, 0xff, 0xc0, 0x0,
    0x1f, 0xff, 0xf8, 0x0, 0x0, 0xff, 0xff, 0x0,
    0x0, 0xf, 0xff, 0xf8, 0x0, 0x0, 0x3f, 0xff,
    0xfc, 0x0, 0x1, 0xff, 0xff, 0xf0, 0x0, 0x1,
    0xff, 0xff, 0xf8, 0x0, 0x7f, 0xff, 0xfe, 0x0,
    0x0, 0xf, 0xff, 0xf8, 0x0, 0x0, 0x7, 0xff,
    0xe0, 0x0, 0x0, 0xf, 0xff, 0x0, 0x0, 0x0,
    0x7f, 0xfc, 0x0, 0x0, 0x1, 0xff, 0xe0, 0x0,
    0x0, 0xf, 0xff, 0x0, 0x0, 0x3, 0xff, 0xe0,
    0x0, 0x0, 0x3, 0xff, 0xe0, 0x80, 0x0, 0x1f,
    0xff, 0x1c, 0x0, 0x3, 0xff, 0xe0, 0x7c, 0x0,
    0xf, 0xff, 0x87, 0xf8, 0x0, 0xff, 0xfc, 0x7f,
    0xf0, 0x1f, 0xff, 0xe7, 0xff, 0xff, 0xff, 0xfe,
    0x3f, 0xff, 0xff, 0xff, 0xf3, 0xff, 0xff, 0xff,
    0xff, 0x9f, 0xff, 0xff, 0xff, 0xf8, 0x7f, 0xff,
    0xff, 0xff, 0x80, 0x7f, 0xff, 0xff, 0xfe, 0x4,
    0xff, 0xff, 0xff, 0xf0, 0xf, 0xff, 0xff, 0xf0,
    0x0, 0xbf, 0xff, 0xfe, 0x0, 0x0, 0x3, 0xff,
    0x0, 0x0,

    /* U+0034 "4" */
    0x0, 0x0, 0xf, 0xff, 0xf0, 0x0, 0x0, 0x7,
    0xff, 0xfc, 0x0, 0x0, 0x3, 0xff, 0xff, 0x0,
    0x0, 0x0, 0xff, 0xff, 0xc0, 0x0, 0x0, 0xf,
    0xff, 0xfc, 0x0, 0x0, 0x1f, 0xff, 0xfc, 0x0,
    0x0, 0xf, 0xff, 0xff, 0x0, 0x0, 0x3, 0xff,
    0xff, 0xc0, 0x0, 0x1, 0xff, 0xff, 0xf0, 0x0,
    0x0, 0xff, 0xff, 0xfc, 0x0, 0x0, 0x3f, 0xf7,
    0xff, 0x0, 0x0, 0x1f, 0xf9, 0xff, 0xc0, 0x0,
    0x7, 0xfe, 0x7f, 0xf0, 0x0, 0x1, 0xff, 0x8f,
    0xfe, 0x0, 0x1, 0xff, 0xc7, 0xff, 0x0, 0x7,
    0xff, 0x1f, 0xfe, 0x0, 0x0, 0x3f, 0xf8, 0xff,
    0xf0, 0x0, 0xf, 0xfe, 0x3f, 0xfc, 0x0, 0x1,
    0xff, 0xc1, 0xff, 0xc0, 0x3, 0xff, 0xc3, 0xff,
    0xc0, 0x0, 0xff, 0xe0, 0xff, 0xf0, 0x0, 0x7f,
    0xf8, 0x3f, 0xfc, 0x0, 0x1f, 0xfc, 0xf, 0xff,
    0x0, 0xf, 0xfe, 0x3, 0xff, 0xc0, 0x3, 0xff,
    0x80, 0xff, 0xf0, 0x3, 0xff, 0xc0, 0x7f, 0xf8,
    0x0, 0x7f, 0xf0, 0xf, 0xff, 0x0, 0x3f, 0xfa,
    0x3, 0xff, 0xe0, 0x1, 0xff, 0xc0, 0xf, 0xfe,
    0x7, 0xff, 0x0, 0x3f, 0xfc, 0x7, 0xff, 0xff,
    0xff, 0xff, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xfe,
    0x3f, 0xff, 0xff, 0xff, 0xff, 0x8f, 0xff, 0xff,
    0xff, 0xff, 0xe3, 0xff, 0xff, 0xff, 0xff, 0xf8,
    0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff,
    0xff, 0xfe, 0xf, 0xff, 0xff, 0xff, 0xff, 0xe3,
    0xff, 0xff, 0xff, 0xff, 0xf8, 0x0, 0x0, 0x7,
    0xff, 0x80, 0x0, 0x0, 0x0, 0xff, 0xf0, 0x0,
    0x0, 0x0, 0x3f, 0xfc, 0x0, 0x0, 0x0, 0xf,
    0xff, 0x0, 0x0, 0x0, 0x3, 0xff, 0xc0, 0x0,
    0x0, 0x0, 0xff, 0xf0, 0x0, 0x0, 0x0, 0x3f,
    0xfc, 0x0, 0x0, 0x0, 0xf, 0xff, 0x0, 0x0,
    0x0, 0x0, 0xff, 0xe0, 0x0, 0x0, 0x0, 0xff,
    0xf0, 0x0, 0x0, 0x0, 0x3f, 0xff, 0x0, 0x0,
    0x0, 0x3f, 0xfc, 0x0, 0x0, 0x0, 0x1f, 0xff,
    0xc0,

    /* U+0035 "5" */
    0x7, 0xff, 0xff, 0xff, 0xe0, 0x7, 0xff, 0xff,
    0xff, 0xe0, 0x3, 0xff, 0xff, 0xff, 0xf0, 0x7,
    0xff, 0xff, 0xff, 0xe0, 0x0, 0xff, 0xff, 0xff,
    0xf8, 0x7, 0xff, 0xff, 0xff, 0xe0, 0x7, 0xff,
    0xff, 0xff, 0xe0, 0x7, 0xff, 0xff, 0xff, 0xe0,
    0x7, 0xff, 0xff, 0xff, 0xe0, 0x7, 0xff, 0xff,
    0xff, 0xe0, 0x7, 0xff, 0x0, 0x0, 0x0, 0x7,
    0xff, 0x0, 0x0, 0x0, 0xf, 0xff, 0x0, 0x0,
    0x0, 0x7, 0xff, 0x0, 0x0, 0x0, 0xf, 0xfe,
    0x0, 0x0, 0x0, 0x7f, 0xf0, 0x0, 0x0, 0x0,
    0xf, 0xfe, 0x0, 0x0, 0x0, 0xf, 0xfe, 0x0,
    0x0, 0x0, 0x3, 0xff, 0x8f, 0xc0, 0x0, 0xf,
    0xff, 0xff, 0xe0, 0x0, 0xf, 0xff, 0xff, 0xf8,
    0x0, 0xf, 0xff, 0xff, 0xfe, 0x0, 0xf, 0xff,
    0xff, 0xff, 0x0, 0xf, 0xff, 0xff, 0xff, 0x80,
    0xf, 0xff, 0xff, 0xff, 0xc0, 0x1f, 0xff, 0xff,
    0xff, 0xc0, 0x7, 0xff, 0xff, 0xff, 0xe0, 0x3,
    0xf8, 0xf, 0xff, 0xf8, 0x0, 0xc4, 0x3, 0xff,
    0xfe, 0x0, 0x0, 0x7, 0xff, 0xe0, 0x0, 0x0,
    0x1, 0xff, 0xf8, 0x0, 0x0, 0x1, 0xff, 0xf8,
    0x0, 0x0, 0x0, 0xff, 0xf8, 0x0, 0x0, 0x0,
    0xff, 0xf8, 0x0, 0x0, 0x0, 0xff, 0xf8, 0x0,
    0x0, 0x0, 0xff, 0xf8, 0x0, 0x0, 0x7, 0xff,
    0xc0, 0x0, 0x0, 0x0, 0xff, 0xf8, 0x4, 0x0,
    0x1, 0xff, 0xf8, 0x1c, 0x0, 0x3, 0xff, 0xe0,
    0x1f, 0x80, 0x3, 0xff, 0xf0, 0x1f, 0xc0, 0x7,
    0xff, 0xf0, 0x3f, 0xf8, 0x1f, 0xff, 0xe0, 0x7f,
    0xff, 0xff, 0xff, 0xe0, 0x7f, 0xff, 0xff, 0xff,
    0xc0, 0xff, 0xff, 0xff, 0xff, 0xc0, 0xff, 0xff,
    0xff, 0xff, 0x80, 0x7f, 0xff, 0xff, 0xff, 0x0,
    0x1f, 0xff, 0xff, 0xfe, 0x80, 0x1, 0xff, 0xff,
    0xff, 0x80, 0x3, 0xff, 0xff, 0xf8, 0x0, 0x4,
    0x7f, 0xff, 0xc0, 0x0, 0x1, 0xcf, 0xfc, 0x0,
    0x0,

    /* U+0036 "6" */
    0x0, 0x0, 0x3f, 0xe0, 0x0, 0x0, 0x0, 0xff,
    0xfe, 0x0, 0x0, 0x1, 0xff, 0xff, 0xe0, 0x0,
    0x1, 0xff, 0xff, 0xfc, 0x0, 0x3, 0xff, 0xff,
    0xfe, 0x0, 0x0, 0x7f, 0xff, 0xff, 0xf0, 0x3,
    0xff, 0xff, 0xff, 0xe0, 0x3, 0xff, 0xff, 0xff,
    0xe0, 0x1, 0xff, 0xff, 0xff, 0xe0, 0x1, 0xff,
    0xff, 0xff, 0xf0, 0x1, 0xff, 0xfc, 0xf, 0xf0,
    0x0, 0xff, 0xf8, 0x1, 0xf0, 0x0, 0xff, 0xf8,
    0x0, 0x30, 0x0, 0x7f, 0xf8, 0x0, 0x0, 0x0,
    0x3f, 0xfc, 0x0, 0x0, 0x0, 0x3f, 0xfc, 0x0,
    0x0, 0x0, 0xff, 0xe0, 0x0, 0x0, 0x0, 0xf,
    0xfe, 0x0, 0x0, 0x0, 0x7, 0xff, 0x0, 0x0,
    0x0, 0x0, 0xff, 0xe0, 0x0, 0x0, 0x3, 0xff,
    0xc0, 0xff, 0x0, 0x1, 0xff, 0xc3, 0xff, 0xf0,
    0x0, 0xff, 0xe3, 0xff, 0xfc, 0x0, 0x7f, 0xf7,
    0xff, 0xff, 0x80, 0x3f, 0xff, 0xff, 0xff, 0xe0,
    0x1f, 0xff, 0xff, 0xff, 0xf0, 0xf, 0xff, 0xff,
    0xff, 0xfc, 0x3, 0xff, 0xff, 0xff, 0xff, 0x83,
    0xff, 0xff, 0xff, 0xff, 0x80, 0x3f, 0xff, 0xe0,
    0xff, 0xf9, 0xff, 0xf8, 0x3, 0xff, 0xc0, 0x7f,
    0xfc, 0x0, 0x7f, 0xf8, 0x3f, 0xfc, 0x0, 0x3f,
    0xfc, 0x1f, 0xfc, 0x0, 0xf, 0xfe, 0xf, 0xfe,
    0x0, 0x7, 0xff, 0x7, 0xff, 0x80, 0x3, 0xff,
    0x81, 0xff, 0xc0, 0x1, 0xff, 0xc7, 0xff, 0x0,
    0x7, 0xff, 0x0, 0x3f, 0xf0, 0x0, 0x7f, 0xf0,
    0x3f, 0xfc, 0x0, 0x3f, 0xf8, 0x3f, 0xfc, 0x0,
    0x7f, 0xf0, 0x7, 0xff, 0x80, 0x1f, 0xfe, 0x1,
    0xff, 0xc0, 0x1f, 0xfe, 0x0, 0xff, 0xf0, 0xf,
    0xff, 0x0, 0x3f, 0xfe, 0x1f, 0xff, 0x0, 0x1f,
    0xff, 0xff, 0xff, 0x80, 0x7, 0xff, 0xff, 0xff,
    0x80, 0x1, 0xff, 0xff, 0xff, 0x80, 0x0, 0x3f,
    0xff, 0xff, 0xe0, 0x0, 0x3f, 0xff, 0xff, 0xc0,
    0x0, 0x1, 0xff, 0xff, 0xf8, 0x0, 0x3, 0xff,
    0xff, 0xc0, 0x0, 0x2, 0x7f, 0xff, 0x80, 0x0,
    0x0, 0x41, 0xf8, 0x0, 0x0,

    /* U+0037 "7" */
    0xff, 0xff, 0xff, 0xff, 0xf1, 0xff, 0xff, 0xff,
    0xff, 0xe3, 0xff, 0xff, 0xff, 0xff, 0xc7, 0xff,
    0xff, 0xff, 0xff, 0x81, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xfe, 0x3f, 0xff, 0xff,
    0xff, 0xfc, 0x7f, 0xff, 0xff, 0xff, 0xf8, 0xff,
    0xff, 0xff, 0xff, 0xe1, 0xff, 0xff, 0xff, 0xff,
    0x80, 0x0, 0x0, 0xf, 0xfe, 0x0, 0x0, 0x0,
    0x3f, 0xfc, 0x0, 0x0, 0x0, 0x7f, 0xf0, 0x0,
    0x0, 0x0, 0xff, 0xe0, 0x0, 0x0, 0x7, 0xff,
    0x0, 0x0, 0x0, 0xff, 0xe0, 0x0, 0x0, 0x0,
    0x3f, 0xf8, 0x0, 0x0, 0x0, 0xff, 0xf0, 0x0,
    0x0, 0x0, 0x7f, 0xf0, 0x0, 0x0, 0x7, 0xff,
    0x80, 0x0, 0x0, 0xf, 0xfe, 0x0, 0x0, 0x0,
    0x3f, 0xfc, 0x0, 0x0, 0x0, 0x7f, 0xf0, 0x0,
    0x0, 0x0, 0xff, 0xe0, 0x0, 0x0, 0x3, 0xff,
    0x80, 0x0, 0x0, 0xf, 0xfe, 0x0, 0x0, 0x0,
    0x1f, 0xfe, 0x0, 0x0, 0x0, 0x3f, 0xfe, 0x0,
    0x0, 0x0, 0xf, 0xff, 0x0, 0x0, 0x3, 0xff,
    0xe0, 0x0, 0x0, 0x3, 0xff, 0xc0, 0x0, 0x0,
    0x7, 0xff, 0x0, 0x0, 0x0, 0x1f, 0xfe, 0x0,
    0x0, 0x0, 0x3f, 0xfc, 0x0, 0x0, 0x0, 0x7f,
    0xf8, 0x0, 0x0, 0x0, 0xff, 0xf0, 0x0, 0x0,
    0xf, 0xff, 0x0, 0x0, 0x0, 0x7, 0xff, 0x80,
    0x0, 0x0, 0xf, 0xff, 0x80, 0x0, 0x0, 0x3f,
    0xfc, 0x0, 0x0, 0x0, 0x3f, 0xfc, 0x0, 0x0,
    0x0, 0x7f, 0xf8, 0x0, 0x0, 0x0, 0xff, 0xf0,
    0x0, 0x0, 0x3, 0xff, 0xe0, 0x0, 0x0, 0x7,
    0xff, 0xc0, 0x0, 0x0, 0xf, 0xff, 0x80, 0x0,
    0x0, 0x1f, 0xff, 0x0, 0x0, 0x0, 0x3f, 0xfe,
    0x0, 0x0, 0x0, 0x7f, 0xfe, 0x0, 0x0, 0x0,
    0x1f, 0xfe, 0x0, 0x0, 0x7, 0xff, 0x80, 0x0,
    0x0, 0x1f, 0xff, 0xc0, 0x0, 0x0,

    /* U+0038 "8" */
    0x0, 0x0, 0xff, 0xc0, 0x0, 0x0, 0x7, 0xff,
    0xfc, 0x0, 0x0, 0x1f, 0xff, 0xff, 0x0, 0x0,
    0x3f, 0xff, 0xff, 0x80, 0x0, 0x7f, 0xff, 0xff,
    0xc0, 0x0, 0x1f, 0xff, 0xff, 0xf8, 0x1, 0xff,
    0xff, 0xff, 0xe0, 0x1, 0xff, 0xff, 0xff, 0xf0,
    0x3, 0xff, 0xe0, 0xff, 0xf0, 0x3, 0xff, 0xc0,
    0x3f, 0xf8, 0x7, 0xff, 0x80, 0x3f, 0xf8, 0x7,
    0xff, 0x0, 0x1f, 0xf8, 0x7, 0xff, 0x0, 0x1f,
    0xf8, 0x7, 0xff, 0x0, 0x1f, 0xf8, 0x3, 0xff,
    0x80, 0xf, 0xfc, 0x7, 0xff, 0x0, 0x1f, 0xf8,
    0x3f, 0xf8, 0x0, 0xff, 0x80, 0x7, 0xff, 0x80,
    0x1f, 0xf8, 0x3, 0xff, 0xc0, 0x1f, 0xf0, 0x0,
    0xff, 0xf8, 0xf, 0xfc, 0x1, 0xff, 0xf8, 0x3f,
    0xe0, 0x1, 0xff, 0xfe, 0x7f, 0xe0, 0x0, 0xff,
    0xff, 0xff, 0xc0, 0x0, 0x7f, 0xff, 0xff, 0x80,
    0x0, 0x3f, 0xff, 0xff, 0x0, 0x0, 0x1f, 0xff,
    0xfe, 0x0, 0x0, 0x1f, 0xff, 0xff, 0x0, 0x0,
    0xf, 0xff, 0xff, 0xe0, 0x0, 0x7f, 0xff, 0xff,
    0xe0, 0x0, 0xf, 0xff, 0xff, 0xfe, 0x7, 0xff,
    0x3f, 0xff, 0xe0, 0x3, 0xff, 0x7, 0xff, 0xf8,
    0x7, 0xff, 0x1, 0xff, 0xfc, 0x7, 0xfe, 0x0,
    0x7f, 0xfc, 0xf, 0xfe, 0x0, 0x3f, 0xfe, 0xf,
    0xfc, 0x0, 0x1f, 0xfe, 0x1f, 0xfc, 0x0, 0xf,
    0xfe, 0xff, 0xe0, 0x0, 0x7f, 0xf0, 0x1f, 0xfc,
    0x0, 0xf, 0xfe, 0x1f, 0xfe, 0x0, 0xf, 0xff,
    0x3f, 0xf8, 0x0, 0x1f, 0xfc, 0x1f, 0xfe, 0x0,
    0xf, 0xfe, 0x1f, 0xfe, 0x0, 0xf, 0xfe, 0xf,
    0xff, 0x0, 0x1f, 0xfe, 0xf, 0xff, 0x80, 0x3f,
    0xfc, 0xf, 0xff, 0xe0, 0xff, 0xfc, 0x7, 0xff,
    0xff, 0xff, 0xf8, 0x3, 0xff, 0xff, 0xff, 0xf8,
    0x3, 0xff, 0xff, 0xff, 0xf0, 0x1, 0xff, 0xff,
    0xff, 0xe8, 0x0, 0xf, 0xff, 0xff, 0xf8, 0x0,
    0x3f, 0xff, 0xff, 0x80, 0x0, 0x5f, 0xff, 0x80,
    0x0, 0x0, 0x0, 0x7f, 0xc0, 0x0,

    /* U+0039 "9" */
    0x0, 0x1, 0xff, 0x80, 0x0, 0x0, 0x0, 0xff,
    0xfe, 0x0, 0x0, 0x0, 0x7f, 0xff, 0xf0, 0x0,
    0x0, 0x1f, 0xff, 0xff, 0x80, 0x0, 0xf, 0xff,
    0xff, 0xf0, 0x0, 0x0, 0x7f, 0xff, 0xff, 0xe0,
    0x0, 0x7f, 0xff, 0xff, 0xf0, 0x0, 0x1f, 0xff,
    0xff, 0xfe, 0x0, 0x7, 0xff, 0xff, 0xff, 0xe0,
    0x0, 0xff, 0xf8, 0x3f, 0xfe, 0x0, 0x3f, 0xfe,
    0x3, 0xff, 0xc0, 0x7, 0xff, 0x80, 0x3f, 0xfc,
    0x0, 0xff, 0xe0, 0x3, 0xff, 0x80, 0x3f, 0xfc,
    0x0, 0x7f, 0xf8, 0x3, 0xff, 0x80, 0x7, 0xff,
    0x80, 0xff, 0xe0, 0x0, 0xff, 0xe0, 0xff, 0xe0,
    0x0, 0xff, 0xe0, 0x3, 0xff, 0x80, 0x3, 0xff,
    0xc0, 0x7f, 0xf0, 0x0, 0x3f, 0xf8, 0x3, 0xff,
    0x80, 0x1, 0xff, 0xc1, 0xff, 0xe0, 0x0, 0xff,
    0xe0, 0x3f, 0xfc, 0x0, 0x3f, 0xfc, 0x3, 0xff,
    0xc0, 0xf, 0xff, 0x80, 0x7f, 0xf8, 0x3, 0xff,
    0xf0, 0xf, 0xff, 0xc1, 0xff, 0xfe, 0x0, 0xff,
    0xff, 0xff, 0xff, 0xc0, 0x3f, 0xff, 0xff, 0xff,
    0xfc, 0x0, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x3f,
    0xff, 0xff, 0xff, 0xe0, 0x0, 0x7f, 0xff, 0xfd,
    0xff, 0xc0, 0x7f, 0xff, 0xe7, 0xff, 0x0, 0x3,
    0xff, 0xfc, 0x7f, 0xf0, 0x0, 0x1f, 0xff, 0xf,
    0xfe, 0x0, 0x0, 0x7f, 0x1, 0xff, 0xc0, 0x0,
    0x0, 0x0, 0x7f, 0xf8, 0x0, 0x0, 0x0, 0xf,
    0xfe, 0x0, 0x0, 0x0, 0x1, 0xff, 0xc0, 0x0,
    0x0, 0x1, 0xff, 0xc0, 0x0, 0x0, 0x0, 0x7,
    0xff, 0x0, 0x0, 0x0, 0x1, 0xff, 0xc0, 0x0,
    0x0, 0x0, 0xff, 0xf0, 0x0, 0xe, 0x0, 0x1f,
    0xff, 0x0, 0x3, 0xe0, 0x7, 0xff, 0xc0, 0x0,
    0xff, 0x3, 0xff, 0xf8, 0x0, 0x3f, 0xff, 0xff,
    0xfe, 0x0, 0xf, 0xff, 0xff, 0xff, 0x80, 0x3,
    0xff, 0xff, 0xff, 0xe0, 0x0, 0x7f, 0xff, 0xff,
    0xfc, 0x0, 0x3, 0xff, 0xff, 0xff, 0x80, 0x0,
    0x7f, 0xff, 0xff, 0xc0, 0x0, 0x0, 0xff, 0xff,
    0xfe, 0x0, 0x0, 0x7f, 0xff, 0xe0, 0x0, 0x0,
    0x11, 0xff, 0xc0, 0x0, 0x0, 0x0, 0xff, 0xfc,
    0x0, 0x0, 0x0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc_mkb_70[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 256, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 682, .box_w = 43, .box_h = 54, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 292, .adv_w = 682, .box_w = 40, .box_h = 52, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 552, .adv_w = 682, .box_w = 42, .box_h = 53, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 831, .adv_w = 682, .box_w = 37, .box_h = 54, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 1081, .adv_w = 682, .box_w = 42, .box_h = 52, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1354, .adv_w = 682, .box_w = 40, .box_h = 53, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 1619, .adv_w = 682, .box_w = 41, .box_h = 54, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1896, .adv_w = 682, .box_w = 39, .box_h = 52, .ofs_x = 4, .ofs_y = 0},
    {.bitmap_index = 2150, .adv_w = 682, .box_w = 40, .box_h = 54, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 2420, .adv_w = 682, .box_w = 43, .box_h = 54, .ofs_x = 0, .ofs_y = -1}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/



/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps_mkb_70[] =
{
    {
        .range_start = 32, .range_length = 1, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 48, .range_length = 10, .glyph_id_start = 2,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LV_VERSION_CHECK(8, 0, 0)
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
static const lv_font_fmt_txt_dsc_t font_dsc_mkb_70 = {
#else
static lv_font_fmt_txt_dsc_t font_dsc_mkb_70 = {
#endif
    .glyph_bitmap = glyph_bitmap_mkb_70,
    .glyph_dsc = glyph_dsc_mkb_70,
    .cmaps = cmaps_mkb_70,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 2,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LV_VERSION_CHECK(8, 0, 0)
    .cache = &cache
#endif
};


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LV_VERSION_CHECK(8, 0, 0)
const lv_font_t mkb_70 = {
#else
lv_font_t mkb_70 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 54,          /*The maximum line height required by the font*/
    .base_line = 1,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -5,
    .underline_thickness = 4,
#endif
    .dsc = &font_dsc_mkb_70           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};



#endif /*#if MKB_70*/
