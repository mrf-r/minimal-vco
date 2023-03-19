#include "SDL2/SDL.h"
#include <stdio.h>

#include "font.h"
#include "mgl_wrap.h"
#include "wid_button.h"

#define SDL_SIZE_X 320
#define SDL_SIZE_Y 240
#define SDL_SCALE 2

#define SDL_LCD_X 122
#define SDL_LCD_Y 32

// static framebuffer
uint32_t fb[SDL_SIZE_X * SDL_SIZE_Y];

int main(int argc, char** argv)
{
    int ret = 0;

    ret = SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    if (ret) {
        printf("init err: %s/r/n", SDL_GetError());
        return ret;
    }
    printf("start\n");
    SDL_Window* screen = SDL_CreateWindow("SDL2 TEST",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SDL_SIZE_X * SDL_SCALE, SDL_SIZE_Y * SDL_SCALE, 0);
    // SDL_WINDOW_FULLSCREEN
    if (screen == NULL) {
        printf("window creation err: %s/r/n", SDL_GetError());
        return -1;
    }
    SDL_Renderer* rend = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (rend == NULL) {
        printf("renderer creation err: %s/r/n", SDL_GetError());
        return -1;
    }
    SDL_Surface* surface = SDL_CreateRGBSurface(0, SDL_LCD_X, SDL_LCD_Y, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    if (surface == NULL) {
        printf("surface creation err: %s/r/n", SDL_GetError());
        return -1;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, surface);
    uint32_t* p = (uint32_t*)surface->pixels;
    for (int y = 0; y < SDL_LCD_Y; y++) {
        for (int x = 0; x < SDL_LCD_X; x++) {
            uint32_t pix = ((x & 1) ^ (y & 1)) ? 0xFF000000 : 0xFF808080;
            p[y * SDL_LCD_X + x] = pix;
        }
    }
    // p[0] = 0xFFFFFFFF;
    // p[1] = 0x000000FF;
    // p[2] = 0x808080FF;
    // p[3] = 0x404040FF;
    SDL_UpdateTexture(texture, nullptr, surface->pixels, surface->pitch);

    SDL_SetRenderDrawColor(rend, 72, 48, 48, 0);
    SDL_RenderClear(rend);

    SDL_Rect rdst = { 10, 10, SDL_LCD_X * SDL_SCALE, SDL_LCD_Y * SDL_SCALE };
    SDL_RenderCopy(rend, texture, nullptr, &rdst);
    // SDL_RenderSetScale(rend, 1.6, 1);
    /*
    - отдельный поток под видео?

    60Hz - 50 сэмплов cv в одном пикселе


    необходим ли отдельный поток под вывод видео?
    какая точность необходима для фиксации двойного клика?
    как вообще организован двойной клик?
    нужно помнить о секите и его интерфейсе, поскольку он топчик
    можно еще предусмотреть второй режим - клавиатура - реактор стайл клавиатура
    для эликсира клавиатура это элемент прибора, но вот для секвои наверное полезно
    */
    Font hex(0xFF00FF00, 0x00000000, rend);
    MglTest mt(rend);
    WidgetButton but(rend, 100, 100, 31, 31);
    but.init("A", "bc", SDL_SCANCODE_A, 0);
    but.draw_outline(false);
    but.draw_led(0xFF000000);

    int td = SDL_GetNumTouchDevices();
    printf("touchdevs %d\r\n", td);
    for (int i = 0; i < td; i++) {
        SDL_TouchID t = SDL_GetTouchDevice(i);
        printf("touch type %d\r\n", SDL_GetTouchDeviceType(t));
    }
    // rdst = { 300, 300, 16 * 6, 16 };
    // SDL_RenderCopy(rend, hex->printHex(56), 0, &rdst);
    // SDL_RenderCopy(rend, hex->printHex(0xDEADBEEF), 0, hex->rect(250, 250, 1));
    // SDL_RenderPresent(rend);
    // SDL_UpdateWindowSurface(screen);

    // SDL_GetKeyboardState
    // SDL_Delay(3000);
    bool quit = false;
    while (!quit) {
        // surface->pixels
        // SDL_Surface
        static uint32_t counter;
        static uint32_t counter_max;
        static uint32_t tick_next = 0;
        uint32_t tick_new = SDL_GetTicks();
        if (tick_new > tick_next) {
            tick_next += 1000;
            if (p[0] == 0xFF000000)
                p[0] = 0xFFFFFFFF;
            else
                p[0] = 0xFF000000;
            counter_max = counter;
            counter = 0;
        }
        counter++;
        for (int i = 0; i < counter_max; i++) {
            p[SDL_LCD_X + i] = i > counter ? 0xFF000000 : 0xFF00FF00;
        }
        // SDL_LoadFile
        // SDL_GetBasePath
        SDL_UpdateTexture(texture, nullptr, surface->pixels, surface->pitch);
        SDL_RenderClear(rend);
        SDL_RenderCopy(rend, texture, nullptr, &rdst);
        // int x,y;
        // int b = SDL_GetMouseState(&x, &y);
        // int b = SDL_GetTouchFinger()
        // SDL_RenderCopy(rend, hex.printBcd(b * 1000000 + x * 1000 + y), 0, hex.rect(250, 180, 3));
        SDL_RenderCopy(rend, hex.printBcd(counter_max), 0, hex.rect(250, 250, 5));
        SDL_RenderCopy(rend, mt.texture(), 0, mt.rect(10, 200, 1));
        SDL_RenderCopy(rend, but.texture(), 0, but.rect());
        SDL_RenderPresent(rend);
        SDL_UpdateWindowSurface(screen);

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                printf(" key: %08X \n", event.key.keysym.scancode);
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    quit = true;
                break;
            case SDL_MOUSEWHEEL:
                printf("wheel : %d\n", event.wheel.y);
                break;

            default:
                // printf(" -- %08X \n", event.type);
                break;
            }
        }
        SDL_Delay(1);
    }

    // delete hex;
    SDL_FreeSurface(surface);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(screen);
    SDL_Quit();
    printf("quit\n");

    return ret;
}