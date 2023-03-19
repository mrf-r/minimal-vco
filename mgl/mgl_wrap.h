#pragma once

#include <SDL2/SDL.h>
#include "mgl_conf.h"

class MglTest {
private:
    SDL_Surface* surface_;
    SDL_Texture* texture_;
    uint32_t* fb_;

public:
    void updateFbFrom1bppFb(uint8_t* fb, uint32_t color_on, uint32_t color_off);
    MglTest(SDL_Renderer* renderer);
    ~MglTest();
    SDL_Texture* texture();
    // SDL_Texture* printHex(uint32_t val);
    SDL_Rect* rect(uint16_t x, uint16_t y, uint8_t scale);
};
