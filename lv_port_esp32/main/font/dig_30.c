/*******************************************************************************
 * Size: 30 px
 * Bpp: 1
 * Opts: 
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef DIG_30
#define DIG_30 1
#endif

#if DIG_30

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap_dig_30[] = {
    /* U+0020 " " */
    0x0,

    /* U+0030 "0" */
    0xf, 0xff, 0x81, 0xff, 0xff, 0x1f, 0xff, 0xfc,
    0xe0, 0x0, 0xee, 0x0, 0x3, 0xf0, 0x0, 0x7f,
    0x80, 0xf, 0xfc, 0x1, 0xff, 0xe0, 0x3f, 0xff,
    0x7, 0xfd, 0xf8, 0xff, 0x8f, 0xdf, 0xf0, 0x7f,
    0xfe, 0x3, 0xff, 0xc0, 0x1f, 0xf8, 0x0, 0xff,
    0x0, 0x7, 0xe0, 0x0, 0x3b, 0x80, 0x3, 0x9f,
    0xff, 0xfc, 0x7f, 0xff, 0xc0, 0xff, 0xf8, 0x0,

    /* U+0031 "1" */
    0x0, 0x0, 0x7, 0x0, 0x1e, 0x0, 0x7c, 0x1,
    0xf8, 0x7, 0xf0, 0x1f, 0xe0, 0x1d, 0xc0, 0x13,
    0x80, 0x7, 0x0, 0xe, 0x0, 0x1c, 0x0, 0x38,
    0x0, 0x70, 0x0, 0xe0, 0x1, 0xc0, 0x3, 0x80,
    0x7, 0x0, 0xe, 0x3, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xc0,

    /* U+0032 "2" */
    0xff, 0xff, 0x87, 0xff, 0xff, 0x3f, 0xff, 0xfc,
    0x0, 0x0, 0xe0, 0x0, 0x3, 0x80, 0x0, 0x1c,
    0x0, 0x0, 0xe0, 0x0, 0x7, 0x0, 0x0, 0x70,
    0x7f, 0xff, 0x8f, 0xff, 0xf8, 0xff, 0xff, 0x7,
    0x0, 0x0, 0x70, 0x0, 0x3, 0x80, 0x0, 0x1c,
    0x0, 0x0, 0xe0, 0x0, 0x3, 0x80, 0x0, 0x1f,
    0xff, 0xfc, 0x7f, 0xff, 0xf0, 0xff, 0xff, 0x80,

    /* U+0033 "3" */
    0x7f, 0xff, 0x83, 0xff, 0xff, 0x1f, 0xff, 0xfc,
    0x0, 0x0, 0xe0, 0x0, 0x3, 0x80, 0x0, 0x1c,
    0x0, 0x0, 0xe0, 0x0, 0x7, 0x0, 0x0, 0x38,
    0x7f, 0xff, 0xc7, 0xff, 0xfe, 0x1f, 0xff, 0xf0,
    0x0, 0x3, 0x80, 0x0, 0x1c, 0x0, 0x0, 0xe0,
    0x0, 0x7, 0x0, 0x0, 0x38, 0x0, 0x3, 0x9f,
    0xff, 0xfd, 0xff, 0xff, 0xc7, 0xff, 0xf8, 0x0,

    /* U+0034 "4" */
    0xe0, 0xc, 0x7, 0x0, 0x70, 0x38, 0x3, 0x81,
    0xc0, 0x1c, 0xe, 0x0, 0xe0, 0x70, 0x7, 0x3,
    0x80, 0x38, 0x1c, 0x1, 0xc0, 0xe0, 0xe, 0x7,
    0x0, 0x70, 0x38, 0x3, 0x80, 0xe0, 0x1c, 0x7,
    0xff, 0xff, 0x1f, 0xff, 0xfc, 0x3f, 0xff, 0xc0,
    0x1, 0xc0, 0x0, 0xe, 0x0, 0x0, 0x70, 0x0,
    0x3, 0x80, 0x0, 0x1c, 0x0, 0x0, 0xe0, 0x0,

    /* U+0035 "5" */
    0x3f, 0xff, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xc0, 0x0, 0xe, 0x0, 0x0, 0x70, 0x0, 0x3,
    0x80, 0x0, 0x1c, 0x0, 0x0, 0xff, 0xff, 0x87,
    0xff, 0xff, 0x1f, 0xff, 0xfc, 0x7f, 0xff, 0xe0,
    0x0, 0x7, 0x80, 0x0, 0x1c, 0x0, 0x0, 0xe0,
    0x0, 0x7, 0x0, 0x0, 0x38, 0x0, 0x3, 0xbf,
    0xff, 0xfd, 0xff, 0xff, 0xcf, 0xff, 0xf8, 0x0,

    /* U+0036 "6" */
    0xf, 0xff, 0x81, 0xff, 0xff, 0x1f, 0xff, 0xfc,
    0xe0, 0x1, 0xee, 0x0, 0x7, 0xf0, 0x0, 0x3f,
    0x80, 0x0, 0xfc, 0x0, 0x0, 0xe0, 0x0, 0x7,
    0xff, 0xfc, 0x3f, 0xff, 0xf9, 0xff, 0xff, 0xee,
    0x0, 0x7, 0x70, 0x0, 0x3f, 0x80, 0x0, 0xfc,
    0x0, 0x7, 0xe0, 0x0, 0x3b, 0x80, 0x3, 0xdf,
    0xff, 0xfc, 0x7f, 0xff, 0xc0, 0xff, 0xf8, 0x0,

    /* U+0037 "7" */
    0x7f, 0xff, 0x83, 0xff, 0xff, 0x1f, 0xff, 0xfc,
    0x0, 0x0, 0xe0, 0x0, 0x3, 0x80, 0x0, 0x1c,
    0x0, 0x0, 0xe0, 0x0, 0xf, 0x0, 0x7, 0xf0,
    0x0, 0x7f, 0x0, 0xf, 0xf0, 0x0, 0x78, 0x0,
    0x7, 0x80, 0x0, 0x38, 0x0, 0x1, 0xc0, 0x0,
    0x1e, 0x0, 0x0, 0xf0, 0x0, 0x7, 0x80, 0x0,
    0x38, 0x0, 0x1, 0xc0, 0x0, 0xe, 0x0, 0x0,

    /* U+0038 "8" */
    0xf, 0xff, 0x81, 0xff, 0xff, 0x1f, 0xff, 0xfc,
    0xe0, 0x0, 0xee, 0x0, 0x7, 0xf0, 0x0, 0x1f,
    0x80, 0x0, 0xfc, 0x0, 0x7, 0xf0, 0x0, 0x7b,
    0xff, 0xff, 0x8f, 0xff, 0xf8, 0xff, 0xff, 0xef,
    0x0, 0x7, 0xf0, 0x0, 0x3f, 0x80, 0x0, 0xfc,
    0x0, 0x7, 0xe0, 0x0, 0x3b, 0x80, 0x3, 0x9f,
    0xff, 0xfc, 0x7f, 0xff, 0xc0, 0xff, 0xf8, 0x0,

    /* U+0039 "9" */
    0xf, 0xff, 0x81, 0xff, 0xff, 0x1f, 0xff, 0xfc,
    0xe0, 0x0, 0xee, 0x0, 0x7, 0xf0, 0x0, 0x1f,
    0x80, 0x0, 0xfc, 0x0, 0x7, 0x70, 0x0, 0x3b,
    0xff, 0xff, 0xcf, 0xff, 0xfe, 0x1f, 0xff, 0xf0,
    0x0, 0x3, 0x80, 0x0, 0x1c, 0x0, 0x0, 0xfc,
    0x0, 0x7, 0xf0, 0x0, 0x3f, 0xc0, 0x3, 0x9f,
    0xff, 0xfc, 0x7f, 0xff, 0xc0, 0xff, 0xf8, 0x0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc_dig_30[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 58, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 394, .box_w = 21, .box_h = 21, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 57, .adv_w = 299, .box_w = 15, .box_h = 22, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 99, .adv_w = 395, .box_w = 21, .box_h = 21, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 155, .adv_w = 394, .box_w = 21, .box_h = 21, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 211, .adv_w = 394, .box_w = 21, .box_h = 21, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 267, .adv_w = 394, .box_w = 21, .box_h = 21, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 323, .adv_w = 394, .box_w = 21, .box_h = 21, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 379, .adv_w = 394, .box_w = 21, .box_h = 21, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 435, .adv_w = 394, .box_w = 21, .box_h = 21, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 491, .adv_w = 395, .box_w = 21, .box_h = 21, .ofs_x = 2, .ofs_y = 0}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/



/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps_dig_30[] =
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

/*-----------------
 *    KERNING
 *----------------*/


/*Pair left and right glyphs for kerning*/
static const uint8_t kern_pair_glyph_ids_dig_30[] =
{
    3, 6,
    3, 9,
    6, 3,
    6, 5,
    6, 9
};

/* Kerning between the respective left and right glyphs
 * 4.4 format which needs to scaled with `kern_scale`*/
static const int8_t kern_pair_values_dig_30[] =
{
    -75, -75, -75, -54, -75
};

/*Collect the kern pair's data in one place*/
static const lv_font_fmt_txt_kern_pair_t kern_pairs_dig_30 =
{
    .glyph_ids = kern_pair_glyph_ids_dig_30,
    .values = kern_pair_values_dig_30,
    .pair_cnt = 5,
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
static lv_font_fmt_txt_dsc_t font_dsc_dig_30 = {
#endif
    .glyph_bitmap = glyph_bitmap_dig_30,
    .glyph_dsc = glyph_dsc_dig_30,
    .cmaps = cmaps_dig_30,
    .kern_dsc = &kern_pairs_dig_30,
    .kern_scale = 16,
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
const lv_font_t dig_30 = {
#else
lv_font_t dig_30 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 22,          /*The maximum line height required by the font*/
    .base_line = 0,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -4,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc_dig_30           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};



#endif /*#if DIG_30*/
