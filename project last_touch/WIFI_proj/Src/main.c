
#include "../include/LIB/std_types.h"
#include "../include/LIB/bit_math.h"

#include "../include/MCAL/RCC/RCC_interface.h"
#include "../include/MCAL/NVIC/NVIC_interface.h"
#include "../include/MCAL/SCB/SCB_interface.h"
#include "../include/MCAl/GPIO/GPIO_interface.h"
#include "../include/MCAL/SYSTICK/SYSTICK_interface.h"
#include "../include/MCAl/USART/USART_INTERFACE.h"
#include "../include/MCAL/ESP8266/ESP8266_INTERFACE.h"
#include "../include/MCAL/CAN/CAN_interface.h"

#include <string.h>


//flag for CAN indicate frame received from CAN
u32 Global_u32CANFlag=0;

//flag for USART record receive
u8 Global_RecordCompleteflag=0;

u8 Global_StartFound=0;


CAN_STRING_Buffer_t Rx_Buffer;
RX_Struct_t Rx_Header;

void CAN_voidCallBackFunction(void)
{
	// read upcoming data and release the FIFO
	Global_u32CANFlag = CAN_u8ReceiveStringTest(1, &Rx_Header, &Rx_Buffer, '\n');

	if (Global_u32CANFlag)
	{
		Rx_Buffer.Buffer[Rx_Buffer.counter] = '\0';
		Rx_Buffer.counter = 0;
	}
}



int main ( void )
{
	/* Create Variable To Receive Data From Server */
	u8* local_PtrData = NULL;

	/*HSI_CLK=8MHZ , all busses prescaller=1*/
	MRCC_Init();
	/*enable clock for GPIOA*/
	MRCC_SetPeripheralConfiguration(RCC_PERIPHERAL_IOPA,RCC_PERIPHERAL_APB2_BUS,RCC_PERIPHERAL_ENABLE);
	/*enable clock for USART1*/
	MRCC_SetPeripheralConfiguration(RCC_PERIPHERAL_USART1,RCC_PERIPHERAL_APB2_BUS,RCC_PERIPHERAL_ENABLE);
	/*enable clock for CAN1*/
	MRCC_SetPeripheralConfiguration(RCC_PERIPHERAL_CAN,RCC_PERIPHERAL_APB1_BUS,RCC_PERIPHERAL_ENABLE);

	/*adjust system group priority with the pre-configured value.. GP_PRI=16 , SUB_PRI=0*/
	MSCB_voidSetSystemGroupPriority();
	/*set priority for systick timer ID=SCB_VECT_SYSTICK_ID , GP_PRI=1 , SUB_PRI=neglected*/
	MNVIC_voidSetIntPriority(SCB_VECT_SYSTICK_ID,1,1);

	/*enable interrupt for CAN-RX0*/
	MNVIC_voidEnablePeripheral(21);
	/*set priority for CAN ID=20 , GP_PRI=2 , SUB_PRI=neglected*/
	MNVIC_voidSetIntPriority(21,2,1);

	/*enable interrupt for USART*/
	MNVIC_voidEnablePeripheral(37);
	/*set priority for USART1 ID=37 , GP_PRI=3 , SUB_PRI=neglected*/
	MNVIC_voidSetIntPriority(37,3,1);


	/*PA0 output -> LED*/
	MGPIO_voidSetDirection(GPIO_PORTA,GPIO_PIN0,GPIO_OUTPUT_10M_SPEED_MODE,GPIO_OUTPUT_PUSH_PULL);
	/*PA9 output -> TX USART1*/
	MGPIO_voidSetDirection(GPIO_PORTA,GPIO_PIN9,GPIO_OUTPUT_10M_SPEED_MODE,GPIO_AF_OUTPUT_PUSH_PULL);
	/*PA10 input -> RX USART1*/
	MGPIO_voidSetDirection(GPIO_PORTA,GPIO_PIN10,GPIO_INPUT_MODE,GPIO_FLOATING_INPUT);
	/*PA12 output -> TX CAN*/
	MGPIO_voidSetDirection(GPIO_PORTA,GPIO_PIN12,GPIO_OUTPUT_10M_SPEED_MODE,GPIO_AF_OUTPUT_PUSH_PULL);
	/*PA11 input -> RX CAN*/
	MGPIO_voidSetDirection(GPIO_PORTA,GPIO_PIN11,GPIO_INPUT_MODE,GPIO_FLOATING_INPUT);

	/*initialize SYSTICK*/
	MSYSTICK_voidInit();

	/* Initialize UART */
	MUSART_VidInit();

	/************************************************* CAN configuration ***************************************/

	// the value with which the comparison will happen according to the mask
	u32 ID = CAN_u32GetProperFilterValues(0x200, 0x525, 0, 0); // the first value for the filter of Inquiry

	// the mask value or the second identifier value depending on the mode
	u32 ID2_MASK = CAN_u32GetProperFilterValues(0x1F0, 0xFFF, 0, 0);

	u32 ID_tx = CAN_u32GetProperFilterValues(0x100, 0x525, 0, 0); // the identifier which this ECU will use to send data to WIFI ECU

	CAN_voidFilterConfiguration(10, CAN_FILTER_MODE_IDENTIFIER_LIST_MODE,
			CAN_FILTER_SCALE_SINGLE_32_BIT, CAN_FILTER_ASSIGNMENT_FIFO_1,
			ID, ID2_MASK, CAN_STATUS_ENABLE);

	CAN_voidInterruptStatus(CAN_EVENT_FIFO_1_PENDING, CAN_STATUS_ENABLE);

	CAN_voidSetCallBack(CAN_voidCallBackFunction);

	CAN_voidInit();
	/***************************************************************************************************/


	/* Setting ESP8266 Mode */
	ESP8266_VidInit();

	/* Connecting To WIFI Network */
	ESP8266_VidConnectToWiFi( (u8 *)"WEDBA56B" , (u8 *)"l4048422" );


	//connect to server
	ESP8266_VidConnectToSrvTcp( (u8 *)"192.168.1.3" , (u8 *)"5050" );

	//send the data in the local_PtrData by the CAN
	//CAN_TransmitStringTest(0,(u8 *)"Restart\n",ID_tx);

	//MSYSTICK_voidSetBusyWait(2000);

	//ask bootloader to send the bank number
	CAN_TransmitStringTest(0,(u8 *)"Get Bank\n",ID_tx);

	//wait to receive the bank number
	while(Global_u32CANFlag!=1);

	Global_u32CANFlag=0;
	//send bank number to the server
	local_PtrData=(u8 *)ESP8266_PTRu8SocketSendData((u8 *) Rx_Buffer.Buffer ,(u8 *)"5");
	//local_PtrData=(u8 *)ESP8266_PTRu8SocketSendData((u8 *)"0x11",(u8 *)"4");

	//clear buffer
	Rx_Buffer.Buffer[0]=0;
	Rx_Buffer.Buffer[1]=0;

	//MSYSTICK_voidSetBusyWait(10000);

	//receive from can the current version
	//send the current version to server
	//local_PtrData=(u8 *)ESP8266_PTRu8SocketSendData((u8 *) "version=3.2" ,(u8 *)"11");
	//if(local_PtrData[38]=='n' && local_PtrData[39]=='o')
	//{ /*do nothing there is no update*/ }
	//else
	//{
	//	local_PtrData=(u8 *)ESP8266_PTRu8SocketSendData((u8 *) "ok" ,(u8 *)"2");
	//}



	while(1)
	{
		//MSYSTICK_voidSetBusyWait(3000000);


		if(Global_RecordCompleteflag==1 )
		{
			while(local_PtrData[Global_StartFound]!=':')
			{
				Global_StartFound++;
				if(Global_StartFound==40)
				{
					Global_StartFound=37;
					break;
				}
			}
			//send the data in the local_PtrData received from server CAN
			CAN_TransmitStringTest(0,(u8 *)&local_PtrData[Global_StartFound+1],ID_tx);

			//wait to receive ok from bootloader CAN
			while(Rx_Buffer.Buffer[0]!='o' && Rx_Buffer.Buffer[1]!='k');

			//clear buffer
			Rx_Buffer.Buffer[0]=0;
			Rx_Buffer.Buffer[1]=0;

			//reset the flag to wait until a new record is received
			Global_RecordCompleteflag=0;
			//send ok to server
			local_PtrData=(u8 *)ESP8266_PTRu8SocketSendData((u8 *) "ok" ,(u8 *)"2");
		}





	}

	return 0;

}
