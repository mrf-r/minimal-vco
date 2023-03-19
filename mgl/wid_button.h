
#include <SDL2/SDL.h>
#include "mgl_font6.h"

#include "widget.h"

#define COLOR_OUTLINE_RELEASED 0xFF000000
#define COLOR_OUTLINE_PRESSED 0xFF808040

typedef enum {
    EVENT_BUTTON_PRESS,
    EVENT_BUTTON_DOUBLE,
    EVENT_BUTTON_HOLD,
    EVENT_BUTTON_RELEASE,
    EVENT_POTENTIOMETER,
    EVENT_ENCODER,
} EventButtonEn;

class WidgetButton : public Widget {
private:
public:
    const char* name_;
    const char* comment_;
    SDL_Scancode scancode_;
    WidgetButton* next_;
    bool pressed_by_screen_;
    bool pressed_by_key_;
    bool pressed_;
    uint8_t midikey_;
    uint32_t color_;
    WidgetButton(SDL_Renderer* renderer, uint16_t x, uint16_t y, uint16_t xsize, uint16_t ysize)
        : Widget(renderer, x, y, xsize, ysize)
    {
    }
    void init(const char* name, const char* comment, SDL_Scancode scancode, uint8_t midikey)
    {
        name_ = name;
        comment_ = comment;
        scancode_ = scancode;
        midikey_ = midikey;
    }
    void link(WidgetButton* next)
    {
        next_ = next;
    }
    void check(uint32_t tick, uint8_t* keystate, uint16_t x, uint16_t y)
    {
        bool p = (keystate[scancode_] != 0) || ((x > x_) && (x < x_ + surface_->w) && (y > y_) && (y < y_ + surface_->h));
    }
    void ev_kbd(bool keystate)
    {
        pressed_by_key_ = keystate;
    }
    // void ev_joy(bool keystate);
    void check_midi(void* event)
    {
        // update gui and do panel logic if there any
        ;
    }

    /////////////////////////
    /////////////////////////
    /////////////////////////
    void draw_outline(bool pressed)
    {
        uint32_t color = pressed ? COLOR_OUTLINE_PRESSED : COLOR_OUTLINE_RELEASED;
        uint16_t xe = surface_->w;
        uint16_t ye = surface_->h;
        uint32_t* bmp = (uint32_t*)surface_->pixels;
        for (int x = 0; x < xe; x++) {
            bmp[x] = color;
            bmp[(ye - 1) * xe + x] = color;
        }
        for (int y = 0; y < ye; y++) {
            bmp[y * xe] = color;
            bmp[y * xe + xe - 1] = color;
        }
        SdlFont sf = SdlFont(surface_);
        sf.setPos(xe / 5, (ye - 8) / 2);
        sf.drawString(name_, color);
        sf.setPos(xe / 5, (ye - 8) / 2 + 8);
        sf.drawString(comment_, color);
    }

    void draw_led(uint32_t color)
    {
        uint16_t xe = surface_->w;
        uint16_t lxs = xe / 5 * 2;
        uint16_t lxe = xe / 5 * 3;
        uint16_t ye = surface_->h;
        uint16_t lye = ye / 5;
        uint32_t* bmp = (uint32_t*)surface_->pixels;
        for (int y = 1; y < lye; y++) {
            for (int x = lxs; x < lxe; x++) {
                bmp[y * surface_->w + x] = color;
            }
        }
    }
};

/*
Button *buttons = new Button[32];
delete[] buttons;

each button has parameters
dest:
 - keyboard press
 - mouse press
 - midi light (intensity, color)
source:
 - midi events
*/
