
#include "LPC17xx.h"
#include "bsp.h"

// AUDIO CORE
#if BLOCK_SIZE * CONTROL_RATE != SAMPLE_RATE
#error "SR-CR relation is odd"
#endif

#define SR_TIMER_FREQ_HZ 96000000
#define SR_TIMER_NOMINAL_PERIOD (SR_TIMER_FREQ_HZ / SAMPLE_RATE - 1)
#if (SAMPLE_RATE * (SR_TIMER_NOMINAL_PERIOD + 1)) != SR_TIMER_FREQ_HZ
#warning "odd sample rate"
#endif
#if SR_TIMER_NOMINAL_PERIOD >= 16384
#error "sync circuit is prone to overflow"
#endif

// ARCH CORE
#define MB_IRQ_EINT_FREQCAP 0x0
#define MB_IRQ_TIM0_SR 0x2
#define MB_IRQ_CAN_CR 0x3
#define MB_IRQ_I2C_MIDI 0x3
#define MB_IRQ_UART3_MIDI 0x3
#define MB_IRQ_USB 0x2

__attribute__((weak)) void audioCallback(int16_t *in, int16_t *out,
                                         uint16_t *ctrl_in) {
  (void)in;
  (void)out;
  (void)ctrl_in;
}

volatile uint32_t counter_sr;
volatile uint32_t counter_cr;
static int16_t audio_adc_in[BLOCK_SIZE * 2];
static int16_t audio_dac_out[BLOCK_SIZE * 2];
static uint32_t timer_sr_period = 65536;
static uint16_t control_adc_in[8];
static int16_t volume = 32767;

void virtualPots(uint8_t pot, uint16_t value) {
  if (pot < 8) {
    control_adc_in[pot] = value;
  }
}

static inline void ledSr(bool on) {
  if (on) {
    LPC_GPIO2->FIOSET = 1 << 6;
    LPC_GPIO2->FIOCLR = 1 << 5;
  } else {
    LPC_GPIO2->FIOCLR = 1 << 6;
    LPC_GPIO2->FIOSET = 1 << 5;
  }
}

static inline void ledCr(bool on) {
  if (on) {
    LPC_GPIO2->FIOSET = 1 << 4;
    LPC_GPIO2->FIOCLR = 1 << 3;
  } else {
    LPC_GPIO2->FIOCLR = 1 << 4;
    LPC_GPIO2->FIOSET = 1 << 3;
  }
}

uint8_t led_analog;
static inline void ledAnalogTap(uint32_t counter_sr) {
  if ((counter_sr & 0xFF) > led_analog) {
    LPC_GPIO2->FIOSET = 1 << 2;
    LPC_GPIO1->FIOCLR = 1 << 31;
  } else {
    LPC_GPIO2->FIOCLR = 1 << 2;
    LPC_GPIO1->FIOSET = 1 << 31;
  }
}
void ledAnalogSet(uint16_t value) {
  uint32_t v = (uint32_t)value * (uint32_t)value;
  v = v >> 24;
  led_analog = v;
}

void bspAudioInit() {
  // ADC
  //  P0.23
  //  P0.24
  //  P0.25 - pot
  //  P1.30 - pot
  // DAC
  //  P0.26

  LPC_GPIO0->FIODIR &= 0xF87FFFFF;  // pins on input
  LPC_GPIO1->FIODIR &= 0xBFFFFFFF;

  // configure no pu/pd, no open drain etc...
  LPC_PINCON->PINMODE1 &= 0xFFCFFFFF;
  LPC_PINCON->PINMODE1 |= 0x00200000;  // no pupd
  LPC_PINCON->PINSEL1 &= 0xFFCFFFFF;
  LPC_PINCON->PINSEL1 |= 0x00200000;  // AOUT

  // TIM0 - audio sample rate
  LPC_TIM0->PR = 0;  // 96MHz tick
  LPC_TIM0->MR0 = SR_TIMER_NOMINAL_PERIOD;
  LPC_TIM0->MCR |= 0x3;  // generate interrupt and reset

  // start audio system
  NVIC_SetPriority(CAN_IRQn, MB_IRQ_CAN_CR);
  NVIC_EnableIRQ(CAN_IRQn);
  NVIC_SetPriority(TIMER0_IRQn, MB_IRQ_TIM0_SR);
  NVIC_EnableIRQ(TIMER0_IRQn);

  LPC_TIM0->TCR = 0x1;  // start

  ///////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////
  for (int i = 0; i < BLOCK_SIZE * 2; i++) {
    // audio_dac_out[i] = i * (65536 / BLOCK_SIZE / 2) - 0x8000;
    audio_dac_out[i] = 0x8000;
  }
}

#define LCG_A 1103515245
#define LCG_B 12345

void TIMER0_IRQHandler() {
  LPC_TIM0->IR = 1;  // reset interrupt
  ledSr(true);

  counter_sr++;
  if ((counter_sr & (BLOCK_SIZE - 1)) == 0) {
    // start CR processing
    NVIC->STIR = CAN_IRQn;
  }

  static uint32_t lcg;
  lcg = lcg * LCG_A + LCG_B;

  uint32_t tim_period = SR_TIMER_NOMINAL_PERIOD * timer_sr_period + lcg / 32768;
  LPC_TIM0->MR0 = tim_period / 65536;

  uint32_t pos = counter_sr & (BLOCK_SIZE * 2 - 1);
  int32_t smpl = (int32_t)volume * audio_dac_out[pos] / 32768 + 0x8000;
  LPC_DAC->DACR = (smpl & 0xFFC0) | 0x10000;

  ledAnalogTap(counter_sr);
  ledSr(false);
}

void CAN_IRQHandler() {
  NVIC_ClearPendingIRQ(CAN_IRQn);
  ledCr(true);
  bool pos = counter_sr & BLOCK_SIZE ? false : true;
  counter_cr++;

  if (pos) {
    audioCallback(&audio_adc_in[BLOCK_SIZE], &audio_dac_out[BLOCK_SIZE],
                  control_adc_in);
  } else {
    audioCallback(&audio_adc_in[0], &audio_dac_out[0], control_adc_in);
  }

  ledCr(false);
}

BspStatus bspSetAudioPeriod(uint32_t relative) {
  // speed is nonlinear, but this circuit is designed for very narrow range
  uint64_t new = relative *SR_TIMER_NOMINAL_PERIOD;
  if (new > 0xFFFFFFFF) {
    return BSP_REQUEST_ERROR;
  }
  timer_sr_period = relative;
  return BSP_OK;
}

void bspSetAudioVolume(int16_t v) { volume = v; }
