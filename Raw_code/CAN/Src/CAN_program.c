/*
 * CAN_program.c
 *
 *  Created on: Mar 12, 2022
 *      Author: noureldeen albrmky
 */

#include "../Inc/LIBRARY/bit_math.h"
#include "../Inc/LIBRARY/standard_types.h"

#include "../Inc/MCAL/CAN/CAN_interface.h"
#include "../Inc/MCAL/CAN/CAN_private.h"
#include "../Inc/MCAL/CAN/CAN_configuration.h"

/****************************************************
 --> the initialization of filter mode and scale must be done in initialization mode
 but
 the filter values can be done in any mode but must be deactivated first
 --> the values for filters is recommended to be done by CAN_u32GetProperFilterValues() to avoid errors
 **************************************************/
void CAN_voidFilterConfiguration(u8 Filter_Bank_number,
									FILTER_MODE_t Filter_Mode,
									FILTER_SCALE_REGISTER_t Filter_Scale,
									FILTER_ASSIGNMENT_t Filter_Assignment,
									u32 Identifier_1, u32 Mask_Identifier_2,
									CAN_STATUS_t Activtion_status)
{

	// enter filter initialization mode by setting FINIT bit
	SET_BIT(CAN1->CAN_FMR, FINIT_BIT);

	// set the mode of the filter bank
	// 0: Mask Mode 1: List Mode
	INS_BIT(CAN1->CAN_FM1R, Filter_Bank_number, Filter_Mode);

	// set the Scale of the filter bank
	// 0: Dual 16-bit 1: Single 32-bit
	INS_BIT(CAN1->CAN_FS1R, Filter_Bank_number, Filter_Scale);

	// set the FIFO
	// 0: FIFO_0 1: FIFO_1
	INS_BIT(CAN1->CAN_FFA1R, Filter_Bank_number, Filter_Assignment);

	// assign identifiers Value
	CAN1->FILTER_BANK[Filter_Bank_number].FxR1 = Identifier_1;
	CAN1->FILTER_BANK[Filter_Bank_number].FxR2 = Mask_Identifier_2;

	// Activation Status
	INS_BIT(CAN1->CAN_FA1R, Filter_Bank_number, Activtion_status);

	// enter Active Filter Mode mode by Clearing FINIT bit
	CLR_BIT(CAN1->CAN_FMR, FINIT_BIT);
}

/*****************************************************
 take the values of filters and format them in the proper form
 used with 32-bit scale only
 ***************************************************/

u32 CAN_u32GetProperFilterValues(u16 std_id_part, u32 extended_id_part, u8 IDE_value, u8 RTR_value)
{
	// byte 1              byte 2              byte 3          byte 4
	// stdid[10:3]  stdid[2:0] extid[17:13]  extid[12:5]   extid[4:0] IDE RTR  0

	u32 local = std_id_part << 21 | extended_id_part << 3 | IDE_value << 2
			| RTR_value << 1;
	return local;
}

/******************************************************************
 this function is used for setting the proper values for Master control register
 and to set the bit timing
 --> if operating mode is not set to Normal at the end of this function use it explicitly
 *****************************************************************/

void CAN_voidInit(void)
{
	// concatenate the values in the configuration file
	// corresponding to the MCR_register "CAN master Control"
	u32 value = CONC_MCR(CAN_TIME_TRIGGERED_COMMUNICATION_MODE,
			CAN_BUS_OFF_MANAGMENT,
			CAN_WAKE_UP_MANAGMENT,
			CAN_NO_RETRANSMISSION,
			CAN_RECEIVE_FIFO_LOCKED_MODE,
			CAN_TRANSMIT_FIFO_PRIORITY);

	CAN1->CAN_MCR = (CAN_WORKING_IN_DEBUG << 16) | (value << 2);

	// enter initialization mode to be able to set the bit timings
	CAN_voidOperatingMode(CAN_OPERATING_MODE_INITIALIZATION);

	// set the bit timing
	CAN_voidSetTiming();

	// test mode if used
	INS_BIT(CAN1->CAN_BTR, LBKM_BIT, CAN_LOOPBACK_MODE);
	INS_BIT(CAN1->CAN_BTR, SILM_BIT, CAN_SILENT_MODE);

	CAN_voidOperatingMode(CAN_OPERATING_MODE_NORMAL);

}

/********************************************************
 * Choose the operating mode for the can
 * then wait for the hardware to ack
 *******************************************************/
void CAN_voidOperatingMode(CAN_OPERATING_MODE_t Operating_Mode)
{
	switch (Operating_Mode)
	{
		case CAN_OPERATING_MODE_NORMAL:
			CLR_BIT(CAN1->CAN_MCR, INRQ_BIT);
			CLR_BIT(CAN1->CAN_MCR, SLEEP_BIT);

			// wait for hardware to ack
			while (GET_BIT(CAN1->CAN_MSR, INRQ_BIT)); // wait till zero
			while (GET_BIT(CAN1->CAN_MSR, SLEEP_BIT)); // wait till zero
			break;

		case CAN_OPERATING_MODE_INITIALIZATION:
			// the status of the CAN bus output CANTX is recessive (high).
			SET_BIT(CAN1->CAN_MCR, INRQ_BIT);
			CLR_BIT(CAN1->CAN_MCR, SLEEP_BIT);

			// wait for hardware to ack
			while (GET_BIT(CAN1->CAN_MSR,INRQ_BIT) == 0); // wait till one
			while (GET_BIT(CAN1->CAN_MSR, SLEEP_BIT)); // wait till zero
			break;

		case CAN_OPERATING_MODE_SLEEP:
			CLR_BIT(CAN1->CAN_MCR, INRQ_BIT);
			SET_BIT(CAN1->CAN_MCR, SLEEP_BIT);

			// wait for hardware to ack
			while (GET_BIT(CAN1->CAN_MSR, INRQ_BIT)); // wait till zero
			while (GET_BIT(CAN1->CAN_MSR,SLEEP_BIT) == 0); // wait till one
			break;
	}
}

static void CAN_voidSetTiming(void)
{
	// set the value of the baudrate which is used to determine
	// the time quanta
	// field start at bit 0 in CAN_BTR register
	INS_FIELD(CAN1->CAN_BTR, 0x3FF, 0, CAN_BAUD_RATE_PRESCALER);

	// number of time segment 1 which is a multiple of time quanta
	// field start at bit 16 in CAN_BTR register
	INS_FIELD(CAN1->CAN_BTR, 0xF, 16, CAN_TIME_SIGMENT_1);

	// number of time segment 2 which is a multiple of time quanta
	// field start at bit 20 in CAN_BTR register
	INS_FIELD(CAN1->CAN_BTR, 0x7, 20, CAN_TIME_SIGMENT_2);

	// maximum number of time quanta the CAN hardware is allowed to
	// lengthen or shorten a bit to perform the resynchronization.
	// field start at bit 24 in CAN_BTR register
	INS_FIELD(CAN1->CAN_BTR, 0x3, 24, CAN_RESYNC_JUMP_WIDTH);
}

/*****************************************
 Activate the required event interrupt
 *****************************************/
void CAN_voidInterruptStatus(CAN_INERRUPT_EVENTS_t Event, CAN_STATUS_t Status)
{
	INS_BIT(CAN1->CAN_IER, Event, Status);
}

u8 CAN_u8ReadErrorFlag(CAN_ERROR_FLAGS_t Flag)
{
	u8 local = GET_BIT(CAN1->CAN_ESR, Flag);
	return local;
}

/*******************************
 * Read LEC "Last error code"
 000: No Error
 001: Stuff Error
 010: Form Error
 011: Acknowledgment Error
 100: Bit recessive Error
 101: Bit dominant Error
 110: CRC Error
 111: Set by software
 *****************************/
u8 CAN_u8GetErrorCode(void)
{
	u8 value = (CAN1->CAN_ESR >> 4) & 0x7;
	return value;
}

/*******************************
 * Read Rec
 *****************************/
u8 CAN_u8GetReceiveErrorCounter(void)
{
	u8 value = (CAN1->CAN_ESR >> 24) & 0xFF;
	return value;
}

/*******************************
 * Read Tec
 *****************************/
u8 CAN_u8GetTransmitErrorCounter(void)
{
	u8 value = (CAN1->CAN_ESR >> 16) & 0xFF;
	return value;
}

/****************************************************
 1- assign the headers of the mailbox
 2- pass the data to be sent
 3- request transmitting
 ***************************************************/
void CAN_TransmitMailBox(u8 MailBoxNum, u8 Data_length, u8 *Data,
							u32 identifier, u8 identifier_type,
							u8 frame_type
							)
{
	//							register content
//	-------------------------------------------------------------------------------
	// byte 1              byte 2              byte 3          byte 4
	// stdid[10:3]  stdid[2:0] extid[17:13]  extid[12:5]   extid[4:0] IDE RTR  TXRQ
//  -------------------------------------------------------------------------------

	// wait for the required transmitting mail box to be empty
	while ( GET_BIT(CAN1->CAN_TSR,(26+MailBoxNum) ) == 0);

	// must use the identifier format function to avoid any errors
	CAN1->TX_MAILBOX[MailBoxNum].CAN_TIR = identifier;

	// length of the data to be sent
	INS_FIELD(CAN1->TX_MAILBOX[MailBoxNum].CAN_TDTR, 0xF, 0, Data_length);

	// lower 4 bytes of data
	CAN1->TX_MAILBOX[MailBoxNum].CAN_TDLR = Data[3] << 24 | Data[2] << 16
			| Data[1] << 8 | Data[0];

	// higher 4 bytes of data
	CAN1->TX_MAILBOX[MailBoxNum].CAN_TDHR = Data[7] << 24 | Data[6] << 16
			| Data[5] << 8 | Data[4];

	// request transmitting
	SET_BIT(CAN1->TX_MAILBOX[MailBoxNum].CAN_TIR, 0);
}

/********************************************************************
 Read the received data from one of the FIFO
 --> it is important to release the mailbox after reading it

 --> need to pass a pointer to the Rx_header struct to receive related data
 --> pass the pointer to the buffer
 *******************************************************************/
void CAN_voidReceiveMailBox(u8 FIFO_num, RX_Struct_t *RX_Header, u8 *Rx_Data)
{

	// get the length of the incoming data
	RX_Header->Data_length = CAN1->RX_MAILBOX[FIFO_num].CAN_RDTR & 0xF;

	// get the identifier type
	RX_Header->Identifier_type = GET_BIT(CAN1->RX_MAILBOX[FIFO_num].CAN_RIR, 2);

	u32 local_extarcted_std_id = (CAN1->RX_MAILBOX[FIFO_num].CAN_RIR >> 21)
			& 0x7FF;	// needs only 11-bit
	u32 local_extarcted_extended_id = (CAN1->RX_MAILBOX[FIFO_num].CAN_RIR >> 3)
			& 0x3FFFF;	// needs only 18-bit

	if (RX_Header->Identifier_type) //standard
	{
		RX_Header->Identifier = local_extarcted_std_id;
	}
	else // extended
	{
		RX_Header->Identifier = (local_extarcted_std_id << 18)
				| local_extarcted_extended_id;
	}

	// index of the filter data passed through
	RX_Header->Filter_Index = (CAN1->RX_MAILBOX[FIFO_num].CAN_RDTR) >> 8 & 0xFF;

	// get the frame type
	RX_Header->Frame_type = GET_BIT(CAN1->RX_MAILBOX[FIFO_num].CAN_RIR, 1);

	Rx_Data[0] = CAN1->RX_MAILBOX[FIFO_num].CAN_RDLR >> 0;
	Rx_Data[1] = CAN1->RX_MAILBOX[FIFO_num].CAN_RDLR >> 8;
	Rx_Data[2] = CAN1->RX_MAILBOX[FIFO_num].CAN_RDLR >> 16;
	Rx_Data[3] = CAN1->RX_MAILBOX[FIFO_num].CAN_RDLR >> 24;

	Rx_Data[4] = CAN1->RX_MAILBOX[FIFO_num].CAN_RDHR >> 0;
	Rx_Data[5] = CAN1->RX_MAILBOX[FIFO_num].CAN_RDHR >> 8;
	Rx_Data[6] = CAN1->RX_MAILBOX[FIFO_num].CAN_RDHR >> 16;
	Rx_Data[7] = CAN1->RX_MAILBOX[FIFO_num].CAN_RDHR >> 24;

	if (FIFO_num)
	{
		SET_BIT(CAN1->CAN_RF1R, 5);
	}
	else
	{
		SET_BIT(CAN1->CAN_RF0R, 5);
	}

}

