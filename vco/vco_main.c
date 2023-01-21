#include "vco.h"

//  SAMPLE_RATE is for button timings only
#define SAMPLE_RATE 192000
#define MENU_DELAY_MS 2000
#define DEBOUNCE_TICKS (50 * SAMPLE_RATE / 1000)
// ADC_AVERAGE_COUNT - please, 2's power as well
#define ADC_AVERAGE_COUNT 8192

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
}

static inline bool buttonStateGet() {
  static bool state = false;
  static uint32_t debounce = 0;
  static uint32_t last_call_sr = 0;
  bool state_new = state;
  uint32_t delta = counter_sr - last_call_sr;
  if (debounce < delta) {
    debounce = 0;
    state_new = false;  // TODO: here
  } else {
    debounce -= delta;
  }

  if (state_new != state) {
    debounce = DEBOUNCE_TICKS;
  }
  state = state_new;
  return state_new;
}

static inline void ledSwitch(bool state) { (void)state; }

volatile uint32_t counter_sr = 0;
static bool button = false;

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

void vcoMain(Vco* vco) {
  static uint32_t state = 0;
  uint32_t cv_average_low;
  uint32_t cv_average_high;
  uint32_t sr;
  uint32_t samples;
  uint32_t menu_delay;
  switch (state) {
    default:
      /////////////////////////////////////////////////////////////////////
      // WAIT
    case SM_WAIT:  // button wait
      if (buttonStateGet()) {
        state++;
      }
      break;
    case SM_WAIT + 1:  // button release wait
      if (buttonStateGet()) {
        ledSwitch(true);
        menu_delay = counter_sr + MENU_DELAY_MS * SAMPLE_RATE / 1000;
        state++;
      }
      break;
    case SM_WAIT + 2:  // wait calibration request
      if (counter_sr > menu_delay) {
        menu_delay = counter_sr + MENU_DELAY_MS * SAMPLE_RATE / 1000;
        state++;
      }
      if (buttonStateGet()) {
        state = SM_CALIB;
      }
      break;
    case SM_WAIT + 3:                     // wait save request
      ledSwitch((counter_sr / 4) & 0x1);  // half brightness
      if (counter_sr > menu_delay) {
        state = SM_WAIT;
      }
      if (buttonStateGet()) {
        state = SM_SAVE;
      }
      break;
      /////////////////////////////////////////////////////////////////////
      // CALIB
    case SM_CALIB:
      cv_average_low = 0;
      samples = 0;
      sr = counter_sr;
      break;
    case SM_CALIB + 1:  // low acquisition
      if (sr != counter_sr) {
        cv_average_low += vco->adc[ADC_PITCH];
        samples++;
        if (samples > ADC_AVERAGE_COUNT) {
          state++;
        }
        sr = counter_sr;
      }
      break;
    case SM_CALIB + 2:  // button wait
      if (buttonStateGet()) {
        cv_average_high = 0;
        samples = 0;
        sr = counter_sr;
        state++;
      }
      break;
    case SM_CALIB + 3:  // high acquisition
      if (sr != counter_sr) {
        cv_average_high += vco->adc[ADC_PITCH];
        samples++;
        if (samples > ADC_AVERAGE_COUNT) {
          // TODO: all the stuff

          int32_t adc_range =
              (cv_average_high - cv_average_low) / CALIB_OCTAVES_DISTANCE;
          vco->calib_scale =
              0x100000000LL / 128 * 12 * ADC_AVERAGE_COUNT / adc_range;
          vco->calib_offset =
              65536 / 128 * 12 * CALIB_OCTAVES_OFFSET -
              ((int64_t)(cv_average_low - (cv_average_high - cv_average_low) /
                                              CALIB_OCTAVES_DISTANCE)) *
                  vco->calib_scale / 65536 / ADC_AVERAGE_COUNT;
          state = SM_WAIT;
        }
        sr = counter_sr;
      }
      break;
      /////////////////////////////////////////////////////////////////////
      // SAVE
  }
}