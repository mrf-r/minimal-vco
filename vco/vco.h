#ifndef _VCO_H
#define _VCO_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  uint16_t adc[6];  // 16 bit left align
  uint16_t timer;
  uint16_t pwm[3];
  uint32_t calib_scale;
  int32_t calib_offset;
} Vco;

void vcoInit(Vco* osc);
void vcoCalibrationLoad(Vco* osc, uint32_t scale, int32_t offset);
void vcoTap(Vco* osc);

#ifdef __cplusplus
}
#endif

#endif  // _VCO_H