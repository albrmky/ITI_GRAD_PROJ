/*
 * SYSTICK_private.h
 *
 *  Created on: Feb 24, 2022
 *      Author: noureldeen albrmky
 */

#ifndef MCAL_SYSTICK_SYSTICK_PRIVATE_H_
#define MCAL_SYSTICK_SYSTICK_PRIVATE_H_

typedef struct {
	volatile u32 STK_CTRL;
	volatile u32 STK_LOAD;
	volatile u32 STK_VAL;
	volatile u32 STK_CALIB;
} SYSTICK_t;

#define SYSTICK	((volatile SYSTICK_t*)0xE000E010)

#define SYSTICK_CLOCK_SOURCE_AHB_8	0
#define SYSTICK_CLOCK_SOURCE_AHB	1

#define SYSTICK_INT_DISABLE		0
#define SYSTICK_INT_ENABLE		1

#endif /* MCAL_SYSTICK_SYSTICK_PRIVATE_H_ */
