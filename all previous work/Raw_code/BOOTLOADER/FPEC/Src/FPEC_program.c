/*
 * FPEC_program.c
 *
 *  Created on: Mar 10, 2022
 *      Author: noureldeen albrmky
 */

#include "../Inc/LIBRARY/bit_math.h"
#include "../Inc/LIBRARY/standard_types.h"

#include "../Inc/MCAL/FPEC/FPEC_interface.h"
#include "../Inc/MCAL/FPEC/FPEC_private.h"
#include "../Inc/MCAL/FPEC/FPEC_configuration.h"

void FLASH_voidControlAccess(FLASH_CONTROL_STATUS_t status)
{
	switch (status)
	{
		case FLASH_LOCKED:
			SET_BIT(FPEC->FLASH_CR, LOCK_BIT);
			break;

		case FLASH_UNLOCKED:
			FPEC->FLASH_KEYR = FLASH_KEY1;
			FPEC->FLASH_KEYR = FLASH_KEY2;
			break;

	}
}

void FLASH_voidOptByteAccess(FLASH_CONTROL_STATUS_t status)
{
	switch (status)
	{
		case FLASH_LOCKED:
			// when cleared option bytes are locked
			CLR_BIT(FPEC->FLASH_CR, OPT_WRE);
			break;

		case FLASH_UNLOCKED:
			FPEC->FLASH_OPTKEYR = FLASH_KEY1;
			FPEC->FLASH_OPTKEYR = FLASH_KEY2;
			break;

	}
}

void FLASH_voidPageErase(u8 PageIndex)
{

	// unlock access to the control register and FPEC
	FLASH_voidControlAccess(FLASH_UNLOCKED);

	// wait for any ongoing operation
	while (GET_BIT(FPEC ->FLASH_SR, BUSY_FLAG));

	// choose the next operation to be page erase
	SET_BIT(FPEC->FLASH_CR, PAGE_ERASE);

	// specify the page address to be erased
	// 0x400 == 1024
	FPEC->FLASH_AR = FLASH_MAIN_MEM_START + PageIndex * 0x400;

	// start the erase operation
	// cleared by HW  when BSY flag is reset
	SET_BIT(FPEC->FLASH_CR, START_BIT);

	// wait for any ongoing operation
	while (GET_BIT(FPEC ->FLASH_SR, BUSY_FLAG));

	// clear the End of Operation Flag
	SET_BIT(FPEC->FLASH_SR, EOP_FLAG);// cleared by writing 1

	// remove the choosing of erasing a page
	// to avoid any conflict with upcoming operation
	CLR_BIT(FPEC->FLASH_CR, PAGE_ERASE);

	// lock access to the control register and FPEC
	FLASH_voidControlAccess(FLASH_LOCKED);

}

void FLASH_voidPageProgram(u32 address, u16 *data, u16 DataLength)
{
	// unlock access to the control register and FPEC
	FLASH_voidControlAccess(FLASH_UNLOCKED);

	// wait for any ongoing operation
	while (GET_BIT(FPEC ->FLASH_SR, BUSY_FLAG));

	// choose the next operation to be page erase
	SET_BIT(FPEC->FLASH_CR, PAGE_PROGRAM);

	// specify the page address to be programmed
	//FPEC->FLASH_AR = address;

	for (int i = 0; i < DataLength; i++)
	{
		// Flash must be written as HalfWord only
		*((u16*) address) = data[i];
		address += 2; // next half word address

		// wait for any ongoing operation
		while (GET_BIT(FPEC ->FLASH_SR, BUSY_FLAG));

		// clear the End of Operation Flag
		SET_BIT(FPEC->FLASH_SR, EOP_FLAG);	// cleared by writing 1

	}

	// remove the choosing of erasing a page
	// to avoid any conflict with upcoming operation
	CLR_BIT(FPEC->FLASH_CR, PAGE_PROGRAM);

	// lock access to the control register and FPEC
	FLASH_voidControlAccess(FLASH_LOCKED);
}

void FLASH_voidINTSTATUS(FLASH_EVENTS_t event, FLASH_INTERRUPT_STATUS_t status)
{
	// unlock access to the control register and FPEC
	FLASH_voidControlAccess(FLASH_UNLOCKED);

	// enable or disable the INT Event
	INS_BIT(FPEC->FLASH_CR, event, status);

	// lock access to the control register and FPEC
	FLASH_voidControlAccess(FLASH_LOCKED);
}

void FLASH_voidWriteDataOptionByte(FLASH_OPTION_BYTES_t option_byte, u8 value)
{

	// wait for any ongoing operation
	while (GET_BIT(FPEC ->FLASH_SR, BUSY_FLAG));

	// unlock access to the control register and FPEC
	FLASH_voidControlAccess(FLASH_UNLOCKED);
	// unlock access to the option bytes
	FLASH_voidOptByteAccess(FLASH_UNLOCKED);

	while (GET_BIT(FPEC ->FLASH_CR, OPT_WRE) == 0);

	// choose the next operation to be option byte erase
	SET_BIT(FPEC->FLASH_CR, OPT_BYTE_ERASE);

	// start the erase operation
	// cleared by HW  when BSY flag is reset
	SET_BIT(FPEC->FLASH_CR, START_BIT);

	// wait for any ongoing operation
	while (GET_BIT(FPEC ->FLASH_SR, BUSY_FLAG));

	// remove option byte erase
	// to avoid any conflict with upcoming operation
	CLR_BIT(FPEC->FLASH_CR, OPT_BYTE_ERASE);

	// clear the End of Operation Flag
	SET_BIT(FPEC->FLASH_SR, EOP_FLAG);// cleared by writing 1

	// choose the next operation to be option byte program
	SET_BIT(FPEC->FLASH_CR, OPT_BYTE_PROGRAM);

	*((u16*) FLASH_OPT_BYTES) = FLASH_RDPRT;

	u32 byte_address = FLASH_OPT_BYTES + 2 * option_byte;

	*((u16*) byte_address) = value;

	// wait for any ongoing operation
	while (GET_BIT(FPEC ->FLASH_SR, BUSY_FLAG));

	// remove option byte program
	// to avoid any conflict with upcoming operation
	CLR_BIT(FPEC->FLASH_CR, OPT_BYTE_PROGRAM);

	// clear the End of Operation Flag
	SET_BIT(FPEC->FLASH_SR, EOP_FLAG);// cleared by writing 1

	// lock access to the option bytes
	FLASH_voidOptByteAccess(FLASH_LOCKED);

	// lock access to the control register and FPEC
	FLASH_voidControlAccess(FLASH_LOCKED);
}

u8 FLASH_u8ReadDataOptionByte(FLASH_OPTION_BYTES_t option_byte)
{

	u16 value;

	u32 byte_address = FLASH_OPT_BYTES + 2 * option_byte;

	value = *((u16*) byte_address);

	return (u8) value;

}
