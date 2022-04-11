
#include "../../include/LIB/std_types.h"
#include "../../include/LIB/bit_math.h"

#include "../../include/MCAL/USART/USART_INTERFACE.h"
#include "../../include/MCAL/ESP8266/ESP8266_INTERFACE.h"
#include "../../include/MCAL/SYSTICK/SYSTICK_interface.h"

u8 volatile Iterator = 0  ;
u8 volatile DataCome[100] ;


void MUSART_CallBack ( void ){

	/* Receive ESP8266 Response */
	DataCome[ Iterator ] = MUSART1_u8ReadDataRegister();
	/* ------------------ */
	Iterator++;
}

void ESP8266_VidInit ( void ){

	/* Set USART1 CallBack To Receive The Response Of The ESP8266 */
	MUSART1_VidSetCallBack( MUSART_CallBack );

	/* Sending AT Command To Check ESP8266 Is Working Or Not  */
	MUSART1_VidSendStringSynch( (u8 *)"AT\r\n" );

	MSYSTICK_voidDelay_ms( 3000 );

	/* Clear ESP8266 Buffer */
	ESP8266_VidClearBuffer();

	MUSART1_VidSendStringSynch( (u8 *)"AT+CWMODE=3\r\n" );
	MSYSTICK_voidDelay_ms( 1000 );

	ESP8266_VidClearBuffer();

	MUSART1_VidSendStringSynch( (u8 *)"AT+CIPMODE=0\r\n" );
	MSYSTICK_voidDelay_ms( 1000 );

}

void ESP8266_VidConnectToWiFi ( u8 * SSID , u8 * Password ){

	ESP8266_VidClearBuffer();

	MUSART1_VidSendStringSynch( (u8 *) "AT+CWJAP=\"" );
	MUSART1_VidSendStringSynch( (u8 *) SSID );
	MUSART1_VidSendStringSynch( (u8 *) "\",\"" );
	MUSART1_VidSendStringSynch( (u8 *) Password);
	MUSART1_VidSendStringSynch( (u8 *) "\"\r\n");
	MSYSTICK_voidDelay_ms( 6000 );

}


void ESP8266_VidConnectToSrvTcp ( u8 * Copy_u8Domain , u8 * Copy_u8Port ){

	ESP8266_VidClearBuffer();

	MUSART1_VidSendStringSynch( (u8 *) "AT+CIPSTART=\"TCP\",\"" );
	MUSART1_VidSendStringSynch( (u8 *) Copy_u8Domain );
	MUSART1_VidSendStringSynch( (u8 *) "\"," );
	MUSART1_VidSendStringSynch( (u8 *) Copy_u8Port );
	MUSART1_VidSendStringSynch( (u8 *) "\r\n" );
	MSYSTICK_voidDelay_ms( 1000 );

}

void ESP8266_VidSendHttpReq( u8 * Copy_u8Key , u8 * Copy_u8Data , u8 * Copy_u8Length ){

	ESP8266_VidClearBuffer();

	MUSART1_VidSendStringSynch( (u8 *) "AT+CIPSEND=" );
	MUSART1_VidSendStringSynch( (u8 *) Copy_u8Length );
	MUSART1_VidSendStringSynch( (u8 *) "\r\n" );
	MSYSTICK_voidDelay_ms( 4000 );

	ESP8266_VidClearBuffer();

	MUSART1_VidSendStringSynch( (u8 *) "GET /update?api_key=" );
	MUSART1_VidSendStringSynch( (u8 *) Copy_u8Key );
	MUSART1_VidSendStringSynch( (u8 *) "&field1=" );
	MUSART1_VidSendStringSynch( (u8 *) Copy_u8Data );
	MUSART1_VidSendStringSynch( (u8 *) "\r\n" );
	MSYSTICK_voidDelay_ms( 20000 );

}

u8   ESP8266_u8ReceiveHttpReq( u8 * Copy_u8ChannelID , u8 * Copy_u8Length ){

	ESP8266_VidClearBuffer();

	MUSART1_VidSendStringSynch( (u8 *) "AT+CIPSEND=" );
	MUSART1_VidSendStringSynch( (u8 *) Copy_u8Length );
	MUSART1_VidSendStringSynch( (u8 *) "\r\n" );
	MSYSTICK_voidDelay_ms( 1000 );

	ESP8266_VidClearBuffer();

	MUSART1_VidSendStringSynch( (u8 *) "GET http://" );
	MUSART1_VidSendStringSynch( (u8 *) Copy_u8ChannelID );
	//MUSART1_VidSendStringSynch( (u8 *) "/var.txt\r\n" );
	MUSART1_VidSendStringSynch( (u8 *) "\r\n" );
	MSYSTICK_voidDelay_ms( 2000 );

	//ESP8266_VidConnectToSrvTcp( (u8 *)"162.253.155.226" , (u8 *)"80" );
	//ESP8266_VidConnectToSrvTcp( (u8 *)"35.173.69.207" , (u8 *)"80" );

	return  DataCome[85] ;

}

u8* ESP8266_PTRu8SocketSendData(u8 * copy_u8DataToSend,u8 * Copy_u8Length)
{

	ESP8266_VidClearBuffer();

	MUSART1_VidSendStringSynch( (u8 *) "AT+CIPSEND=" );
	MUSART1_VidSendStringSynch( (u8 *) Copy_u8Length );
	MUSART1_VidSendStringSynch( (u8 *) "\r\n" );
	MSYSTICK_voidDelay_ms( 100 );
	ESP8266_VidClearBuffer();

	MUSART1_VidSendStringSynch( (u8 *) copy_u8DataToSend);
	MSYSTICK_voidDelay_ms( 200 );

	//connect to server
	//ESP8266_VidConnectToSrvTcp( (u8 *)"192.168.1.3" , (u8 *)"5050" );

	return DataCome;
}


void ESP8266_VidClearBuffer ( void ){

	u8 LOC_u8Iterator1 = 0 ;
	Iterator     = 0 ;

	for( LOC_u8Iterator1 = 0 ; LOC_u8Iterator1 < 100 ; LOC_u8Iterator1++ ){

		DataCome[ LOC_u8Iterator1 ] = 0 ;

	}

}
