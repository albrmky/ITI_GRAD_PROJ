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

static u16 Address_Upper_16_bit; // to store the upper 16-bit address coming from the 04 record type 

extern char Current_version[10]; // declared in the main.c file and modified here by FLASH_Write_CurrentVersion() function 

// the boundaries of the Flash Banks
extern u8 FirstBankStartPage;
extern u8 FirstBankSize;
extern u8 SecondBankStartPage;
extern u8 SecondBankSize;

/****************************************************************
 called to modify the global variable Address_Upper_16_bit
 ***************************************************************/
void FLASH_Assign_UpperAddress(u16 address)
{
	Address_Upper_16_bit = address;
}

/****************************************************************
 Lock or Unlock the Write operation to 
 the Flash memory and FPEC peripheral 
 ***************************************************************/
static void FLASH_voidControlAccess(FLASH_CONTROL_STATUS_t status)
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

/****************************************************************
 Lock or Unlock the Write operation to 
 the option bytes
 ***************************************************************/
static void FLASH_voidOptByteAccess(FLASH_CONTROL_STATUS_t status)
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

/****************************************************************
 Erase a whole page of flash 
 takes the index of the page as input  0:63
 ***************************************************************/
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

/****************************************************************
 Write a whole record in the memory 
 address_temp parameter is extracted from the record itself
 data and DataLength pass through some external logic after
 being extracted from the record before they  are passed to this function  
 ***************************************************************/
void FLASH_voidWrite_record(u16 address_temp, u16 *data, u16 DataLength)
{
	// unlock access to the control register and FPEC
	FLASH_voidControlAccess(FLASH_UNLOCKED);

	// wait for any ongoing operation
	while (GET_BIT(FPEC ->FLASH_SR, BUSY_FLAG));

	// choose the next operation to be page erase
	SET_BIT(FPEC->FLASH_CR, PAGE_PROGRAM);

	// specify the page address to be programmed
	//FPEC->FLASH_AR = address;

	// the parameter is the lower 16-bit of the address only
	u32 address = (Address_Upper_16_bit << 16) + address_temp;

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

/****************************************************************
 Write Data to Flash 
 address,data and DataLength need to be specified explicitly
 
 ***************************************************************/
void FLASH_voidWrite(u32 address, u16 *data, u16 DataLength)
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

/****************************************************************
 Enable or Disable Interrupt for a Certain Event in FPEC Peripheral
 e.g. error when writing address which is not erased (!= 0xFFFF)
 error when try to write a write-protected address
 or Trigger interrupt at EOP (End of Operation)
 ***************************************************************/
void FLASH_voidINTSTATUS(FLASH_EVENTS_t event, FLASH_INTERRUPT_STATUS_t status)
{
	// unlock access to the control register and FPEC
	FLASH_voidControlAccess(FLASH_UNLOCKED);

	// enable or disable the INT Event
	INS_BIT(FPEC->FLASH_CR, event, status);

	// lock access to the control register and FPEC
	FLASH_voidControlAccess(FLASH_LOCKED);
}

/****************************************************************
 Write to Data Option Byte 
 ***************************************************************/
void FLASH_voidWriteDataOptionByte(FLASH_OPTION_BYTES_t option_byte, u8 value)
{

	// wait for any ongoing operation
	while (GET_BIT(FPEC ->FLASH_SR, BUSY_FLAG));

	// unlock access to the control register and FPEC
	FLASH_voidControlAccess(FLASH_UNLOCKED);
	// unlock access to the option bytes
	FLASH_voidOptByteAccess(FLASH_UNLOCKED);

	// just make sure that option byte write access is enabled 
	// can be removed
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

	// after option byte erase read of Flash is protected
	// so we need to enable it again 
	*((u16*) FLASH_OPT_BYTES) = FLASH_RDPRT;

	// get the address of the Data Option Byte 1  or 2 
	u32 byte_address = FLASH_OPT_BYTES + 2 * option_byte;

	// write the required value of the option byte  
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

/******************************************************************************
 Read Data Option Byte 
 *****************************************************************************/
u8 FLASH_u8ReadDataOptionByte(FLASH_OPTION_BYTES_t option_byte)
{
	u16 value = 0; // variable to be returned 

	// get the address of the Data Option Byte 1  or 2 
	u32 byte_address = FLASH_OPT_BYTES + 2 * option_byte;

	value = *((u16*) byte_address);
	return (u8) value;
}

////////////////////////////////////////////

u8 strlength(char *str)
{
	u8 counter = 0;
	while (*str++ != '\0')
	{
		counter++;
	}
	return counter;
}

/********************************************************************
 Write the version of the FirmWare in the Last 12 HalfWord
 of the Flash Bank
 -> address is Specified explicitly when calling the function 
 -> to make the logic easier this function write a char in a halfword
 -> reading function handle how to reverse the padding 
 ******************************************************************/
void FLASH_Write_CurrentVersion(u32 address, char *data)
{

	// unlock access to the control register and FPEC
	FLASH_voidControlAccess(FLASH_UNLOCKED);

	// wait for any ongoing operation
	while (GET_BIT(FPEC ->FLASH_SR, BUSY_FLAG));

	// choose the next operation to be page erase
	SET_BIT(FPEC->FLASH_CR, PAGE_PROGRAM);

	int i;

	for (i = 0; i < strlength(data); i++)
	{
		// Flash must be written as HalfWord only
		*((u16*) address) = data[i];
		address += 2; // next half word address

		// wait for any ongoing operation
		while (GET_BIT(FPEC ->FLASH_SR, BUSY_FLAG));

		// clear the End of Operation Flag
		SET_BIT(FPEC->FLASH_SR, EOP_FLAG);	// cleared by writing 1

	}
	*((u16*) address) = '\0'; // a delimiter

	// remove the choosing of erasing a page
	// to avoid any conflict with upcoming operation
	CLR_BIT(FPEC->FLASH_CR, PAGE_PROGRAM);

	// lock access to the control register and FPEC
	FLASH_voidControlAccess(FLASH_LOCKED);
}

/*************************************************************************
 Read the Version of the FirmWare in the Active Bank
 -> it reads the Data Option byte to know the active bank
 -> according to the Active bank it can extract the Version 
 -> the value is written to Current_Version Global variable which is defined in main.c and externed in this file  
 *************************************************************************/
void FLASH_Read_CurrentVersion(void)
{
	// read option byte 
	u8 dumb = FLASH_u8ReadDataOptionByte(FLASH_OPT_BYTE_DATA_1);

	// hold the address of the start of version data acording to the active bank 
	u16 *address;
	if (dumb == 0x11)
	{
		address = (u16*) (0x08000000
				+ 0x400 * (FirstBankStartPage + FirstBankSize)
				- 12);
	}
	else if (dumb == 0x22)
	{
		address = (u16*) (0x08000000
				+ 0x400 * (SecondBankStartPage + SecondBankSize)
				- 12);
	}

	u8 counter = 0; // counter to traverse in the Current_Version Buffer 
	do
	{

		// A single character is written in a halfword (16-bit)
		// so we read a half word then typecast it to char to remove the padding 
		Current_version[counter++] = (char) (*address);
	} while (*address++ != '\n');		// using do while to include the \n 

}

