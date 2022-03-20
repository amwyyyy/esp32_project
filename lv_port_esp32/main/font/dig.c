/*******************************************************************************
 * Size: 36 px
 * Bpp: 4
 * Opts: 
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef DIG
#define DIG 1
#endif

#if DIG

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0030 "0" */
    0x0, 0x0, 0x5, 0xbe, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xfe, 0xb5, 0x0, 0x0, 0x0, 0x0, 0x2c,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xfc, 0x10, 0x0, 0x0, 0x2e, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x20,
    0x0, 0xd, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xfd, 0x0, 0x7, 0xff,
    0xff, 0xe7, 0x44, 0x44, 0x44, 0x44, 0x44, 0x47,
    0xef, 0xff, 0xf6, 0x0, 0xdf, 0xff, 0xe1, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x2, 0xef, 0xff,
    0xc0, 0xf, 0xff, 0xf6, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x2c, 0xff, 0xff, 0x1, 0xff,
    0xff, 0x30, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3,
    0xaf, 0xff, 0xff, 0xf1, 0x2f, 0xff, 0xf3, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x3b, 0xff, 0xff, 0xff,
    0xff, 0x12, 0xff, 0xff, 0x30, 0x0, 0x0, 0x0,
    0x4, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xf1, 0x2f,
    0xff, 0xf3, 0x0, 0x0, 0x0, 0x5c, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0x12, 0xff, 0xff, 0x30,
    0x0, 0x5, 0xdf, 0xff, 0xff, 0xff, 0xfa, 0x6f,
    0xff, 0xf1, 0x2f, 0xff, 0xf3, 0x0, 0x6d, 0xff,
    0xff, 0xff, 0xff, 0x92, 0x3, 0xff, 0xff, 0x12,
    0xff, 0xff, 0x46, 0xef, 0xff, 0xff, 0xff, 0xf9,
    0x10, 0x0, 0x3f, 0xff, 0xf1, 0x2f, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0x81, 0x0, 0x0, 0x3,
    0xff, 0xff, 0x12, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xe7, 0x10, 0x0, 0x0, 0x0, 0x3f, 0xff, 0xf1,
    0x2f, 0xff, 0xff, 0xff, 0xfe, 0x70, 0x0, 0x0,
    0x0, 0x0, 0x3, 0xff, 0xff, 0x11, 0xff, 0xff,
    0xff, 0xd6, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x3f, 0xff, 0xf1, 0xf, 0xff, 0xfd, 0x50, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x6, 0xff, 0xff,
    0x0, 0xdf, 0xff, 0xe1, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x1, 0xef, 0xff, 0xc0, 0x7, 0xff,
    0xff, 0xe7, 0x44, 0x44, 0x44, 0x44, 0x44, 0x47,
    0xef, 0xff, 0xf6, 0x0, 0xd, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfd,
    0x0, 0x0, 0x2e, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xfe, 0x20, 0x0, 0x0,
    0x2d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xfd, 0x20, 0x0, 0x0, 0x0, 0x6, 0xbe,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xb5, 0x0,
    0x0, 0x0,

    /* U+0031 "1" */
    0x0, 0x0, 0x0, 0x0, 0xaf, 0xb1, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0xbf, 0xff, 0x90,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xbf, 0xff,
    0xfb, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xaf,
    0xff, 0xff, 0xc0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0xaf, 0xff, 0xff, 0xfc, 0x0, 0x0, 0x0, 0x0,
    0x0, 0xaf, 0xff, 0xff, 0xff, 0xc0, 0x0, 0x0,
    0x0, 0x0, 0x4f, 0xff, 0xff, 0xff, 0xfc, 0x0,
    0x0, 0x0, 0x0, 0x7, 0xff, 0xff, 0xef, 0xff,
    0xc0, 0x0, 0x0, 0x0, 0x0, 0x1e, 0xff, 0x89,
    0xff, 0xfc, 0x0, 0x0, 0x0, 0x0, 0x0, 0x17,
    0x40, 0x9f, 0xff, 0xc0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x9, 0xff, 0xfc, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x9f, 0xff, 0xc0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x9, 0xff, 0xfc,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x9f,
    0xff, 0xc0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x9, 0xff, 0xfc, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x9f, 0xff, 0xc0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x9, 0xff, 0xfc, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x9f, 0xff, 0xc0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x9, 0xff,
    0xfc, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x9f, 0xff, 0xc0, 0x0, 0x0, 0x0, 0x0, 0x24,
    0x44, 0x4a, 0xff, 0xfd, 0x44, 0x44, 0x41, 0x0,
    0x8f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xf5, 0xf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xd0, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xfc, 0x5, 0xef, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xfd, 0x30,

    /* U+0032 "2" */
    0x4, 0xef, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xfe, 0xb5, 0x0, 0x0, 0x0, 0xef, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xfc, 0x10, 0x0, 0xf, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x20,
    0x0, 0x8f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xfd, 0x0, 0x0, 0x13,
    0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x36,
    0xef, 0xff, 0xf6, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x2, 0xef, 0xff,
    0xc0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x7, 0xff, 0xff, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x3f, 0xff, 0xf2, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x6, 0xff,
    0xff, 0x10, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x1, 0xef, 0xff, 0xd0, 0x0,
    0x0, 0x0, 0x2, 0x33, 0x33, 0x33, 0x33, 0x33,
    0x35, 0xdf, 0xff, 0xf7, 0x0, 0x0, 0x1, 0x9e,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xfe, 0x0, 0x0, 0x5, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x30, 0x0,
    0x5, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xfe, 0x30, 0x0, 0x1, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc6,
    0x0, 0x0, 0x0, 0x9f, 0xff, 0xfb, 0x20, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0xe, 0xff, 0xfc, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xff, 0xff,
    0x50, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0xf, 0xff, 0xf6, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0xdf, 0xff, 0xd1, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x7, 0xff,
    0xff, 0xd6, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44,
    0x44, 0x44, 0x20, 0x0, 0xe, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x80, 0x0, 0x3e, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x10, 0x0,
    0x2d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xf0, 0x0, 0x0, 0x6, 0xbe,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xe5, 0x0,

    /* U+0033 "3" */
    0x4, 0xef, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xfe, 0xb5, 0x0, 0x0, 0x0, 0xef, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xfc, 0x10, 0x0, 0xf, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x20,
    0x0, 0x8f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xfd, 0x0, 0x0, 0x24,
    0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x47,
    0xef, 0xff, 0xf6, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x2, 0xef, 0xff,
    0xc0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x7, 0xff, 0xff, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x4f, 0xff, 0xf1, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0xff,
    0xff, 0x10, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x3f, 0xff, 0xf1, 0x0,
    0x0, 0x1, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33,
    0x33, 0x36, 0xff, 0xff, 0x10, 0x0, 0x4, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xf1, 0x0, 0x0, 0xcf, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x10,
    0x0, 0xb, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xf1, 0x0, 0x0, 0x2c,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0x10, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x3f, 0xff, 0xf1,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x3, 0xff, 0xff, 0x10, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x3f, 0xff, 0xf1, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x5, 0xff, 0xff,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x1, 0xdf, 0xff, 0xd0, 0x0, 0x13,
    0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x36,
    0xdf, 0xff, 0xf7, 0x0, 0x7f, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfd,
    0x0, 0xf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xfe, 0x20, 0x0, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xfd, 0x20, 0x0, 0x5, 0xef, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xb6, 0x0,
    0x0, 0x0,

    /* U+0034 "4" */
    0x2, 0xab, 0x30, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x6b, 0x80, 0x0, 0x0, 0x0, 0x0, 0xef, 0xff,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x4f, 0xff, 0x80,
    0x0, 0x0, 0x0, 0x1f, 0xff, 0xf3, 0x0, 0x0,
    0x0, 0x0, 0x8, 0xff, 0xfc, 0x0, 0x0, 0x0,
    0x2, 0xff, 0xff, 0x30, 0x0, 0x0, 0x0, 0x0,
    0x8f, 0xff, 0xc0, 0x0, 0x0, 0x0, 0x2f, 0xff,
    0xf3, 0x0, 0x0, 0x0, 0x0, 0x8, 0xff, 0xfc,
    0x0, 0x0, 0x0, 0x2, 0xff, 0xff, 0x30, 0x0,
    0x0, 0x0, 0x0, 0x8f, 0xff, 0xc0, 0x0, 0x0,
    0x0, 0x2f, 0xff, 0xf3, 0x0, 0x0, 0x0, 0x0,
    0x8, 0xff, 0xfc, 0x0, 0x0, 0x0, 0x2, 0xff,
    0xff, 0x30, 0x0, 0x0, 0x0, 0x0, 0x8f, 0xff,
    0xc0, 0x0, 0x0, 0x0, 0x2f, 0xff, 0xf3, 0x0,
    0x0, 0x0, 0x0, 0x8, 0xff, 0xfc, 0x0, 0x0,
    0x0, 0x2, 0xff, 0xff, 0x30, 0x0, 0x0, 0x0,
    0x0, 0x8f, 0xff, 0xc0, 0x0, 0x0, 0x0, 0x1f,
    0xff, 0xf3, 0x0, 0x0, 0x0, 0x0, 0x8, 0xff,
    0xfc, 0x0, 0x0, 0x0, 0x0, 0xff, 0xff, 0x60,
    0x0, 0x0, 0x0, 0x0, 0x8f, 0xff, 0xc0, 0x0,
    0x0, 0x0, 0xd, 0xff, 0xfd, 0x10, 0x0, 0x0,
    0x0, 0x8, 0xff, 0xfc, 0x0, 0x0, 0x0, 0x0,
    0x7f, 0xff, 0xfe, 0x64, 0x44, 0x44, 0x44, 0xaf,
    0xff, 0xd4, 0x44, 0x42, 0x0, 0x0, 0xef, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xf8, 0x0, 0x3, 0xef, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0,
    0x0, 0x2, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0x0, 0x0, 0x0,
    0x6b, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xfe, 0x40, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x8, 0xff, 0xfc, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x8f, 0xff, 0xc0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x8, 0xff, 0xfc,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x8f, 0xff, 0xc0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x8, 0xff, 0xfc, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x6f, 0xff,
    0xa0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0xaf, 0xc1, 0x0, 0x0,
    0x0, 0x0,

    /* U+0035 "5" */
    0x0, 0x2b, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xe4, 0x0, 0x2f, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xf0, 0xc, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x1f, 0xff,
    0xf7, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44,
    0x44, 0x44, 0x20, 0x2, 0xff, 0xff, 0x30, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x2f, 0xff, 0xf3, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x2, 0xff,
    0xff, 0x30, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x2f, 0xff, 0xf3, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x2, 0xff, 0xff, 0x30, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1f,
    0xff, 0xf7, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44,
    0x43, 0x0, 0x0, 0x0, 0x1, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xb4,
    0x0, 0x0, 0xc, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfa, 0x0, 0x0,
    0x2f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xfa, 0x0, 0x0, 0x2b, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xf5, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x18, 0xff, 0xff, 0xc0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x8, 0xff, 0xff, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x3f, 0xff, 0xf1, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x6, 0xff, 0xff,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x1, 0xdf, 0xff, 0xd0, 0x0, 0x24,
    0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x46,
    0xef, 0xff, 0xf7, 0x0, 0x8f, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe,
    0x0, 0xf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xfe, 0x20, 0x0, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xfd, 0x20, 0x0, 0x5, 0xef, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xb6, 0x0,
    0x0, 0x0,

    /* U+0036 "6" */
    0x0, 0x0, 0x5, 0xbe, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xfe, 0xb5, 0x0, 0x0, 0x0, 0x0, 0x2c,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xfc, 0x10, 0x0, 0x0, 0x2e, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfd, 0x10,
    0x0, 0xd, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xfc, 0x0, 0x7, 0xff,
    0xff, 0xe7, 0x44, 0x44, 0x44, 0x44, 0x44, 0x47,
    0xff, 0xff, 0xf7, 0x0, 0xdf, 0xff, 0xe2, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x2, 0xef, 0xff,
    0xe0, 0xf, 0xff, 0xf7, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x5, 0xff, 0xff, 0x11, 0xff,
    0xff, 0x40, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0xe, 0xff, 0xd0, 0x2f, 0xff, 0xf3, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3b,
    0xa2, 0x2, 0xff, 0xff, 0x30, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x2f,
    0xff, 0xf6, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44,
    0x32, 0x0, 0x0, 0x0, 0x2, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x92,
    0x0, 0x0, 0x2f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf5, 0x0, 0x2,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xf5, 0x0, 0x2f, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xf1, 0x2, 0xff, 0xff, 0x30, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x2b, 0xff, 0xff, 0x90,
    0x2f, 0xff, 0xf3, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0xb, 0xff, 0xfe, 0x1, 0xff, 0xff,
    0x30, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x5f, 0xff, 0xf1, 0xf, 0xff, 0xf6, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0xff, 0xff,
    0x10, 0xdf, 0xff, 0xd1, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0xaf, 0xff, 0xf0, 0x7, 0xff,
    0xff, 0xd6, 0x44, 0x44, 0x44, 0x44, 0x44, 0x46,
    0xcf, 0xff, 0xfa, 0x0, 0xd, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x30, 0x0, 0x2e, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0x70, 0x0, 0x0,
    0x2d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0x60, 0x0, 0x0, 0x0, 0x6, 0xbe,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc7, 0x10,
    0x0, 0x0,

    /* U+0037 "7" */
    0x5, 0xef, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xfe, 0xb5, 0x0, 0x0, 0x0, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xfc, 0x20, 0x0, 0xf, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x20,
    0x0, 0x8f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xfd, 0x0, 0x0, 0x24,
    0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x47,
    0xef, 0xff, 0xf6, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x2, 0xef, 0xff,
    0xd0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x6, 0xff, 0xff, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x7f, 0xff, 0xf0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x2, 0x8f, 0xff,
    0xfc, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x29, 0xdf, 0xff, 0xff, 0xff, 0x50, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x8f, 0xff,
    0xff, 0xff, 0xff, 0xa0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0xaf, 0xff, 0xff, 0xff, 0xff,
    0xa0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x8f, 0xff, 0xff, 0xff, 0xfb, 0x40, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x2f, 0xff, 0xff,
    0xa5, 0x30, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0xa, 0xff, 0xff, 0x60, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0xff, 0xff, 0xa0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x4f, 0xff, 0xf3,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x7, 0xff, 0xff, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0xbf, 0xff, 0xb0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0xf, 0xff, 0xf7,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x3, 0xff, 0xff, 0x40, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x7f, 0xff, 0xf0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x9, 0xff, 0xfc,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x6f, 0xff, 0x70, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x7b, 0x80, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0,

    /* U+0038 "8" */
    0x0, 0x0, 0x5, 0xbe, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xfe, 0xb5, 0x0, 0x0, 0x0, 0x0, 0x1c,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xfc, 0x10, 0x0, 0x0, 0x1e, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfd, 0x10,
    0x0, 0xc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xfc, 0x0, 0x6, 0xff,
    0xff, 0xe7, 0x44, 0x44, 0x44, 0x44, 0x44, 0x47,
    0xef, 0xff, 0xf5, 0x0, 0xcf, 0xff, 0xe2, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x2, 0xef, 0xff,
    0xb0, 0xf, 0xff, 0xf7, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x7, 0xff, 0xff, 0x1, 0xff,
    0xff, 0x40, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x4f, 0xff, 0xf1, 0xf, 0xff, 0xf7, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x7, 0xff,
    0xff, 0x0, 0xdf, 0xff, 0xe1, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x2, 0xef, 0xff, 0xc0, 0x7,
    0xff, 0xff, 0xe6, 0x33, 0x33, 0x33, 0x33, 0x33,
    0x36, 0xef, 0xff, 0xf6, 0x0, 0xd, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xfd, 0x0, 0x0, 0x2f, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x20, 0x0,
    0x5, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xf5, 0x0, 0x2, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xf1, 0x0, 0x9f, 0xff, 0xfa, 0x20, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x2b, 0xff, 0xff, 0x90,
    0xf, 0xff, 0xfb, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0xb, 0xff, 0xfe, 0x1, 0xff, 0xff,
    0x40, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x4f, 0xff, 0xf1, 0x1f, 0xff, 0xf6, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x6, 0xff, 0xff,
    0x0, 0xef, 0xff, 0xd1, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x1, 0xdf, 0xff, 0xd0, 0x8, 0xff,
    0xff, 0xd7, 0x44, 0x44, 0x44, 0x44, 0x44, 0x47,
    0xef, 0xff, 0xf7, 0x0, 0x1e, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe,
    0x0, 0x0, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xfe, 0x20, 0x0, 0x0,
    0x2d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xfd, 0x20, 0x0, 0x0, 0x0, 0x6, 0xbe,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xb6, 0x0,
    0x0, 0x0,

    /* U+0039 "9" */
    0x0, 0x0, 0x5, 0xbe, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xfe, 0xb5, 0x0, 0x0, 0x0, 0x0, 0x1c,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xfc, 0x20, 0x0, 0x0, 0x1d, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x20,
    0x0, 0xc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xfd, 0x0, 0x5, 0xff,
    0xff, 0xe7, 0x44, 0x44, 0x44, 0x44, 0x44, 0x47,
    0xef, 0xff, 0xf7, 0x0, 0xbf, 0xff, 0xf2, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x2, 0xef, 0xff,
    0xd0, 0xf, 0xff, 0xf8, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x7, 0xff, 0xff, 0x11, 0xff,
    0xff, 0x40, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x3f, 0xff, 0xf2, 0xf, 0xff, 0xf7, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0xff,
    0xff, 0x20, 0xcf, 0xff, 0xe2, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x3f, 0xff, 0xf2, 0x6,
    0xff, 0xff, 0xe6, 0x33, 0x33, 0x33, 0x33, 0x33,
    0x33, 0x36, 0xff, 0xff, 0x20, 0xc, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xf2, 0x0, 0x1e, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x20,
    0x0, 0x1c, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xf2, 0x0, 0x0, 0x5,
    0xbe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0x20, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x3f, 0xff, 0xf2,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x3, 0xff, 0xff, 0x20, 0x3, 0x30,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x3f, 0xff, 0xf2, 0x9, 0xff, 0xc1, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x5, 0xff, 0xff,
    0x10, 0xff, 0xff, 0xd1, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x1, 0xdf, 0xff, 0xd0, 0xe, 0xff,
    0xff, 0xe6, 0x44, 0x44, 0x44, 0x44, 0x44, 0x46,
    0xdf, 0xff, 0xf7, 0x0, 0x3e, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfd,
    0x0, 0x0, 0x3e, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xfe, 0x20, 0x0, 0x0,
    0x2d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xfd, 0x20, 0x0, 0x0, 0x0, 0x6, 0xbe,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xb6, 0x0,
    0x0, 0x0,

    /* U+003A ":" */
    0x0, 0x8d, 0xc6, 0x0, 0x9f, 0xff, 0xf6, 0xf,
    0xff, 0xff, 0xc0, 0xff, 0xff, 0xfc, 0xa, 0xff,
    0xff, 0x60, 0x19, 0xee, 0x70, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x8c, 0xc5, 0x0, 0x9f,
    0xff, 0xf5, 0xf, 0xff, 0xff, 0xc0, 0xff, 0xff,
    0xfc, 0xa, 0xff, 0xff, 0x70, 0x19, 0xee, 0x70
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 473, .box_w = 27, .box_h = 25, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 338, .adv_w = 358, .box_w = 19, .box_h = 25, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 576, .adv_w = 473, .box_w = 27, .box_h = 25, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 914, .adv_w = 473, .box_w = 27, .box_h = 25, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1252, .adv_w = 473, .box_w = 27, .box_h = 25, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1590, .adv_w = 473, .box_w = 27, .box_h = 25, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1928, .adv_w = 473, .box_w = 27, .box_h = 25, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 2266, .adv_w = 473, .box_w = 27, .box_h = 25, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 2604, .adv_w = 473, .box_w = 27, .box_h = 25, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 2942, .adv_w = 473, .box_w = 27, .box_h = 25, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 3280, .adv_w = 165, .box_w = 7, .box_h = 16, .ofs_x = 2, .ofs_y = 4}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/



/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 48, .range_length = 11, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    }
};

/*-----------------
 *    KERNING
 *----------------*/


/*Pair left and right glyphs for kerning*/
static const uint8_t kern_pair_glyph_ids[] =
{
    2, 5,
    2, 8,
    2, 11,
    5, 2,
    5, 4,
    5, 8,
    11, 4,
    11, 8
};

/* Kerning between the respective left and right glyphs
 * 4.4 format which needs to scaled with `kern_scale`*/
static const int8_t kern_pair_values[] =
{
    -63, -63, -54, -63, -45, -63, -28, -120
};

/*Collect the kern pair's data in one place*/
static const lv_font_fmt_txt_kern_pair_t kern_pairs =
{
    .glyph_ids = kern_pair_glyph_ids,
    .values = kern_pair_values,
    .pair_cnt = 8,
    .glyph_ids_size = 0
};

/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LV_VERSION_CHECK(8, 0, 0)
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = &kern_pairs,
    .kern_scale = 23,
    .cmap_num = 1,
    .bpp = 4,
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
const lv_font_t dig = {
#else
lv_font_t dig = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 25,          /*The maximum line height required by the font*/
    .base_line = 0,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -5,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};



#endif /*#if DIG*/

