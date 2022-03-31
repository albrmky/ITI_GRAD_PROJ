#include "../Inc/LIBRARY/bit_math.h"
#include "../Inc/LIBRARY/standard_types.h"

#include "../Inc/MCAL/RCC/RCC_interface.h"
#include "../Inc/MCAL/NVIC/NVIC_interface.h"
#include "../Inc/MCAL/GPIO/GPIO_interface.h"
#include "../Inc/MCAL/SYSTICK/SYSTICK_interface.h"
#include "../Inc/MCAL/USART/USART_interface.h"
#include "../Inc/MCAL/CAN/CAN_interface.h"
//#include "../Inc/HAL/ESP/ESP_interface.h"

// Bad naming
CAN_STRING_Buffer_t CAN_BUFFER;
RX_Struct_t Rx_Header;
CAN_STRING_Buffer_t UART_BUFFER;

u8 Action_Flag = 0;

void CAN_RX1_IRQHandler(void) // for record
{
	// read upcoming data and release the FIFO
	u8 status;
	status = CAN_u8ReceiveStringTest(1, &Rx_Header, &CAN_BUFFER, '\n');

	if (status)
	{
		CAN_BUFFER.Buffer[CAN_BUFFER.counter] = '\0';
		CAN_BUFFER.counter = 0;
		Action_Flag = 1;
	}
}

void USART1_IRQHandler(void)
{
	u8 data = MUSART_u8ReceiveChar();
	UART_BUFFER.Buffer[UART_BUFFER.counter++] = data;
	if (data == '\n')
	{
		UART_BUFFER.Buffer[UART_BUFFER.counter] = '\0';
		UART_BUFFER.counter = 0;

		Action_Flag = 2;
	}
}

int main(void)
{
	/************************************************* RCC configuration ***************************************/
	MRCC_voidInit();
	MRCC_voidSetPeripheralClock(RCC_GPIOC, RCC_STATUS_ON);
	MRCC_voidSetPeripheralClock(RCC_GPIOA, RCC_STATUS_ON);
	MRCC_voidSetPeripheralClock(RCC_USART1, RCC_STATUS_ON);
	MRCC_voidSetPeripheralClock(RCC_CAN, RCC_STATUS_ON);

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

	u32 ID_tx = CAN_u32GetProperFilterValues(0x100, 0x525, 0, 0); // the identifier which this ECU will use to send data to WIFI ECU

	CAN_voidFilterConfiguration(10, CAN_FILTER_MODE_IDENTIFIER_LIST_MODE,
			CAN_FILTER_SCALE_SINGLE_32_BIT, CAN_FILTER_ASSIGNMENT_FIFO_1,
			ID, ID2_MASK, CAN_STATUS_ENABLE);

	CAN_voidInterruptStatus(CAN_EVENT_FIFO_1_PENDING, CAN_STATUS_ENABLE);
	CAN_voidInit();
	/************************************************* NVIC configuration ***************************************/
	MNVIC_voidEnableInterrupt(NVIC_USART1);
	MNVIC_voidEnableInterrupt(NVIC_CAN_RX1);
	/************************************************* USART configuration ***************************************/
	MUSART_voidInit();
	MUSART_SetINTStatus(USART_READ_DATA_REG_NOT_EMPTY, USART_ENABLE);

	/************************************************* ESP configuration ******************/
	// data about server ip and wifi password configured in ESP_configuration.h file
	ESP_voidInit();
	ESP_voidConnectToWifi();
	ESP_voidConnectToSrvTCP();

	while (1)
	{
		while (0 == Action_Flag);
		if (Action_Flag == 1)
		{
			//MUSART_voidTransmittString(CAN_BUFFER.Buffer);
			ESP_voidSend(CAN_BUFFER.Buffer, CAN_BUFFER.counter);
			Action_Flag = 0;
		}
		else if (Action_Flag == 2)
		{
			CAN_TransmitStringTest(0, UART_BUFFER.Buffer, ID_tx);
			Action_Flag = 0;
		}

	}			//while(1)
	return 1;
}			// main
