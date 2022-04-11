
#include "../Inc/LIBRARY/bit_math.h"
#include "../Inc/LIBRARY/standard_types.h"

#include "../Inc/MCAL/FPEC/FPEC_interface.h"
#include "../Inc/MCAL/SYSTICK/SYSTICK_interface.h"

u16 Data_Buffer[8];
u8 Data_Buffer_Counter;
u8 finish_flag = 0;

extern char New_version[10];

extern u8 FirstBankStartPage;
extern u8 FirstBankSize;
extern u8 SecondBankStartPage;
extern u8 SecondBankSize;

/* Convert a ascii to a hex digi */
u8 ASCII_To_HexDigit(char ch)
{
	u8 value = 0;
	if (ch >= '0' && ch <= '9')
	{
		value = ch - '0';
	}
	else if (ch >= 'A' && ch <= 'F')
	{
		value = ch - 'A' + 10;
	}
	else if (ch >= 'a' && ch <= 'f')
	{
		value = ch - 'a' + 10;
	}

	return value;
}

u8 ASCII_To_u8Number(char High_Nibble, char Low_Nibble)
{
	u8 value = (ASCII_To_HexDigit(High_Nibble) << 4)
			+ ASCII_To_HexDigit(Low_Nibble);
	return value;
}

u8 Check_Record(u8 *record)
{
	// extract the length of data field 
	// because the byte is represented by two hex digits it must be multiplied by two e.g. 6 bytes == 12 hex digits
	// then add 10 which are the length of LL , AAAA , TT and CC 
	u8 length = ASCII_To_u8Number(record[1], record[2]) * 2 + 10; // adding the following fields (ll ,aaaa,tt,cc)

	// the value which will hold the sum of bytes 
	u8 checksum = 0;

	// status to be returned 0 means valid 1  means wrong  
	u8 status = 0;

	// sum all bytes including the checksum byte 
	for (int i = 1; i <= length; i += 2)
	{
		checksum += ASCII_To_u8Number(record[i], record[i + 1]);
	}

	if (checksum)
	{
		status = 1;
	}

	return status;
}

void Parse_Record(u8 *record)
{
	u8 LL = ASCII_To_u8Number(record[1], record[2]); // length of data

	u16 AAAA = (ASCII_To_u8Number(record[3], record[4]) << 8)
			+ ASCII_To_u8Number(record[5], record[6]); // address associated with the record

	u8 TT = ASCII_To_u8Number(record[7], record[8]); // type of the record

	// counter to travesr the Data_Buffer 
	Data_Buffer_Counter = 0;

	// convert the Data in the record to half word values to be used when writing the flash to the record
	for (int i = 0; i < LL * 2; i += 4)
	{
		Data_Buffer[Data_Buffer_Counter++] = (ASCII_To_u8Number(
				record[i + 9 + 2], record[i + 9 + 3]) << 8)
				+ ASCII_To_u8Number(record[i + 9 + 0], record[i + 9 + 1]);
	}

	if (TT == 0) // data record
	{
		// we use the AAAA field to pass it to this function which will be merged 
		// with the upper 16-bit from any previous opeartion 
		// LL/2 because we converted the data bytes into half words be able to written in the Flash
		FLASH_voidWrite_record(AAAA, Data_Buffer, LL / 2);
	}
	else if (TT == 4)
	{
		// we extract the 16-bit upper address from data field then assign it for next operations 
		u16 temp = (ASCII_To_u8Number(record[9], record[10]) << 8)
				+ ASCII_To_u8Number(record[11], record[12]); // address associated with the record
		FLASH_Assign_UpperAddress(temp);
	}
	else if (TT == 1) // last record in the file 
	{
		// read data option byte to know the address of the current active bank
		u8 dumb = FLASH_u8ReadDataOptionByte(FLASH_OPT_BYTE_DATA_1);
		if (dumb == 0x11)
		{
			// write the version at the end of bank 2
			FLASH_Write_CurrentVersion(
					0x08000000 + 0x400 * (SecondBankStartPage + SecondBankSize)
							- 12, New_version);
			// change the value of data option byte  
			FLASH_voidWriteDataOptionByte(FLASH_OPT_BYTE_DATA_1, 0x22);
		}
		else if (dumb == 0x22)
		{
			FLASH_Write_CurrentVersion(
					0x08000000 + 0x400 * (FirstBankStartPage + FirstBankSize)
							- 12, New_version);
			FLASH_voidWriteDataOptionByte(FLASH_OPT_BYTE_DATA_1, 0x11);
		}

		// set this flag to jump to the application code immediately 
		finish_flag = 1;
	}
}
