#ifndef _VCO_H
#define _VCO_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_ADC 4096
#define PITCH_OCTAVES_RANGE 5
#define CALIB_OCTAVES_DISTANCE 2
#define CALIB_OCTAVES_OFFSET 1

typedef enum {
  ADC_PITCH = 0,
  ADC_OCTAVE,
  ADC_GEN1AMP,
  ADC_SYNCPHASE,
  ADC_GEN2PITCH,
  ADC_SYNC,
} VcoAdcEn;

typedef struct {
  uint16_t adc[6];  // 16 bit left align
  uint16_t timer;
  uint16_t pwm[3];
  int32_t calib_scale;
  int32_t calib_offset;

  int32_t lcg;

  int32_t gen1;
  int32_t gen2;

  uint32_t cycles;
} Vco;

void vcoInit(Vco* vco);
void vcoCalibrationLoad(Vco* vco, uint32_t scale, int32_t offset);
void vcoTap(Vco* vco);

#ifdef __cplusplus
}
#endif

#endif  // _VCO_H