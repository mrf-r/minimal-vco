#pragma once
#include <SDL2/SDL.h>

class Font {
private:
    static constexpr uint8_t charwidth = 6;
    static constexpr uint8_t font[16 * 8] = {
        // 0
        0b01110000,
        0b10001000,
        0b10011000,
        0b10101000,
        0b11001000,
        0b10001000,
        0b01110000,
        0b00000000,
        // 1
        0b00100000,
        0b01100000,
        0b00100000,
        0b00100000,
        0b00100000,
        0b00100000,
        0b01110000,
        0b00000000,
        // 2
        0b01110000,
        0b10001000,
        0b00001000,
        0b00010000,
        0b00100000,
        0b01000000,
        0b11111000,
        0b00000000,
        // 3
        0b11111000,
        0b00010000,
        0b00100000,
        0b00010000,
        0b00001000,
        0b10001000,
        0b01110000,
        0b00000000,
        // 4
        0b00010000,
        0b00110000,
        0b01010000,
        0b10010000,
        0b11111000,
        0b00010000,
        0b00010000,
        0b00000000,
        // 5
        0b11111000,
        0b10000000,
        0b11110000,
        0b00001000,
        0b00001000,
        0b10001000,
        0b01110000,
        0b00000000,
        // 6
        0b00110000,
        0b01000000,
        0b10000000,
        0b11110000,
        0b10001000,
        0b10001000,
        0b01110000,
        0b00000000,
        // 7
        0b11111000,
        0b00001000,
        0b00010000,
        0b00010000,
        0b00100000,
        0b00100000,
        0b00100000,
        0b00000000,
        // 8
        0b01110000,
        0b10001000,
        0b10001000,
        0b01110000,
        0b10001000,
        0b10001000,
        0b01110000,
        0b00000000,
        // 9
        0b01110000,
        0b10001000,
        0b10001000,
        0b01111000,
        0b00001000,
        0b00010000,
        0b01100000,
        0b00000000,
        // A
        0b01110000,
        0b10001000,
        0b10001000,
        0b10001000,
        0b11111000,
        0b10001000,
        0b10001000,
        0b00000000,
        // B
        0b11110000,
        0b10001000,
        0b10001000,
        0b11110000,
        0b10001000,
        0b10001000,
        0b11110000,
        0b00000000,
        // C
        0b01110000,
        0b10001000,
        0b10000000,
        0b10000000,
        0b10000000,
        0b10001000,
        0b01110000,
        0b00000000,
        // D
        0b11110000,
        0b10001000,
        0b10001000,
        0b10001000,
        0b10001000,
        0b10001000,
        0b11110000,
        0b00000000,
        // E
        0b11111000,
        0b10000000,
        0b10000000,
        0b11110000,
        0b10000000,
        0b10000000,
        0b11111000,
        0b00000000,
        // F
        0b11111000,
        0b10000000,
        0b10000000,
        0b11110000,
        0b10000000,
        0b10000000,
        0b10000000,
        0b00000000,
    };
    static uint32_t bitmap_data_8888[sizeof(font) * 8];
    SDL_Surface* surface;
    SDL_Texture* texture;
    uint32_t* fb;

public:
    Font(uint32_t color_on, uint32_t color_off, SDL_Renderer* renderer)
    {
        for (int i = 0; i < sizeof(font); i++) {
            for (int b = 0; b < 8; b++) {
                bitmap_data_8888[i * 8 + b] = ((font[i] >> (7 - b)) & 0x1) ? color_on : color_off;
            }
        }
        surface = SDL_CreateRGBSurface(0, 8 * charwidth, 8, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        fb = (uint32_t*)surface->pixels;
    }
    ~Font()
    {
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }

    SDL_Texture* printHex(uint32_t val)
    {
        bool leadzero = true;
        for (int d = 0; d < 8; d++) {
            int dig = (val >> (28 - (d * 4))) & 0xF;
            if ((leadzero) && (dig == 0)) {
                for (int y = 0; y < 8; y++) {
                    for (int x = 0; x < charwidth; x++) {
                        fb[8 * charwidth * y + d * charwidth + x] = bitmap_data_8888[7];
                    }
                }
            } else {
                leadzero = false;
                for (int y = 0; y < 8; y++) {
                    for (int x = 0; x < charwidth; x++) {
                        fb[8 * charwidth * y + d * charwidth + x] = bitmap_data_8888[dig * 8 * 8 + y * 8 + x];
                    }
                }
            }
        }
        SDL_UpdateTexture(texture, nullptr, surface->pixels, surface->pitch);
        return texture;
    }
    SDL_Texture* printBcd(uint32_t val)
    {
        uint32_t bcd = 0;
        if (val > 0x99999999) {
            bcd = 0x99999999;
        } else {
            int shift = 0;
            while (val > 0) {
                uint32_t d = val / 10;
                bcd |= (val - d * 10) << shift;
                val = d;
                shift += 4;
            }
        }
        return printHex(bcd);
    }
    SDL_Rect* rect(uint16_t x, uint16_t y, uint8_t scale)
    {
        if ((scale < 1) || (scale > 20))
            scale = 1;
        static SDL_Rect r = { x, y, 8 * 6 * scale, 8 * scale };
        return &r;
    }
};

uint32_t Font::bitmap_data_8888[sizeof(Font::font) * 8];
