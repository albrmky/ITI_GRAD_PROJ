#include "../Inc/LIBRARY/bit_math.h"
#include "../Inc/LIBRARY/standard_types.h"

#include "../Inc/MCAL/RCC/RCC_interface.h"
#include "../Inc/MCAL/GPIO/GPIO_interface.h"
#include "../Inc/MCAL/SYSTICK/SYSTICK_interface.h"
#include "../Inc/MCAL/NVIC/NVIC_interface.h"

#include "../Inc/MCAL/CAN/CAN_interface.h"

u8 Tx_Data[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
u8 Rx_Data[8];

RX_Struct_t RX_Header;

void USB_LP_CAN_RX0_IRQHandler(void)
{
	CAN_voidReceiveMailBox(0, &RX_Header, Rx_Data);
	asm("NOP");
}

void CAN_RX1_IRQHandler(void)
{
	CAN_voidReceiveMailBox(1, &RX_Header, Rx_Data);
	asm("NOP");
}

int main(void)
{
	/*
	 * 1- enable clock for the can peripheral and the port(A)
	 * 2- configure the pins
	 * 3- enable the NVIC vectors corresponding to the value
	 * 4- configure the filters to be used
	 * 5- enable the needed interrupt
	 * 6- transmit the data
	 */

	MRCC_voidInit();
	MRCC_voidSetPeripheralClock(RCC_GPIOA, RCC_STATUS_ON);
	MRCC_voidSetPeripheralClock(RCC_CAN, RCC_STATUS_ON);

	MGPIO_voidSetPinDirection(GPIO_PORTA, 11, GPIO_MODE_INPUT,
			GPIO_INPUT_CNFG_FLOATING); // CAN RX A11
	MGPIO_voidSetPinDirection(GPIO_PORTA, 12, GPIO_MODE_OUTPUT_2_MHZ,
			GPIO_OUTPUT_CNFG_ALT_PP); // CAN TX A12

//	NVIC_CAN_RX1
//	NVIC_USB_LP_CAN_RX0
	MNVIC_voidEnableInterrupt(NVIC_USB_LP_CAN_RX0);
	MNVIC_voidEnableInterrupt(NVIC_CAN_RX1);

	CAN_voidInit();

	// the value with which the comparison will happen according to the mask
	u32 ID = CAN_u32GetProperFilterValues(0x100, 0x525, 0, 0);

	// the mask value
	u32 ID2_MASK = CAN_u32GetProperFilterValues(0x1F0, 0xFFF, 0, 0);

	u32 ID_tx = CAN_u32GetProperFilterValues(0x105, 0x525, 0, 0);

	CAN_voidFilterConfiguration(10, CAN_FILTER_MODE_IDENTIFIER_MASK_MODE,
			CAN_FILTER_SCALE_SINGLE_32_BIT, CAN_FILTER_ASSIGNMENT_FIFO_0,
			ID, ID2_MASK, CAN_STATUS_ENABLE);

	CAN_voidInterruptStatus(CAN_EVENT_FIFO_0_PENDING, CAN_STATUS_ENABLE);
	//CAN_voidInterruptStatus(CAN_EVENT_FIFO_1_PENDING, CAN_STATUS_ENABLE);

	CAN_TransmitMailBox(0, 8, Tx_Data, ID_tx, CAN_ID_STANDARD, CAN_DATA_FRAME);

	while (1)
	{

	}
	return 1;
}
