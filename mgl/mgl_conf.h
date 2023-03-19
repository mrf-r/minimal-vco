#ifndef _MGL_CONF_H
#define _MGL_CONF_H

// do not include this file!!

#include "mgl.h"
// #include "mgl_font6.h"
#include "fnt_5.h"

// TODO: this is whong
static const MglFont mgl_f5 = {
    .bitmap_data_horiz = (void*)fnt_5_blcd,
    // .bitmap_data_vert = (void*)fnt_5_slcd,
    .symbol_width = fnt_5_width,
    .bmp_width = 8,
    .bmp_height = 8,
    .startchar = 0
};


// MINIMAL GRAPHIC LIBRARY
// for popular display panels with integrated framebuffer
// average rgb tft/oled display has rgb-pixel left-to-right top-to-down scan
// direct output (displaySetZone, displayOutPixel)
// rmw operations are impossible
#define MGL_DISPLAY_MONOCHROME 0
// average monochrome graphic panel has vertical byte (8 pixels column) left-to-right top-to-down scan
// output through framebuffer, mgl_update() needed (lcd_update)
// rmw operations are possible (invert), but prohibited

#define MGL_DISPLAY_WIDTH 122
#define MGL_DISPLAY_HEIGHT 32

/*
большая текстура 320х240
uint32_t* pixel_data;
uint16_t tx = 0, ty = 0, txs = 0, tys = 0;
uint16_t tcx = 0, tcy = 0;
static inline void displaySetZone(uint16_t xpos, uint16_t ypos, uint16_t xsize, uint16_t ysize)
{
    tx = xpos, ty = ypos, txs = xsize, tys = ysize;
    tcx = xpos, tcy = ypos;
}
static inline void displayOutPixel(MglColor color)
{
    pixel_data[tcy * MGL_DISPLAY_WIDTH + tcx] = color.wrd | 0xFF000000;
    tcx += 1;
    if (tcx >= tx + txs) {
        tcx = tx;
        tcy += 1;
        if (tcy >= ty + tys) {
            tcy = ty;
        }
    }
}
*/

#endif // _MGL_CONF_H
