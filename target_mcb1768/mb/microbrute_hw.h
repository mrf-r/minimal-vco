/*
 * microbrute_hw.h
 *
 *  Created on: 19 ���. 2019 �.
 *      Author: ���
 *
 *
 *      so it's about a time to rewrite it all completely
 *
 *      adc and i2c dac buttons leds and pots scan rate - 3kHz
 *      r2r, pwm dac sampling and kbd scan - 48kHz
 *
 *
 *      60hz - sw timer
 *      	display
 *
 *      3khz - async timer
 *      	adc
 *      	i2c
 *      	buttons
 *      	leds
 *      	60Hz display
 *
 *      48kHz - hw timer
 */
#ifndef NEWCODE_MICROBRUTE_HW_H_
#define NEWCODE_MICROBRUTE_HW_H_

#include "stdint.h"

#define PLACE_AT_AHB_SRAM __attribute__((section(".RamAHB32_noinit")))
#define PLACE_AT_USB_SRAM __attribute__((section(".RamUSB_noinit")))
#define PLACE_AT_CCM_SRAM __attribute__((section(".RamLoc32_noinit")))



//constants
//*****************************************************************************************************
//OUTPUT
//GPIO 0
static uint32_t* const bbo_osc_reset     = (uint32_t*)(0x22000000 + (0x9C000 + 0x14)*32 + 2*4);
static uint32_t* const bbo_gate_out      = (uint32_t*)(0x22000000 + (0x9C000 + 0x14)*32 + 3*4);
static uint32_t* const bbo_glide_mode    = (uint32_t*)(0x22000000 + (0x9C000 + 0x14)*32 + 4*4);
static uint32_t* const bbo_disp_reset    = (uint32_t*)(0x22000000 + (0x9C000 + 0x14)*32 + 5*4);    //P0_05 0=reset
static uint32_t* const bbo_disp_ce       = (uint32_t*)(0x22000000 + (0x9C000 + 0x14)*32 + 6*4);    //P0_06 0=enable
static uint32_t* const bbo_disp_cd       = (uint32_t*)(0x22000000 + (0x9C000 + 0x14)*32 + 8*4);    //P0_08 0=command
static uint32_t* const bbo_led_seq_rate  = (uint32_t*)(0x22000000 + (0x9C000 + 0x14)*32 + 22*4);
//GPIO 1
static uint32_t* const bbo_ldac          = (uint32_t*)(0x22000000 + (0x9C000 + 0x34)*32 + 0*4);
static uint32_t* const bbo_led_lfo_rate  = (uint32_t*)(0x22000000 + (0x9C000 + 0x34)*32 + 4*4);
static uint32_t* const bbo_led_record    = (uint32_t*)(0x22000000 + (0x9C000 + 0x34)*32 + 8*4);
static uint32_t* const bbo_disp_9ven     = (uint32_t*)(0x22000000 + (0x9C000 + 0x34)*32 + 14*4);   //P1_14 0=disable
static uint32_t* const bbo_led_ld1       = (uint32_t*)(0x22000000 + (0x9C000 + 0x34)*32 + 18*4);
//GPIO 2
static uint32_t* const bbo_led_oct_m2    = (uint32_t*)(0x22000000 + (0x9C000 + 0x54)*32 + 2*4);
static uint32_t* const bbo_led_oct_m1    = (uint32_t*)(0x22000000 + (0x9C000 + 0x54)*32 + 3*4);
static uint32_t* const bbo_led_oct_z     = (uint32_t*)(0x22000000 + (0x9C000 + 0x54)*32 + 4*4);
static uint32_t* const bbo_led_oct_p1    = (uint32_t*)(0x22000000 + (0x9C000 + 0x54)*32 + 5*4);
static uint32_t* const bbo_led_oct_p2    = (uint32_t*)(0x22000000 + (0x9C000 + 0x54)*32 + 6*4);
static uint32_t* const bbo_audio_switch  = (uint32_t*)(0x22000000 + (0x9C000 + 0x54)*32 + 7*4);

//INPUT
//FIO0PIN0 - 0x2009C014
static volatile uint32_t* const bbi_oct_dn     = (uint32_t*)(0x22000000 + (0x9C000 + 0x14)*32 + 10*4);  //
static volatile uint32_t* const bbi_oct_up     = (uint32_t*)(0x22000000 + (0x9C000 + 0x14)*32 + 11*4);  //
//FIO1PIN 0 - 0x2009C034
static volatile uint32_t* const bbi_play       = (uint32_t*)(0x22000000 + (0x9C000 + 0x34)*32 + 9*4);   //
static volatile uint32_t* const bbi_rec        = (uint32_t*)(0x22000000 + (0x9C000 + 0x34)*32 + 10*4);  //
static volatile uint32_t* const bbi_enc_1      = (uint32_t*)(0x22000000 + (0x9C000 + 0x34)*32 + 15*4);  //
static volatile uint32_t* const bbi_enc_but    = (uint32_t*)(0x22000000 + (0x9C000 + 0x34)*32 + 16*4);  //
static volatile uint32_t* const bbi_enc_2      = (uint32_t*)(0x22000000 + (0x9C000 + 0x34)*32 + 17*4);  //
static volatile uint32_t* const bbi_mw_dst     = (uint32_t*)(0x22000000 + (0x9C000 + 0x34)*32 + 19*4);  //

static volatile uint32_t* const bbi_kb_sp0     = (uint32_t*)(0x22000000 + (0x9C000 + 0x34)*32 + 22*4);  //
static volatile uint32_t* const bbi_kb_sp1     = (uint32_t*)(0x22000000 + (0x9C000 + 0x34)*32 + 23*4);  //
static volatile uint32_t* const bbi_kb_sp2     = (uint32_t*)(0x22000000 + (0x9C000 + 0x34)*32 + 24*4);  //
static volatile uint32_t* const bbi_kb_sp3     = (uint32_t*)(0x22000000 + (0x9C000 + 0x34)*32 + 25*4);  //
static volatile uint32_t* const bbi_kb_fp0     = (uint32_t*)(0x22000000 + (0x9C000 + 0x34)*32 + 26*4);  //
static volatile uint32_t* const bbi_kb_fp1     = (uint32_t*)(0x22000000 + (0x9C000 + 0x34)*32 + 27*4);  //
static volatile uint32_t* const bbi_kb_fp2     = (uint32_t*)(0x22000000 + (0x9C000 + 0x34)*32 + 28*4);  //
static volatile uint32_t* const bbi_kb_fp3     = (uint32_t*)(0x22000000 + (0x9C000 + 0x34)*32 + 29*4);  //

//FIO2PIN 0 - 0x2009C054
static volatile uint32_t* const bbi_lfo_snc    = (uint32_t*)(0x22000000 + (0x9C000 + 0x54)*32 + 8*4);   //
static volatile uint32_t* const bbi_tap        = (uint32_t*)(0x22000000 + (0x9C000 + 0x54)*32 + 11*4);  //
static volatile uint32_t* const bbi_gate       = (uint32_t*)(0x22000000 + (0x9C000 + 0x54)*32 + 12*4);  //
//FIO3PIN 0 - 0x2009C074
static volatile uint32_t* const bbi_lfo_wv_sqr = (uint32_t*)(0x22000000 + (0x9C000 + 0x74)*32 + 25*4);  //
static volatile uint32_t* const bbi_lfo_wv_tri = (uint32_t*)(0x22000000 + (0x9C000 + 0x74)*32 + 26*4);  //
//FIO4PIN0 - 0x2009C094

static volatile uint32_t* const ssp1_sr_bsy = (uint32_t*)(0x42000000 + (0x30000 + 0x0C)*32 + 4*4);   //1 = BUSY
static volatile uint32_t* const ssp1_sr_tnf = (uint32_t*)(0x42000000 + (0x30000 + 0x0C)*32 + 1*4);   //1 = BUSY

typedef struct
{
	uint16_t pitchwh;
	uint16_t modwh;
	uint16_t lforate;
	uint16_t seqrate;
}control_adc_in_t;
typedef union
{
	struct
	{
		uint16_t pitch;
		uint16_t filter;
		uint16_t amp;
		uint16_t ext;
	};
	uint8_t bytes[8];
	uint32_t words[2];
}control_dac_out_t;

#define DISP_RESET(state) *bbo_disp_reset=state
#define DISP_ENABLE(state) *disp_ce=state
#define DISP_CD(state) *bbo_disp_cd=state
#define DISP_9VEN(state) *bbo_disp_9ven=state
#define DISP_RESET_ON    0
#define DISP_RESET_OFF   1
#define DISP_CE_ENABLE   0
#define DISP_CE_DISABLE  1
#define DISP_CD_COMMAND  0
#define DISP_CD_DATA     1
#define DISP_9V_ENABLE   1
#define DISP_9V_DISABLE  0
/*
#define disp_reset_on    0
#define disp_reset_off   1
#define disp_ce_enable   0
#define disp_ce_disable  1
#define disp_cd_command  0
#define disp_cd_data     1
#define disp_9v_enable   1
#define disp_9v_disable  0
*/
#define KBD_ROWSEL_INIT 0x003F8000
#define KBD_ROWSEL_0    0x00008000
#define KBD_ROWSEL_1    0x00010000
#define KBD_ROWSEL_2    0x00020000
#define KBD_ROWSEL_3    0x00040000
#define KBD_ROWSEL_4    0x00080000
#define KBD_ROWSEL_5    0x00100000
#define KBD_ROWSEL_6    0x00200000

#define LED_OCT_0 0x00000004
#define LED_OCT_1 0x00000008
#define LED_OCT_2 0x00000010
#define LED_OCT_3 0x00000020
#define LED_OCT_4 0x00000040
#define LED_OCT_R 0x0000007C

#define GATE_OUT_ON 1
#define GATE_OUT_OFF 0
#define OSC_RESET_STOP 0
#define OSC_RESET_WORK 1
#define GLIDE_MODE_OFF 0
#define GLIDE_MODE_SLIDE 1
#define AUDIO_SWITCH_DAC 0
#define AUDIO_SWITCH_BRUTE 1

/*
 * dma - one time 32 words
 * 1 - ADC audio start with TIM0 MAT0.1 on rising edge
 * 2 - ADC control start now
 *
 *
 *
 */

//#define ADCCAP_0_ALL___ 0x00000037
#define ADCCAP_0_AUDIO 0x07200204
#define ADCCAP_1_PITCH 0x07200201
#define ADCCAP_2_MODWH 0x07200202
#define ADCCAP_3_LFORT 0x07200210
#define ADCCAP_4_SEQRT 0x07200220

//#define ADCCAP_AUDIOSTART 0x01200204
//#define ADCSTP_1_PITCH 0x00200205
//#define ADCSTP_2_MODWH 0x00200206
//#define ADCSTP_3_LFORT 0x00200214
//#define ADCSTP_4_SEQRT 0x00200224
//#define ADCCAP_STOP    0x00200200
#define ADCDMACH_CTRL  0x04480020 // 32 transfers, source inc
#define ADCDMACH_CFG   0x00000901 //
//LPC_ADC->ADCR |= 0x00200000; //ADC PDN reset
//LPC_ADC->ADCR |= 0x00000200; //ADC CLKDIV - divider-1

void microbrute_mainloop(void);
void delay_ms(uint16_t ms);
void init_periph(void);
void display_update_restart(void);
void display_update_transfer_data(void);
void midi_handling(void);



















#endif /* NEWCODE_MICROBRUTE_HW_H_ */
