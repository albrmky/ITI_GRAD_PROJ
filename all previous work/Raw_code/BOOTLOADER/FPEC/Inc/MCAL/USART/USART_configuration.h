/*
 * USART_configuration.h
 *
 *  Created on: Mar 1, 2022
 *      Author: noureldeen albrmky
 */

#ifndef MCAL_USART_USART_CONFIGURATION_H_
#define MCAL_USART_USART_CONFIGURATION_H_

#define USART_BAUD_RATE			9600

/********************************************
 choose the used width of the transceived data
 options:
 USART_DATA_WIDTH_8_BIT
 USART_DATA_WIDTH_9_BIT
 ********************************************/
#define USART_DATA_WIDTH		USART_DATA_WIDTH_8_BIT

/**************************************************
 choose the stop bit width
 options:
 USART_STOP_BIT_WIDTH_1	
 USART_STOP_BIT_WIDTH_0_5
 USART_STOP_BIT_WIDTH_1_5
 USART_STOP_BIT_WIDTH_2	
 *************************************************/
#define USART_STOP_BIT_WIDTH	USART_STOP_BIT_WIDTH_1

/***************************************************
 choose the type of parity
 options:
 USART_PARITY_ODD
 USART_PARITY_EVEN
 **************************************************/
#define USART_PARITY_TYPE		USART_PARITY_EVEN

/**************************************************
 choose the status of the USART
 options:
 USART_STATUS_ENABLE
 USART_STATUS_DISABLE
 **************************************************/
#define USART_PARITY_STATUS		USART_STATUS_DISABLE

#endif /* MCAL_USART_USART_CONFIGURATION_H_ */
