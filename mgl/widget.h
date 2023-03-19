#include <SDL2/SDL.h>

class Widget {
public:
    SDL_Renderer* renderer_;
    SDL_Surface* surface_;
    SDL_Texture* texture_;
    uint32_t* fb_;
    uint16_t x_, y_;

public:
    Widget(SDL_Renderer* renderer, uint16_t xsize, uint16_t ysize)
    {
        renderer_ = renderer;
        surface_ = SDL_CreateRGBSurface(0, xsize, ysize, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
        texture_ = SDL_CreateTextureFromSurface(renderer, surface_);
        x_ = 0;
        y_ = 0;
    }
    Widget(SDL_Renderer* renderer, uint16_t x, uint16_t y, uint16_t xsize, uint16_t ysize)
    {
        renderer_ = renderer;
        surface_ = SDL_CreateRGBSurface(0, xsize, ysize, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
        texture_ = SDL_CreateTextureFromSurface(renderer, surface_);
        x_ = x;
        y_ = y;
    }
    ~Widget()
    {
        SDL_DestroyTexture(texture_);
        SDL_FreeSurface(surface_);
    }
    void move(uint16_t x, uint16_t y)
    {
        x_ = x;
        y_ = y;
    }
    SDL_Texture* texture()
    {
        SDL_UpdateTexture(texture_, nullptr, surface_->pixels, surface_->pitch);
        return texture_;
    }
    SDL_Rect* rect(uint16_t x, uint16_t y)
    {
        static SDL_Rect r = { x, y, surface_->w, surface_->h };
        return &r;
    }
    SDL_Rect* rect()
    {
        static SDL_Rect r = { x_, y_, surface_->w, surface_->h };
        return &r;
    }
    void render(uint8_t scale)
    {
        SDL_UpdateTexture(texture_, nullptr, surface_->pixels, surface_->pitch);
        SDL_RenderCopy(renderer_, texture_, 0, rect());
    }
};
