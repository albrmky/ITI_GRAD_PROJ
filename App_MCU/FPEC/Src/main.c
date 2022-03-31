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
#include <String.h>

//UART_MODE
//CAN_MODE
#define UART_MODE

#define SCB_AIRCR *((volatile u32*)0xE000ED0C)

typedef void (*Function_t)(void); // pointer to function who take void and return nothing
Function_t addr_to_call = 0; // an instance of that data type

CAN_STRING_Buffer_t Rx_Buffer; // receive any incoming data wither from CAN or UART
RX_Struct_t Rx_Header; // related field with data received by the CAN frame

// flags for parsing the record
u8 Start_Parsing_Flag; // wait for '\n' -> parse -> wait again
u8 First_Record_Flag; // to clean the target bank before writing the flash
extern u8 finish_flag; // indicate that flashing is ended ant make the code starts running immediately

/* change those according to the code */
u8 FirstBankStartPage = 14;
u8 FirstBankSize = 25;
u8 SecondBankStartPage = 39;
u8 SecondBankSize = 25;

/* store the storage */
char Current_version[10];
char New_version[10];

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

#ifdef CAN_MODE
void CAN_RX1_IRQHandler(void) // for record
{
	// read upcoming data and release the FIFO
	u8 status;
	status = CAN_u8ReceiveStringTest(1, &Rx_Header, &Rx_Buffer, '\n');

	if (status)
	{
		Start_Parsing_Flag = 1;
		asm("NOP");
		Rx_Buffer.counter = 0;
	}
}

#endif

#ifdef UART_MODE
void USART1_IRQHandler(void)
{
	u8 data = MUSART_u8ReceiveChar();
	Rx_Buffer.Buffer[Rx_Buffer.counter++] = data;
	if (data == '\n')
	{
		Start_Parsing_Flag = 1;
		Rx_Buffer.counter = 0;
	}
}
#endif

int main(void)
{

	/************************************************* RCC configuration ***************************************/
	// must be HSI
	// prescaler for AHB must be 1
	MRCC_voidInit();
	MRCC_voidSetPeripheralClock(RCC_GPIOC, RCC_STATUS_ON);
	MRCC_voidSetPeripheralClock(RCC_GPIOA, RCC_STATUS_ON);
	MRCC_voidSetPeripheralClock(RCC_FLITF, RCC_STATUS_ON);

#ifdef UART_MODE
	MRCC_voidSetPeripheralClock(RCC_USART1, RCC_STATUS_ON); // for testing
#endif

#ifdef CAN_MOD
	MRCC_voidSetPeripheralClock(RCC_CAN, RCC_STATUS_ON);
#endif
	/************************************************* GPIO configuration ***************************************/
	MGPIO_voidSetPinDirection(GPIO_PORTC, 13, GPIO_MODE_OUTPUT_10_MHZ,
			GPIO_OUTPUT_CNFG_GP_PP); // LED

#ifdef UART_MODE
	MGPIO_voidSetPinDirection(GPIO_PORTA, 9, GPIO_MODE_OUTPUT_10_MHZ,
			GPIO_OUTPUT_CNFG_ALT_PP); // TX
	MGPIO_voidSetPinDirection(GPIO_PORTA, 10, GPIO_MODE_INPUT,
			GPIO_INPUT_CNFG_FLOATING); // RX
	MRCC_voidSetPeripheralClock(RCC_CAN, RCC_STATUS_ON);
#endif

#ifdef CAN_MODE
	MGPIO_voidSetPinDirection(GPIO_PORTA, 11, GPIO_MODE_INPUT,
			GPIO_INPUT_CNFG_FLOATING); // CAN RX A11
	MGPIO_voidSetPinDirection(GPIO_PORTA, 12, GPIO_MODE_OUTPUT_2_MHZ,
			GPIO_OUTPUT_CNFG_ALT_PP); // CAN TX A12
#endif

#ifdef CAN_MODE
	/************************************************* CAN configuration ***************************************/

	// the value with which the comparison will happen according to the mask
	u32 ID = CAN_u32GetProperFilterValues(0x100, 0x525, 0, 0); // the first value for the filter of Inquiry

	// the mask value or the second identifier value depending on the mode
	u32 ID2_MASK = CAN_u32GetProperFilterValues(0x1F0, 0xFFF, 0, 0);

	u32 ID_tx = CAN_u32GetProperFilterValues(0x100, 0x525, 0, 0); // the identifier which this ECU will use to send data to WIFI ECU

	CAN_voidFilterConfiguration(10, CAN_FILTER_MODE_IDENTIFIER_LIST_MODE,
			CAN_FILTER_SCALE_SINGLE_32_BIT, CAN_FILTER_ASSIGNMENT_FIFO_1,
			ID, ID2_MASK, CAN_STATUS_ENABLE);

	CAN_voidInterruptStatus(CAN_EVENT_FIFO_1_PENDING, CAN_STATUS_ENABLE);

	CAN_voidInit();
#endif
	/************************************************* NVIC configuration ***************************************/

#ifdef UART_MODE
	MNVIC_voidEnableInterrupt(NVIC_USART1); // no need when activating the CAN
#endif

#ifdef CAN_MODE
	MNVIC_voidEnableInterrupt(NVIC_CAN_RX1);
#endif

	/************************************************* FLASH configuration ***************************************/
	u8 dumb = FLASH_u8ReadDataOptionByte(FLASH_OPT_BYTE_DATA_1); // get the current active bank
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
#ifdef UART_MODE
	// not needed when activating the can
	MUSART_voidInit();
	MUSART_SetINTStatus(USART_READ_DATA_REG_NOT_EMPTY, USART_ENABLE);
#endif
	/************************************************* SYSTICK configuration ***************************************/

	MSYSTICK_voidInit();
	MSYSTICK_voidSetIntervalSingle(5000000, func);

	u8 status = 0; // for verify the received record

	while (1)
	{

		// wait for this flag to be raised
		// this happens when a record which is terminated with \n is received
		while (Start_Parsing_Flag == 0);

		if (Rx_Buffer.Buffer[0] == ':')
		{

			// if first record erase BANKx
			if (First_Record_Flag == 0)
			{
				// read option byte to know which is the current active bank
				// 0x11 bank 1
				// 0x22 bank 2
				u8 active_bank = FLASH_u8ReadDataOptionByte(
						FLASH_OPT_BYTE_DATA_1);

				if (active_bank == 0x11)// if bank1 is the active bank ,erase bank2
				{
					for (u8 i = SecondBankStartPage;
							i < SecondBankStartPage + SecondBankSize; i++)
					{
						FLASH_voidPageErase(i);
					}
				}
				else if (active_bank == 0x22) // if bank2 is the active bank ,erase bank1
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
			status = Check_Record(Rx_Buffer.Buffer);

			// if record is valid
			// start parsing
			if (status == 0)
			{
				Parse_Record(Rx_Buffer.Buffer);
			}

			// send ok\n
#ifdef UART_MODE
			MUSART_voidTransmittString("OK\n");
#endif

#ifdef CAN_MODE
			CAN_TransmitStringTest(0, "OK\n", ID_tx);
#endif

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

		else if (0 == strcmp("Get Bank\n", (char*) Rx_Buffer.Buffer))
		{

			if (dumb == 0x11)
			{
#ifdef UART_MODE
				MUSART_voidTransmittString("0x11\n");
#endif

#ifdef CAN_MODE
				CAN_TransmitStringTest(0, "0x11\n", ID_tx);
#endif
			}

			else if (dumb == 0x33) // don't ask why
			{
				asm("nop");
			}
			else if (dumb == 0x22)
			{
#ifdef UART_MODE
				MUSART_voidTransmittString("0x22\n");
#endif

#ifdef CAN_MODE
				CAN_TransmitStringTest(0, "0x22\n", ID_tx);
#endif
			}
		}
		else if (0 == strcmp("Get Version\n", (char*) Rx_Buffer.Buffer))
		{
#ifdef CAN_MODE
			CAN_TransmitStringTest(0, Current_version, ID_tx); // '\n' are implicitly included
#endif
#ifdef UART_MODE
			MUSART_voidTransmittString(Current_version); // '\n' are implicitly included
#endif

		}
		else if (0 == strcmp("Restart\n", (char*) Rx_Buffer.Buffer))
		{
			SCB_AIRCR = 0x05FA0004; // request a system reset
		}
		else if (0 == strncmp("Version = ", (char*) Rx_Buffer.Buffer, 10))
		{
			u8 *dumb = Rx_Buffer.Buffer + 10;
			u8 new_counter = 0;
			while (*dumb != '\n')
			{
				New_version[new_counter++] = *dumb++;
			}
			New_version[new_counter++] = '\n';
			New_version[new_counter++] = '\0';
		}

		// clear the flag to wait for the next record or inquiry
		Start_Parsing_Flag = 0;

	}			//while(1)
	return 1;
}			// main
