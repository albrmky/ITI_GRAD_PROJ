/*
 * SYSTICK_interface.h
 *
 *  Created on: Feb 24, 2022
 *      Author: noureldeen albrmky
 */

#ifndef MCAL_SYSTICK_SYSTICK_INTERFACE_H_
#define MCAL_SYSTICK_SYSTICK_INTERFACE_H_

typedef enum {
	SYSTICK_STATUS_DISABLE,
	SYSTICK_STATUS_ENABLE
} SYSTICK_STATUS_t;

typedef enum {
	SYSTICK_PRELOAD_IMMEDIATE,
	SYSTICK_PRELOAD_WAIT_UF
} SYSTICK_PRELOAD_t;

void MSYSTICK_voidInit(void);
void MSYSTICK_voidSTATUS(SYSTICK_STATUS_t status);
void MSYSTICK_voidPreload(SYSTICK_PRELOAD_t type, u32 value);
void MSYSTICK_voidBusyWait(u32 NumberOfTicks);
void MSYSTICK_voidDelay_MS(u32 Delay_In_MS);

void MSYSTICK_voidSetIntervalSingle(u32 Num_Ticks, void (*ptr)(void));

void MSYSTICK_voidSetIntervalPeriodic(u32 Num_Ticks, void (*ptr)(void));

void MSYSTICK_voidStopPeriodicFunction(void);

u32 MSYSTICK_u32MS_TO_TICKS(u32 copy_timeMS);

void SysTick_Handler(void);

u32 MSYSTICK_u32GetRemaniningTime(void);

u32 MSYSTICK_u32GetElapsedTime(void);

#endif /* MCAL_SYSTICK_SYSTICK_INTERFACE_H_ */
