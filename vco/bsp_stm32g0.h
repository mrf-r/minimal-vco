#ifndef _BSP_STM32G0_H
#define _BSP_STM32G0_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// 32 bytes,
typedef struct {
  uint32_t code;
  int32_t scale;
  int32_t offset;
  uint32_t reserved0;
  uint32_t reserved1;
  uint32_t reserved2;
  uint32_t reserved3;
  uint32_t reserved4;
} SaveBlock;

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
