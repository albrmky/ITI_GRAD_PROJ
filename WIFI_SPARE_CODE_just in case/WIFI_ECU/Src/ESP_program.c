/*
 * ESP_program.c
 *
 *  Created on: Mar 10, 2022
 *      Author: noureldeen albrmky
 */

#include "../Inc/LIBRARY/bit_math.h"
#include "../Inc/LIBRARY/standard_types.h"

#include "../Inc/MCAL/USART/USART_interface.h"
#include "../Inc/MCAL/SYSTICK/SYSTICK_interface.h"

#include "../Inc/HAL/ESP/ESP_interface.h"
#include "../Inc/HAL/ESP/ESP_private.h"
#include "../Inc/HAL/ESP/ESP_configuration.h"

#define BUFFER_LENGTH	100

static u8 buffer[BUFFER_LENGTH];
static u8 counter;

//void USART1_IRQHandler(void)
//{
//	buffer[counter] = MUSART_u8ReceiveChar();
//	counter++;
//	counter %= BUFFER_LENGTH;
//
//}

void ESP_voidInit(void)
{
	MUSART_voidTransmittString("AT\r\n");
	MSYSTICK_voidDelay_MS(500);

	MUSART_voidTransmittString("AT+RST\r\n");
	MSYSTICK_voidDelay_MS(500);

	ESP_voidClearBuffer();
	MUSART_voidTransmittString("ATE0\r\n");
	MSYSTICK_voidDelay_MS(3000);

	ESP_voidClearBuffer();
	MUSART_voidTransmittString("AT+CWMODE=3\r\n");
	MSYSTICK_voidDelay_MS(3000);

	ESP_voidClearBuffer();
	MUSART_voidTransmittString("AT+CIPMODE=0\r\n");
	MSYSTICK_voidDelay_MS(3000);
}

void ESP_voidConnectToWifi(void)
{
	ESP_voidClearBuffer();
	MUSART_voidTransmittString("AT+CWJAP_CUR=\"");
	MUSART_voidTransmittString(WIFI_SSID);
	MUSART_voidTransmittString("\",\"");
	MUSART_voidTransmittString(WIFI_PSK);
	MUSART_voidTransmittString("\"\r\n");

	MSYSTICK_voidDelay_MS(8000);
}

void NumToStr(u8 num, char *str)
{
	int dumb1 = num % 10;
	num /= 10;
	int dumb2 = num % 10;
	str[0] = dumb2;
	str[1] = dumb1;
	str[2] = '\0';
}

void ESP_voidSend(char *Data, u8 Data_length)
{
	char num_str[5];
	NumToStr(Data_length, num_str);

	MUSART_voidTransmittString("AT+CIPSEND=");
	MUSART_voidTransmittString(num_str);
	MUSART_voidTransmittString("\r\n");
	MSYSTICK_voidDelay_MS(500);

	MUSART_voidTransmittString(Data);
}

void ESP_voidConnectToSrvTCP(void)
{
	ESP_voidClearBuffer();
	MUSART_voidTransmittString("AT+CIPSTART=\"TCP\",\"");
	MUSART_voidTransmittString(SERVER_IP_ADDRESS);
	MUSART_voidTransmittString("\",");
	MUSART_voidTransmittString(SERVER_PORT);
	MUSART_voidTransmittString("\r\n");

	MSYSTICK_voidDelay_MS(2000);

}

void ESP_voidClearBuffer(void)
{
	for (u8 i = 0; i < BUFFER_LENGTH; i++)
	{
		buffer[i] = 0;
	}
	counter = 0;
}

