/*
 * microbrute_global.h
 *
 *  Created on: Feb 24, 2020
 *      Author: lu
 */

#ifndef MBSRC_MICROBRUTE_GLOBAL_H_
#define MBSRC_MICROBRUTE_GLOBAL_H_

#include "stdint.h"
#include "midi.h"
#include "dsp.h"
#include "mgl.h"
/*
USE VOLATILE FOR VARIABLES< THAT ARE USED BY FEW THREADS
*/



#define CRITICAL_START  __disable_fault_irq();
#define CRITICAL_END    __enable_fault_irq();

//irq
#define MB_IRQ_EINT_FREQCAP 0x0
#define MB_IRQ_TIM0_SR      0x1
#define MB_IRQ_CAN_CR       0x2
#define MB_IRQ_UART3_MIDI   0x2
#define MB_IRQ_USB          0x2

//
typedef enum
{
	MODSEL_CONST = 0,
	MODSEL_MODWHEEL,
	//MODSEL_BREATH,
	MODSEL_VELOCITY,
	MODSEL_VELOCITY_NOTEON,
	MODSEL_ENVELOPE2,
	MODSEL_LFO,
	MODSEL_PITCHWHEEL,
	MODSEL_KEYTRACK,
	MODSEL_TOTAL
}modsel_en;

typedef struct
{
	uint8_t mod_select1;
	uint8_t mod_select2;
	int16_t amplify1;
	int16_t amplify2;
	int16_t offset1;
	int16_t offset2;
	int16_t offsetend;
	//uint16_t slew; TODO?
}mod_slot_t;

typedef volatile struct//do not save parameters from pots (i.e. glidetime or lfo freq)
{
	//modulation matrix
	mod_slot_t mod_filter;
	mod_slot_t mod_amp;
	mod_slot_t mod_extcv;
	mod_slot_t mod_lfoamp;

	//envelope 2
	uint8_t env2a;
	uint8_t env2d;
	uint8_t env2s;
	uint8_t env2r;
	uint8_t env2retrig; //01 - firstnote/everynote

	//oscillator
	uint8_t osc_retrig; //0/1/2 - off/firstnote/everynote
	uint8_t osc_pbrange;
	uint8_t osc_glidemode;
	uint8_t env_gateretrig;

	//lfo
	uint8_t lfokeysync; //0/1/2 - off/firstnote/everynote
	uint16_t lfosyncstart;
	uint8_t lfomidisync;
	uint8_t lfowave; //wave

	//TODO: sequencer data

	uint32_t validity_key;

}synth_patch_t;

typedef volatile struct
{
	uint8_t extgate_sync; //
	uint8_t synth_midi_channel; //synth reaction to usb/midi
	uint8_t keyboard_midi_channel; //always connected to synth engine though, only output to usb/midi
	uint8_t midi_routing; // 0 - off, 1 - uart soft thru, 2 - local off

}system_settings_t;
//knobs
typedef struct
{
	uint16_t lock_value;
	uint8_t lock;
	uint8_t compare_val;
	void (*cbk)(uint16_t val);
}knob_status_t;

typedef struct
{
	void (*cr)(uint32_t cr_counter, uint16_t *in, uint16_t *out, uint16_t *eint); //SR COUNTER >> CR_SHIFT !!
	void (*msi)(midi_message_t m); //midi stream input
	void (*asyncmain)(uint32_t cr_counter);
	void (*initialize)(uint8_t prevmode);
}audiofunction_t;

typedef struct
{
	uint16_t ch4_pitch; //please write 12 bit values only!!!
	uint16_t ch3_filter;
	uint16_t ch2_amp;
	uint16_t ch1_ext;
}dac_data_t;


extern volatile uint32_t sr_counter; //cr timestamp
extern volatile uint32_t cr_counter; //cr timestamp

extern synth_patch_t mb_patch;
extern system_settings_t mb_settings;

extern knob_status_t pitchwheel;
extern knob_status_t modwheel;
extern knob_status_t lfo_rate;
extern knob_status_t seq_rate;

extern volatile uint16_t modsources[MODSEL_TOTAL];


extern audiofunction_t audiofunction;
extern const audiofunction_t audiof_bootup;
void delay_ms(uint16_t ms);
#ifdef DEBUG
void deb_ccnt_minmax_reset(void);
void deb_ccnt_restart(void);
uint32_t deb_ccnt_lap(void);
#endif

//voice allocator
void va_extgate(uint8_t val);
void microbrute_synth_port(midi_message_t m);


#define SR_CTRL_VECTOR_GATE_OFF 0x01
#define SR_CTRL_VECTOR_GATE_ON  0x02
#define SR_CTRL_VECTOR_OSC_RST  0x04
#define SR_CTRL_VECTOR_GLIDE    0x08


//lfo
void lfo_cr_tap(int32_t lcg);
void lfo_wswitch(uint8_t swpos);

//display
void gui_toggle_display(void);
void gui_framerate(void);

// #define CTRL_CUSTOMCODE_DISPLAYUPDATE   0x01
// #define CTRL_CUSTOMCODE_DISPLAYUPDATE_2 0x02
// #define CTRL_CUSTOMCODE_PANELSWITCH     0x01

/*
 *

typedef struct
{
	audiosample_t audioin[AUDIOBLOCK_SIZE*2];
	audiosample_t audioout[AUDIOBLOCK_SIZE*2];
	uint32_t eintsspos[AUDIOBLOCK_SIZE*2];
	uint32_t einthrpos[AUDIOBLOCK_SIZE*2];

	uint16_t ch4_pitch; //please write 12 bit values only!!!
	uint16_t ch3_filter;
	uint16_t ch2_amp;
	uint16_t ch1_ext;

	uint8_t gate;
	uint8_t oscsync;
	uint8_t glide;
}synth_state_t;



typedef struct
{
	void (*load)(uint32_t id);            //only additional controls and data load
	void (*redraw_static)(void);          //draw static elements (that don't change while running)
	void (*redraw_active)(int8_t);        //redraw dynamic changes
	void (*close)(void);                  //unload something
	//
	void (*encoder_turn)(int8_t val);      //callbacks
	void (*encoder_press)(void);
	void (*encoder_release)(void);
	void (*tap_press)(void);
	void (*tap_release)(void);
	//
	void (*noteon)(uint8_t note, uint8_t velocity);
	void (*noteoff)(uint8_t note, uint8_t velocity);
}menu_page_t;

*/


#endif /* MBSRC_MICROBRUTE_GLOBAL_H_ */
