/*
 * NVIC_private.h
 *
 *  Created on: Feb 23, 2022
 *      Author: noureldeen albrmky
 */

#ifndef MCAL_NVIC_NVIC_PRIVATE_H_
#define MCAL_NVIC_NVIC_PRIVATE_H_

//#define NVIC_REG_ISER0    ((volatile u32*)0xE000E100)
//#define NVIC_REG_ICER0    ((volatile u32*)0xE000E180)
//#define NVIC_REG_ISPR0    ((volatile u32*)0xE000E200)
//#define NVIC_REG_ICPR0    ((volatile u32*)0xE000E280)
//#define NVIC_REG_IABR0    ((volatile u32*)0xE000E300)
//#define NVIC_REG_IPR0     *((volatile u32*)0xE000E400)
//#define NVIC_REG_STIR     *((volatile u32*)0xE000EF00)

typedef struct {
	volatile u32 NVIC_ISER[32];
	volatile u32 NVIC_ICER[32];
	volatile u32 NVIC_ISPR[32];
	volatile u32 NVIC_ICPR[32];
	volatile u32 NVIC_IABR[32];
	volatile u32 RESERVED[32];
	volatile u8 NVIC_IPR[132]; // for 21 register of 4-byte size
} NVIC_t;

#define NVIC ((volatile NVIC_t*)0XE000E100)

// both are register in system control block
#define SCB_REG_AIRCR  *((volatile u32*)0xE000ED0C)

#define SCB_REG_SHPR  ((volatile u32*)0xE000ED18)

#endif /* MCAL_NVIC_NVIC_PRIVATE_H_ */
