#ifndef _BSP_STM32G0_H
#define _BSP_STM32G0_H

#include <stdint.h>
#include <stdbool.h>
#include "vco.h"

#ifdef __cplusplus
extern "C" {
#endif


bool bspButtonGet(void);
void bspLedSet(bool state);
int bspNvmLoad(SaveBlock* sb);
int bspNvmSave(SaveBlock* sb);
uint32_t bspTimerGet(void);
void init(void);
void loop(void);

#ifdef __cplusplus
}
#endif

#endif // _BSP_STM32G0_H
