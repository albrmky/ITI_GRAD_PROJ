/*
 * GPIO_interface.h
 *
 *  Created on: Feb 15, 2022
 *      Author: noureldeen albrmky
 */

#ifndef MCAL_GPIO_GPIO_INTERFACE_H_
#define MCAL_GPIO_GPIO_INTERFACE_H_

typedef enum {
	GPIO_MODE_INPUT,
	GPIO_MODE_OUTPUT_10_MHZ,
	GPIO_MODE_OUTPUT_2_MHZ,
	GPIO_MODE_OUTPUT_50_MHZ

} GPIO_MODE;

typedef enum {
	GPIO_OUTPUT_CNFG_GP_PP,
	GPIO_OUTPUT_CNFG_GP_OD,
	GPIO_OUTPUT_CNFG_ALT_PP,
	GPIO_OUTPUT_CNFG_ALT_OD,
	GPIO_INPUT_CNFG_ANALOG,
	GPIO_INPUT_CNFG_FLOATING,
	GPIO_INPUT_CNFG_PUSH_PULL

} GPIO_CONFIGURATION;

typedef enum {
	GPIO_PORTA,
	GPIO_PORTB,
	GPIO_PORTC
} GPIO_PORT;

typedef enum {
	GPIO_LOW,
	GPIO_HIGH,
} GPIO_VALUE;

void MGPIO_voidSetPinDirection(GPIO_PORT copy_u8Port, u8 copy_u8Pin,
		GPIO_MODE copy_u8Mode, GPIO_CONFIGURATION copy_u8CNFG);

void MGPIO_voidSetPinValue(GPIO_PORT copy_u8Port, u8 copy_u8Pin,
		GPIO_VALUE copy_u8Value);

STATUS MGPIO_u8GetPinValue(GPIO_PORT copy_u8Port, u8 copy_u8Pin);

void MGPIO_voidLockPins(GPIO_PORT copy_u8Port, u16 copy_u8LockPattern);

void MGPIO_voidWriteField(GPIO_PORT copy_u8Port, u16 copy_u16Mask,
		u16 copy_u16Value);

void MGPIO_voidSetPortDirection(GPIO_PORT copy_u8Port, u8 copy_Start,
		u8 copy_end, GPIO_MODE copy_u8Mode, GPIO_CONFIGURATION copy_u8CNFG);

STATUS MGPIO_u16GetPort(GPIO_PORT copy_u8Port, u16 *ptr_value);

#define GPIO_LOWER_BYTE_MASK	0x00FF

//////////////////////////////////////////////////////////////////
/* AFIO typedefs and functions interface */

typedef enum {
	AFIO_EXTI0_LINE,
	AFIO_EXTI1_LINE,
	AFIO_EXTI2_LINE,
	AFIO_EXTI3_LINE,
	AFIO_EXTI4_LINE,
	AFIO_EXTI5_LINE,
	AFIO_EXTI6_LINE,
	AFIO_EXTI7_LINE,
	AFIO_EXTI8_LINE,
	AFIO_EXTI9_LINE,
	AFIO_EXTI10_LINE,
	AFIO_EXTI11_LINE,
	AFIO_EXTI12_LINE,
	AFIO_EXTI13_LINE,
	AFIO_EXTI14_LINE,
	AFIO_EXTI15_LINE
} AFIO_EXTI_LINE;

typedef enum {
	AFIO_EXTI_GPIOA,
	AFIO_EXTI_GPIOB,
	AFIO_EXTI_GPIOC

} AFIO_EXTI_LINE_SOURCE;

void MAFIO_voidSetEXTILineSource(AFIO_EXTI_LINE copy_u8line,
		AFIO_EXTI_LINE_SOURCE copy_u8Line);
#endif /* MCAL_GPIO_GPIO_INTERFACE_H_ */
