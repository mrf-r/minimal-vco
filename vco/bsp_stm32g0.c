#include <stdbool.h>

#include "stm32g0xx_hal.h"

bool bspButtonGet() {
  return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11) == GPIO_PIN_SET ? true : false;
}

void bspLedSet(bool state) {
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, state ? GPIO_PIN_SET : GPIO_PIN_SET);
}

extern ADC_HandleTypeDef hadc1;

void bspAdcTap(void* data) {
  HAL_ADC_Stop(&hadc1);
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)data, 6);
}

// NVM

// #define SAVEPAGE (FLASH_PAGE_NB - 1)
#define SAVEPAGE (15)
#define SAVEADDRESS (FLASH_BASE + FLASH_PAGE_SIZE * SAVEPAGE)

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

#define SAVEBLOCKS_TOTAL (FLASH_PAGE_SIZE / sizeof(SaveBlock))
SaveBlock* const nvm = (SaveBlock*)SAVEADDRESS;

#define NVM_CODE_VALID 0xACE5BEA1
#define NVM_CODE_FREE 0xFFFFFFFF
// last bank

// load to get
int bspNvmLoad(SaveBlock* sb) {
  int ret = -1;
  int lastblock = -1;
  for (int i = 0; i < SAVEBLOCKS_TOTAL; i++) {
    if (nvm[i].code == NVM_CODE_VALID) {
      lastblock = i;
    } else {
      // if .text is here
      if (nvm[i].code != NVM_CODE_FREE) {
        while (1) {
          HAL_Delay(500);
          HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_12);
        }
      }
    }
  }
  if (lastblock >= 0) {
    uint8_t* src = (uint8_t*)&nvm[lastblock];
    uint8_t* dst = (uint8_t*)sb;
    for (int i = 0; i < sizeof(SaveBlock); i++) {
      dst[i] = src[i];
    }
    ret = 0;
  }
  return ret;
}

int nvm_save(SaveBlock* sb) {
  int ret = 0;
  // find last block
  int lastblock = -1;
  for (int i = 0; i < SAVEBLOCKS_TOTAL; i++) {
    if (nvm[i].code == NVM_CODE_VALID) {
      lastblock = i;
    } else {
      // if .text is here
      if (nvm[i].code != NVM_CODE_FREE) {
        while (1) {
          HAL_Delay(500);
          HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_12);
        }
      }
    }
  }
  if ((lastblock + 1) < SAVEBLOCKS_TOTAL) {
    // just write
    if (HAL_FLASH_Unlock() == HAL_OK) {
      sb->code = NVM_CODE_VALID;
      uint64_t* dst = (uint64_t*)&nvm[lastblock + 1];
      uint64_t* src = (uint64_t*)sb;
      for (int i = 0; i < sizeof(SaveBlock) / 8; i++) {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, (uint32_t)&dst[i],
                              src[i]) != HAL_OK) {
          ret = -3;
        }
      }
      if (HAL_FLASH_Lock() != HAL_OK) {
        ret = -4;
      }
    } else
      ret = -1;
  } else {
    // erase and write
    lastblock = -1;
    if (HAL_FLASH_Unlock() == HAL_OK) {
      FLASH_EraseInitTypeDef erase;
      erase.TypeErase = FLASH_TYPEERASE_PAGES;
      erase.Banks = FLASH_BANK_1;
      erase.Page = SAVEPAGE;
      erase.NbPages = 1;
      uint32_t page_error;
      if (HAL_FLASHEx_Erase(&erase, &page_error) == HAL_OK) {
        sb->code = NVM_CODE_VALID;
        uint64_t* dst = (uint64_t*)&nvm[0];
        uint64_t* src = (uint64_t*)sb;

        for (int i = 0; i < sizeof(SaveBlock) / 8; i++) {
          if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, (uint32_t)&dst[i],
                                src[i]) != HAL_OK) {
            ret = -3;
          }
        }
        if (HAL_FLASH_Lock() != HAL_OK) {
          ret = -4;
        }
      } else
        ret = -2;
    } else
      ret = -1;
  }

  // readback
  uint8_t* dst = (uint8_t*)&nvm[lastblock + 1];
  uint8_t* src = (uint8_t*)sb;
  for (int i = 0; i < sizeof(SaveBlock); i++) {
    if (src[i] != dst[i]) {
      ret = -5;
    }
  }
  return ret;
}
