/*
 * USART_interface.h
 *
 *  Created on: Mar 1, 2022
 *      Author: noureldeen albrmky
 */

#ifndef MCAL_USART_USART_INTERFACE_H_
#define MCAL_USART_USART_INTERFACE_H_

typedef enum {

	USART_READ_DATA_REG_NOT_EMPTY = 5,
	USART_TRANSMISSION_COMPLETE,
	USART_TRANSMITT_DATA_REG_EMPTY
} USART_FLAGS_t;

typedef enum {
	USART_DISABLE,
	USART_ENABLE
} USART_STATUS_t;

void MUSART_voidInit(void);

void MUSART_voidTransmittChar(char copy_cData);

u8 MUSART_u8ReceiveChar(void);

void MUSART_SetINTStatus(USART_FLAGS_t copy_eUsartEvent,
		USART_STATUS_t copy_estatus);
void MUSART_voidTransmittString(char *ptr_string);

#endif /* MCAL_USART_USART_INTERFACE_H_ */
