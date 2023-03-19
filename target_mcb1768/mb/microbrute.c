/*
 * microbrute.c
 *
 *  Created on: Sep 28, 2020
 *      Author: lu
 */


#include "microbrute_global.h"
#include "microbrute_hw.h"
#include "dsp.h"

synth_patch_t mb_patch;
system_settings_t mb_settings;

knob_status_t ks_pitchwheel;
knob_status_t ks_modwheel;
knob_status_t ks_lfo_rate;
knob_status_t ks_seq_rate;

volatile uint16_t modsources[MODSEL_TOTAL];




extern volatile uint16_t eint_state[32]; // sync/frequency counter
extern volatile uint16_t audio_adc_in[32];
extern volatile uint16_t audio_dac_out[32];

void cr_passthru(uint32_t cr_c, uint16_t *in, uint16_t *out, uint16_t *eint)
{
	/*
	REPEAT(
	if (*eint)
	{
		*out = 0xFFFF;
		*eint = 0;
	}
	else
	{
		*out = (*in>>1) + 0x4000; //divide by 2 to clear view of interrupts
	}
	in++;
	out++;
	eint++;
	) //REPEAT END
	// */

	REPEAT( //PASSTHRU
	*out = *in;
	out++;
	in++;
	)


	/*
	// TEST CALIBRATION SIGNAL
	if (cr_c & 1)
	{
		REPEAT(
		*out = 0x8000;
		out++;
		)
	}
	else
	{
		REPEAT8(
		*out = 0x0;
		out++;
		)
		REPEAT8(
		*out = 0xFFFF;
		out++;
		)
	}
	// */


	/*
	*out = 0xFFFF; out++; //0
	*out = 0xFFFF; out++;
	*out = 0xFFFF; out++;
	*out = 0xFFFF; out++;
	*out = 0x0000; out++; //4
	*out = 0x0000; out++;
	*out = 0x0000; out++;
	*out = 0x0000; out++;
	*out = 0x8000; out++; //8
	*out = 0x8000; out++;
	*out = 0x8000; out++;
	*out = 0x8000; out++;
	*out = 0x8000; out++; //12
	*out = 0x8000; out++;
	*out = 0x8000; out++;
	*out = 0x8000; out++; */
}

const audiofunction_t audiof_bootup =
{
		cr_passthru,      // 	void (*cr)(uint32_t cr_counter);
		0,      // 	void (*msi)(midi_message_t m); //midi stream input
		0,      // 	void (*asyncmain)(void);
		0       // 	void (*initialize)(uint8_t prevmode);
};

audiofunction_t audiofunction = audiof_bootup;


void microbrute_mainloop()
{
	//init midi stuff ports and other


	//init parameters
	init_periph();
	//init USB

	//load default parameters



	//init audiofunction
	if (audiofunction.initialize)
		audiofunction.initialize(0);
	for(;;)
	{
		//control panel capture


		//main
		midi_handling();

		//sequencer stuff

		//additional
		if (audiofunction.asyncmain)
			audiofunction.asyncmain(cr_counter);

		i2c_daemon(); //TODO

		gui_framerate();


	}
}


void async()
{
	//CONTROLS


	//keyboard block
	// octave keys

	//LFO block
	// wave switch - async
	// sync switch - async

	//main control block
	// seq rate knob
	// state switch
	// tap button
	// mw dest switch?



	//sequencer / arp processing
	//events processing / synth engine

	//lcd block
	// keep tracking of state - run scope or any other screen saver
	//uint32_t screen_last_activity_time = 0;

	//audiofunction async

}
