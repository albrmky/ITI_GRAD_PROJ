#include "../Inc/LIBRARY/bit_math.h"
#include "../Inc/LIBRARY/standard_types.h"

#include "../Inc/MCAL/RCC/RCC_interface.h"
#include "../Inc/MCAL/GPIO/GPIO_interface.h"
#include "../Inc/MCAL/NVIC/NVIC_interface.h"
#include "../Inc/MCAL/CAN/CAN_interface.h"
#include <string.h>

#define SCB_AIRCR *((volatile u32*)0xE000ED0C)
#define SCB_VTOR  *((volatile u32*)0xE000ED08)

CAN_STRING_Buffer_t Rx_Buffer;
RX_Struct_t Rx_Header;
u8 restart_flag;

void CAN_RX1_IRQHandler(void) // for record
{
	// read upcoming data and release the FIFO
	u8 status;
	status = CAN_u8ReceiveStringTest(1, &Rx_Header, &Rx_Buffer, '\n');

	if (status)
	{

		Rx_Buffer.counter = 0;

		if (0 == strcmp("Restart\n", Rx_Buffer.Buffer))
		{
			restart_flag = 1;

		}

	}
}

int main(void)
{

	//SCB_VTOR = 0x8003804;
	/************************************************* RCC configuration ***************************************/
	MRCC_voidInit();
	MRCC_voidSetPeripheralClock(RCC_GPIOC, RCC_STATUS_ON);
	MRCC_voidSetPeripheralClock(RCC_GPIOA, RCC_STATUS_ON);
	MRCC_voidSetPeripheralClock(RCC_CAN, RCC_STATUS_ON);

	/************************************************* GPIO configuration ***************************************/
	MGPIO_voidSetPinDirection(GPIO_PORTC, 13, GPIO_MODE_OUTPUT_10_MHZ,
			GPIO_OUTPUT_CNFG_GP_PP); //LED

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
	MNVIC_voidEnableInterrupt(NVIC_CAN_RX1);

	/**************************************************Main program*************************/

	/* Loop forever */
	while (1)
	{
		if (restart_flag == 1)
		{
			SCB_AIRCR = 0x05FA0004; // request a system reset
		}

		MGPIO_voidSetPinValue(GPIO_PORTC, 13, GPIO_HIGH);
		for (int i = 0; i < 400000; i++);

		MGPIO_voidSetPinValue(GPIO_PORTC, 13, GPIO_LOW);
		for (int i = 0; i < 400000; i++);

	}
	return 1;
}
