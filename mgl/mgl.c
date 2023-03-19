

#include "mgl.h"
#include "mgl_conf.h"

#define MAX_X (MGL_DISPLAY_WIDTH - 1)
#define MAX_Y (MGL_DISPLAY_HEIGHT - 1)

#if MGL_DISPLAY_MONOCHROME == 1
#define MGL_DISPLAY_ROWS ((MGL_DISPLAY_HEIGHT + 7) / 8)
static uint8_t framebuffer[MGL_DISPLAY_WIDTH * MGL_DISPLAY_ROWS];
static uint16_t fb_x = 0, fb_y = 0;
static uint16_t fb_xs = 0, fb_xe = 0;
static uint16_t fb_ys = 0, fb_ye = 0;
static void displaySetZone(const uint16_t wax, const uint16_t way, const uint16_t wax_size, const uint16_t way_size)
{
    fb_xs = wax, fb_xe = wax + wax_size;
    fb_ys = way, fb_ye = way + way_size;
    fb_x = wax;
    fb_y = way;
}
static void displayOutPixel(MglColor c)
{
    switch (c.wrd & 0x3) {
    case 0:
        framebuffer[fb_y / 8 * MGL_DISPLAY_WIDTH + fb_x] &= ~(1 << fb_y % 8);
        break;
    case 1:
        framebuffer[fb_y / 8 * MGL_DISPLAY_WIDTH + fb_x] |= 1 << fb_y % 8;
        break;
    case 2:
        framebuffer[fb_y / 8 * MGL_DISPLAY_WIDTH + fb_x] ^= 1 << fb_y % 8;
        break;
    default:
    }
    fb_x++;
    if (fb_x >= fb_xe) {
        fb_x = fb_xs;
        fb_y++;
        if (fb_y >= fb_ye) {
            fb_y = fb_ys;
        }
    }
}
void displayFramebufferUpdate(uint8_t* fb);
#else // MGL_DISPLAY_MONOCHROME
void displaySetZone(const uint16_t wax, const uint16_t way, const uint16_t wax_size, const uint16_t way_size);
void displayOutPixel(MglColor c);
#endif // MGL_DISPLAY_MONOCHROME

// this function doesn't needed in
void mglUpdateDisplay()
{
#if MGL_DISPLAY_MONOCHROME == 1
    displayFramebufferUpdate(framebuffer);
#endif
}

static MglColor palette[MGL_PALETTE_TOTAL];
static int16_t pos_x;
static int16_t pos_y;
// working area
static uint16_t wax;
static uint16_t way;
static uint16_t wax_size;
static uint16_t way_size;
static const MglFont* wa_font;
static MglAlignEn walign;

void mglInitDefaults()
{
    mglSetFont(&mgl_f5);
    mglSetWorkingArea(0, 0, MGL_DISPLAY_WIDTH, MGL_DISPLAY_HEIGHT);
    mglSetCursor(0, 0);
    mglExSetPaletteColor(MGL_PALETTE_BLANK, mglExColorFromRgb(0, 0, 0));
    mglExSetPaletteColor(MGL_PALETTE_TEXT_FORE, mglExColorFromRgb(0x3D, 0x7D, 0x3D));
    mglExSetPaletteColor(MGL_PALETTE_TEXT_BACK, mglExColorFromRgb(0, 0, 0));
    mglExSetPaletteColor(MGL_PALETTE_BITMAP_FORE, mglExColorFromRgb(0x3D, 0x7D, 0x3D));
    mglExSetPaletteColor(MGL_PALETTE_BITMAP_BACK, mglExColorFromRgb(0, 0, 0));
    mglExSetPaletteColor(MGL_PALETTE_PIXEL, mglExColorFromRgb(0x3D, 0x7D, 0x3D));
}

void mglSetCursor(const int16_t x, const int16_t y)
{
    pos_x = x;
    pos_y = y;
}

void mglSetFont(const MglFont* font)
{
    wa_font = font;
}

void mglSetAlign(MglAlignEn align)
{
    walign = align;
}

void mglSetWorkingArea(const uint16_t x, const uint16_t y, const uint16_t xsize, const uint16_t ysize)
{
    if ((x < MGL_DISPLAY_WIDTH) && (y < MGL_DISPLAY_HEIGHT)) {
        if (xsize > MGL_DISPLAY_WIDTH - x)
            wax_size = MGL_DISPLAY_WIDTH - x;
        else
            wax_size = xsize;
        if (ysize > MGL_DISPLAY_HEIGHT - y)
            way_size = MGL_DISPLAY_HEIGHT - y;
        else
            way_size = ysize;
        wax = x;
        pos_x = x;
        way = y;
        pos_y = y;
    } else {
        wax_size = 0;
        way_size = 0;
        pos_x = 0;
        pos_y = 0;
    }
}

void mglFillBlank()
{
    uint32_t transfers = (wax_size) * (way_size);
    uint32_t i;
    displaySetZone(wax, way, wax_size, way_size);
    for (i = 0; i < transfers; i++)
        displayOutPixel(palette[MGL_PALETTE_BLANK]);
}

void mglDrawBitmap(const void* bitmap, const uint8_t width, const uint8_t height)
{
    if (((pos_x + width) > wax) && (pos_x < (wax + wax_size))) {
        uint8_t xstart = pos_x < wax ? wax - pos_x : 0;
        uint8_t xend = (pos_x + width) > (wax + wax_size) ? width - ((pos_x + width) - (wax + wax_size)) : width;
        //
        if (((pos_y + height) > way) && (pos_y < (way + way_size))) {
            uint16_t ix, iy;
            uint8_t ystart = pos_y < way ? way - pos_y : 0;
            uint8_t yend = (pos_y + height) > (way + way_size) ? height - ((pos_y + height) - (way + way_size)) : height;
            displaySetZone(pos_x + xstart, pos_y + ystart, xend - xstart, yend - ystart);
            for (iy = ystart; iy < yend; iy++) {
                uint32_t bmpline;
                if (width > 8) {
                    if (width > 16)
                        bmpline = ((uint32_t*)bitmap)[iy];
                    else
                        bmpline = ((uint16_t*)bitmap)[iy] << 16;
                } else
                    bmpline = ((uint8_t*)bitmap)[iy] << 24;
                for (ix = xstart; ix < xend; ix++) {
                    displayOutPixel((bmpline >> (31 - ix)) & 0x1 ? palette[MGL_PALETTE_BITMAP_FORE] : palette[MGL_PALETTE_BITMAP_BACK]);
                }
            }
        }
    }
    pos_x += width; // shift cursor pos
}

void mglDrawChar(const char c) // we need this function because font width and bmp_width are different things
{
    uint8_t ch = c - wa_font->startchar; // set offset (non printable ascii)
    uint8_t width = wa_font->symbol_width[ch]; // char width
    uint8_t height = wa_font->bmp_height; // char width
    // calculate char start pos and length
    if (((pos_x + width) > wax) && (pos_x < (wax + wax_size))) {
        uint8_t xstart = pos_x < wax ? wax - pos_x : 0;
        uint8_t xend = (pos_x + width) > (wax + wax_size) ? width - ((pos_x + width) - (wax + wax_size)) : width;
        //
        if (((pos_y + height) > way) && (pos_y < (way + way_size))) {
            uint16_t ix, iy;
            uint8_t ystart = pos_y < way ? way - pos_y : 0;
            uint8_t yend = (pos_y + height) > (way + way_size) ? height - ((pos_y + height) - (way + way_size)) : height;
            displaySetZone(pos_x + xstart, pos_y + ystart, xend - xstart, yend - ystart);
            for (iy = ystart + ch * height; iy < ch * height + yend; iy++) {
                uint32_t bmpline;
                if (wa_font->bmp_width > 8) {
                    if (wa_font->bmp_width > 16)
                        bmpline = ((uint32_t*)wa_font->bitmap_data_horiz)[iy];
                    else
                        bmpline = ((uint16_t*)wa_font->bitmap_data_horiz)[iy] << 16;
                } else
                    bmpline = ((uint8_t*)wa_font->bitmap_data_horiz)[iy] << 24;
                for (ix = xstart; ix < xend; ix++) {
                    displayOutPixel((bmpline >> (31 - ix)) & 0x1 ? palette[MGL_PALETTE_TEXT_FORE] : palette[MGL_PALETTE_TEXT_BACK]);
                }
            }
        }
    }
    pos_x += width; // shift cursor pos
}

void mglFillPattern(const uint8_t* bitmap, const uint8_t pixmod)
{
    uint16_t height = way + way_size - pos_y; // char width
    if (pos_x < (wax + wax_size)) {
        uint16_t xstart = pos_x < wax ? pos_x - wax : 0;
        uint16_t xend = wax + wax_size - pos_x;
        if (((pos_y + height) > way) && (pos_y < (way + way_size))) {
            uint32_t i;
            uint8_t pixpos = 0;

            uint16_t ystart = pos_y < way ? pos_y - way : 0;
            uint16_t yend = (pos_y + height) > (way + way_size) ? height - ((pos_y + height) - (way + way_size)) : height;
            displaySetZone(pos_x + xstart, pos_y + ystart, xend, yend);
            for (i = 0; i < ((uint32_t)((xend - xstart) * (yend - ystart))); i++) // no warnings!
            {
                if (bitmap[pixpos >> 3] & (1 << (pixpos & 7)))
                    displayOutPixel(palette[MGL_PALETTE_BITMAP_FORE]);
                else
                    displayOutPixel(palette[MGL_PALETTE_BITMAP_BACK]);
                pixpos++;
                if (pixpos == pixmod)
                    pixpos = 0;
            }
        }
    }
    pos_x = wax + wax_size; // shift cursor pos
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

void mglDrawString(const char* ch)
{
    switch (walign) {
    case MGL_ALIGN_CENTER:
        mglSetCursor(pos_x - mglGetStringLength(ch) / 2, pos_y);
        break;
    case MGL_ALIGN_RIGHT:
        mglSetCursor(pos_x - mglGetStringLength(ch), pos_y);
        break;
    default:;
    }
    while (*ch) {
        mglDrawChar(*ch);
        ch++;
    }
}

// TODO delete deprecated
void mglFillTextEnd()
{
    uint8_t height = wa_font->bmp_height; // char width
    if (pos_x < (wax + wax_size)) {
        uint8_t xstart = pos_x < wax ? pos_x - wax : 0;
        uint16_t xend = wax + wax_size - pos_x;
        if (((pos_y + height) > way) && (pos_y < (way + way_size))) {
            uint16_t ix, iy;
            uint8_t ystart = pos_y < way ? pos_y - way : 0;
            uint8_t yend = (pos_y + height) > (way + way_size) ? height - ((pos_y + height) - (way + way_size)) : height;
            displaySetZone(pos_x + xstart, pos_y + ystart, xend, yend);
            for (iy = ystart; iy < yend; iy++) {
                for (ix = xstart; ix < xend; ix++) {
                    displayOutPixel(palette[MGL_PALETTE_TEXT_BACK]);
                }
            }
        }
    }
    pos_x = wax + wax_size; // shift cursor pos
}
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

const char h2d[16] = "0123456789ABCDEF";

void mglDrawHex32(uint32_t v)
{
    mglDrawChar(h2d[(v >> 28) & 0xF]);
    mglDrawChar(h2d[(v >> 24) & 0xF]);
    mglDrawChar(h2d[(v >> 20) & 0xF]);
    mglDrawChar(h2d[(v >> 16) & 0xF]);
    mglDrawChar(h2d[(v >> 12) & 0xF]);
    mglDrawChar(h2d[(v >> 8) & 0xF]);
    mglDrawChar(h2d[(v >> 4) & 0xF]);
    mglDrawChar(h2d[v & 0xF]);
}
void mglDrawHex16(uint16_t v)
{
    mglDrawChar(h2d[(v >> 12) & 0xF]);
    mglDrawChar(h2d[(v >> 8) & 0xF]);
    mglDrawChar(h2d[(v >> 4) & 0xF]);
    mglDrawChar(h2d[v & 0xF]);
}

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
void mglExSetPaletteColor(const uint8_t number, const MglColor color)
{
    if (number < MGL_PALETTE_TOTAL)
        palette[number] = color;
}

MglColor mglExColorFromRgb(uint8_t red, uint8_t green, uint8_t blue)
{
    MglColor c;
    c.red = red;
    c.green = green;
    c.blue = blue;
    return c;
}

MglColor mglExColorFromHsv(uint8_t hue, uint8_t saturation, uint8_t value)
{
    MglColor c;
    uint32_t red, green, blue;
    if (hue < 86) {
        uint8_t h = hue;
        red = (85 - h) * 771;
        green = h * 771;
        blue = 0;
    } else {
        if (hue < 171) {
            uint8_t h = hue - 85;
            red = 0;
            green = (85 - h) * 771;
            blue = h * 771;
        } else {
            uint8_t h = hue - 170;
            red = h * 771;
            green = 0;
            blue = (85 - h) * 771;
        }
    }
    if (red > 32768)
        red = 32768;
    if (green > 32768)
        green = 32768;
    if (blue > 32768)
        blue = 32768;
    // saturation
    red = red * saturation / 256 + (256 - saturation) * 64;
    green = green * saturation / 256 + (256 - saturation) * 64;
    blue = blue * saturation / 256 + (256 - saturation) * 64;
    // value
    if (value > 128) { // to bright
        value = value - 128;
        red = red * (128 - value) / 128 + value * 256;
        green = green * (128 - value) / 128 + value * 256;
        blue = blue * (128 - value) / 128 + value * 256;
    } else { // to dark
        red = red * value / 128;
        green = green * value / 128;
        blue = blue * value / 128;
    }
    c.red = red >> 7;
    c.green = green >> 7;
    c.blue = blue >> 7;
    return c;
}

MglColor mglExAlphablend(uint16_t intensity, MglColor lowest, MglColor highest)
{
    MglColor r;
    int32_t ca = ((highest.red - lowest.red) * intensity) / 65536 + lowest.red;
    r.red = ca;
    ca = ((highest.green - lowest.green) * intensity) / 65536 + lowest.green;
    r.green = ca;
    ca = ((highest.blue - lowest.blue) * intensity) / 65536 + lowest.blue;
    r.blue = ca;
    return r;
}
MglColor mglExShine(uint16_t intensity, MglColor highest, MglColor lowest)
{
    MglColor r;
    int32_t ca = highest.red + lowest.red * intensity / 65536;
    r.red = ca > 255 ? 255 : ca;
    ca = highest.green + lowest.green * intensity / 65536;
    r.green = ca > 255 ? 255 : ca;
    ca = highest.blue + lowest.blue * intensity / 65536;
    r.blue = ca > 255 ? 255 : ca;
    return r;
}

void mglHsvTestFill()
{
    uint16_t x, y;
    // first
    displaySetZone(0, 0, MGL_DISPLAY_WIDTH, MGL_DISPLAY_HEIGHT);
    for (y = 0; y < MGL_DISPLAY_HEIGHT; y++)
        for (x = 0; x < MGL_DISPLAY_WIDTH; x++)
            displayOutPixel(mglExColorFromHsv(x * 256 / MGL_DISPLAY_WIDTH, y * 256 / MGL_DISPLAY_HEIGHT, 128));
}
void mglHsvTestFill2()
{
    uint16_t x, y;
    // second
    displaySetZone(0, 0, MGL_DISPLAY_WIDTH, MGL_DISPLAY_HEIGHT);
    for (y = 0; y < MGL_DISPLAY_HEIGHT; y++)
        for (x = 0; x < MGL_DISPLAY_WIDTH; x++)
            displayOutPixel(mglExColorFromHsv(x * 256 / MGL_DISPLAY_WIDTH, 255, y * 128 / MGL_DISPLAY_HEIGHT + 128));
}
