/*
JOY
 P1.20 - center
 P1.23 - A - up
 P1.24 - B - right
 P1.25 - D - down
 P1.26 - C - left
 P2.10 - INT0
LED
 P1.28
 P1.29
 P1.31
 P2.2
 P2.3
 P2.4
 P2.5
 P2.6
ADC
 P0.23 0 - 9
 P0.24 1 - 8
 P0.25 2 - 7 (pot)
 P1.30 4 - 21
DAC
 P0.26 3 - 6

 adc
 P1.31 5
 P0.3  6
 P0.2  7

LCD
 P0.6 - CS
 P0.7 - WR/SCL
 P0.8 - SDO - SSP1
 P0.9 - SDI
 P4.28 - BL control
SDIO
 P0.15 - CLK
 P0.16 - NSS
 P0.17 - MISO - SSP0
 P0.18 - MOSI
 P4.29 - CARDPRESENT
COM
 P0.2 - UART0
 P0.3
 P2.0 - UART1
 P2.1
*/

#include "LPC17xx.h"
#include "bsp.h"
#include "rtt.h"
#include "vco.h"
#include "SEGGER_RTT.h"
#include "GLCD.h"

#define NEWLINE "\r\n"

static inline void ledTimeToggle() {
  if (LPC_GPIO1->FIOPIN & (1 << 28)) {
    LPC_GPIO1->FIOSET = 1 << 29;
    LPC_GPIO1->FIOCLR = 1 << 28;
  } else {
    LPC_GPIO1->FIOCLR = 1 << 29;
    LPC_GPIO1->FIOSET = 1 << 28;
  }
}

// #include "basic_osc.h"
// BasicOcs b;
Vco v;
volatile int32_t deb_arr[1024];

static uint32_t lfo_acc;
extern volatile int8_t modmatrix[3][8];

#include "basic_osc.h"

void audioCallback(int16_t *in, int16_t *out, uint16_t *ctrl_in) {
  (void)in;
  (void)ctrl_in;

  lfo_acc += (0x100000000ULL / CONTROL_RATE) / 4;
  int32_t lfo_tri = boscTriangleRaw((int32_t)lfo_acc);
  // controls input
  for (int i = 0; i < 6; i++) {
    int32_t p = modmatrix[0][i] * 256;
    p += (lfo_tri / 256 * modmatrix[1][i] / 65536);
    if (p > 32768 - 1)
      p = 32768 - 1;
    else if (p < -32768)
      p = -32768;
    v.ctrl[i] = p + 0x8000;  // to unsig
  }
  // v.ctrl[CTRL_PITCH] = (((uint16_t)modmatrix[0][0] + 128) * 256) |
  //                      (((uint16_t)modmatrix[0][1] + 128));
  // v.ctrl[CTRL_OCTAVE] = 0;
  // v.ctrl[CTRL_MIX] = 0x8000;
  // v.ctrl[CTRL_PHASE] = 0x8000;
  // v.ctrl[CTRL_2PITCH] = 0;
  // v.ctrl[CTRL_SYNC] = 0;

  for (int i = 0; i < BLOCK_SIZE; i++) {
    static int p = 0;
    p = (p + 1) & 1023;
    int32_t pg = v.gen1;
    vcoTap(&v);
    // int32_t tri = boscTriangleRaw(v.gen1);
    // int32_t sin = boscParabolicSineRaw(tri);

    // scopeWrite(v.pwm[0] >> 9, v.gen1 < pg ? 1 : 0);

#if DEBUG == 1
    deb_arr[p] = v.debug1;
#endif
    // out[i] = boscParabolicSine(&b) / 65536;

    // conversion to signed is due to optimized osc
    out[i] = v.out[1];
    // out[i] = sin / 65536;

    // void scopeWrite(int8_t sample, bool trigger);
    // scopeWrite(out[i] / 256, v.gen1 < pg ? 1 : 0);
    void scopeXY(int16_t x, int16_t y);
    scopeXY(v.gen1 / 0x10000, out[i]);
  }
}

int main(void) {
  // enable DWT and other debug systems
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  // enable cycle counter
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
  // init hw
  // debug leds
  LPC_GPIO1->FIODIR |= 0xB0000000;
  LPC_GPIO2->FIODIR |= 0x0000007C;
  // audio
  vcoInit(&v);
  // boscInit(&b, 800);
  bspAudioInit();

  GLCD_Init();
  GLCD_Clear(0x0);
  // GLCD_SetTextColor(0xFC00);
  // GLCD_SetBackColor(0x003F);
  // GLCD_DisplayString(0, 0, 0, "hello!");
  void menuRedraw();
  menuRedraw();

  // adc and dac
  // timer clk irq sr
  // eth irq block

  // buttons
  vcoTap(&v);

  while (1) {
    static uint32_t counter_prev = 0;
    if (counter_cr > counter_prev) {
      counter_prev += CONTROL_RATE;
      ledTimeToggle();
    }

    // void delayUs(uint32_t time_us);
    // delayUs(1000);
    // ledTimeToggle();

    vcoMain(&v);
    // scope update

    void menuTap();
    menuTap();
  };

  // start lcd (вообще можно раздуть за ХУ-осциллограф)
  // start midi (serial)
  // start sdcard

  return 0;
}

// SYSTEM
#define XTAL (12000000UL)   /* Oscillator frequency               */
#define OSC_CLK (XTAL)      /* Main oscillator frequency          */
#define RTC_CLK (32768UL)   /* RTC oscillator frequency           */
#define IRC_OSC (4000000UL) /* Internal RC oscillator frequency   */

// configured for:
// input - 24MHz
// Core - 96 MHz
// USB - 48 MHz
// Periphery - 24 MHz
// enabled: PWM1, I2C0, SSP1, ADC, GPIO, UART3, USB, DMA

#define CLOCK_SETUP 1
#define SCS_Val 0x00000020        // 12 MHz
#define CLKSRCSEL_Val 0x00000001  // main osc as pll src
#define PLL0_SETUP 1
// 2 * 12 * 0x60 / 0x6 / 4 = 96 MHz   PLL0 output = 384 MHz
#define PLL0CFG_Val 0x0005005F
#define PLL1_SETUP 0
#define PLL1CFG_Val 0x00000021
#define CCLKCFG_Val 0x00000003    // PLL0 div 4
#define USBCLKCFG_Val 0x00000007  // PLL0 div 8
#define PCLKSEL0_Val 0x00001014   // FULL FREQ: TIM0, TIM1, PWM - 96 MHz
#define PCLKSEL1_Val 0x00000000   // all others - 24 MHz
// USB DMA GPIO PCAD SSP1 SSP0 UART1 UART0 TIM1 TIM0 enabled
#define PCONP_Val 0xA020941E  // DMA???
#define CLKOUTCFG_Val 0x00000030

#define FLASH_SETUP 1
#define FLASHCFG_Val 0x0000403A

void SystemInit(void) {
  ////////////////////////////////////////////////////////////////////////////////////
  // clock
  const uint32_t PLL0_CONNECT_FLG = (1 << 25) | (1 << 24);
  const uint32_t PLL1_CONNECT_FLG = (1 << 8) | (1 << 9);

#if (CLOCK_SETUP) /* Clock Setup                        */
  LPC_SC->SCS = SCS_Val;
  if (LPC_SC->SCS & (1 << 5)) { /* If Main Oscillator is enabled  */
    while ((LPC_SC->SCS & (1 << 6)) == 0)
      ; /* Wait for Oscillator to be ready    */
  }

  LPC_SC->CCLKCFG = CCLKCFG_Val; /* Setup Clock Divider                */
  /* Periphral clock must be selected before PLL0 enabling and connecting
   * - according errata.lpc1768-16.March.2010 -
   */
  LPC_SC->PCLKSEL0 = PCLKSEL0_Val; /* Peripheral Clock Selection         */
  LPC_SC->PCLKSEL1 = PCLKSEL1_Val;

#if (PLL0_SETUP)
  LPC_SC->CLKSRCSEL = CLKSRCSEL_Val; /* Select Clock Source for PLL0       */

  LPC_SC->PLL0CFG = PLL0CFG_Val; /* configure PLL0                     */
  LPC_SC->PLL0FEED = 0xAA;
  LPC_SC->PLL0FEED = 0x55;

  LPC_SC->PLL0CON = 0x01; /* PLL0 Enable                        */
  LPC_SC->PLL0FEED = 0xAA;
  LPC_SC->PLL0FEED = 0x55;
  while (!(LPC_SC->PLL0STAT & (1 << 26)))
    ; /* Wait for PLOCK0                    */

  LPC_SC->PLL0CON = 0x03; /* PLL0 Enable & Connect              */
  LPC_SC->PLL0FEED = 0xAA;
  LPC_SC->PLL0FEED = 0x55;
  while ((LPC_SC->PLL0STAT & PLL0_CONNECT_FLG) != PLL0_CONNECT_FLG)
    ; /* Wait for PLLC0_STAT & PLLE0_STAT */
#endif

#if (PLL1_SETUP)
  LPC_SC->PLL1CFG = PLL1CFG_Val;
  LPC_SC->PLL1FEED = 0xAA;
  LPC_SC->PLL1FEED = 0x55;

  LPC_SC->PLL1CON = 0x01; /* PLL1 Enable                        */
  LPC_SC->PLL1FEED = 0xAA;
  LPC_SC->PLL1FEED = 0x55;
  while (!(LPC_SC->PLL1STAT & (1 << 10)))
    ; /* Wait for PLOCK1                    */

  LPC_SC->PLL1CON = 0x03; /* PLL1 Enable & Connect              */
  LPC_SC->PLL1FEED = 0xAA;
  LPC_SC->PLL1FEED = 0x55;
  while ((LPC_SC->PLL1STAT & PLL1_CONNECT_FLG) != PLL1_CONNECT_FLG)
    ; /* Wait for PLLC1_STAT & PLLE1_STAT */
#else
  LPC_SC->USBCLKCFG = USBCLKCFG_Val; /* Setup USB Clock Divider            */
#endif
  LPC_SC->PCONP = PCONP_Val; /* Power Control for Peripherals      */

  LPC_SC->CLKOUTCFG = CLKOUTCFG_Val; /* Clock Output Configuration         */
#endif

#if (FLASH_SETUP == 1) /* Flash Accelerator Setup            */
  LPC_SC->FLASHCFG = FLASHCFG_Val;
#endif

  // SCB->VTOR = 0x00000000 & 0x3FFFFF80;
  (void)PLL0_CONNECT_FLG;
  (void)PLL1_CONNECT_FLG;

  ////////////////////////////////////////////////////////////////////////////////////
  // variables
  void _startup_variables_init(void);
  _startup_variables_init();

  SEGGER_RTT_Init();
  debugPrintStr(NEWLINE "- = openocd RTT console = -" NEWLINE);
  ////////////////////////////////////////////////////////////////////////////////////
}
