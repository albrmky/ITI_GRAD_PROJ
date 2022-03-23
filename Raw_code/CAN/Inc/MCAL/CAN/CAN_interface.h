/*
 * CAN_interface.h
 *
 *  Created on: Mar 12, 2022
 *      Author: noureldeen albrmky
 */

#ifndef MCAL_CAN_CAN_INTERFACE_H_
#define MCAL_CAN_CAN_INTERFACE_H_

typedef enum {
	CAN_FILTER_MODE_IDENTIFIER_MASK_MODE,
	CAN_FILTER_MODE_IDENTIFIER_LIST_MODE
} FILTER_MODE_t;

typedef enum {
	CAN_FILTER_SCALE_DUAL_16_BIT,
	CAN_FILTER_SCALE_SINGLE_32_BIT
} FILTER_SCALE_REGISTER_t;

typedef enum {
	CAN_FILTER_ASSIGNMENT_FIFO_0,
	CAN_FILTER_ASSIGNMENT_FIFO_1
} FILTER_ASSIGNMENT_t;

typedef enum {
	CAN_STATUS_DISABLE,
	CAN_STATUS_ENABLE
} CAN_STATUS_t;

typedef enum
{
	CAN_OPERATING_MODE_NORMAL,
	CAN_OPERATING_MODE_INITIALIZATION,
	CAN_OPERATING_MODE_SLEEP
} CAN_OPERATING_MODE_t;

typedef enum
{
	CAN_EVENT_TRANSMIT_MB,
	CAN_EVENT_FIFO_0_PENDING, // if there is at least one message received
	CAN_EVENT_FIFO_0_FULL,
	CAN_EVENT_FIFO_0_OVERRUN,
	CAN_EVENT_FIFO_1_PENDING, // if there is at least one message received
	CAN_EVENT_FIFO_1_FULL,
	CAN_EVENT_FIFO_1_OVERRUN,
	CAN_EVENT_WARNING = 8, //REC or TEC >=96
	CAN_EVENT_PASSIVE,
	CAN_EVENT_BUS_OFF,
	CAN_EVENT_LAST_ERROR_CODE,
	CAN_EVENT_ERROR = 15, // for any error condition pending in CAN_ESR
	CAN_EVENT_WAKEUP,
	CAN_EVENT_SLEEP

} CAN_INERRUPT_EVENTS_t;

typedef enum
{
	CAN_ERROR_WARNING_LIMIT,
	CAN_ERROR_PASSIVE_LIMIT,
	CAN_ERROR_BUS_OFF,
} CAN_ERROR_FLAGS_t;

typedef struct
{
	u8 *Data;
	u8 Data_length;
	u32 Identifier;
	u8 Identifier_type;
	u8 Frame_type;
} TX_Struct_t;

typedef struct
{
	//u8 *Data;
	u8 Data_length;
	u32 Identifier;
	u8 Identifier_type;
	u8 Frame_type;
	u8 Filter_Index;
} RX_Struct_t;

typedef enum
{
	CAN_ID_EXTENDED,
	CAN_ID_STANDARD
} IDENTIFIER_TYPE_t;

typedef enum
{
	CAN_DATA_FRAME,
	CAN_REMOTE_FRAME
} CAN_FRAME_t;

void CAN_voidFilterConfiguration(u8 Filter_Bank_number,
									FILTER_MODE_t Filter_Mode,
									FILTER_SCALE_REGISTER_t Filter_Scale,
									FILTER_ASSIGNMENT_t Filter_Assignment,
									u32 Identifier_1, u32 Mask_Identifier_2,
									CAN_STATUS_t Activtion_status);

void CAN_voidInit(void);
void CAN_voidOperatingMode(CAN_OPERATING_MODE_t Operating_Mode);
void CAN_voidInterruptStatus(CAN_INERRUPT_EVENTS_t Event, CAN_STATUS_t Status);

u8 CAN_u8ReadErrorFlag(CAN_ERROR_FLAGS_t Flag);
u8 CAN_u8GetErrorCode(void);
u8 CAN_u8GetReceiveErrorCounter(void);
u8 CAN_u8GetTransmitErrorCounter(void);

void CAN_TransmitMailBox(u8 MailBoxNum, u8 Data_length, u8 *Data,
							u32 identifier, u8 identifier_type,
							u8 frame_type
							);
void CAN_voidReceiveMailBox(u8 FIFO_num, RX_Struct_t *RX_Header, u8 *Rx_Data);
u32 CAN_u32GetProperFilterValues(u16 std_id_part, u32 extended_id_part, u8 IDE_value, u8 RTR_value);

#endif /* MCAL_CAN_CAN_INTERFACE_H_ */
