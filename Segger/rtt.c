#include <stdint.h>
#include <stdbool.h>
#include "SEGGER_RTT.h"
#include "stdarg.h"
#include "stdio.h"

void delayUs(uint32_t us);

#define RTT_TIMEOUT_MS 200
#define PRINTBUFSIZE 256
static char printbuf[PRINTBUFSIZE];
static const char hex[17] = "0123456789ABCDEF ";
static bool rtt_active = true;

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#define ASSERT(...)
#define CORE_FREQ 96000000

// Dummy cycle blocking
__attribute__((weak)) void delayUs(uint32_t time_us) {
  ASSERT(time_us);
  volatile uint32_t c = time_us * ((uint32_t)CORE_FREQ / 1000 / 6);
  __asm volatile(
      "1:                \n"
      "  SUBS %0, %0, #1 \n"
      "  BNE  1b         \n" ::"r"(c));
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void rttSend(const char *c, uint16_t length) {
  uint16_t remain = length;
  while ((rtt_active) && remain) {
    uint32_t timeout = RTT_TIMEOUT_MS * 100;
    unsigned free = 0;
    do {
      delayUs(10);
      timeout--;
      if (timeout == 0) {
        rtt_active = false;
        return;
      }
      free = SEGGER_RTT_GetAvailWriteSpace(0);
    } while (!free);
    remain -= SEGGER_RTT_WriteNoLock(0, &c[length - remain],
                                     remain <= free ? remain : free);
  }
}

uint8_t rttReceive(char *str, uint8_t buffer_size) {
  unsigned ret = SEGGER_RTT_ReadNoLock(0, str, buffer_size);
  if (ret) rtt_active = true;
  return ret;
}

void rttPrint(const char *str, ...) {
  int32_t out_count;
  va_list ap;
  va_start(ap, str);
  out_count = vsnprintf(printbuf, sizeof(printbuf), str, ap);
  rttSend(printbuf, out_count);
  va_end(ap);
}

void debugPrintStr(const char *str) {
  uint16_t cc = 0;
  while (str[cc++]) {
  }
  rttSend(str, cc);
}

void debugSend(const char *str, uint16_t len) { rttSend(str, len); }

void debugPrintInt(const int value) {
  char out[10];
  out[0] = '0';
  out[1] = 'x';
  bool started = false;
  for (int i = 0; i < 8; i++) {
    int digit = (value >> (28 - i * 4)) & 0xF;
    if ((started) || (digit)) {
      started = true;
    } else {
      digit = 16;
    }
    out[2 + i] = hex[digit];
  }
  rttSend(out, sizeof(out));
}

// print in 10-base number (%d)
void debugPrintDecimal(const int value) {
  char out[11] = "xxxxxxxxxxx";
  uint32_t h;
  bool neg;
  if (value == 0) {
    rttSend("0", 1);
  }
  if (value < 0) {
    neg = true;
    h = -value;
  } else {
    neg = false;
    h = value;
  }
  for (int pos = 10; pos >= 0; pos--) {
    if (h == 0) {
      if (neg) {
        out[pos] = '-';
        rttSend(&out[pos], 11 - pos);
      } else {
        rttSend(&out[pos + 1], 10 - pos);
      }
      return;
    } else {
      uint32_t d = h / 10;
      out[pos] = hex[h - d * 10];
      h = d;
    }
  }
  rttSend("error", 5);
  ASSERT(false);
}
