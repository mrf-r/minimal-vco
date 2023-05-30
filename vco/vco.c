#include "vco.h"
#include "bsp.h"
#include "scale.h"

#define GEN2_MAX_OCTAVE_OFFSET 4

#define CELL_STEPS (65536 / TABLE_SIZE)
// static inline void oscIncGet(uint16_t pitch, uint32_t* inc, uint32_t* recp) {
void oscIncGet(uint16_t pitch, uint32_t* inc, uint32_t* recp) {
  uint32_t pos = pitch / CELL_STEPS;
  uint32_t spos = pitch & (CELL_STEPS - 1);
  uint32_t v0 = table_pitch_inc[pos].inc;
  *recp = table_pitch_inc[pos].recp;
  int32_t diff = table_pitch_inc[pos + 1].inc - v0;
  *inc = v0 + diff * spos / CELL_STEPS;
}

void vcoInit(Vco* vco) {
  // vco->calib_scale =
  //     ((65536 / 128 * 12 * PITCH_OCTAVES_RANGE) * 65536) / MAX_ADC;
  // vco->calib_offset = 65536 / 128 * 12;
}

void vcoCalibrationLoad(Vco* vco, uint32_t scale, int32_t offset) {
  vco->calib_scale = scale;
  vco->calib_offset = offset;
}

static inline int32_t pd(uint32_t inc, int32_t noise16) {
  return (inc / 65536) * noise16;
}

void vcoTap(Vco* vco) {
#ifdef DEBUG
  uint32_t cycles = bspTimerGet();
#endif  // DEBUG
  int32_t lcg = vco->lcg;
  lcg = lcg * 1103515245 + 12345;
  vco->lcg = lcg;
  int32_t lcg16 = lcg / 65536;

  // pitch calibration
  // int32_t base_pitch =
  //     vco->adc[CTRL_PITCH] * vco->calib_scale / 65536 + vco->calib_offset;
  // if (base_pitch < 0) {
  //   base_pitch = 0;
  // } else {
  //   if (base_pitch > 65535) {
  //     base_pitch = 65535;
  //   }
  // }

  uint32_t base_inc;
  uint32_t base_recp;
  oscIncGet(vco->ctrl[CTRL_PITCH], &base_inc, &base_recp);
  // gen1 core
  int32_t gen1new = vco->gen1 + base_inc;
  // int32_t gen1new = vco->gen1 + 0x100000000ULL / SAMPLE_RATE * 2600;

  // gen1 octave
  // PD must be calculated for every octave
  const uint32_t oct_fade_steps = 65536 / 4;
  uint32_t oct_mul = vco->ctrl[CTRL_OCTAVE] / oct_fade_steps + 1;
  uint32_t oct_fade = vco->ctrl[CTRL_OCTAVE] & (oct_fade_steps - 1);
  // get 2 octaves of gen1 signal with PDAM
  int32_t gen1o1 = gen1new * (1 << oct_mul) + pd(base_inc * oct_mul, lcg16);
  int32_t gen1o2 = gen1new * (2 << oct_mul) + pd(base_inc * oct_mul, lcg16);
  // x-fade them

  // int32_t gen1full = gen1o1 / oct_fade_steps * (oct_fade_steps - 1 -
  // oct_fade) +
  //                    gen1o2 / oct_fade_steps * oct_fade;
  int32_t gen1full = gen1o1 / (int32_t)oct_fade_steps *
                         (int32_t)(oct_fade_steps - 1 - oct_fade) +
                     gen1o2 / (int32_t)oct_fade_steps * (int32_t)oct_fade;

  // vco->pwm[0] = ((gen1full + 0x80000000) / 65536 * MAX_PWM + (uint32_t)lcg16)
  // / 65536;
  // vco->debug1 = base_inc;
  vco->out[0] = gen1full / 65536;
  // GEN2 is hard synced to gen1core
  uint32_t inc2 = base_inc + base_inc / (65536 / GEN2_MAX_OCTAVE_OFFSET) *
                                 vco->ctrl[CTRL_2PITCH];
  int32_t gen2new;
  if (gen1new < vco->gen1) {
    // sync gen2
    uint32_t adc_sync = vco->ctrl[CTRL_SYNC] * vco->ctrl[CTRL_SYNC] / 65536;
    if (adc_sync < (uint32_t)lcg16) { // TODO: not the full
      uint32_t subpos_norm = (gen1new & 0x7FFFFFFF) * base_recp;
      gen2new = (subpos_norm / 65536) * inc2 / 65536;
      // random phase on a rare events
      gen2new += lcg16 * adc_sync;
      // phasemod disabled, need depth knob
      gen2new += vco->ctrl[CTRL_PHASE] * 65536;
    } else {
      goto nosync;
    }
  } else {
  nosync:
    gen2new = vco->gen2 + inc2;
  }
  vco->gen1 = gen1new;

  // vco->pwm[1] = gen2new / 65536 + 0x8000;

  int32_t gen2o = gen2new + pd(gen2new - vco->gen2, lcg16);
  vco->gen2 = gen2new;

  int32_t adc_mix = vco->ctrl[CTRL_MIX] - 65536 / 2;
  int32_t adc_mix_abs = adc_mix < 0 ? -adc_mix : adc_mix;
  int32_t mix = (gen2o / 65536) * (65536 - 1 - adc_mix_abs) +
                (gen1full / 65536) * adc_mix;
  vco->out[1] = mix / 65536;
  // ((mix + 0x80000000) / 65536 * MAX_PWM + (uint32_t)lcg16) / 65536;

#ifdef DEBUG
  uint32_t proc_cycles = bspTimerGet() - cycles;
  if (vco->cycles < proc_cycles)
    vco->cycles = proc_cycles;
  else
    vco->cycles--;
#endif  // DEBUG
}
