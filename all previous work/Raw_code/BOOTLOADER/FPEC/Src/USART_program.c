/*
 * USART_program.c
 *
 *  Created on: Mar 1, 2022
 *      Author: noureldeen albrmky
 */

#include "../Inc/LIBRARY/bit_math.h"
#include "../Inc/LIBRARY/standard_types.h"

#include "../Inc/MCAL/USART/USART_interface.h"
#include "../Inc/MCAL/USART/USART_private.h"
#include "../Inc/MCAL/USART/USART_configuration.h"

extern u32 RCC_APB2_Frequency;

void MUSART_voidInit(void)
{
// the data width
	INS_BIT(USART1->USART_CR1, 12, USART_DATA_WIDTH);

//parity
	CLR_BIT(USART1->USART_CR1, 11);

// stop bit
	INS_FIELD(USART1->USART_CR2, 0b11, 12, 0);

// set baud rate
	MUSART_voidSetBaudRate(USART_BAUD_RATE);

// enable
	SET_BIT(USART1->USART_CR1, 13); //UE
	SET_BIT(USART1->USART_CR1, 3); //TE
	SET_BIT(USART1->USART_CR1, 2); //TE

}

static void MUSART_voidSetBaudRate(u32 copy_BR)
{
	f32 DIV = RCC_APB2_Frequency / (16.0 * USART_BAUD_RATE);
	u16 DIV_mant = (u16) DIV;
	f32 DIV_frac = (DIV - DIV_mant) * 16;
	if (DIV_frac > 15)
	{
		DIV_mant++;
		DIV_frac = 0;
	}
	INS_FIELD(USART1->USART_BRR, 0xF, 0, ((u16 )DIV_frac));
	INS_FIELD(USART1->USART_BRR, 0xFFF, 4, DIV_mant);
}

void MUSART_voidTransmittChar(char copy_cData)
{
	while (GET_BIT(USART1->USART_SR,7) == 0);
	USART1->USART_DR = copy_cData;
}

u8 MUSART_u8ReceiveChar(void)
{
	u8 local = USART1->USART_DR;
	return local;
}

void MUSART_SetINTStatus(USART_FLAGS_t copy_eUsartEvent,
		USART_STATUS_t copy_estatus)
{
	INS_BIT(USART1->USART_CR1, copy_eUsartEvent, copy_estatus);
}

void MUSART_voidTransmittString(char *ptr_string)
{
	while (*ptr_string != '\0')
	{
		MUSART_voidTransmittChar(*ptr_string++);
	}
}
