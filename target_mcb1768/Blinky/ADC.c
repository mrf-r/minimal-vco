/*----------------------------------------------------------------------------
 * Name:    ADC.c
 * Purpose: low level ADC functions
 * Note(s): possible defines select the used ADC interface:
 *            __ADC_IRQ   - ADC works in Interrupt mode
 *                        - ADC works in polling mode (default)
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2008-2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "LPC17xx.H"                         /* LPC17xx definitions           */
#include "ADC.h"

uint16_t AD_last;                            /* Last converted value          */
uint8_t  AD_done = 0;                        /* AD conversion done flag       */

/*----------------------------------------------------------------------------
  Function that initializes ADC
 *----------------------------------------------------------------------------*/
void ADC_Init (void) {

  LPC_SC->PCONP |= ((1 << 12) | (1 << 15));  /* enable power to ADC & IOCON   */

  LPC_PINCON->PINSEL1  &= ~( 3 << 18);
  LPC_PINCON->PINSEL1  |=  ( 1 << 18);       /* P0.25 is AD0.2                */
  LPC_PINCON->PINMODE1 &= ~( 3 << 18);
  LPC_PINCON->PINMODE1 |=  ( 2 << 18);       /* P0.25 no pull up/down         */

  LPC_ADC->ADCR        =  ( 1 <<  2) |       /* select AD0.2 pin              */
                          ( 4 <<  8) |       /* ADC clock is 25MHz/5          */
                          ( 1 << 21);        /* enable ADC                    */

#ifdef __ADC_IRQ
  LPC_ADC->ADINTEN     =  ( 1 <<  8);        /* global enable interrupt       */

  NVIC_EnableIRQ(ADC_IRQn);                  /* enable ADC Interrupt          */
#endif
}


/*----------------------------------------------------------------------------
  start AD Conversion
 *----------------------------------------------------------------------------*/
void ADC_StartCnv (void) {
  LPC_ADC->ADCR &= ~( 7 << 24);              /* stop conversion               */
  LPC_ADC->ADCR |=  ( 1 << 24);              /* start conversion              */
}


/*----------------------------------------------------------------------------
  stop AD Conversion
 *----------------------------------------------------------------------------*/
void ADC_StopCnv (void) {

  LPC_ADC->ADCR &= ~( 7 << 24);              /* stop conversion               */
}


/*----------------------------------------------------------------------------
  get converted AD value
 *----------------------------------------------------------------------------*/
uint16_t ADC_GetCnv (void) {

#ifndef __ADC_IRQ
  while (!(LPC_ADC->ADGDR & ( 1UL << 31)));  /* Wait for Conversion end       */
  AD_last = (LPC_ADC->ADGDR >> 4) & ADC_VALUE_MAX; /* Store converted value   */

  AD_done = 1;
#endif

  return(AD_last);
}


/*----------------------------------------------------------------------------
  A/D IRQ: Executed when A/D Conversion is done
 *----------------------------------------------------------------------------*/
#ifdef __ADC_IRQ
void ADC_IRQHandler(void) {
  volatile uint32_t adstat;

  adstat = LPC_ADC->ADSTAT;		             /* Read ADC clears interrupt     */

  AD_last = (LPC_ADC->ADGDR >> 4) & ADC_VALUE_MAX; /* Store converted value   */

  AD_done = 1;
}
#endif
