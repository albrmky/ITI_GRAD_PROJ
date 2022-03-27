/*
 * RCC_program.c
 *
 *  Created on: Feb 14, 2022
 *      Author: noureldeen albrmky
 */

#include "../Inc/LIBRARY/bit_math.h"
#include "../Inc/LIBRARY/standard_types.h"

#include "../Inc/MCAL/RCC/RCC_interface.h"
#include "../Inc/MCAL/RCC/RCC_configuration.h"
#include "../Inc/MCAL/RCC/RCC_private.h"

u32 RCC_F_CPU;
u32 RCC_AHB_Frequency;
u32 RCC_APB1_Frequency;
u32 RCC_APB2_Frequency;

void MRCC_voidInit(void)
{
	/* assign the system clock global variable*/
	RCC_F_CPU = RCC_SYSTEM_CLOCK;

	/* assign the AHB frequency global variable*/
#if RCC_AHB_BUS_PRESCALER <=10
	RCC_AHB_Frequency = RCC_F_CPU >> (RCC_AHB_BUS_PRESCALER - 7);
#else
	RCC_AHB_Frequency = RCC_F_CPU>>(RCC_AHB_BUS_PRESCALER-7+1);
#endif

	/* assign the APB1 & APB2 frequency global variable */
	RCC_APB1_Frequency = RCC_F_CPU >> (RCC_APB1_BUS_PRESCALER - 3);
	RCC_APB2_Frequency = RCC_F_CPU >> (RCC_APB2_BUS_PRESCALER - 3);

	MRCC_voidSetBusClocks(RCC_AHB_BUS_PRESCALER, RCC_APB1_BUS_PRESCALER,
	RCC_APB2_BUS_PRESCALER);

#if RCC_SYSTEM_CLOCK_SOURCE == RCC_SOURCE_PLL

#if RCC_PLL_INPUT == RCC_PLL_INPUT_SOURCE_HSI_2

	MRCC_voidEnableClockSource(RCC_HSI, RCC_STATUS_ON);
	MRCC_u8CheckClockSource(RCC_HSI);
	MRCC_voidSetClockSource(RCC_HSI);
#else
	MRCC_voidEnableClockSource(RCC_HSE, RCC_STATUS_ON);
	MRCC_u8CheckClockSource(RCC_HSE);
	MRCC_voidSetClockSource(RCC_HSE);
#endif

	// enable HSE /2 if used
	if (RCC_PLL_INPUT == RCC_PLL_SOURCE_HSE_2)
	SET_BIT(RCC->RCC_CFGR, 17); //PLLXTRE

	// choose source
	SET_BIT(RCC->RCC_CFGR, 16); //PLLSRC
	INS_FIELD(RCC->RCC_CFGR, 0x1111, 18, (RCC_PLL_MUL_FACTOR-2));

#endif

	MRCC_voidEnableClockSource(RCC_SYSTEM_CLOCK_SOURCE, RCC_STATUS_ON);

	MRCC_u8CheckClockSource(RCC_SYSTEM_CLOCK_SOURCE);

	MRCC_voidSetClockSource(RCC_SYSTEM_CLOCK_SOURCE);

}

u8 MRCC_u8CheckClockSource(RCC_CLOCK_SOURCE copy_u8CLK)
{
	u32 max_wait_time = 50000;
	u32 counter = 0;
	u8 state = 1;

	if ((copy_u8CLK != RCC_HSI) && (copy_u8CLK != RCC_HSE)
			&& (copy_u8CLK != RCC_PLL)) state = 0;
	else
	{
		while (!GET_BIT(RCC->RCC_CR, copy_u8CLK))
		{
			counter++;
			state = 0;
			if (counter == max_wait_time) break;
		}
	}
	return state;
}

void MRCC_voidSetClockSource(RCC_CLOCK_SOURCE copy_u8Source)
{
	switch (copy_u8Source)
	{
		case RCC_HSI:
			INS_FIELD((RCC->RCC_CFGR), 0b11, 0, 0);
			break;
		case RCC_HSE:
			INS_FIELD((RCC->RCC_CFGR), 0b11, 0, 1);
			break;
		case RCC_PLL:
			INS_FIELD((RCC->RCC_CFGR), 0b11, 0, 2);
			break;
	}
}

void MRCC_voidEnableClockSource(RCC_CLOCK_SOURCE copy_u8ClockSource,
		RCC_STATES copy_u8state)
{
	// the insert function offset value is determined with the bus variable
	// which is specified in the interface file
	INS_BIT(RCC->RCC_CR, (copy_u8ClockSource - 1), copy_u8state); // source_ON bit
}

void MRCC_voidSetBusClocks(RCC_AHB_PRESCALER copy_AHB,
		RCC_APB_PRESCALER copy_APB1, RCC_APB_PRESCALER copy_APB2)
{
	// AHB prescaler is different 4-bits wide

	INS_FIELD(RCC->RCC_CFGR, 0b1111, 4, copy_AHB);
	INS_FIELD(RCC->RCC_CFGR, 0b111, 8, copy_APB1);
	INS_FIELD(RCC->RCC_CFGR, 0b111, 11, copy_APB2);

}

void MRCC_voidSetPeripheralClock(RCC_BUS copy_u8Bus, u8 copy_u8PeripheralNum,
		RCC_STATES copy_periph_status)
{
	// dis/enable the desired peripheral on the specified bus
	// the insert function ofset value is determened with the bus variable
	// which is specified in the interface file
	if (copy_u8Bus == RCC_AHB_BUS)
	{
		INS_BIT(RCC->RCC_AHBENR, copy_u8PeripheralNum, copy_periph_status);
	}
	else if (copy_u8Bus == RCC_APB1_BUS)
	{
		INS_BIT(RCC->RCC_APB1ENR, copy_u8PeripheralNum, copy_periph_status);
	}
	else if (copy_u8Bus == RCC_APB2_BUS)
	{
		INS_BIT(RCC->RCC_APB2ENR, copy_u8PeripheralNum, copy_periph_status);
	}
}

void MRCC_voidConfigurePLL(RCC_PLL_SOURCE source, u8 mul_factor)
{
	if (source == RCC_PLL_SOURCE_HSE_2)
	{
		SET_BIT(RCC->RCC_CFGR, 17); //PLLXTRE
		SET_BIT(RCC->RCC_CFGR, 16); //PLLSRC
	}
	else if (source == RCC_PLL_SOURCE_HSE)
	{
		CLR_BIT(RCC->RCC_CFGR, 17); //PLLXTRE
		SET_BIT(RCC->RCC_CFGR, 16); //PLLSRC
	}
	else if (source == RCC_PLL_SOURCE_HSI_2)
	{
		CLR_BIT(RCC->RCC_CFGR, 17); //PLLXTRE
		CLR_BIT(RCC->RCC_CFGR, 16); //PLLSRC
	}

	if (mul_factor > 16) mul_factor = 16;
	if (mul_factor < 2) mul_factor = 2;

	INS_FIELD(RCC->RCC_CFGR, 0x1111, 18, (mul_factor - 2));
}
