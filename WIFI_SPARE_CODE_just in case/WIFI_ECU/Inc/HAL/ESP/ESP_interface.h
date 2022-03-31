/*
 * ESP_interface.h
 *
 *  Created on: Mar 10, 2022
 *      Author: noureldeen albrmky
 */

#ifndef HAL_ESP_ESP_INTERFACE_H_
#define HAL_ESP_ESP_INTERFACE_H_

void ESP_voidInit(void);

void ESP_voidConnectToWifi(void);

void ESP_voidConnectToSrvTCP(void);

void ESP_voidSend(char *Data, u8 Data_length);

void ESP_voidClearBuffer(void);

#endif /* HAL_ESP_ESP_INTERFACE_H_ */
