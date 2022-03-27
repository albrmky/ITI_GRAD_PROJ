#include "../Inc/LIBRARY/bit_math.h"
#include "../Inc/LIBRARY/standard_types.h"

#include "../Inc/MCAL/FPEC/FPEC_interface.h"
#include "../Inc/MCAL/RCC/RCC_interface.h"
#include "../Inc/MCAL/NVIC/NVIC_interface.h"
#include "../Inc/MCAL/GPIO/GPIO_interface.h"
#include "../Inc/MCAL/SYSTICK/SYSTICK_interface.h"
#include "../Inc/MCAL/USART/USART_interface.h"
#include "../Inc/MCAL/CAN/CAN_interface.h"
#include "../Inc/Parser.h"

typedef void (*Function_t)(void); // pointer to function who take void and return nothing
Function_t addr_to_call = 0; // an instance of that data type

// flags for parsing the record
u8 Start_Parsing_Flag; // wait for '\n' -> parse -> wait again
u8 First_Record_Flag; // to clean the target bank before writing the flash
extern u8 finish_flag; // indicate that flashing is ended ant make the code starts running immediately

// receive buffer for record
u8 Recv_Buffer[50]; // where data of records are  stored
u8 Recv_Temp_Buffer[8];
RX_Struct_t Recv_Header;
u8 Recv_Counter; // counter of the data record

// receive buffer for inquiry
u8 Inquire_Buffer[50]; // buffer for inquiring data
u8 Inquire_Temp_Buffer[8];
RX_Struct_t Inquire_Header;
u8 Inquire_Counter; // counter for inquiring buffer

/* change those acccording to the code */
u8 FirstBankStartPage = 10;
u8 FirstBankSize = 24;
u8 SecondBankStartPage = 34;
u8 SecondBankSize = 30;

/* store the storage */
char Current_version[10];
char New_version[10] = "3.2\n";

// this function do not need any further modification
void func(void)
{
#define SCB_VTOR *((volatile u32*)0xE000ED08)
	u8 dumb = FLASH_u8ReadDataOptionByte(FLASH_OPT_BYTE_DATA_1); // according to the option byte move the vector table
	if (dumb == 0x11)
	{
		SCB_VTOR = 0x08000000 + 0x400 * FirstBankStartPage;
	}
	else if (dumb == 0x22)
	{
		SCB_VTOR = 0x08000000 + 0x400 * SecondBankStartPage;
	}

	// the address of the reset vector table
	addr_to_call = *(Function_t*) (SCB_VTOR + 4);
	addr_to_call();
}

void USB_LP_CAN_RX0_IRQHandler(void) // for inquiry
{
	CAN_voidReceiveMailBox(0, &Inquire_Header, Inquire_Temp_Buffer);
	u8 temp;
	for (int i = 0; i < Inquire_Header.Data_length; i++)
	{
		temp = Inquire_Temp_Buffer[i];
		Inquire_Buffer[Inquire_Counter++] = temp;
	}
	if (temp == '\n')
	{
		if (Inquire_Buffer[0] == 'G' && Inquire_Buffer[4] == 'V') // Get Version?\n
		{
			//send current version by CAN ->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>to be continued make sure \n is sent
		}
		else if (Inquire_Buffer[0] == 'G' && Inquire_Buffer[4] == 'A') // Get Active Bankn\n
		{
			//send current bank by CAN ->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>to be continued 0x11 send 1 if 0x22 send 2 followed by \n
		}
		Inquire_Counter = 0;
	}
}

void CAN_RX1_IRQHandler(void) // for record
{
	// read upcoming data and release the FIFO
	CAN_voidReceiveMailBox(1, &Recv_Header, Recv_Temp_Buffer);

	u8 temp;

	//move the upcoming data to the main buffer 
	for (int i = 0; i < Recv_Header.Data_length; i++)
	{
		temp = Recv_Temp_Buffer[i];
		Recv_Buffer[Recv_Counter++] = temp;
	}
	if (temp == '\n') // if \n is received start to take action 
	{
		Start_Parsing_Flag = 1;
		Recv_Counter = 0;
	}
}

void USART1_IRQHandler(void)
{
	u8 data = MUSART_u8ReceiveChar();
	Recv_Buffer[Recv_Counter++] = data;

	if (data == '\n')
	{
		Start_Parsing_Flag = 1;
		Recv_Counter = 0;
	}
}

int main(void)
{

	/************************************************* RCC configuration ***************************************/
	// must be HSI
	// prescaler for AHB must be 1
	MRCC_voidInit();
	MRCC_voidSetPeripheralClock(RCC_GPIOC, RCC_STATUS_ON);
	MRCC_voidSetPeripheralClock(RCC_GPIOA, RCC_STATUS_ON);
	MRCC_voidSetPeripheralClock(RCC_FLITF, RCC_STATUS_ON);
	MRCC_voidSetPeripheralClock(RCC_USART1, RCC_STATUS_ON);

	/************************************************* GPIO configuration ***************************************/
	MGPIO_voidSetPinDirection(GPIO_PORTC, 13, GPIO_MODE_OUTPUT_10_MHZ,
			GPIO_OUTPUT_CNFG_GP_PP); // LED

	MGPIO_voidSetPinDirection(GPIO_PORTA, 9, GPIO_MODE_OUTPUT_10_MHZ,
			GPIO_OUTPUT_CNFG_ALT_PP); // TX
	MGPIO_voidSetPinDirection(GPIO_PORTA, 10, GPIO_MODE_INPUT,
			GPIO_INPUT_CNFG_FLOATING); // RX
	MRCC_voidSetPeripheralClock(RCC_CAN, RCC_STATUS_ON);

	MGPIO_voidSetPinDirection(GPIO_PORTA, 11, GPIO_MODE_INPUT,
			GPIO_INPUT_CNFG_FLOATING); // CAN RX A11
	MGPIO_voidSetPinDirection(GPIO_PORTA, 12, GPIO_MODE_OUTPUT_2_MHZ,
			GPIO_OUTPUT_CNFG_ALT_PP); // CAN TX A12

	/************************************************* CAN configuration ***************************************/

	// the value with which the comparison will happen according to the mask
	u32 ID = CAN_u32GetProperFilterValues(0x100, 0x525, 0, 0); // the first value for the filter of Inquiry

	// the mask value or the second identifier value depending on the mode
	u32 ID2_MASK = CAN_u32GetProperFilterValues(0x1F0, 0xFFF, 0, 0);

	u32 ID_tx = CAN_u32GetProperFilterValues(0x105, 0x525, 0, 0); // the identifier which this ECU will use to send data to WIFI ECU

	CAN_voidFilterConfiguration(10, CAN_FILTER_MODE_IDENTIFIER_MASK_MODE,
			CAN_FILTER_SCALE_SINGLE_32_BIT, CAN_FILTER_ASSIGNMENT_FIFO_0,
			ID, ID2_MASK, CAN_STATUS_ENABLE);

	CAN_voidInterruptStatus(CAN_EVENT_FIFO_0_PENDING, CAN_STATUS_ENABLE);
	CAN_voidInterruptStatus(CAN_EVENT_FIFO_1_PENDING, CAN_STATUS_ENABLE);

	CAN_voidInit();

	// filters to be configured one for inquiry buffer and another for data buffer  ->>>>>>>>>>>>>>> to be continued 

	/************************************************* NVIC configuration ***************************************/
	MNVIC_voidEnableInterrupt(NVIC_USART1); // not needed when activating the CAN
	MNVIC_voidEnableInterrupt(NVIC_USB_LP_CAN_RX0);
	MNVIC_voidEnableInterrupt(NVIC_CAN_RX1);

	/************************************************* FLASH configuration ***************************************/

	FLASH_Read_CurrentVersion(); // save the current version in the Current_version Array

	/* the following commented block used only for testing
	 * leave it for any further modification
	 */
	//FLASH_voidPageErase(8);
	//FLASH_voidWrite((0x08000000 + 0x400 * 8), data, len);
	//FLASH_voidWrite((0x08000000 + 0x400 * 64-4), data, len);
	//FLASH_voidWrite((0x08000000 + 0x400 * 4 + 0x10), data2, len);
	//FLASH_voidWriteDataOptionByte(FLASH_OPT_BYTE_DATA_1, 0x11);
	//u8 dumb = FLASH_u8ReadDataOptionByte(FLASH_OPT_BYTE_DATA_1);
	/************************************************* USART configuration ***************************************/

	// not needed when activating the can
	MUSART_voidInit();
	MUSART_SetINTStatus(USART_READ_DATA_REG_NOT_EMPTY, USART_ENABLE);

	/************************************************* SYSTICK configuration ***************************************/
	MSYSTICK_voidInit();
	MSYSTICK_voidSetIntervalSingle(6000000, func);

	u8 status = 0;
	while (1)
	{

		// wait for this flag to be raised
		// this happens when a record which is terminated with \n is received
		while (Start_Parsing_Flag == 0);

		// if first record erase BANKx
		if (First_Record_Flag == 0)
		{
			// read option byte to know which is the current active bank
			// 0x11 bank 1
			// 0x22 bank 2
			u8 active_bank = FLASH_u8ReadDataOptionByte(FLASH_OPT_BYTE_DATA_1);

			if (active_bank == 0x11)// if bank 1  is the active bank erase bank 2
			{
				for (u8 i = SecondBankStartPage;
						i < SecondBankStartPage + SecondBankSize; i++)
				{
					FLASH_voidPageErase(i);
				}
			}
			else if (active_bank == 0x22) // if bank 2  is the active bank erase bank 1
			{
				for (u8 i = FirstBankStartPage;
						i < FirstBankStartPage + FirstBankSize; i++)
				{
					FLASH_voidPageErase(i);
				}
			}
			First_Record_Flag = 1; // no erasing anymore
		}

		// check record by using the checksum byte
		status = Check_Record(Recv_Buffer);

		// if record is valid
		// start parsing
		if (status == 0)
		{
			Parse_Record(Recv_Buffer);
		}

		// send ok\n flashing the record
		// need to be substituted with CAN send ->>>>>>>>>>>>>>>>>>>>>> to be continued
		MUSART_voidTransmittString("OK\n");

		// clear the flag to wait for the next one
		Start_Parsing_Flag = 0;

		// if the last record is received no need to wait 6 seconds
		if (finish_flag)
		{
			MSYSTICK_voidPreload(SYSTICK_PRELOAD_IMMEDIATE, 100);
		}
		else
		{
			MSYSTICK_voidSetIntervalSingle(6000000, func);
		}

	}
	return 1;
}
