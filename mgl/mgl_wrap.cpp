#include "mgl_wrap.h"

#include "mgl.h"
#include "mgl_conf.h"

static MglTest* out;
static uint32_t* fb;

extern "C" void displayFramebufferUpdate(uint8_t* fb)
{
    out->updateFbFrom1bppFb(fb, 0xFFFFFFFF, 0xFF000000);
}

void MglTest::updateFbFrom1bppFb(uint8_t* fb, uint32_t color_on, uint32_t color_off)
{
    uint16_t rows = MGL_DISPLAY_HEIGHT / 8;
    for (int yr = 0; yr < rows; yr++) {
        for (int yp = 0; yp < 8; yp++) {
            for (int x = 0; x < MGL_DISPLAY_WIDTH; x++) {
                bool pixel = fb[yr * MGL_DISPLAY_WIDTH + x] & (1 << yp);
                fb_[(yr * 8 + yp) * MGL_DISPLAY_WIDTH + x] = pixel ? color_on : color_off;
            }
        }
    }
}
MglTest::MglTest(SDL_Renderer* renderer)
{
    surface_ = SDL_CreateRGBSurface(0, MGL_DISPLAY_WIDTH, MGL_DISPLAY_HEIGHT, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    texture_ = SDL_CreateTextureFromSurface(renderer, surface_);
    fb_ = (uint32_t*)surface_->pixels;
    out = this;
    fb = fb_;
    // fb_[0] = 0xFF000000;
    // fb_[1] = 0xFFFFFFFF;
    // return;
    mglInitDefaults();
    uint8_t pattern = 0x1;
    mglSetWorkingArea(0, 0, MGL_DISPLAY_WIDTH, MGL_DISPLAY_HEIGHT);
    mglFillBlank();
    mglSetWorkingArea(0, 0, MGL_DISPLAY_WIDTH, 1);
    mglFillPattern(&pattern, 2);
    mglSetWorkingArea(0, 0, 1, MGL_DISPLAY_HEIGHT);
    mglFillPattern(&pattern, 2);
    mglSetWorkingArea(MGL_DISPLAY_WIDTH - 1, 1, 1, MGL_DISPLAY_HEIGHT - 1);
    mglFillPattern(&pattern, 2);
    mglSetWorkingArea(1, MGL_DISPLAY_HEIGHT - 1, MGL_DISPLAY_WIDTH - 1, 1);
    mglFillPattern(&pattern, 2);
    mglSetWorkingArea(0, 0, MGL_DISPLAY_WIDTH, MGL_DISPLAY_HEIGHT);

    mglSetCursor(10, 0);
    // mglExSetPaletteColor(MGL_PALETTE_TEXT_FORE, mglExColorFromRgb(0, 0, 255));
    mglDrawString("Hello, world!");
    // mglExSetPaletteColor(MGL_PALETTE_TEXT_FORE, mglExColorFromRgb(0, 255, 0));

    mglSetCursor(10, 6);
    mglDrawString("QUICK BROWN FOX JUMPS OVER THE LAZY DOG");
    // mglExSetPaletteColor(MGL_PALETTE_TEXT_FORE, mglExColorFromRgb(255, 0, 0));
    mglSetCursor(10, 12);
    mglDrawString("quick brown fox jumps over the lazy dog");
    // mglExSetPaletteColor(MGL_PALETTE_TEXT_FORE, mglExColorFromRgb(255, 255, 0));
    mglSetCursor(10, 18);
    mglDrawString("1234567890");
    // mglExSetPaletteColor(MGL_PALETTE_TEXT_FORE, mglExColorFromRgb(255, 255, 255));
    mglSetCursor(10, 24);
    mglDrawString("!@#$%^&*()_+-={}:<>?");
    mglSetCursor(10, 60);
    const uint8_t bmp[] = { 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA };
    mglDrawBitmap(bmp, 8, 8);
    mglDrawBitmap(bmp, 8, 8);
    mglDrawBitmap(bmp, 8, 8);

    mglHsvTestFill2();
    mglUpdateDisplay();
}
MglTest::~MglTest()
{
    SDL_DestroyTexture(texture_);
    SDL_FreeSurface(surface_);
}
SDL_Texture* MglTest::texture()
{
    SDL_UpdateTexture(texture_, nullptr, surface_->pixels, surface_->pitch);
    return texture_;
}

// SDL_Texture* printHex(uint32_t val);
SDL_Rect* MglTest::rect(uint16_t x, uint16_t y, uint8_t scale)
{
    if ((scale < 1) || (scale > 20))
        scale = 1;
    static SDL_Rect r = { x, y, MGL_DISPLAY_WIDTH * scale, MGL_DISPLAY_HEIGHT * scale };
    return &r;
}

static uint16_t xs = 0, ys = 0, xe = 0, ye = 0;
static uint16_t x = 0, y = 0;
extern "C" __attribute__((weak)) void displaySetZone(const uint16_t wax, const uint16_t way, const uint16_t wax_size, const uint16_t way_size)
{
    x = xs = wax, y = ys = way;
    xe = wax + wax_size, ye = way + way_size;
}
extern "C" __attribute__((weak)) void displayOutPixel(MglColor c)
{
    fb[MGL_DISPLAY_WIDTH * y + x] = c.wrd | 0xFF000000;
    x++;
    if (x == xe) {
        x = xs;
        y++;
        if (y == ye) {
            y = ys;
        }
    }
}