#include "../Inc/LIBRARY/bit_math.h"
#include "../Inc/LIBRARY/standard_types.h"

#include "../Inc/MCAL/FPEC/FPEC_interface.h"
#include "../Inc/MCAL/RCC/RCC_interface.h"
#include "../Inc/MCAL/NVIC/NVIC_interface.h"
#include "../Inc/MCAL/GPIO/GPIO_interface.h"

#define len 16
u16 data[len] =
		{ 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65 };
u16 data2[len] =
		{ 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48 };

void FLASH_IRQHandler(void)
{
	asm("NOP");
}

int main(void)
{
	// must be HSI
	// prescaler for AHB must be 1
	MRCC_voidInit();

	MRCC_voidSetPeripheralClock(RCC_GPIOC, RCC_STATUS_ON);
	MRCC_voidSetPeripheralClock(RCC_FLITF, RCC_STATUS_ON);

	MGPIO_voidSetPinDirection(GPIO_PORTC, 13, GPIO_MODE_OUTPUT_10_MHZ,
			GPIO_OUTPUT_CNFG_GP_PP);

	// flash global interrupt
	MNVIC_voidEnableInterrupt(NVIC_FLASH);

	// enable the interrupt when page error happen
	//FLASH_voidINTSTATUS(FLASH_EVENT_ERROR, FLASH_ENABLE);

	FLASH_voidPageErase(4);
	//FLASH_voidPageErase(5);
	FLASH_voidPageProgram((0x08000000 + 0x400 * 4), data, len);
	FLASH_voidPageProgram((0x08000000 + 0x400 * 4 + 0x10), data2, len);

	FLASH_voidWriteDataOptionByte(FLASH_OPT_BYTE_DATA_1, 0xDD);

	// still not working
	u8 dumb = FLASH_u8ReadDataOptionByte(FLASH_OPT_BYTE_DATA_1);

	while (1)
	{
		MGPIO_voidSetPinValue(GPIO_PORTC, 13, GPIO_HIGH);
		for (int i = 0; i < 1000000; i++);
		MGPIO_voidSetPinValue(GPIO_PORTC, 13, GPIO_LOW);
		for (int i = 0; i < 1000000; i++);

	}
	return 1;
}
