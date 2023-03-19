#include "mgl.h"

typedef enum {
    DGL_SCOPE_ADJ_VERTICAL_ZOOM = 0,
    DGL_SCOPE_ADJ_HORIZONTAL_VZOOM,
    DGL_SCOPE_ADJ_SELECT_START,
    DGL_SCOPE_ADJ_SELECT_END,
    DGL_SCOPE_ADJ_SELECT_LENGTH,
    DGL_SCOPE_ADJ_START_OFFSET,
    DGL_SCOPE_ADJ_OFFSET_SHIFT_INT16T
} dgl_scopeadj_en;

#define DGL_SCOPETYPE_8U_BYTE 0
#define DGL_SCOPETYPE_8S_BYTE 1
#define DGL_SCOPETYPE_16U_WORD 2
#define DGL_SCOPETYPE_16S_WORD 3
#define DGL_SCOPETYPE_32U_WORD 4
#define DGL_SCOPETYPE_32S_WORD 5

// can't find other ways
static inline uint32_t dgl_clz(uint32_t value)
{
    uint32_t result;
    asm volatile("CLZ %0, %1"
                 : "=r"(result)
                 : "r"(value));
    return (result);
}
uint8_t dgl_log2fast(uint16_t intensity)
{
    if (intensity) {
        uint32_t sp = (31 - dgl_clz(intensity)) * 16;
        uint32_t lin = (intensity - (1 << sp)) << (16 - sp);
        lin = ((lin * 16) >> 16) + sp;
        return (uint8_t)lin;
    }
    return 0;
}

uint16_t scopebuffer[256];
uint8_t dgl_scope_type; // type of data
uint16_t dgl_scope_smppx; // samples per pixel - horizontal scale
uint32_t dgl_scope_smpxval; // pixel normalization to 0..FFFF
int32_t dgl_scope_vscalef; // vertical scale factor, from 8 to max
uint32_t dgl_scope_length;
uint32_t dgl_scope_selection_start;
uint32_t dgl_scope_selection_end;
uint32_t dgl_scope_start_offset;
int16_t dgl_scope_sel_startpix;
uint16_t dgl_scope_sel_startalpha;
int16_t dgl_scope_sel_endpix;
uint16_t dgl_scope_sel_endalpha;
void* dgl_scope_signal_data;

#define LCG_A 1103515245
#define LCG_B 12345
uint32_t dgl_lcg;
static inline int16_t dgl_lcg_tap()
{
    int16_t res;
    dgl_lcg = dgl_lcg * LCG_A + LCG_B;
    res = (int16_t)(dgl_lcg >> 16);
    return res;
}
static inline uint32_t dgl_smpposrand(uint32_t samplepos)
{
    int32_t nr = ((int32_t)dgl_scope_smppx) * dgl_lcg_tap() / 65536;
    int32_t nsp = nr + samplepos;
    if (nsp < 0)
        nsp = 0;
    else if (nsp > dgl_scope_length)
        nsp = dgl_scope_length;
    return nsp;
}

void dgl_scope_adjust(dgl_scopeadj_en command, uint32_t data)
{
    switch (command) {
    case DGL_SCOPE_ADJ_VERTICAL_ZOOM: {
        data &= 0xFFFF;
        data = (data * 0xF000) >> 16;
        switch (dgl_scope_type) {
        case DGL_SCOPETYPE_8U_BYTE:
        case DGL_SCOPETYPE_8S_BYTE: {
            uint32_t coarse = 1 << (data >> 13); // 8 steps exponent function
            uint32_t tz = ((coarse * (data & 0x1FFF)) >> 13) + coarse; // 0..255
            uint32_t max = 0xFF / (way_size - 2);
            dgl_scope_vscalef = tz > max ? 0x100 : tz * (way_size - 2);
        } break;
        case DGL_SCOPETYPE_16U_WORD:
        case DGL_SCOPETYPE_16S_WORD: {
            uint32_t coarse = 1 << (data >> 12); // 16 steps exponent function
            uint32_t tz = ((coarse * (data & 0xFFF)) >> 12) + coarse; // 0..65535
            uint32_t max = 0xFFFF / (way_size - 2);
            dgl_scope_vscalef = tz > max ? 0x10000 : tz * (way_size - 2);
        } break;
        case DGL_SCOPETYPE_32U_WORD:
        case DGL_SCOPETYPE_32S_WORD: {
            uint32_t coarse = 1 << (data >> 11); // 32 steps exponent function
            uint32_t tz = ((((uint64_t)coarse) * (data & 0x7FF)) >> 11) + coarse; // 0..2^32
            uint32_t max = 0x7FFFFFFF / (way_size - 2);
            dgl_scope_vscalef = tz > max ? 0x80000000 : tz * (way_size - 2);
        } break;
        }
    } break;
    case DGL_SCOPE_ADJ_HORIZONTAL_VZOOM: {
        data &= 0xFFFF;
        uint32_t coarse = 1 << (data >> 12); // 16 steps exponent function
        dgl_scope_smppx = ((coarse * (data & 0xFFF)) >> 12) + coarse; // linear interpolate
        dgl_scope_smpxval = 0xFFFFFFFF / dgl_scope_smppx;
    } break;
    case DGL_SCOPE_ADJ_SELECT_START:
        dgl_scope_selection_start = data;
        break;
    case DGL_SCOPE_ADJ_SELECT_END:
        dgl_scope_selection_end = data;
        break;
    case DGL_SCOPE_ADJ_SELECT_LENGTH:
        dgl_scope_selection_end = dgl_scope_selection_start + data;
        break;
    case DGL_SCOPE_ADJ_START_OFFSET:
        if (data > dgl_scope_length)
            data = dgl_scope_length - 1;
        dgl_scope_start_offset = data;
        break;
    case DGL_SCOPE_ADJ_OFFSET_SHIFT_INT16T: {
        int32_t newoffset = dgl_scope_start_offset;
        newoffset = newoffset + (dgl_scope_smppx * ((int16_t)data) / 65536);
        if (newoffset < 0)
            newoffset = 0;
        else if (((uint32_t)newoffset) > dgl_scope_length)
            newoffset = dgl_scope_length - 1;
        dgl_scope_start_offset = newoffset;
    } break;
    }
    if ((dgl_scope_selection_end) && (dgl_scope_selection_end > dgl_scope_selection_start)) {
        // calculate
        int32_t endpix = (dgl_scope_selection_end - dgl_scope_start_offset) / dgl_scope_smppx;
        int32_t startpix = (dgl_scope_selection_start - dgl_scope_start_offset) / dgl_scope_smppx;
        if (startpix == endpix) {
            uint16_t alpha = 0x10000 * (dgl_scope_selection_end - dgl_scope_selection_start) / dgl_scope_smppx;
            dgl_scope_sel_endalpha = alpha;
            dgl_scope_sel_startalpha = alpha;
        } else {
            dgl_scope_sel_endalpha = 0x10000 * ((dgl_scope_selection_end - dgl_scope_start_offset) % dgl_scope_smppx) / dgl_scope_smppx;
            dgl_scope_sel_startalpha = 0x10000 * ((dgl_scope_smppx - (dgl_scope_selection_start - dgl_scope_start_offset)) % dgl_scope_smppx) / dgl_scope_smppx;
        }
        if (endpix > 0x7FFF)
            endpix = 0x7FFF;
        else if (endpix < -0x7FFF)
            endpix = -0x7FFF;
        if (startpix > 0x7FFF)
            startpix = 0x7FFF;
        else if (startpix < -0x7FFF)
            startpix = -0x7FFF;
        dgl_scope_sel_endpix = endpix;
        dgl_scope_sel_startpix = startpix;
    } else {
        dgl_scope_sel_startpix = 0;
        dgl_scope_sel_startalpha = 0;
        dgl_scope_sel_endpix = 0;
        dgl_scope_sel_endalpha = 0;
    }
}

// way size must be 10 for 8 pixels height !
void dgl_scope_init(uint8_t signal_type, void* signal_data, uint32_t length)
{
    dgl_scope_signal_data = signal_data;
    dgl_scope_type = signal_type;
    dgl_scope_length = length;
    if (way_size > 256)
        way_size = 256;
    dgl_scope_vscalef = way_size - 2;

    // horizontal scale
    uint32_t v = length / wax_size; // compute the next highest power of 2 of 32-bit v
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;

    dgl_scope_smppx = v > 0xFFFF ? 0xFFFF : v;
    dgl_scope_smpxval = 0xFFFFFFFF / dgl_scope_smppx;

    dgl_scope_selection_start = 0;
    dgl_scope_selection_end = 0;
    dgl_scope_start_offset = 0;

    dgl_scope_sel_startpix = 0;
    dgl_scope_sel_startalpha = 0;
    dgl_scope_sel_endpix = 0;
    dgl_scope_sel_endalpha = 0;
}

static inline void dclr(uint8_t size)
{
    uint32_t* mp = (uint32_t*)&scopebuffer[0];
    uint8_t i;
    size = (size >> 1) + 1;
    for (i = 0; i < size; i++)
        mp[i] = 0;
}

#define DGL_SELECTION_BF 0x8000
void dgl_drawscope()
{
    uint16_t i; // iterations
    uint32_t endpx = (dgl_scope_length - dgl_scope_start_offset) / dgl_scope_smppx + 1;
    uint16_t hcycles = wax_size; // = wax_size>endpx?endpx:wax_size;

    // clear area after end of data
    if (wax_size > endpx) {
        displaySetZone(wax + endpx, way, wax_size - endpx, way_size);
        for (i = 0; i < (wax_size - endpx) * way_size; i++)
            displayOutPixel(palette[DGL_PALETTE_BLANK]);
        hcycles = endpx;
    }

    for (i = 0; i < hcycles; i++) {
        uint16_t j;
        uint32_t wposc = i * dgl_scope_smppx + dgl_scope_start_offset;
        uint32_t ccycles = (wposc + dgl_scope_smppx) > dgl_scope_length ? dgl_scope_length - wposc : dgl_scope_smppx;
        uint8_t cshift = (dgl_scope_vscalef + 1) / 2; //+1;
        // memset(scopebuffer,0,way_size*2);
        dclr(way_size);
        switch (dgl_scope_type) {
        case DGL_SCOPETYPE_8U_BYTE: {
            uint8_t* vd = (uint8_t*)dgl_scope_signal_data; // !
            for (j = 0; j < dgl_scope_smppx; j++) {
                int32_t dataval = (vd[wposc + j] - 128) * dgl_scope_vscalef / 256 + cshift;
                if (dataval < 0)
                    dataval = 0;
                if (dataval > way_size)
                    dataval = way_size;
                scopebuffer[dataval]++;
            }
        } break;
        case DGL_SCOPETYPE_8S_BYTE: {
            int8_t* vd = (int8_t*)dgl_scope_signal_data; // !
            for (j = 0; j < dgl_scope_smppx; j++) {
                int32_t dataval = vd[wposc + j] * dgl_scope_vscalef / 256 + cshift;
                if (dataval < 0)
                    dataval = 0;
                if (dataval > way_size)
                    dataval = way_size;
                scopebuffer[dataval]++;
            }
        } break;
        case DGL_SCOPETYPE_16U_WORD: {
            uint16_t* vd = (uint16_t*)dgl_scope_signal_data; // !
            for (j = 0; j < dgl_scope_smppx; j++) {
                int32_t dataval = (vd[wposc + j] - 32768) * dgl_scope_vscalef / 65536 + cshift;
                if (dataval < 0)
                    dataval = 0;
                if (dataval > way_size)
                    dataval = way_size;
                scopebuffer[dataval]++;
            }
        } break;
        case DGL_SCOPETYPE_16S_WORD: {
            int16_t* vd = (int16_t*)dgl_scope_signal_data; // !
            for (j = 0; j < ccycles; j++) {

                int32_t dataval = vd[wposc + j] * dgl_scope_vscalef / 65536 + cshift; // vd[dgl_smpposrand(wposc + j)]*dgl_scope_vscalef/65536+cshift;
                if (dataval < 0)
                    dataval = 0;
                if (dataval > way_size)
                    dataval = way_size;
                scopebuffer[dataval]++;
            }
        } break;
        case DGL_SCOPETYPE_32U_WORD: {
            uint32_t* vd = (uint32_t*)dgl_scope_signal_data; // !
            for (j = 0; j < dgl_scope_smppx; j++) {
                int64_t dataval = (vd[wposc + j] - 0x80000000) * dgl_scope_vscalef / 0x80000000 + cshift;
                if (dataval < 0)
                    dataval = 0;
                if (dataval > way_size)
                    dataval = way_size;
                scopebuffer[dataval]++;
            }
        } break;
        case DGL_SCOPETYPE_32S_WORD: {
            int32_t* vd = (int32_t*)dgl_scope_signal_data; // !
            for (j = 0; j < dgl_scope_smppx; j++) {
                int64_t dataval = vd[wposc + j] * dgl_scope_vscalef / 0x80000000 + cshift;
                if (dataval < 0)
                    dataval = 0;
                if (dataval > way_size)
                    dataval = way_size;
                scopebuffer[dataval]++;
            }
        } break;
        }

        displaySetZone(wax + i, way, 1, way_size);
        // find background
        MglColor c;
        uint16_t bf = 0;
        // MglColor bg;
        if (dgl_scope_sel_startpix < i)
            bf = DGL_SELECTION_BF;
        else if (dgl_scope_sel_startpix == i)
            bf = DGL_SELECTION_BF * dgl_scope_sel_startalpha / 65536;
        if (dgl_scope_sel_endpix < i)
            bf = 0;
        else if (dgl_scope_sel_endpix == i)
            bf = DGL_SELECTION_BF * dgl_scope_sel_startalpha / 65536;
        // overload
        {
            uint32_t grv = scopebuffer[way_size - 1] * dgl_scope_smpxval / 65536;
            grv = dgl_log2fast(grv);
            c = mglExAlphablend(grv << 8, palette[DGL_PALETTE_BLANK], palette[DGL_PALETTE_SCOPE_OUTSIDE]);
            c = mglExShine(bf, c, palette[DGL_PALETTE_SELECTION]);
            displayOutPixel(c);
        }
        for (j = 2; j < way_size; j++) {
            uint32_t grv = scopebuffer[way_size - j] * dgl_scope_smpxval / 65536;
            grv = dgl_log2fast(grv);
            c = mglExAlphablend(grv << 8, palette[DGL_PALETTE_BLANK], palette[DGL_PALETTE_SCOPE]);
            c = mglExShine(bf, c, palette[DGL_PALETTE_SELECTION]);
            displayOutPixel(c);
        }
        // overload
        {
            uint32_t grv = scopebuffer[0] * dgl_scope_smpxval / 65536;
            grv = dgl_log2fast(grv);
            c = mglExAlphablend(grv << 8, palette[DGL_PALETTE_BLANK], palette[DGL_PALETTE_SCOPE_OUTSIDE]);
            c = mglExShine(bf, c, palette[DGL_PALETTE_SELECTION]);
            displayOutPixel(c);
        }
    }
}





//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
// xy is just for fun. it uses exessive amount of memory and cycles
// so please, comment me
// #define DGL_XY_SCOPE

#ifdef DGL_XY_SCOPE

#include "dsp.h"
extern MglColor palette[DGL_PALETTE_TOTAL];
extern uint32_t cr_counter; // TODO globalize??

void displaySetZone(uint16_t xpos, uint16_t ypos, uint16_t xsize, uint16_t ysize);
void displayOutPixel(MglColor color);
uint16_t persist = 0xC000;
uint16_t glowth = 0x1000;
#define DGL_XY_XSIZE 96 // 96
#define DGL_XY_YSIZE 64 // 64
#define DGL_XY_FRAMERATE_SHIFT 7
#define DGL_XY_PERSISTANCE persist // 0xC000 //0xEFFF
#define DGL_XY_MAX_INTENSITY_CAPTURED ((AUDIOBLOCK_SIZE << DGL_XY_FRAMERATE_SHIFT) - 1)
// shift 7 = 3000 CR / 128 = 23.4 fps
// shift 8 256 =11.7Hz
// shift 9 512 =5.8Hz
// max sppx = 48000/3000 AUDIOFRAME * 128 = 2048
// persistance - 0x7FFF = 1/2 of signal
#include "daisy_hw.h"
volatile uint16_t MEMORY_AHB_4 dgl_xy_buffer[4][DGL_XY_XSIZE][DGL_XY_YSIZE];

uint8_t xy_fb_pos;
// MglColor mglExAlphablend(uint16_t intensity, MglColor lowest, MglColor highest);
// #include <string.h>

volatile uint32_t t2time;
volatile uint32_t t2start; // = mt_get();//TODO: turn off
void dgl_xy_refresh()
{

    uint32_t crpos = cr_counter >> DGL_XY_FRAMERATE_SHIFT;
    uint8_t c_cap_bp = crpos & 0x3;
    // uint8_t c_clear_bp = (crpos+1)&0x3;
    uint8_t c_adiv_bp = (crpos + 2) & 0x3;
    uint8_t c_aresult_bp = (crpos + 3) & 0x3;
    t2start = CORE_CYCLECOUNTER;
    if (xy_fb_pos != c_cap_bp) {
        xy_fb_pos = c_cap_bp;

        // clear
        // memset(&dgl_xy_buffer[c_clear_bp][0][0],0,DGL_XY_XSIZE*DGL_XY_YSIZE*2);
        // alphablend and simultaneous output
        uint16_t x, y;
        uint16_t lcd_xpos = 0;
        uint16_t lcd_ypos = 0;
        for (x = 0; x < DGL_XY_XSIZE; x++) {
            displaySetZone(lcd_xpos + x, lcd_ypos, 1, DGL_XY_YSIZE);
            for (y = 0; y < DGL_XY_YSIZE; y++) {
                MglColor c;
                uint32_t respx;
                uint16_t oldv = dgl_xy_buffer[c_adiv_bp][x][y];
                uint16_t newv = dgl_xy_buffer[c_aresult_bp][x][y];

                //
                // dgl_xy_buffer[c_clear_bp][x][y]=0;
                respx = newv * glowth + ((uint32_t)oldv * DGL_XY_PERSISTANCE / 65536);
                // respx = ((int32_t)newv - oldv)*DGL_XY_PERSISTANCE/65536 + oldv;
                // this is not crossfade.. more like saturated integration
                // respx = respx>DGL_XY_MAX_INTENSITY_CAPTURED?DGL_XY_MAX_INTENSITY_CAPTURED:respx;
                respx = respx > 0xFFFF ? 0xFFFF : respx;
                dgl_xy_buffer[c_adiv_bp][x][y] = 0;
                dgl_xy_buffer[c_aresult_bp][x][y] = respx; //>0xFFFF?0xFFFF:respx;

                // respx = respx?((respx*xy_intensity_mul)>>16)+0x2666:0;
                // c = mglExAlphablend(respx<<1,palette[DGL_PALETTE_BLANK],palette[DGL_PALETTE_SCOPE]);

                respx = dgl_log2fast(respx);
                // respx *=2;
                // respx = respx>0xFF?0xFF:respx;
                // c.green = respx;
                c = mglExColorFromHsv(respx, 255, respx);
                displayOutPixel(c);
                // lcd_sendpixel(respx);
            }
        }
    }
    // 1 - buffer capture from sr
    // 2 - alpha blend with previous
    // 3 - output to lcd
    t2time = CORE_CYCLECOUNTER - t2start;
}
void dgl_xy_cr(uint32_t cr, uint16_t* x, uint16_t* y)
{
    uint8_t bp = (cr >> DGL_XY_FRAMERATE_SHIFT) & 0x3;
    REPEAT(
        uint32_t xp = (*x) * DGL_XY_XSIZE / 65536;
        uint32_t yp = (*y) * DGL_XY_YSIZE / 65536;
        dgl_xy_buffer[bp][xp][yp]++;
        x++;
        y++;)
}
void dgl_xy_cr_scopemusic(uint32_t cr, audiosample32_t* in)
{
    uint8_t bp = (cr >> DGL_XY_FRAMERATE_SHIFT) & 0x3;
    REPEAT(
        int32_t xp = (((in->left >> 8) + 0x8000) & 0xFFFF) * DGL_XY_XSIZE / 65536;
        int32_t yp = (((in->right >> 8) + 0x8000) & 0xFFFF) * DGL_XY_YSIZE / 65536;
        /*
        if (xp<0)
                xp=0;
        else
                if (xp>DGL_XY_XSIZE-1)
                        xp=DGL_XY_XSIZE-1;
        if (yp<0)
                yp=0;
        else
                if (yp>DGL_XY_XSIZE-1)
                        yp=DGL_XY_YSIZE-1;
                        */
        dgl_xy_buffer[bp][xp][yp]++;
        in++;)
}
void dgl_xy_cr_stereo(uint32_t cr, audiosample16_t* in)
{
    uint8_t bp = (cr >> DGL_XY_FRAMERATE_SHIFT) & 0x3;
    REPEAT(
        // transform coordinates 45 deg
        int32_t x = (in->right - in->left) * DGL_XY_XSIZE / 131072 + DGL_XY_XSIZE / 2;
        int32_t y = (in->right + in->left) * DGL_XY_YSIZE / 131072 + DGL_XY_YSIZE / 2;
        dgl_xy_buffer[bp][x][y]++;
        in++;)
}
void dgl_xy_cr_45(uint32_t cr, int16_t* l, int16_t* r)
{
    uint8_t bp = (cr >> DGL_XY_FRAMERATE_SHIFT) & 0x3;
    REPEAT(
        // transform coordinates 45 deg
        int32_t x = (*r - *l) * DGL_XY_XSIZE / 131072 + DGL_XY_XSIZE / 2;
        int32_t y = (*r + *l) * DGL_XY_YSIZE / 131072 + DGL_XY_YSIZE / 2;
        dgl_xy_buffer[bp][x][y]++;
        r++;
        l++;)
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

// Y_T scope??

#else

void dgl_xy_refresh()
{
    mglSetWorkingArea(0, 0, MGL_DISPLAY_WIDTH, MGL_DISPLAY_HEIGHT);
    dgl_drawstringpartial("xy scope disabled");
}
void dgl_xy_cr(uint32_t cr, uint16_t* x, uint16_t* y)
{
    (void)cr;
    (void)x;
    (void)y;
}

void dgl_xy_cr_stereo(uint32_t cr, audiosample16_t* in)
{
    (void)cr;
    (void)in;
}
void dgl_xy_cr_45(uint32_t cr, int16_t* l, int16_t* r)
{
    (void)cr;
    (void)l;
    (void)r;
}
#endif
