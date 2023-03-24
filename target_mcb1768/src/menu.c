
#include <stdint.h>
#include <stdbool.h>
#include "bsp.h"
#include "GLCD.h"

#define OFFSET_Y 5
#define OFFSET_X 4
#define DELTA_X 4
#define DELTA_Y 2

static const char *mx[] = {
    "PTC", "OCT", "AMP", "PHS", "2PT", "SNC", "LF1", "LF2",
};

static const char *my[] = {
    "abs",
    "lf1",
    "lf2",
};

static int8_t cx;
static int8_t cy;

volatile int8_t modmatrix[sizeof(my) / sizeof(my[1])]
                         [sizeof(mx) / sizeof(mx[1])];

static bool edit_mode = false;

static char *matrixHex(uint8_t x, uint8_t y) {
  const char hex[] = "0123456789ABCDEF";
  static char out[3];
  uint8_t value = (uint8_t)modmatrix[y][x];
  out[0] = hex[value >> 4];
  out[1] = hex[value & 0xF];
  out[2] = '\0';
  return out;
}

void menuRedraw() {
  for (unsigned y = 0; y < sizeof(my) / sizeof(my[1]); y++) {
    for (unsigned x = 0; x < sizeof(mx) / sizeof(mx[1]); x++) {
      if ((y == (unsigned)cy) && (x == (unsigned)cx)) {
        GLCD_SetBackColor(0x39E7);
        if (edit_mode)
          GLCD_SetTextColor(0xF9E7);
        else
          GLCD_SetTextColor(White);
      } else {
        GLCD_SetBackColor(Black);
        GLCD_SetTextColor(White);
      }
      GLCD_DisplayString(y * DELTA_Y + OFFSET_Y, x * DELTA_X + OFFSET_X, 0,
                         (unsigned char *)matrixHex(x, y));
    }
  }
  // draw dest line
  for (unsigned x = 0; x < sizeof(mx) / sizeof(mx[1]); x++) {
    GLCD_SetBackColor(Black);
    if (x == (unsigned)cx) {
      GLCD_SetTextColor(Red);
    } else {
      GLCD_SetTextColor(White);
    }
    GLCD_DisplayString(OFFSET_Y - DELTA_Y, x * DELTA_X + OFFSET_X - 1, 0,
                       (unsigned char *)mx[x]);
  }
  // draw src line
  for (unsigned y = 0; y < sizeof(my) / sizeof(my[1]); y++) {
    GLCD_SetBackColor(Black);
    if (y == (unsigned)cy) {
      GLCD_SetTextColor(Red);
    } else {
      GLCD_SetTextColor(White);
    }
    GLCD_DisplayString(y * DELTA_Y + OFFSET_Y, OFFSET_X - DELTA_X, 0,
                       (unsigned char *)my[y]);
  }
}

typedef enum {
  BUT_OK = 0,
  BUT_UP,
  BUT_RIGHT,
  BUT_DOWN,
  BUT_LEFT,
  BUT_BLUE
} ButEn;

void trig(ButEn val) {
  if (!edit_mode) {
    // draw current cross with normal colors
    GLCD_SetBackColor(Black);
    GLCD_SetTextColor(White);
    GLCD_DisplayString(cy * DELTA_Y + OFFSET_Y, cx * DELTA_X + OFFSET_X, 0,
                       (unsigned char *)matrixHex(cx, cy));

    GLCD_DisplayString(OFFSET_Y - DELTA_Y, cx * DELTA_X + OFFSET_X - 1, 0,
                       (unsigned char *)mx[cx]);
    GLCD_DisplayString(cy * DELTA_Y + OFFSET_Y, OFFSET_X - DELTA_X, 0,
                       (unsigned char *)my[cy]);
    // change position
    switch (val) {
      case BUT_RIGHT:
        cx += 1;
        if (cx >= (int8_t)(sizeof(mx) / sizeof(mx[1]))) cx = 0;
        break;
      case BUT_LEFT:
        cx -= 1;
        if (cx < 0) cx = sizeof(mx) / sizeof(mx[1]) - 1;
        break;
      case BUT_UP:
        cy -= 1;
        if (cy < 0) cy = sizeof(my) / sizeof(my[1]) - 1;
        break;
      case BUT_DOWN:
        cy += 1;
        if (cy >= (int8_t)(sizeof(my) / sizeof(my[1]))) cy = 0;
        break;
      default:
        break;
    }
    // draw current cross with selected colorsGLCD_SetBackColor(0x39E7);
    GLCD_SetBackColor(0x39E7);
    GLCD_SetTextColor(White);
    GLCD_DisplayString(cy * DELTA_Y + OFFSET_Y, cx * DELTA_X + OFFSET_X, 0,
                       (unsigned char *)matrixHex(cx, cy));
    GLCD_SetBackColor(Black);
    GLCD_SetTextColor(Red);
    GLCD_DisplayString(OFFSET_Y - DELTA_Y, cx * DELTA_X + OFFSET_X - 1, 0,
                       (unsigned char *)mx[cx]);
    GLCD_DisplayString(cy * DELTA_Y + OFFSET_Y, OFFSET_X - DELTA_X, 0,
                       (unsigned char *)my[cy]);
  }
  if (val == BUT_BLUE) {
    edit_mode = !edit_mode;
    if (edit_mode) {
      GLCD_SetBackColor(0x39E7);
      GLCD_SetTextColor(0xF9E7);

    } else {
      GLCD_SetBackColor(0x39E7);
      GLCD_SetTextColor(White);
    }
    GLCD_DisplayString(cy * DELTA_Y + OFFSET_Y, cx * DELTA_X + OFFSET_X, 0,
                       (unsigned char *)matrixHex(cx, cy));
  }
}

void menuTap() {
  // either parameter change or cursor change

  // param change - just redraw value

  // cursor change - redraw current, change cursor, redraw current

  //  P1.20 - center
  //  P1.23 - A - up
  //  P1.24 - B - right
  //  P1.25 - D - down
  //  P1.26 - C - left
  //  P2.10 - INT0

  uint8_t buttons = (((LPC_GPIO1->FIOPIN >> 20) & 0x1) << 0) |
                    (((LPC_GPIO1->FIOPIN >> 23) & 0xF) << 1) |
                    (((LPC_GPIO2->FIOPIN >> 10) & 0x1) << 5);
  static uint8_t buttons_prev = 0;
  uint8_t buttons_change = buttons_prev ^ buttons;
  buttons_prev = buttons;
  GLCD_SetTextColor(Green);

  if (buttons_change & (1 << BUT_OK)) {
    if (buttons & (1 << BUT_OK)) {
      // rel
      GLCD_DisplayChar(0, 0, 0, ' ');
    } else {
      // prs
      GLCD_DisplayChar(0, 0, 0, 'o');
    }
  }
  if (buttons_change & (1 << BUT_UP)) {
    if (buttons & (1 << BUT_UP)) {
      // rel
      GLCD_DisplayChar(0, 1, 0, ' ');
    } else {
      // prs
      GLCD_DisplayChar(0, 1, 0, '^');
      trig(BUT_UP);
    }
  }
  if (buttons_change & (1 << BUT_RIGHT)) {
    if (buttons & (1 << BUT_RIGHT)) {
      // rel
      GLCD_DisplayChar(0, 2, 0, ' ');
    } else {
      // prs
      GLCD_DisplayChar(0, 2, 0, '>');
      trig(BUT_RIGHT);
    }
  }
  if (buttons_change & (1 << BUT_DOWN)) {
    if (buttons & (1 << BUT_DOWN)) {
      // rel
      GLCD_DisplayChar(0, 3, 0, ' ');
    } else {
      // prs
      GLCD_DisplayChar(0, 3, 0, '_');
      trig(BUT_DOWN);
    }
  }
  if (buttons_change & (1 << BUT_LEFT)) {
    if (buttons & (1 << BUT_LEFT)) {
      // rel
      GLCD_DisplayChar(0, 4, 0, ' ');
    } else {
      // prs
      GLCD_DisplayChar(0, 4, 0, '<');
      trig(BUT_LEFT);
    }
  }
  GLCD_SetTextColor(Blue);
  if (buttons_change & (1 << BUT_BLUE)) {
    if (buttons & (1 << BUT_BLUE)) {
      // rel
      GLCD_DisplayChar(0, 5, 0, ' ');
    } else {
      // prs
      GLCD_DisplayChar(0, 5, 0, '#');
      trig(BUT_BLUE);
    }
  }
  if (edit_mode) {
    int32_t v = modmatrix[cy][cx];
    // sorry for that
    uint8_t bv = (~buttons) & 0x3F;
    uint32_t delay_ms = 80;
    if (bv) {
      switch (bv) {
        case 1 << BUT_RIGHT:
          delay_ms = 10;
          // fall through
        case 1 << BUT_UP:
          v += 1;
          break;
        case 1 << BUT_LEFT:
          delay_ms = 10;
          // fall through
        case 1 << BUT_DOWN:
          v -= 1;
          break;
        case 1 << BUT_OK:
          v = 0;
          break;
        default:
          break;
      }
      if (v > 127)
        v = 127;
      else if (v < -128)
        v = -128;
      // bspDelayMs(delay_ms);
    }
    modmatrix[cy][cx] = v;
    GLCD_SetBackColor(0x39E7);
    GLCD_SetTextColor(0xF9E7);
    GLCD_DisplayString(cy * DELTA_Y + OFFSET_Y, cx * DELTA_X + OFFSET_X, 0,
                       (unsigned char *)matrixHex(cx, cy));
  }
  void scopeDraw(void);
  scopeDraw();
}

// todo: scope
#define SCOPE_SIZE_X 320
#define SCOPE_SIZE_Y 128
#define SCOPE_OFFSET_Y 100
static uint32_t scope_sync = 0;
static uint8_t scope_buffer[SCOPE_SIZE_X * 2];
#define SCOPE_DIVIDER 8
void scopeWrite(uint8_t sample, bool trigger) {
  static uint32_t pos = 0;
  static uint32_t sync = 0;
  if ((scope_sync ^ sync) && trigger) {
    pos = 0;
    sync = scope_sync;
  }
  if (pos < SCOPE_SIZE_X * SCOPE_DIVIDER) {
    scope_buffer[scope_sync * SCOPE_SIZE_X + pos / SCOPE_DIVIDER] = sample;
    pos++;
  }
}

void scopeDraw() {
  // draw black prev
  // draw color current
  uint32_t syncinv = scope_sync ^ 1;
  uint8_t *b_set = &scope_buffer[SCOPE_SIZE_X * (scope_sync)];
  uint8_t *b_clr = &scope_buffer[SCOPE_SIZE_X * (syncinv)];
  // GLCD_Bargraph(0, SCOPE_OFFSET_Y, SCOPE_SIZE_X, SCOPE_SIZE_Y,Black);
  GLCD_WindowMax();
  for (int i = 0; i < SCOPE_SIZE_X; i++) {
    GLCD_SetTextColor(Black);
    GLCD_PutPixel(i, b_clr[i] / (256 / SCOPE_SIZE_Y) + SCOPE_OFFSET_Y);
    GLCD_SetTextColor(Cyan);
    GLCD_PutPixel(i, b_set[i] / (256 / SCOPE_SIZE_Y) + SCOPE_OFFSET_Y);
  }
  scope_sync = syncinv;
}