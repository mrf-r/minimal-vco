
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "lcdn.h"

#include "monochrome_graphic_lib.h"

typedef struct
{
    volatile uint8_t SM_BOTH_COM; //0x60000000   0x0
    volatile uint8_t x0[0xFFFF]; //
    volatile uint8_t SM_2_COM; //0x60010000   0x10000
    volatile uint8_t x1[0xFFFF]; //
    volatile uint8_t SM_1_COM; //0x60020000   0x10000
    volatile uint8_t x2[0x1FFFF]; //
    volatile uint8_t SM_BOTH_DAT; //0x60040000   0x20000
    volatile uint8_t x3[0xFFFF]; //
    volatile uint8_t SM_2_DAT; //0x60050000   0x10000
    volatile uint8_t x4[0xFFFF]; //
    volatile uint8_t SM_1_DAT; //0x60060000   0x10000
} LCD_TypeDef;

#define EXT_SRAM_BASE ((uint32_t)0x60000000) /*!< FSMC memory base address*/
#define LCD ((LCD_TypeDef*)EXT_SRAM_BASE)

#define LED_LIMIT 0xFF
#define V0_LIMIT 0x40

void lcd_init(void)
{
    RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIODEN | RCC_AHB1ENR_GPIOEEN);
    RCC->AHB3ENR |= RCC_AHB3ENR_FSMCEN;
    RCC->APB2ENR |= RCC_APB2ENR_TIM9EN; //2kHz
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; //LED
    //connect fsmc ports
    //D - 0 1 4 5 11 12 13 14 15
    //E - 7 8 9 10
    //connect tim9 ports
    //E - 5
    //connect tim3 ports
    //B - 0 1
    //C - 6 7
    //GPIO out
    //PE6
    GPIOB->AFR[0] |= 0x00000022;
    GPIOC->AFR[0] |= 0x22000000;
    GPIOD->AFR[0] |= 0x00CC00CC;
    GPIOD->AFR[1] |= 0xCCCCC000;
    GPIOE->AFR[0] |= 0xC0300000;
    GPIOE->AFR[1] |= 0x00000CCC;
    GPIOB->MODER |= (GPIO_MODER_MODER0_1 | GPIO_MODER_MODER1_1);
    GPIOC->MODER |= (GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1);
    GPIOD->MODER |= (GPIO_MODER_MODER0_1 | GPIO_MODER_MODER1_1 | GPIO_MODER_MODER4_1 | GPIO_MODER_MODER5_1 | GPIO_MODER_MODER11_1 | GPIO_MODER_MODER12_1 | GPIO_MODER_MODER13_1 | GPIO_MODER_MODER14_1 | GPIO_MODER_MODER15_1);
    GPIOE->MODER |= (GPIO_MODER_MODER5_1 | GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_1 | GPIO_MODER_MODER8_1 | GPIO_MODER_MODER9_1 | GPIO_MODER_MODER10_1);

    FSMC_Bank1->BTCR[0] = 0x00001001;
    //FSMC_Bank1->BTCR[1]  = 0x0FFF45FF;//timings need to be fixed. but it works, and i am too lazy to recalculate

    // enable 2kHz clock for sbn1661g controller
    TIM9->CR1 |= (TIM_CR1_ARPE | TIM_CR1_CKD_1);
    TIM9->CCMR1 |= (TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2);
    TIM9->CCER |= TIM_CCER_CC1E;
    TIM9->PSC = 0x1;
    TIM9->ARR = 0xA100;
    TIM9->CCR1 = 0x5080;
    TIM9->CR1 |= TIM_CR1_CEN;
    // reset
    GPIOE->BSRR = 1 << 6;
    HAL_Delay(10);
    GPIOE->BSRR = 1 << (6 + 16);

    // lcd backlight pwm
    TIM3->CR1 |= (TIM_CR1_ARPE | TIM_CR1_CKD_1); //enable display backlight PWM
    TIM3->CCMR1 |= (TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2);
    TIM3->CCMR2 |= (TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_2);
    TIM3->CCER |= (TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E | TIM_CCER_CC4E);
    TIM3->ARR = 0xFF; //0xA7;
    TIM3->CR1 |= TIM_CR1_CEN;
    TIM3->CCR1 = 0x20; //maximum - half (0x80)
    TIM3->CCR2 = 0x60; //maximum - full (0xFF)
    TIM3->CCR3 = 0x38;
    TIM3->CCR4 = 0x40;
    HAL_Delay(100);
    //***********************************************************************************************************************************************************
    //small display initialization
    LCD->SM_BOTH_COM = 0xE2; //soft reset
    LCD->SM_BOTH_COM = 0xA4; //static drive off
    LCD->SM_BOTH_COM = 0xA9; //duty cycle 1/32		A8 - 1/16
    LCD->SM_BOTH_COM = 0xA0; //CW output
    LCD->SM_BOTH_COM = 0xEE; //RMF OFF						(E0 - start RMF)
    LCD->SM_BOTH_COM = 0xC0; //startline 0				C0 to DF - 0 to 32 lines shift image
    LCD->SM_BOTH_COM = 0xAF; //dispon						AE - off
    // LCD->SM_BOTH_COM = 0xB8; //page 0						B8 to BB - page 0 to 3
    // LCD->SM_BOTH_COM = 0x00; //col ad 0					00 to 3D - col 0 to 61
}

void lcd_update()
{
    for (int line = 0; line < 4; line++) {
        // set start write address for both controllers
        LCD->SM_BOTH_COM = 0xB8 + line;
        LCD->SM_BOTH_COM = 0x00;
        // update each one separately
        for (int i = 0; i < 61; i++) {
            LCD->SM_1_DAT = mgl_framebuffer[line * 122 + i];
        }
        for (int i = 0; i < 61; i++) {
            LCD->SM_2_DAT = mgl_framebuffer[line * 122 + 61 + i];
        }
    }
}

void lcd_set_color(MglColor color)
{
    if (color.red > LED_LIMIT) {
        color.red = LED_LIMIT;
    }
    if (color.green > LED_LIMIT) {
        color.green = LED_LIMIT;
    }
    if (color.blue > LED_LIMIT) {
        color.blue = LED_LIMIT;
    }
    if (color.contrast > V0_LIMIT) {
        color.contrast = V0_LIMIT;
    }
    TIM3->CCR2 = color.red;
    TIM3->CCR3 = color.green;
    TIM3->CCR4 = color.blue;
    TIM3->CCR1 = color.contrast;
}
MglColor lcd_get_color()
{
    MglColor color;
    color.contrast = TIM3->CCR1;
    color.red = TIM3->CCR2;
    color.green = TIM3->CCR3;
    color.blue = TIM3->CCR4;
    return color;
}
