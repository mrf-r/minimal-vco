
// TODO
// add #pragma once to export script
// export 2 files for vertical and horizontal screens
// fix regular dgl functions

#ifndef _MONOCHROME_GRAPHIC_LIB_H
#define _MONOCHROME_GRAPHIC_LIB_H

#include <stdint.h>

/* MONOCHROME GRAPHIC LIBRARY
 *
 * steps to start
 * 1 - define display size
 * 2 - configure display like on example below:
 *      after writing bytes 0x55, 0xAA, 0x33, 0xCC, 0x0F, 0xF0 we got this pattern on the top left:
 *      +-------------------------
 *      | O   O   O
 *      |   O O   O
 *      | O     O O
 *      |   O   O O
 *      | O   O     O
 *      |   O O     O
 *      | O     O   O
 *      |   O   O   O
 *      |
 *      |
 *      (bytes are vertically oriented LSB with on top, scan from left to right and then from top to bottom)
 * 3 - add panel update from mgl_framebuffer[] array routine
 * 4 - draw your own fonts with awesome SG Bitmap Font Editor for Windows PC - http://corosan.ru/sgfed/
 *      add them to the bottom of this file
 * 5 - in case you are using DMA, check that it can access frame buffer
 *      use __attribute__((section("section_name")))
 */

#define MGL_DISPLAY_WIDTH 122
#define MGL_DISPLAY_HEIGHT 32

//font structure
//you can draw your font and generate this with included export script for SG Bitmap Font Editor software by Vyacheslav Grigotyev
//SG Bitmap Font Editor: http://corosan.ru/sgfed/
typedef struct
{
    void* bitmap_data_horiz; //horizontal fill one pixel
    void* bitmap_data_vert; //horizontal fill vertical line of 8 pixels
    const uint8_t* symbol_width; //char width array
    uint8_t bmp_width; //number of vertical bytes/words / horizontal type (byte/halfword/word)
    uint8_t bmp_height; //number of horizontal bytes/words / vertical type
    uint8_t startchar; //start char (0x0 or 0x20)
} MglFont;

typedef enum {
    MGL_COLOR_LOW = 0,
    MGL_COLOR_HIGH,
    MGL_COLOR_INVERT
} mgl_color_en;

typedef enum {
    MGL_ALIGN_LEFT = 0,
    MGL_ALIGN_CENTER,
    MGL_ALIGN_RIGHT
} mgl_align_en;

#define MGL_DISPLAY_ROWS ((MGL_DISPLAY_HEIGHT + 7) / 8)

extern uint8_t mgl_framebuffer[MGL_DISPLAY_WIDTH * MGL_DISPLAY_ROWS];
void mgl_setworkingarea(uint16_t x, uint16_t y, uint16_t xsize, uint16_t ysize);
void mgl_setcursor(int16_t x, int16_t y);
void mgl_setfont(const MglFont* font);
void mgl_setcolor(mgl_color_en color);
void mglSetAlign(mgl_align_en align);
void mglFillBlank(mgl_color_en color);
void mgl_drawbmp(const void* bitmap, uint8_t width, uint8_t height);
void mgl_drawchar(const char c);
uint16_t mglGetStringLength(const char* ch);
void mglDrawString(const char* ch);
void mgl_printf(const char* formatstring, ...);
void mgl_hexvalue32(uint32_t v);
void mgl_hexvalue16(uint16_t v);
void mgl_update(void);
void mgl_init(void);

//extern const MglFont font6; //do not work here
extern const MglFont font3;
extern const MglFont font5;
extern const MglFont font_c16;
extern const MglFont font_m4;

#endif // _MONOCHROME_GRAPHIC_LIB_H