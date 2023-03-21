
#include <stdint.h>
#include <stdbool.h>
#include "bsp.h"
#include "GLCD.h"

#define OFFSET_Y 10
#define OFFSET_X 6
#define DELTA_X 6
#define DELTA_Y 2

static const char *mx[] = {
    "PTCH", "OCTV", "1AMP", "PHSE", "2PCH", "SYNC", "LFO1", "LFO2",
};

static const char *my[] = {
    "abs",
    "lfo1",
    "lfo2",
};

static int8_t cx;
static int8_t cy;

static int16_t
    modmatrix[sizeof(my) / sizeof(my[1])][sizeof(mx) / sizeof(mx[1])] = {
        1, 10, 100, 1000, 10000, -1, -10, -100, -1000, -10000};

static char *matrixHex(uint8_t x, uint8_t y) {
  const char hex[] = "0123456789ABCDEF ";
  static char out[6];
  int16_t value = modmatrix[y][x];
  bool negative = false;
  out[0] = ' ';
  if (value < 0) {
    value = -value;
    negative = true;
  }
  bool started = false;
  for (int i = 1; i < 4; i++) {
    int digit = (value >> (12 - i * 4)) & 0xF;
    if ((started) || (digit)) {
      if ((!started) && (negative)) out[i - 1] = '-';
      started = true;
    } else {
      digit = 16;
    }
    out[i] = hex[digit];
  }
  out[4] = '\0';
  return out;
}

void menuRedraw() {
  for (unsigned y = 0; y < sizeof(my) / sizeof(my[1]); y++) {
    for (unsigned x = 0; x < sizeof(mx) / sizeof(mx[1]); x++) {
      if ((y == (unsigned)cy) && (x == (unsigned)cx)) {
        GLCD_SetBackColor(DarkGrey);
        GLCD_SetTextColor(Red);
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
    GLCD_DisplayString(OFFSET_Y - DELTA_Y, x * DELTA_X + OFFSET_X, 0,
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

void menuTap() {
  ;
  ;
}