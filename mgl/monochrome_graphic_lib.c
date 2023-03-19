
#include "stdarg.h"
#include "stdio.h"

#include "lcdn.h"
#include "monochrome_graphic_lib.h"

#include "fnt_3.h"
#include "fnt_5.h"
#include "fnt_c16.h"
#include "fnt_m4.h"

/*
 * library only works with frame buffer
 * bit 0
 * bit 1
 * bit 2
 * bit 3 byte0, byte1, byte2, byte3, ...
 * bit 4
 * bit 5
 * bit 6
 * bit 7
 *       byte(MGL_DISPLAY_WIDTH+0), byte(MGL_DISPLAY_WIDTH+1), ...
 *
 * 128*32 = 4 rows of 128 bytes
 * in array: first 128 elements - first row, then second, etc..
 * in byte lsb
 */

uint8_t mgl_framebuffer[MGL_DISPLAY_WIDTH * MGL_DISPLAY_ROWS];

static int16_t pos_x;
static int16_t pos_y;
// working area
static uint8_t wa_enabled;
static uint16_t wax;
static uint16_t way;
static uint16_t wax_size;
static uint16_t way_size;
static const MglFont* wa_font = &font_c16;
static mgl_color_en wcolor;
static mgl_align_en walign; // horizontal only

// set cursor anywhere. position can be negative
void mgl_setcursor(int16_t x, int16_t y)
{
    pos_x = x;
    pos_y = y;
}
void mgl_setfont(const MglFont* font)
{
    wa_font = font;
}
void mgl_setcolor(mgl_color_en color)
{
    wcolor = color;
}

// horizontal string only
void mglSetAlign(mgl_align_en align)
{
    walign = align;
}

// set rectangle zone, where drawing affected
void mgl_setworkingarea(uint16_t x, uint16_t y, uint16_t xsize, uint16_t ysize)
{
    wa_enabled = !((x > MGL_DISPLAY_WIDTH) || (y > MGL_DISPLAY_HEIGHT));
    if (xsize > MGL_DISPLAY_WIDTH - x)
        xsize = MGL_DISPLAY_WIDTH - x;
    if (ysize > MGL_DISPLAY_HEIGHT - y)
        ysize = MGL_DISPLAY_HEIGHT - y;
    wax = x;
    pos_x = x;
    wax_size = xsize;
    way = y;
    pos_y = y;
    way_size = ysize;
}

// fill working area rectangle
void mglFillBlank(mgl_color_en color)
{
    if (wa_enabled) {
        uint8_t first_row = way / 8;
        uint8_t last_row = (way + way_size) / 8;
        uint16_t x, y;
        switch (color) {
        case MGL_COLOR_LOW:
        default:
            for (y = first_row; y <= last_row; y++) {
                uint16_t offset = y * MGL_DISPLAY_WIDTH + wax;
                uint8_t mask = 0xFF;
                if (y == first_row)
                    mask &= 0xFF << (way & 7);
                else if (y == last_row)
                    mask &= 0xFF >> (8 - ((way + way_size) & 7));
                mask = ~mask;
                for (x = 0; x < wax_size; x++) {
                    mgl_framebuffer[offset + x] &= mask;
                }
            }
            break;
        case MGL_COLOR_HIGH:
            for (y = first_row; y <= last_row; y++) {
                uint16_t offset = y * MGL_DISPLAY_WIDTH + wax;
                uint8_t mask = 0xFF;
                if (y == first_row)
                    mask &= 0xFF << (way & 7);
                else if (y == last_row)
                    mask &= 0xFF >> (8 - ((way + way_size) & 7));
                // mask = ~mask;
                for (x = 0; x < wax_size; x++) {
                    mgl_framebuffer[offset + x] |= mask;
                }
            }
            break;
        case MGL_COLOR_INVERT:
            for (y = first_row; y <= last_row; y++) {
                uint16_t offset = y * MGL_DISPLAY_WIDTH + wax;
                uint8_t mask = 0xFF;
                if (y == first_row)
                    mask &= 0xFF << (way & 7);
                else if (y == last_row)
                    mask &= 0xFF >> (8 - ((way + way_size) & 7));
                // mask = ~mask;
                for (x = 0; x < wax_size; x++) {
                    mgl_framebuffer[offset + x] ^= mask;
                }
            }
            break;
        }
    }
}
// we need this function because font width and bmp_width are different things
void mgl_drawchar_deprecated(char c, uint8_t color)
{
    if (wa_enabled) {
        uint8_t ch = c - wa_font->startchar; // set offset (non printable ascii)
        uint8_t width = wa_font->symbol_width[ch]; // char width
        uint8_t height = wa_font->bmp_height; // char width
        // uint8_t bmp_width = wa_font->bmp_width; //char width
        // calculate char start pos and length
        if (((pos_x + width) > wax) && (pos_x < (wax + wax_size))) {
            uint8_t xstart = pos_x < wax ? wax - pos_x : 0; // ERROR HERE
            uint8_t xend = (pos_x + width) > (wax + wax_size) ? width - ((pos_x + width) - (wax + wax_size)) : width;
            //
            if (((pos_y + height) > way) && (pos_y < (way + way_size))) {
                // uint16_t ix, iy;
                uint8_t ystart = pos_y < way ? way - pos_y : 0;
                uint8_t yend = (pos_y + height) > (way + way_size) ? height - ((pos_y + height) - (way + way_size)) : height;

                ////////////
                uint8_t first_row = (pos_y + ystart) / 8;
                uint8_t last_row = (pos_y + yend) / 8;
                uint16_t x;
                uint8_t y;
                uint16_t chp = ch * wa_font->bmp_width;
                int8_t fshift = (first_row + 1) * 8 - pos_y;
                for (y = first_row; y <= last_row; y++) {
                    uint16_t offset = y * MGL_DISPLAY_WIDTH + pos_x; // framebuffer
                    uint8_t mask = 0xFF; // this is for wa limiting ?
                    if (y == first_row) // limit top (low bits)
                    {
                        int16_t sh = way - first_row * 8;
                        if (sh > 0)
                            mask <<= sh;
                    }
                    if (y == last_row) // limit bottom (high bits)
                    {
                        int16_t sh = (last_row + 1) * 8 - (way + way_size); //
                        if (sh > 0)
                            mask &= 0xFF >> sh;
                    }
                    // mask = ~mask;
                    if (wa_font->bmp_height > 8) {
                        if (wa_font->bmp_height > 16) {
                            for (x = xstart; x < xend; x++) {
                                uint32_t pat;
                                if (y != first_row)
                                    pat = ((uint32_t*)wa_font->bitmap_data_vert)[chp + x] >> (fshift - 8);
                                else
                                    pat = ((uint32_t*)wa_font->bitmap_data_vert)[chp + x] << (8 - fshift); // 32bit - 4 or 5 calls
                                mgl_framebuffer[offset + x] ^= pat & mask;
                            }
                        } else {
                            for (x = xstart; x < xend; x++) {
                                uint32_t pat = ((uint16_t*)wa_font->bitmap_data_vert)[chp + x] << 8; // 16bit - 2 or 3 calls
                                pat = pat >> fshift;
                                mgl_framebuffer[offset + x] ^= pat & mask;
                            }
                        }
                    } else
                        for (x = xstart; x < xend; x++) {
                            uint32_t pat = ((uint8_t*)wa_font->bitmap_data_vert)[chp + x] << 8; // 8bits - 1 or 2 calls
                            pat = pat >> fshift;
                            mgl_framebuffer[offset + x] ^= pat & mask;
                        }
                    fshift += 8;
                }
            }
        }
        pos_x += width; // shift cursor pos
    }
}

// draw custom character up to 32 pixels height (array of either 8, 16 or 32 bit values)
void mgl_drawbmp(const void* bitmap, uint8_t width, uint8_t height) // up to 32x32 pixels
{
    if (wa_enabled) {
        if (((pos_x + width) > wax) && (pos_x < (wax + wax_size))) {
            uint8_t xstart = pos_x < wax ? wax - pos_x : 0; // ERROR HERE
            uint8_t xend = (pos_x + width) > (wax + wax_size) ? wax + wax_size - pos_x : width;
            if (((pos_y + height) > way) && (pos_y < (way + way_size))) {
                // uint16_t ix, iy;
                uint8_t ystart = pos_y < way ? way - pos_y : 0;
                uint8_t yend = (pos_y + height) > (way + way_size) ? way + way_size - pos_y : height;
                uint8_t first_row = (pos_y + ystart) / 8;
                uint8_t last_row = (pos_y + yend) / 8;
                uint16_t x;
                uint8_t y;
                int8_t fshift = (first_row + 1) * 8 - pos_y;
                switch (wcolor) {
                case MGL_COLOR_LOW:
                    for (y = first_row; y <= last_row; y++) {
                        uint16_t offset = y * MGL_DISPLAY_WIDTH + pos_x; // framebuffer
                        uint8_t mask = 0xFF; // this is for wa limiting ?
                        if (y == first_row) // limit top (low bits)
                        {
                            int16_t sh = way - first_row * 8;
                            if (sh > 0)
                                mask <<= sh;
                        }
                        if (y == last_row) // limit bottom (high bits)
                        {
                            int16_t sh = (last_row + 1) * 8 - (way + way_size); //
                            if (sh > 0)
                                mask &= 0xFF >> sh;
                        }
                        // mask = ~mask;
                        if (height > 8) {
                            if (height > 16) {
                                for (x = xstart; x < xend; x++) {
                                    uint32_t pat;
                                    if (y != first_row)
                                        pat = ((uint32_t*)bitmap)[x] >> (fshift - 8);
                                    else
                                        pat = (((uint32_t*)bitmap)[x] << 8) >> fshift; // 32bit - 4 or 5 calls
                                    mgl_framebuffer[offset + x] &= ~(pat & mask);
                                }
                            } else {
                                for (x = xstart; x < xend; x++) {
                                    uint32_t pat = ((uint16_t*)bitmap)[x] << 8; // 16bit - 2 or 3 calls
                                    pat = pat >> fshift;
                                    mgl_framebuffer[offset + x] &= ~(pat & mask);
                                }
                            }
                        } else
                            for (x = xstart; x < xend; x++) {
                                uint32_t pat = ((uint8_t*)bitmap)[x] << 8; // 8bits - 1 or 2 calls
                                pat = pat >> fshift;
                                mgl_framebuffer[offset + x] &= ~(pat & mask);
                            }
                        fshift += 8;
                    }

                    break;
                case MGL_COLOR_HIGH:
                    for (y = first_row; y <= last_row; y++) {
                        uint16_t offset = y * MGL_DISPLAY_WIDTH + pos_x; // framebuffer
                        uint8_t mask = 0xFF; // this is for wa limiting ?
                        if (y == first_row) // limit top (low bits)
                        {
                            int16_t sh = way - first_row * 8;
                            if (sh > 0)
                                mask <<= sh;
                        }
                        if (y == last_row) // limit bottom (high bits)
                        {
                            int16_t sh = (last_row + 1) * 8 - (way + way_size); //
                            if (sh > 0)
                                mask &= 0xFF >> sh;
                        }
                        // mask = ~mask;
                        if (height > 8) {
                            if (height > 16) {
                                for (x = xstart; x < xend; x++) {
                                    uint32_t pat;
                                    if (y != first_row)
                                        pat = ((uint32_t*)bitmap)[x] >> (fshift - 8);
                                    else
                                        pat = (((uint32_t*)bitmap)[x] << 8) >> fshift; // 32bit - 4 or 5 calls
                                    mgl_framebuffer[offset + x] |= pat & mask;
                                }
                            } else {
                                for (x = xstart; x < xend; x++) {
                                    uint32_t pat = ((uint16_t*)bitmap)[x] << 8; // 16bit - 2 or 3 calls
                                    pat = pat >> fshift;
                                    mgl_framebuffer[offset + x] |= pat & mask;
                                }
                            }
                        } else
                            for (x = xstart; x < xend; x++) {
                                uint32_t pat = ((uint8_t*)bitmap)[x] << 8; // 8bits - 1 or 2 calls
                                pat = pat >> fshift;
                                mgl_framebuffer[offset + x] |= pat & mask;
                            }
                        fshift += 8;
                    }

                    break;
                default:
                case MGL_COLOR_INVERT:
                    for (y = first_row; y <= last_row; y++) {
                        uint16_t offset = y * MGL_DISPLAY_WIDTH + pos_x; // framebuffer
                        uint8_t mask = 0xFF; // this is for wa limiting ?
                        if (y == first_row) // limit top (low bits)
                        {
                            int16_t sh = way - first_row * 8;
                            if (sh > 0)
                                mask <<= sh;
                        }
                        if (y == last_row) // limit bottom (high bits)
                        {
                            int16_t sh = (last_row + 1) * 8 - (way + way_size); //
                            if (sh > 0)
                                mask &= 0xFF >> sh;
                        }
                        // mask = ~mask;
                        if (height > 8) {
                            if (height > 16) {
                                for (x = xstart; x < xend; x++) {
                                    uint32_t pat;
                                    if (y != first_row)
                                        pat = ((uint32_t*)bitmap)[x] >> (fshift - 8);
                                    else
                                        pat = (((uint32_t*)bitmap)[x] << 8) >> fshift; // 32bit - 4 or 5 calls
                                    mgl_framebuffer[offset + x] ^= pat & mask;
                                }
                            } else {
                                for (x = xstart; x < xend; x++) {
                                    uint32_t pat = ((uint16_t*)bitmap)[x] << 8; // 16bit - 2 or 3 calls
                                    pat = pat >> fshift;
                                    mgl_framebuffer[offset + x] ^= pat & mask;
                                }
                            }
                        } else
                            for (x = xstart; x < xend; x++) {
                                uint32_t pat = ((uint8_t*)bitmap)[x] << 8; // 8bits - 1 or 2 calls
                                pat = pat >> fshift;
                                mgl_framebuffer[offset + x] ^= pat & mask;
                            }
                        fshift += 8;
                    }

                    break;
                }
            }
        }
        pos_x += width; // shift cursor pos
    }
}

void mgl_drawchar(const char c) // we need this function because font width and bmp_width are different things
{
    int32_t ch = c - wa_font->startchar; // set offset (non printable ascii)
    if ((c > 127) || (ch < 0))
        return;
    uint8_t width = wa_font->symbol_width[ch]; // char width
    uint8_t height = wa_font->bmp_height; // char width
    uint8_t bmpw = wa_font->bmp_width;
    void* charbmp;
    if (height > 8)
        if (height > 16)
            charbmp = (void*)&(((uint32_t*)wa_font->bitmap_data_vert)[ch * bmpw]); // 32bit
        else
            charbmp = (void*)&(((uint16_t*)wa_font->bitmap_data_vert)[ch * bmpw]); // 16bit
    else
        charbmp = (void*)&(((uint8_t*)wa_font->bitmap_data_vert)[ch * bmpw]); // 8bit
    mgl_drawbmp(charbmp, width, height);
}

uint16_t mglGetStringLength(const char* ch)
{
    uint16_t strlength = 0;
    while (*ch) {
        uint8_t c = *ch - wa_font->startchar;
        strlength += wa_font->symbol_width[c];
        ch++;
    }
    return strlength;
}

// you can set alignment
void mglDrawString(const char* ch)
{
    switch (walign) {
    case MGL_ALIGN_CENTER:
        mgl_setcursor(pos_x - mglGetStringLength(ch) / 2, pos_y);
        break;
    case MGL_ALIGN_RIGHT:
        mgl_setcursor(pos_x - mglGetStringLength(ch), pos_y);
        break;
    default:;
    }
    while (*ch) {
        mgl_drawchar(*ch);
        ch++;
    }
}

// 32 chars max!
void mgl_printf(const char* formatstring, ...)
{
    int size = 0;
    char buff[32];
    // memset(buff, 0, sizeof(buff));
    va_list args;
    va_start(args, formatstring);
    size = vsnprintf(buff, sizeof(buff), formatstring, args);
    if (size)
        mglDrawString(buff);
    va_end(args);
}

const char h2d[17] = "0123456789ABCDEF";
// output value in non-proportional mode
void mgl_hexvalue32(uint32_t v)
{
    uint16_t xp = pos_x;
    uint16_t yp = pos_y;
    mgl_drawchar(h2d[(v >> 28) & 0xF]);
    xp += wa_font->bmp_width;
    mgl_setcursor(xp, yp);
    mgl_drawchar(h2d[(v >> 24) & 0xF]);
    xp += wa_font->bmp_width;
    mgl_setcursor(xp, yp);
    mgl_drawchar(h2d[(v >> 20) & 0xF]);
    xp += wa_font->bmp_width;
    mgl_setcursor(xp, yp);
    mgl_drawchar(h2d[(v >> 16) & 0xF]);
    xp += wa_font->bmp_width;
    mgl_setcursor(xp, yp);
    mgl_drawchar(h2d[(v >> 12) & 0xF]);
    xp += wa_font->bmp_width;
    mgl_setcursor(xp, yp);
    mgl_drawchar(h2d[(v >> 8) & 0xF]);
    xp += wa_font->bmp_width;
    mgl_setcursor(xp, yp);
    mgl_drawchar(h2d[(v >> 4) & 0xF]);
    xp += wa_font->bmp_width;
    mgl_setcursor(xp, yp);
    mgl_drawchar(h2d[v & 0xF]);
}

// output value in non-proportional mode
void mgl_hexvalue16(uint16_t v)
{
    uint16_t xp = pos_x;
    uint16_t yp = pos_y;
    mgl_drawchar(h2d[(v >> 12) & 0xF]);
    xp += wa_font->bmp_width;
    mgl_setcursor(xp, yp);
    mgl_drawchar(h2d[(v >> 8) & 0xF]);
    xp += wa_font->bmp_width;
    mgl_setcursor(xp, yp);
    mgl_drawchar(h2d[(v >> 4) & 0xF]);
    xp += wa_font->bmp_width;
    mgl_setcursor(xp, yp);
    mgl_drawchar(h2d[v & 0xF]);
}

void mgl_update()
{
    lcd_update();
}

void mgl_init()
{
    // for rgb display
    // all other
    mgl_setfont((MglFont*)&font5);
    mgl_setworkingarea(0, 0, MGL_DISPLAY_WIDTH, MGL_DISPLAY_HEIGHT);
    mgl_setcolor(MGL_COLOR_HIGH);
    mglSetAlign(MGL_ALIGN_LEFT);
    mglFillBlank(MGL_COLOR_LOW);
    mgl_update();
    // mgl_setcursor(10,10);
    // mglDrawString("hello, ph!");
    // while(1);
}
