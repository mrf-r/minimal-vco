#ifndef _LCDN_H
#define _LCDN_H

#include <stdint.h>

typedef union {
    struct {
        uint8_t blue;
        uint8_t green;
        uint8_t red;
        uint8_t contrast;
    };
    uint32_t wrd;
} MglColor;

void lcd_init(void);
void lcd_update(void);
void lcd_set_color(MglColor color);
MglColor lcd_get_color(void);

#endif // _LCDN_H
