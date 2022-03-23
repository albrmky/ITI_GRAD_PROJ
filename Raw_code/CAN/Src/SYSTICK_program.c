#include "../Inc/LIBRARY/bit_math.h"
#include "../Inc/LIBRARY/standard_types.h"

#include "../Inc/MCAL/SYSTICK/SYSTICK_interface.h"
#include "../Inc/MCAL/SYSTICK/SYSTICK_private.h"
#include "../Inc/MCAL/SYSTICK/SYSTICK_configuration.h"

/*
 * SYSTICK_program.c
 *
 *  Created on: Feb 24, 2022
 *      Author: noureldeen albrmky
 */

extern u32 RCC_F_CPU;
u32 SYSTICK_Frequency;

void (*SYSTICK_CALLBACK)(void) = NULL;
u8 SYSTICK_ISR_TYPE_PERIODIC = 0;

void MSYSTICK_voidInit(void)
{
	// assign the global variable SYSTICK_Frequency according to the used input source
#if SYSTICK_INPUT_SOURCE==SYSTICK_CLOCK_SOURCE_AHB
	SYSTICK_Frequency = RCC_F_CPU;
#elif SYSTICK_INPUT_SOURCE == SYSTICK_CLOCK_SOURCE_AHB_8
	SYSTICK_Frequency = RCC_F_CPU / 8;
#endif

	// decide if input source is HSE or HSE/8
	INS_BIT(SYSTICK->STK_CTRL, 2, SYSTICK_INPUT_SOURCE);
	// decide the interrupt state 
	INS_BIT(SYSTICK->STK_CTRL, 1, SYSTICK_INT_STATUS);

#if (SYSTICK_PRELOAD_VALUE)
	// assign an initial valuefor the systick
	SYSTICK->STK_LOAD = SYSTICK_PRELOAD_VALUE;
#else
#error	SYSTICK_PRELOAD_VALUE in Configuration file must have a value
#endif
}


/**************************************************
just to enable the systick
**************************************************/
void MSYSTICK_voidSTATUS(SYSTICK_STATUS_t status)
{
	INS_BIT(SYSTICK->STK_CTRL, 0, status);
}


/*************************************************
preload the SYSTICK immediately or wait to finish the current counting loop
**************************************************/
void MSYSTICK_voidPreload(SYSTICK_PRELOAD_t type, u32 value)
{
	if (type == SYSTICK_PRELOAD_IMMEDIATE)
	{
		SYSTICK->STK_LOAD = value;
		SYSTICK->STK_VAL = 0;

	}
	else
	{
		SYSTICK->STK_LOAD = value;
	}
}

void MSYSTICK_voidBusyWait(u32 NumberOfTicks)
{
	// load value then clear current value
	SYSTICK->STK_LOAD = NumberOfTicks;
	SYSTICK->STK_VAL = 0;

	// enable systick
	SET_BIT(SYSTICK->STK_CTRL, 0);

	// wait to finish
	while (!GET_BIT(SYSTICK->STK_CTRL, 16));

	// disable systick
	CLR_BIT(SYSTICK->STK_CTRL, 0);

}

void MSYSTICK_voidDelay_MS(u32 Delay_In_MS)
{

	// load value then clear current value
	SYSTICK->STK_LOAD = Delay_In_MS * (SYSTICK_Frequency / 1000);
	SYSTICK->STK_VAL = 0;

	// enable systick
	SET_BIT(SYSTICK->STK_CTRL, 0);

	// wait to finish
	while (!GET_BIT(SYSTICK->STK_CTRL, 16));

	// disable systick
	CLR_BIT(SYSTICK->STK_CTRL, 0);

}

void MSYSTICK_voidSetIntervalSingle(u32 Num_Ticks, void (*ptr)(void))
{
	// disable systick
	CLR_BIT(SYSTICK->STK_CTRL, 0);

	// disable Int
	CLR_BIT(SYSTICK->STK_CTRL, 1);

	// load value then clear current value
	SYSTICK->STK_LOAD = Num_Ticks;
	SYSTICK->STK_VAL = 0;

	// set call_back
	SYSTICK_CALLBACK = ptr;

	// set flag to zero
	SYSTICK_ISR_TYPE_PERIODIC = 0;

	// enable systick
	SET_BIT(SYSTICK->STK_CTRL, 0);

	// enable Int
	SET_BIT(SYSTICK->STK_CTRL, 1);

}

void MSYSTICK_voidSetIntervalPeriodic(u32 Num_Ticks, void (*ptr)(void))
{
	// disable systick
	CLR_BIT(SYSTICK->STK_CTRL, 0);

	// disable Int
	CLR_BIT(SYSTICK->STK_CTRL, 1);

	// load value then clear current value
	SYSTICK->STK_LOAD = Num_Ticks;
	SYSTICK->STK_VAL = 0;

	// set call_back
	SYSTICK_CALLBACK = ptr;

	// set flag to one
	SYSTICK_ISR_TYPE_PERIODIC = 1;

	// enable systick
	SET_BIT(SYSTICK->STK_CTRL, 0);

	// enable Int
	SET_BIT(SYSTICK->STK_CTRL, 1);

}

void MSYSTICK_voidStopPeriodicFunction(void)
{
	// disable systick
	CLR_BIT(SYSTICK->STK_CTRL, 0);

	// disable Int
	CLR_BIT(SYSTICK->STK_CTRL, 1);

	// load value then clear current value
	SYSTICK->STK_LOAD = 0;
	SYSTICK->STK_VAL = 0;
}


/******************************************
convert from milliseconds to ticks
*******************************************/
u32 MSYSTICK_u32MS_TO_TICKS(u32 copy_timeMS)
{
	return copy_timeMS * (SYSTICK_Frequency / 1000);
}



void SysTick_Handler(void)
{
	if (SYSTICK_ISR_TYPE_PERIODIC == 0)
	{
		// disable systick
		CLR_BIT(SYSTICK->STK_CTRL, 0);

		// disable Int
		CLR_BIT(SYSTICK->STK_CTRL, 1);

		// load value then clear current value
		SYSTICK->STK_LOAD = 0;
		SYSTICK->STK_VAL = 0;
	}

	SYSTICK_CALLBACK();

}

u32 MSYSTICK_u32GetRemaniningTime(void)
{
	return SYSTICK->STK_VAL;
}

u32 MSYSTICK_u32GetElapsedTime(void)
{
	return SYSTICK->STK_LOAD - SYSTICK->STK_VAL;
}
