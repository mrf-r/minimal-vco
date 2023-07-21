#include "bsp.h"
#include "audio.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_flash_ex.h"

volatile uint32_t counter_sr;
volatile uint32_t counter_cr;

void Error_Handler(void);

bool bspButtonGet() {
  return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11) == GPIO_PIN_SET ? true : false;
}

void bspLedSet(bool state) {
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, state ? GPIO_PIN_SET : GPIO_PIN_SET);
}

// NVM

// #define SAVEPAGE (FLASH_PAGE_NB - 1)
#define FLASH_PAGE_SIZE (128 * 1024)
#define SAVEADDRESS 0x08060000
#define SAVEBLOCKS_TOTAL (FLASH_PAGE_SIZE / sizeof(SaveBlock))
SaveBlock* const nvm = (SaveBlock*)SAVEADDRESS;

#define NVM_CODE_VALID 0xACE5BEA1
#define NVM_CODE_FREE 0xFFFFFFFF
// last bank

// load to get
int bspNvmLoad(SaveBlock* sb) {
  int ret = -1;
  int lastblock = -1;
  for (int i = 0; i < (int)SAVEBLOCKS_TOTAL; i++) {
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
    for (int i = 0; i < (int)sizeof(SaveBlock); i++) {
      dst[i] = src[i];
    }
    ret = 0;
  }
  return ret;
}

int bspNvmSave(SaveBlock* sb) {
  int ret = 0;
  // find last block
  int lastblock = -1;
  for (int i = 0; i < (int)SAVEBLOCKS_TOTAL; i++) {
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
  if ((lastblock + 1) < (int)SAVEBLOCKS_TOTAL) {
    // just write
    if (HAL_FLASH_Unlock() == HAL_OK) {
      sb->code = NVM_CODE_VALID;
      uint64_t* dst = (uint64_t*)&nvm[lastblock + 1];
      uint64_t* src = (uint64_t*)sb;
      for (int i = 0; i < (int)sizeof(SaveBlock) / 8; i++) {
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
      erase.TypeErase = FLASH_TYPEERASE_SECTORS;
      erase.Banks = FLASH_BANK_1;
      erase.Sector = FLASH_SECTOR_7;
      erase.NbSectors = 1;
      uint32_t page_error;
      if (HAL_FLASHEx_Erase(&erase, &page_error) == HAL_OK) {
        sb->code = NVM_CODE_VALID;
        uint64_t* dst = (uint64_t*)&nvm[0];
        uint64_t* src = (uint64_t*)sb;

        for (int i = 0; i < (int)sizeof(SaveBlock) / 8; i++) {
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
      (void)page_error;
    } else
      ret = -1;
  }

  // readback
  uint8_t* dst = (uint8_t*)&nvm[lastblock + 1];
  uint8_t* src = (uint8_t*)sb;
  for (int i = 0; i < (int)sizeof(SaveBlock); i++) {
    if (src[i] != dst[i]) {
      ret = -5;
    }
  }
  return ret;
}

// SIGNALS STUFF

Vco vco;
extern volatile uint32_t counter_sr;
extern TIM_HandleTypeDef htim3;
extern ADC_HandleTypeDef hadc1;
SaveBlock sb;

uint32_t bspTimerGet() { return __HAL_TIM_GET_COUNTER(&htim3); }

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef* htim) {
  if (htim == &htim3) {
    __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, vco.out[0]);
    __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, vco.out[1]);
    __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_3, vco.out[2]);
  }
  HAL_ADC_Stop(&hadc1);
  // HAL_ADC_Start_DMA(&hadc1, (uint32_t*)vco.ctrl, 6);
  counter_sr++;
  vcoTap(&vco);
}

// MAIN

void init() {
  HAL_ADCEx_Calibration_Start(&hadc1);
  // HAL_ADC_Stop(&hadc1);
  // HAL_ADC_Start_DMA(&hadc1, (uint32_t*)vco.ctrl, 6);

  if (HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1) != HAL_OK) {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2) != HAL_OK) {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3) != HAL_OK) {
    Error_Handler();
  }

  vcoInit(&vco);
  if (bspNvmLoad(&sb) == 0) {
    vcoCalibrationLoad(&vco, sb.scale, sb.offset);
  } else {
    // load defaults
    // vcoCalibrationLoad(&vco, 0, 0);
  }

  HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM3_IRQn);

  // test run
  vcoTap(&vco);
}

void vcoMain(Vco* vco);

void loop() {
  vcoMain(&vco);
}
