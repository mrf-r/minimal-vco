#ifndef _VCO_H
#define _VCO_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_ADC 4096UL
#define PITCH_OCTAVES_RANGE 5
#define CALIB_OCTAVES_DISTANCE 2
#define CALIB_OCTAVES_OFFSET 1

// 32 bytes,
typedef struct {
  uint32_t code;
  int32_t scale;
  int32_t offset;
  uint32_t reserved0;
  uint32_t reserved1;
  uint32_t reserved2;
  uint32_t reserved3;
  uint32_t reserved4;
} SaveBlock;

typedef enum {
  ADC_PITCH = 0,
  ADC_OCTAVE,
  ADC_GEN1AMP,
  ADC_SYNCPHASE,
  ADC_GEN2PITCH,
  ADC_SYNC,
} VcoAdcEn;

typedef struct {
  // uint16_t timer;
  // IO
  uint16_t adc[6];  // 16 bit left align
  uint16_t pwm[3];

  // internal
  int32_t lcg;
  int32_t gen1;
  int32_t gen2;

  // saveblock
  int32_t calib_scale;
  int32_t calib_offset;

  // main
  uint32_t state;
  uint32_t cv_average_low;
  uint32_t cv_average_high;
  uint32_t sr;
  uint32_t samples;
  uint32_t menu_delay;

#ifdef DEBUG
  uint32_t cycles;
#endif  // DEBUG
} Vco;

void vcoInit(Vco* vco);
void vcoCalibrationLoad(Vco* vco, uint32_t scale, int32_t offset);
void vcoTap(Vco* vco);
void vcoMain(Vco* vco);

#ifdef __cplusplus
}
#endif

#endif  // _VCO_H