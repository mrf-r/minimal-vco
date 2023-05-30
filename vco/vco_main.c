#include "vco.h"
#include "bsp.h"
#include "audio.h"

//  SAMPLE_RATE is for button timings only
// #define SAMPLE_RATE 192000
#define MENU_DELAY_MS 2000
#define DEBOUNCE_TICKS (50 * SAMPLE_RATE / 1000)
// ADC_AVERAGE_COUNT - please, 2's power as well
#define ADC_AVERAGE_COUNT 8192

extern volatile uint32_t counter_sr;
// static bool button = false;

SaveBlock sb;

static inline void delayMs(uint32_t ms) {
  uint32_t sr_end = ms * (SAMPLE_RATE / 1000) + counter_sr;
  while (counter_sr > sr_end)
    ;
  while (counter_sr < sr_end)
    ;
}
static inline uint32_t timeMsDelta() {
  static uint32_t last_call_sr = 0;
  uint32_t delta = counter_sr - last_call_sr;
  delta = delta / (SAMPLE_RATE / 1000);
  return delta;
}

static inline bool buttonStateGet() {
  static bool state = false;
  static uint32_t debounce = 0;
  static uint32_t last_call_sr = 0;
  bool state_new = state;
  uint32_t delta = counter_sr - last_call_sr;
  if (debounce < delta) {
    debounce = 0;
    state_new = bspButtonGet();  // TODO: here
  } else {
    debounce -= delta;
  }

  if (state_new != state) {
    debounce = DEBOUNCE_TICKS;
  }
  state = state_new;
  return state_new;
}

/*
algo:
1 - play c1 (1.000 V)
2 - push button (led will lit)
3 - play c3 (3.000 V)
4 - push button ()

*/

#define SM_WAIT 0
#define SM_CALIB 0x20
#define SM_SAVE 0x40

extern SaveBlock sb;

void vcoMain(Vco* vco) {
  // static uint32_t vco->state = 0;
  // static uint32_t vco->cv_average_low = 0;
  // static uint32_t vco->cv_average_high = 0;
  // static uint32_t vco->sr = 0;
  // static uint32_t vco->samples = 0;
  // static uint32_t vco->menu_delay = 0;
  switch (vco->state) {
    default:
      while (1)
        ;
      break;

    /////////////////////////////////////////////////////////////////////
    // WAIT
    case SM_WAIT:  // button wait
      if (buttonStateGet()) {
        vco->state++;
      }
      break;
    case SM_WAIT + 1:  // button release wait
      if (buttonStateGet()) {
        bspLedSet(true);
        vco->menu_delay = counter_sr + MENU_DELAY_MS * SAMPLE_RATE / 1000;
        vco->state++;
      }
      break;
    case SM_WAIT + 2:  // wait calibration request
      if (counter_sr > vco->menu_delay) {
        vco->menu_delay = counter_sr + MENU_DELAY_MS * SAMPLE_RATE / 1000;
        vco->state++;
      }
      if (buttonStateGet()) {
        vco->state = SM_CALIB;
      }
      break;
    case SM_WAIT + 3:                     // wait save request
      bspLedSet((counter_sr / 4) & 0x1);  // half brightness
      if (counter_sr > vco->menu_delay) {
        vco->state = SM_WAIT;
      }
      if (buttonStateGet()) {
        vco->state = SM_SAVE;
      }
      break;

    /////////////////////////////////////////////////////////////////////
    // CALIB
    case SM_CALIB:
      vco->cv_average_low = 0;
      vco->samples = 0;
      vco->sr = counter_sr;
      break;
    case SM_CALIB + 1:  // low acquisition
      if (vco->sr != counter_sr) {
        vco->cv_average_low += vco->ctrl[CTRL_PITCH];
        vco->samples++;
        if (vco->samples > ADC_AVERAGE_COUNT) {
          vco->state++;
        }
        vco->sr = counter_sr;
      }
      break;
    case SM_CALIB + 2:  // button wait
      if (buttonStateGet()) {
        vco->cv_average_high = 0;
        vco->samples = 0;
        vco->sr = counter_sr;
        vco->state++;
      }
      break;
    case SM_CALIB + 3:  // high acquisition
      if (vco->sr != counter_sr) {
        vco->cv_average_high += vco->ctrl[CTRL_PITCH];
        vco->samples++;
        if (vco->samples > ADC_AVERAGE_COUNT) {
          int32_t adc_range =
              (vco->cv_average_high - vco->cv_average_low) / CALIB_OCTAVES_DISTANCE;
          vco->calib_scale =
              0x100000000LL / 128 * 12 * ADC_AVERAGE_COUNT / adc_range;
          vco->calib_offset =
              65536 / 128 * 12 * CALIB_OCTAVES_OFFSET -
              ((int64_t)(vco->cv_average_low - (vco->cv_average_high - vco->cv_average_low) /
                                              CALIB_OCTAVES_DISTANCE)) *
                  vco->calib_scale / 65536 / ADC_AVERAGE_COUNT;
          vco->state = SM_WAIT;
        }
        vco->sr = counter_sr;
      }
      break;

    /////////////////////////////////////////////////////////////////////
    // SAVE
    case SM_SAVE:
      sb.scale = vco->calib_scale;
      sb.offset = vco->calib_offset;
      bspNvmSave(&sb);
      vco->state = SM_WAIT;
      break;
  }
}
