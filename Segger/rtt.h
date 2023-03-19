#ifndef _RTT_H
#define _RTT_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void rttSend(const char *c, uint16_t length);
uint8_t rttReceive(char *str, uint8_t buffer_size);
void rttPrint(const char *str, ...);
void debugSend(const char *str, uint16_t len);
void debugPrintStr(const char *str);
void debugPrintInt(const int value);
void debugPrintDecimal(const int value);

#ifdef __cplusplus
}
#endif

#endif // _RTT_H