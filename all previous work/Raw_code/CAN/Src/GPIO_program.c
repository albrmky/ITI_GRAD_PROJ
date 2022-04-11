/*
 * GPIO_program.c
 *
 *  Created on: Feb 15, 2022
 *      Author: noureldeen albrmky
 */

#include "../Inc/LIBRARY/bit_math.h"
#include "../Inc/LIBRARY/standard_types.h"

#include "../Inc/MCAL/GPIO/GPIO_interface.h"
#include "../Inc/MCAL/GPIO/GPIO_private.h"
#include "../Inc/MCAL/GPIO/GPIO_configuration.h"

/***************************************************
 give a contagious range of pins in a certain port the same configuration
 ****************************************************/
void MGPIO_voidSetPortDirection(GPIO_PORT copy_u8Port, u8 copy_Start,
								u8 copy_end, GPIO_MODE copy_u8Mode,
								GPIO_CONFIGURATION copy_u8CNFG)
{
	for (int i = copy_Start; i <= copy_end; i++)
	{
		MGPIO_voidSetPinDirection(copy_u8Port, i, copy_u8Mode, copy_u8CNFG);
	}
}

void MGPIO_voidSetPinDirection(GPIO_PORT copy_u8Port, u8 copy_u8Pin,
								GPIO_MODE copy_u8Mode,
								GPIO_CONFIGURATION copy_u8CNFG)
{
	u8 field = (copy_u8CNFG << 2) + copy_u8Mode;
	if (copy_u8Pin < 8)
	{
		switch (copy_u8Port)
		{
			case GPIO_PORTA:
				INS_FIELD(GPIOA->GPIO_CRL, 0b1111, (copy_u8Pin * 4), field);
				break;
			case GPIO_PORTB:
				INS_FIELD(GPIOB->GPIO_CRL, 0b1111, (copy_u8Pin * 4), field);
				break;

			case GPIO_PORTC:
				INS_FIELD(GPIOC->GPIO_CRL, 0b1111, (copy_u8Pin * 4), field);
				break;

		}
	}
	else if (copy_u8Pin >= 8 && copy_u8Pin < 16)
	{
		switch (copy_u8Port)
		{
			case GPIO_PORTA:
				INS_FIELD(GPIOA->GPIO_CRH, 0b1111, ((copy_u8Pin - 8) * 4),
						field);
				break;
			case GPIO_PORTB:
				INS_FIELD(GPIOB->GPIO_CRH, 0b1111, ((copy_u8Pin - 8) * 4),
						field);
				break;

			case GPIO_PORTC:
				INS_FIELD(GPIOC->GPIO_CRH, 0b1111, ((copy_u8Pin - 8) * 4),
						field);
				break;

		}
	}
}

void MGPIO_voidSetPinValue(GPIO_PORT copy_u8Port, u8 copy_u8Pin,
							GPIO_VALUE copy_u8Value)
{
	if (copy_u8Pin < 16)
	{

		if (copy_u8Value == GPIO_HIGH)
		{
			switch (copy_u8Port)
			{
				case GPIO_PORTA:
					GPIOA->GPIO_BSRR = 1 << copy_u8Pin; // make advantage of atomic set
					break;
				case GPIO_PORTB:
					GPIOB->GPIO_BSRR = 1 << copy_u8Pin;
					break;

				case GPIO_PORTC:
					GPIOC->GPIO_BSRR = 1 << copy_u8Pin;
					break;
			}

		}
		else if (copy_u8Value == GPIO_LOW)
		{
			switch (copy_u8Port)
			{
				case GPIO_PORTA:
					GPIOA->GPIO_BRR = 1 << copy_u8Pin; // make advantage of atomic clear
					break;
				case GPIO_PORTB:
					GPIOB->GPIO_BRR = 1 << copy_u8Pin;
					break;

				case GPIO_PORTC:
					GPIOC->GPIO_BRR = 1 << copy_u8Pin;
					break;
			}

		}

	}
}

STATUS MGPIO_u8GetPinValue(GPIO_PORT copy_u8Port, u8 copy_u8Pin)
{
	STATUS status = ZERO;
	if (copy_u8Pin < 16)
	{
		switch (copy_u8Port)
		{
			case GPIO_PORTA:
				status = GET_BIT(GPIOA->GPIO_IDR, copy_u8Pin);
				break;
			case GPIO_PORTB:
				status = GET_BIT(GPIOB->GPIO_IDR, copy_u8Pin);
				break;

			case GPIO_PORTC:
				status = GET_BIT(GPIOC->GPIO_IDR, copy_u8Pin);
				break;
		}

	}
	else
	{
		status = ERROR;
	}
	return status;
}

/*****************************************************
 read the port value
 ******************************************************/
STATUS MGPIO_u16GetPort(GPIO_PORT copy_u8Port, u16 *ptr_value)
{
	STATUS status = ZERO;
	switch (copy_u8Port)
	{
		case GPIO_PORTA:
			*ptr_value = GPIOA->GPIO_IDR;
			status = ONE;
			break;
		case GPIO_PORTB:
			*ptr_value = GPIOB->GPIO_IDR;
			status = ONE;
			break;

		case GPIO_PORTC:
			*ptr_value = GPIOC->GPIO_IDR;
			status = ONE;
			break;
	}
	return status;
}

void MGPIO_voidLockPins(GPIO_PORT copy_u8Port, u16 copy_u8LockPattern)
{
	switch (copy_u8Port)
	{
		case GPIO_PORTA:
			GPIOA->GPIO_LCKR = 0x10000 + copy_u8LockPattern;
			GPIOA->GPIO_LCKR = copy_u8LockPattern;
			GPIOA->GPIO_LCKR = 0x10000 + copy_u8LockPattern;
			GPIOA->GPIO_LCKR;
			break;

		case GPIO_PORTB:
			GPIOB->GPIO_LCKR = 0x10000 + copy_u8LockPattern;
			GPIOB->GPIO_LCKR = copy_u8LockPattern;
			GPIOB->GPIO_LCKR = 0x10000 + copy_u8LockPattern;
			GPIOB->GPIO_LCKR;
			break;

		case GPIO_PORTC:
			GPIOC->GPIO_LCKR = 0x10000 + copy_u8LockPattern;
			GPIOC->GPIO_LCKR = copy_u8LockPattern;
			GPIOC->GPIO_LCKR = 0x10000 + copy_u8LockPattern;
			GPIOC->GPIO_LCKR;
			break;
	}

}

/***************************************************************
 write a certain fieldin the port value
 ***************************************************************/
void MGPIO_voidWriteField(GPIO_PORT copy_u8Port, u16 copy_u16Mask,
							u16 copy_u16Value)
{

	u16 local_value_High = copy_u16Mask & copy_u16Value;
	u16 local_value_Low = copy_u16Mask & (~copy_u16Value);

	switch (copy_u8Port)
	{
		case GPIO_PORTA:
			GPIOA->GPIO_BSRR = local_value_High;
			GPIOA->GPIO_BRR = local_value_Low;
			break;

		case GPIO_PORTB:
			GPIOB->GPIO_BSRR = local_value_High;
			GPIOB->GPIO_BRR = local_value_Low;
			break;

		case GPIO_PORTC:
			GPIOC->GPIO_BSRR = local_value_High;
			GPIOC->GPIO_BRR = local_value_Low;
			break;
	}
}

/************************************************************
 decide which pinx portn will be the source for EXTI_LINEx
 *************************************************************/
void MAFIO_voidSetEXTILineSource(AFIO_EXTI_LINE copy_u8line,
									AFIO_EXTI_LINE_SOURCE copy_u8Line_source)
{
	volatile u32 *reg = AFIO_EXTICR1 + (copy_u8line / 4);

	INS_FIELD((*reg), 0b1111, (4 * (copy_u8line % 4)), copy_u8Line_source);

}
