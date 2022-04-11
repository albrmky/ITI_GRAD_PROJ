/*
 * SYSTICK_configuration.h
 *
 *  Created on: Feb 24, 2022
 *      Author: noureldeen albrmky
 */

#ifndef MCAL_SYSTICK_SYSTICK_CONFIGURATION_H_
#define MCAL_SYSTICK_SYSTICK_CONFIGURATION_H_

/**********************************************
 Choose the input clock source for systick
 options:
 SYSTICK_CLOCK_SOURCE_AHB_8
 SYSTICK_CLOCK_SOURCE_AHB
 ************************************************/
#define SYSTICK_INPUT_SOURCE	SYSTICK_CLOCK_SOURCE_AHB_8

/**********************************************
 Choose the the status of the int when time out
 options:
 SYSTICK_INT_DISABLE
 SYSTICK_INT_ENABLE
 ************************************************/
#define SYSTICK_INT_STATUS	SYSTICK_INT_DISABLE

/**********************************************
 from 0 to  (16 7777 216)
 *********************************************/
#define SYSTICK_PRELOAD_VALUE	15

#endif /* MCAL_SYSTICK_SYSTICK_CONFIGURATION_H_ */
