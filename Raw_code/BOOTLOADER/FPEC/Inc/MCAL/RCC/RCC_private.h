/*
 * RCC_private.h
 *
 *  Created on: Feb 14, 2022
 *      Author: noureldeen albrmky
 */

#ifndef MCAL_RCC_RCC_PRIVATE_H_
#define MCAL_RCC_RCC_PRIVATE_H_

typedef struct {
	volatile u32 RCC_CR;
	volatile u32 RCC_CFGR;
	volatile u32 RCC_CIR;
	volatile u32 RCC_APB2RSTR;
	volatile u32 RCC_APB1RSTR;
	volatile u32 RCC_AHBENR;
	volatile u32 RCC_APB2ENR;
	volatile u32 RCC_APB1ENR;
	volatile u32 RCC_BDCR;
	volatile u32 RCC_CSR;

} RCC_t;

#define RCC	((volatile RCC_t*)0x40021000)

#define PLL_RDY	25
#define HSE_RDY	17
#define HSI_RDY	1

#define PLL_ON	24
#define HSE_ON	16
#define HSI_ON	0

// number of Ready bits
#define RCC_SOURCE_HSI	1
#define RCC_SOURCE_HSE	17
#define RCC_SOURCE_PLL	25

#define RCC_APB_PRESCALER_NO	3
#define RCC_APB_PRESCALER_2		4
#define RCC_APB_PRESCALER_4		5
#define RCC_APB_PRESCALER_8		6
#define RCC_APB_PRESCALER_16	7

#define RCC_AHB_PRESCALER_NO	7
#define RCC_AHB_PRESCALER_2		8
#define RCC_AHB_PRESCALER_4		9
#define RCC_AHB_PRESCALER_8		10
#define RCC_AHB_PRESCALER_16	11
#define RCC_AHB_PRESCALER_64	12
#define RCC_AHB_PRESCALER_128	13
#define RCC_AHB_PRESCALER_256	14
#define RCC_AHB_PRESCALER_512	15

#define RCC_PLL_INPUT_SOURCE_HSI_2	0
#define RCC_PLL_INPUT_SOURCE_HSE_2	1
#define RCC_PLL_INPUT_SOURCE_HSE	2

#endif /* MCAL_RCC_RCC_PRIVATE_H_ */
