#ifndef _MGL_H
#define _MGL_H

// MINIMAL GRAPHIC LIBRARY

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef union {
    struct {
        uint8_t blue;
        uint8_t green;
        uint8_t red;
        uint8_t alpha; // not used by library
    };
    uint32_t wrd;
} MglColor;

typedef struct
{
    void* bitmap_data_horiz; // horizontal fill one pixel
    // void* bitmap_data_vert; // horizontal fill vertical line of 8 pixels
    const uint8_t* symbol_width; // char width array
    uint8_t bmp_width; // number of vertical bytes/words / horizontal type (byte/halfword/word)
    uint8_t bmp_height; // number of horizontal bytes/words / vertical type
    uint8_t startchar; // start char (0x0 or 0x20)
} MglFont;

typedef enum {
    MGL_ALIGN_LEFT = 0,
    MGL_ALIGN_CENTER,
    MGL_ALIGN_RIGHT
} MglAlignEn;

typedef enum {
    MGL_PALETTE_BLANK = 0,
    MGL_PALETTE_TEXT_FORE,
    MGL_PALETTE_TEXT_BACK,
    MGL_PALETTE_BITMAP_FORE,
    MGL_PALETTE_BITMAP_BACK,
    MGL_PALETTE_PIXEL,
    MGL_PALETTE_TOTAL,
} MglPaletteEn;

void mglUpdateDisplay(void);
void mglInitDefaults(void);
void mglSetCursor(const int16_t x, const int16_t y);
void mglSetFont(const MglFont* font);
void mglSetAlign(MglAlignEn align);
void mglSetWorkingArea(const uint16_t x, const uint16_t y, const uint16_t xsize, const uint16_t ysize);
void mglFillBlank(void);
void mglDrawBitmap(const void* bitmap, const uint8_t width, const uint8_t height);
void mglDrawChar(const char c);
void mglFillPattern(const uint8_t* bitmap, const uint8_t pixmod);
uint16_t mglGetStringLength(const char* ch);
void mglDrawString(const char* ch);
// TODO delete deprecated
void mglFillTextEnd(void);
/////////////////////////////////////////////////////////////////////////////////

void mglDrawHex32(uint32_t v);
void mglDrawHex16(uint16_t v);
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
void mglExSetPaletteColor(const uint8_t number, const MglColor color);
MglColor mglExColorFromRgb(uint8_t red, uint8_t green, uint8_t blue);
MglColor mglExColorFromHsv(uint8_t hue, uint8_t saturation, uint8_t value);
MglColor mglExAlphablend(uint16_t intensity, MglColor lowest, MglColor highest);
MglColor mglExShine(uint16_t intensity, MglColor highest, MglColor lowest);
void mglHsvTestFill(void);
void mglHsvTestFill2(void);

#ifdef __cplusplus
}
#endif

#endif /* _MGL_H */
