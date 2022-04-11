/*
 * FPEC_interface.h
 *
 *  Created on: Mar 10, 2022
 *      Author: noureldeen albrmky
 */

#ifndef MCAL_FPEC_FPEC_INTERFACE_H_
#define MCAL_FPEC_FPEC_INTERFACE_H_

typedef enum
{
	FLASH_LOCKED,
	FLASH_UNLOCKED
} FLASH_CONTROL_STATUS_t;

typedef enum
{
	FLASH_DISBALE,
	FLASH_ENABLE
} FLASH_INTERRUPT_STATUS_t;

typedef enum
{
	FLASH_EVENT_EOP = 12,
	FLASH_EVENT_ERROR = 10
} FLASH_EVENTS_t;

typedef enum
{
	FLASH_OPT_BYTE_DATA_0 = 2,
	FLASH_OPT_BYTE_DATA_1 = 3
} FLASH_OPTION_BYTES_t;

void FLASH_voidPageErase(u8 PageIndex);
void FLASH_voidWrite(u32 address, u16 *data, u16 DataLength);
void FLASH_voidWrite_record(u16 address_temp, u16 *data, u16 DataLength);

void FLASH_voidINTSTATUS(FLASH_EVENTS_t event, FLASH_INTERRUPT_STATUS_t status);
void FLASH_Assign_UpperAddress(u16 address);

void FLASH_voidWriteDataOptionByte(FLASH_OPTION_BYTES_t option_byte, u8 value);
u8 FLASH_u8ReadDataOptionByte(FLASH_OPTION_BYTES_t option_byte);

void FLASH_Read_CurrentVersion(void);
void FLASH_Write_CurrentVersion(u32 address, char *data);

#endif /* MCAL_FPEC_FPEC_INTERFACE_H_ */
