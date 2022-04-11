/*
 * USART_private.h
 *
 *  Created on: Mar 1, 2022
 *      Author: noureldeen albrmky
 */

#ifndef MCAL_USART_USART_PRIVATE_H_
#define MCAL_USART_USART_PRIVATE_H_

typedef struct {
	volatile u32 USART_SR;
	volatile u32 USART_DR;
	volatile u32 USART_BRR;
	volatile u32 USART_CR1;
	volatile u32 USART_CR2;
	volatile u32 USART_CR3;
	volatile u32 USART_GTPR;
} USART_t;

#define USART1	((volatile USART_t*)(0x40013800))

static void MUSART_voidSetBaudRate(u32 copy_BR);

// options for USART_DATA_WIDTH
#define USART_DATA_WIDTH_8_BIT		0
#define USART_DATA_WIDTH_9_BIT		1

#define USART_STOP_BIT_WIDTH_1		0
#define USART_STOP_BIT_WIDTH_0_5	1
#define USART_STOP_BIT_WIDTH_1_5	2
#define USART_STOP_BIT_WIDTH_2		3

#define USART_PARITY_ODD			1
#define USART_PARITY_EVEN			0

#define USART_STATUS_ENABLE			1
#define USART_STATUS_DISABLE		0

#endif /* MCAL_USART_USART_PRIVATE_H_ */
