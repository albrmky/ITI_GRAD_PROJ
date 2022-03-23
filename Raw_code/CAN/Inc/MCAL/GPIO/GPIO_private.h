/*
 * GPIO_private.h
 *
 *  Created on: Feb 15, 2022
 *      Author: noureldeen albrmky
 */

#ifndef MCAL_GPIO_GPIO_PRIVATE_H_
#define MCAL_GPIO_GPIO_PRIVATE_H_

typedef struct {
	volatile u32 GPIO_CRL;
	volatile u32 GPIO_CRH;
	volatile u32 GPIO_IDR;
	volatile u32 GPIO_ODR;
	volatile u32 GPIO_BSRR;
	volatile u32 GPIO_BRR;
	volatile u32 GPIO_LCKR;
} GPIO_t;

#define GPIOA ((volatile GPIO_t*)(0x40010800))
#define GPIOB ((volatile GPIO_t*)(0x40010C00))
#define GPIOC ((volatile GPIO_t*)(0x40011000))

#define AFIO_EXTICR1   ((volatile u32*)(0x40010008))

#endif /* MCAL_GPIO_GPIO_PRIVATE_H_ */
