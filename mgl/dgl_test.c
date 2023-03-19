/*
 * dgl_test.c
 *
 *  Created on: Nov 14, 2020
 *      Author: lu
 */



#include "dgl.h"



void displaySetZone(uint16_t xpos, uint16_t ypos, uint16_t xsize, uint16_t ysize);
void displayOutPixel(MglColor color);



char* lines[] = {
"button1", "BUTTTON2", "Button3", "but_4",
"BUT_5", "BUTTTON 6", "whatever", "something",
"Encoder1","Encoder1","Encoder1","Encoder1",
"Encoder1","Encoder1","Encoder1","Encoder1",
"Func1","F2","PRESS ME", "don't touch!"
};
#define DGL_COLOR_SYSLINES mglExColorFromRgb(64,64,64) //TODO solig lines with 64:64:64 color
#define DGL_COLOR_SYSENCPUSH mglExColorFromRgb(0,255,0)
#define DGL_COLOR_SYSENCTURN mglExColorFromRgb(255,0,0)
#define DGL_COLOR_SYSFUNC mglExColorFromRgb(255,0,255)
const uint8_t sysencpush[] = {0x00, 0x00, 0x00, 0x82, 0x44, 0x28, 0x10, 0x00};
const uint8_t sysencturn[] = {0x00, 0x38, 0x44, 0x82, 0x82, 0x82, 0x44, 0x38};
char* sysfunc[] = {"F1", "F2", "F3", "F4"};
uint8_t linepattern[] = {0x85, 0xAA};



#define WBSIZE 4000

volatile int16_t __attribute__((section(".noinit"))) wavebuf[WBSIZE];

uint16_t phase2sin(uint16_t ph)
{
	if (ph>>15)
	{
		//high
		ph = ~ph;
	}
	//ph <<=1;
	//triangle

	if (ph>0x4000)
	{
		uint32_t v = 0x7FFF - ph;
		v = v * v;
		ph = 0xFFFF - v/0x2000;
	}
	else
	{
		uint32_t v = ph;
		v = v * v;
		ph = v/0x2000;
	}
	// */
	return ph;
}

void fillwb()
{
	uint16_t i;
	uint16_t phase = 0x7000;
	uint16_t pinc = 13 + 65536 * 45 / 3 / WBSIZE;
	for (i = 0; i<WBSIZE; i++)
	{
		phase += pinc;
		wavebuf[i] = phase2sin(phase) - 0x8000;
	}
}


void dgl_test()
{
	mglExSetPaletteColor(DGL_PALETTE_SCOPE,mglExColorFromRgb(0,255,0));
	mglExSetPaletteColor(DGL_PALETTE_SCOPE_OUTSIDE,mglExColorFromRgb(255,255,0));
	mglExSetPaletteColor(DGL_PALETTE_SCOPE_CURSOR,mglExColorFromRgb(128,255,128));
	mglExSetPaletteColor(DGL_PALETTE_SELECTION,mglExColorFromRgb(32,255,32));
	uint16_t x,y;
	//x = dgl_log2fast(123);
	mglSetWorkingArea(0,0,MGL_DISPLAY_WIDTH,MGL_DISPLAY_HEIGHT);
	mglFillBlank();
	//delay

	//first
	displaySetZone(0,0,MGL_DISPLAY_WIDTH,MGL_DISPLAY_HEIGHT);
	for (y = 0; y < MGL_DISPLAY_HEIGHT; y++)
		for (x = 0; x<MGL_DISPLAY_WIDTH; x++)
			displayOutPixel(mglExColorFromHsv(x*256/MGL_DISPLAY_WIDTH,y*256/MGL_DISPLAY_HEIGHT,128));


	//second
	displaySetZone(0,0,MGL_DISPLAY_WIDTH,MGL_DISPLAY_HEIGHT);
	for (y = 0; y < MGL_DISPLAY_HEIGHT; y++)
		for (x = 0; x<MGL_DISPLAY_WIDTH; x++)
			displayOutPixel(mglExColorFromHsv(x*256/MGL_DISPLAY_WIDTH,255,y*256/MGL_DISPLAY_HEIGHT));

	//
	mglExSetPaletteColor(DGL_PALETTE_BLANK, mglExColorFromRgb(0, 0, 0));
	mglExSetPaletteColor(DGL_PALETTE_FONT, mglExColorFromHsv(0, 0, 180));
	mglFillBlank();

	mglSetCursor(10,10);
	mglSetFont((MglFont*)&font6);
	dgl_drawstringpartial("Hello!");

	mglSetWorkingArea(22,22,MGL_DISPLAY_WIDTH,MGL_DISPLAY_HEIGHT);
	mglSetCursor(20,20);
	dgl_drawstringpartial("Hello!");


	mglSetWorkingArea(0,0,45,35);
	mglSetCursor(30,30);
	dgl_drawstringpartial("Hello!");


	mglSetWorkingArea(0,0,MGL_DISPLAY_WIDTH,MGL_DISPLAY_HEIGHT);
	mglSetCursor(20,50);
	dgl_drawstring("Ready!");

	/////////////////////////////////////////////////////////////////////////////////////////////////

//*





	mglSetWorkingArea(0,0,MGL_DISPLAY_WIDTH,MGL_DISPLAY_HEIGHT);
	mglFillBlank();

	//palette[DGL_PALETTE_BLANK] = 0x0;
	mglExSetPaletteColor(DGL_PALETTE_BLANK, mglExColorFromRgb(0, 0, 0));
	mglExSetPaletteColor(DGL_PALETTE_BITMAP, mglExColorFromHsv(0, 0, 100));
	mglExSetPaletteColor(DGL_PALETTE_FONT, mglExColorFromRgb(255,255,255));
	//uint16_t x,y;
	mglSetWorkingArea(0,17,320,1);
	mglFillPattern(linepattern, 2);

	mglSetWorkingArea(0,36,320,1);
	mglFillPattern(linepattern, 2);

	mglSetWorkingArea(0,231,320,1);
	mglFillPattern(linepattern, 11);

	//draw symbols encpush
	//mglExSetPaletteColor(DGL_PALETTE_BLANK, mglExColorFromHsv(0, 0, 40));


	mglExSetPaletteColor(DGL_PALETTE_BITMAP, DGL_COLOR_SYSENCPUSH);

	for(y = 0; y<2; y++)
	{
		for(x = 0; x<4; x++)
		{
			mglSetWorkingArea(x*80, y*9, 79, 8);
			mglDrawBitmap((void*)sysencpush, 8, 8);
			dgl_drawstring(lines[y*4+x]);
		}
	}
	mglExSetPaletteColor(DGL_PALETTE_BITMAP, DGL_COLOR_SYSENCTURN);
	//draw symbols encturn
	for(y = 0; y<2; y++)
	{
		for(x = 0; x<4; x++)
		{
			mglSetWorkingArea(x*80, y*9+19, 79, 8);
			mglDrawBitmap((void*)sysencturn, 8, 8);
			dgl_drawstring(lines[y*4+x+8]);
		}
	}


	//draw system strings

	for(x = 0; x<4; x++)
	{
		mglSetWorkingArea(x*80, 232, 79, 8);
		mglExSetPaletteColor(DGL_PALETTE_FONT, DGL_COLOR_SYSFUNC);
		dgl_drawstringpartial(sysfunc[x]);
		mglExSetPaletteColor(DGL_PALETTE_FONT, mglExColorFromRgb(255,255,255));
		dgl_drawstring(lines[x+16]);
	}

	mglExSetPaletteColor(DGL_PALETTE_BLANK, mglExColorFromHsv(256/3, 100, 60));
	mglSetWorkingArea(0,37,320,240-47);
	mglFillBlank();


	//scope


	mglExSetPaletteColor(DGL_PALETTE_BLANK, mglExColorFromRgb(0, 0, 0));
	mglExSetPaletteColor(DGL_PALETTE_SCOPE, mglExColorFromRgb(0, 255, 0));
	mglExSetPaletteColor(DGL_PALETTE_SCOPE_OUTSIDE, mglExColorFromRgb(255, 128, 0));
	mglExSetPaletteColor(DGL_PALETTE_SCOPE_CURSOR, mglExColorFromRgb(0, 0, 255));
	mglExSetPaletteColor(DGL_PALETTE_SELECTION, mglExColorFromRgb(255, 128, 128));

	fillwb();
	dgl_scope_init(DGL_SCOPETYPE_16S_WORD, (void*)wavebuf, WBSIZE);
	dgl_scope_adjust(DGL_SCOPE_ADJ_SELECT_START, WBSIZE / 4);
	dgl_scope_adjust(DGL_SCOPE_ADJ_SELECT_END, WBSIZE / 4 * 3);
	//dgl_scope_adjust(DGL_SCOPE_ADJ_VERTICAL_ZOOM, 100);
	dgl_drawscope();


	uint16_t vz;
	while(1)
	{
		vz +=100;
		dgl_scope_adjust(DGL_SCOPE_ADJ_HORIZONTAL_VZOOM, vz);
		//dgl_scope_adjust(DGL_SCOPE_ADJ_SELECT_START, ((uint32_t)vz)*WBSIZE/65536);
		dgl_drawscope();
	}

	displaySetZone(0,37,320,240-47);//(160, 120, 20, 20);
	while(1)
		displayOutPixel(mglExColorFromRgb(64,64,64));
// */





}
