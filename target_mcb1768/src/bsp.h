#ifndef _BSP_H
#define _BSP_H

#include <stdint.h>
#include <stdbool.h>

#include "LPC17xx.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  BSP_OK = 0,
  BSP_FAIL,
  BSP_REQUEST_ERROR,
  BSP_BUSY,
  BSP_NOT_READY,
} BspStatus;

#define BSP

static inline uint32_t bspTimerGet(void) { return DWT->CYCCNT; }

extern volatile uint32_t counter_sr;
extern volatile uint32_t counter_cr;

void bspDelayMs(uint32_t ms);

void bspAudioInit(void);
BspStatus bspSetAudioPeriod(uint32_t relative);
void bspSetAudioVolume(int16_t v);
//__attribute__((weak))
void audioCallback(int16_t *in, int16_t *out, uint16_t *ctrl_in);

static inline bool bspButtonGet(void) { return false; }
static inline void bspLedToggle(void) {}
static inline void bspLedSet(bool state) { (void)state; }

static inline BspStatus bspNvmSave(void *sb) {
  (void)sb;
  return BSP_OK;
}
static inline BspStatus bspNvmLoad(void *sb) {
  (void)sb;
  return BSP_OK;
}

#ifdef __cplusplus
}
#endif

#endif  // _BSP_H