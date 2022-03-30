/*
 * RCC_configuration.h
 *
 *  Created on: Feb 14, 2022
 *      Author: noureldeen albrmky
 */

#ifndef MCAL_RCC_RCC_CONFIGURATION_H_
#define MCAL_RCC_RCC_CONFIGURATION_H_

/***************************************
 choose the source of the system clock
 options:
 RCC_SOURCE_HSI
 RCC_SOURCE_HSE
 RCC_SOURCE_PLL

 **************************************/
#define RCC_SYSTEM_CLOCK_SOURCE	RCC_SOURCE_HSE

#define RCC_SYSTEM_CLOCK	8000000

/************************************************
 choose the input source for PLL
 options:
 RCC_PLL_INPUT_SOURCE_HSI_2
 RCC_PLL_INPUT_SOURCE_HSE_2
 RCC_PLL_INPUT_SOURCE_HSE
 *************************************************/
#define RCC_PLL_INPUT	RCC_PLL_INPUT_SOURCE_HSE

/************************************************
 multiplication factor for the PLL
 options:
 from 2:16
 **************************************************/
#define RCC_PLL_MUL_FACTOR		3

/**********************************************
 define the prescaler for the AHB bus
 options:
 RCC_AHB_PRESCALER_NO
 RCC_AHB_PRESCALER_2
 RCC_AHB_PRESCALER_4
 RCC_AHB_PRESCALER_8
 RCC_AHB_PRESCALER_16
 RCC_AHB_PRESCALER_64
 RCC_AHB_PRESCALER_128
 RCC_AHB_PRESCALER_256
 RCC_AHB_PRESCALER_512
 ************************************************/
#define RCC_AHB_BUS_PRESCALER	RCC_AHB_PRESCALER_NO

/*******************************************
 define the prescaler for APB1 & APB2 BUSES
 options:
 RCC_APB_PRESCALER_NO
 RCC_APB_PRESCALER_2
 RCC_APB_PRESCALER_4
 RCC_APB_PRESCALER_8
 RCC_APB_PRESCALER_16
 ********************************************/
#define RCC_APB1_BUS_PRESCALER	RCC_APB_PRESCALER_2
#define RCC_APB2_BUS_PRESCALER	RCC_APB_PRESCALER_2

#endif /* MCAL_RCC_RCC_CONFIGURATION_H_ */
