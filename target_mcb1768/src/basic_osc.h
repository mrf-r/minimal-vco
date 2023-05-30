#ifndef _BASIC_OSC_H
#define _BASIC_OSC_H

#include <stdint.h>
#include "audio.h"

#define SR SAMPLE_RATE
#define FREQ 826

typedef struct {
  int32_t increment;
  int32_t accumulator;
} BasicOcs;

static inline void boscInit(BasicOcs *b, int32_t freq) {
  b->increment = freq * (0x100000000LL / SR);
}

static inline int32_t boscSaw(BasicOcs *b) {
  b->accumulator += b->increment;
  return b->accumulator;
}

static inline int32_t boscTriangle(BasicOcs *b) {
  int32_t tri = boscSaw(b);
  if (tri < 0) tri = ~tri;
  tri = tri * 2;
  tri -= 0x80000000;
  return tri;
}

static inline int32_t boscTriangleRaw(volatile int32_t saw) {
  int32_t tri = saw;
  if (tri < 0) tri = ~tri;
  tri = tri * 2;
  tri -= 0x80000000;
  return tri;
}

static inline int32_t boscParabolicSine(BasicOcs *b) {
  volatile int32_t ps = boscTriangle(b);
  if (ps < 0) {
    ps = ps + 0x80000000;
    ps = (ps / 65536) * (ps / 32768);
    ps = ps - 0x80000000;
  } else {
    ps = 0x7FFFFFFF - ps;
    ps = (ps / 65536) * (ps / 32768);
    ps = 0x7FFFFFFF - ps;
  }
  return ps;
}

static inline int32_t boscParabolicSineRaw(volatile int32_t tri) {
  int32_t ps = tri;
  if (ps < 0) {
    ps = ps + 0x80000000;
    ps = (ps / 65536) * (ps / 32768);
    ps = ps - 0x80000000;
  } else {
    ps = 0x7FFFFFFF - ps;
    ps = (ps / 65536) * (ps / 32768);
    ps = 0x7FFFFFFF - ps;
  }
  return ps;
}

#endif  // _BASIC_OSC_H