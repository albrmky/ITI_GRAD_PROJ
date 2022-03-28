/*
 * CAN_private.h
 *
 *  Created on: Mar 12, 2022
 *      Author: noureldeen albrmky
 */

#ifndef MCAL_CAN_CAN_PRIVATE_H_
#define MCAL_CAN_CAN_PRIVATE_H_

typedef struct {
	volatile u32 CAN_TIR; //identifier
	volatile u32 CAN_TDTR; //data length and time stamp
	volatile u32 CAN_TDLR; // data LOW
	volatile u32 CAN_TDHR; // data HIGH
} TX_MAILBOX_t;

typedef struct {
	volatile u32 CAN_RIR; //identifier
	volatile u32 CAN_RDTR; //data length and time stamp
	volatile u32 CAN_RDLR; // data LOW
	volatile u32 CAN_RDHR; // data HIGH
} RX_MAILBOX_t;

typedef struct {
	volatile u32 FxR1; // filter register 1
	volatile u32 FxR2; // filter register 2
} FILTER_BANK_t;

typedef struct {
	volatile u32 CAN_MCR; // Master control register
	volatile u32 CAN_MSR; // Master Status register
	volatile u32 CAN_TSR; // Transmit status register
	volatile u32 CAN_RF0R; // Receive FIFO 0
	volatile u32 CAN_RF1R; // Receive FIFO 1
	volatile u32 CAN_IER; // Interrupt enable register
	volatile u32 CAN_ESR; // Error status Register
	volatile u32 CAN_BTR; // Bit Timing Register
	volatile u32 reserved_1[88];

	TX_MAILBOX_t TX_MAILBOX[3]; // transmitt mail boxes
	RX_MAILBOX_t RX_MAILBOX[2]; // Receive mail boxes

	volatile u32 reserved_2[12];

	volatile u32 CAN_FMR; // filter Master Register
	volatile u32 CAN_FM1R; // filter Mode Register
	volatile u32 reserved_3;
	volatile u32 CAN_FS1R; // filter Scale Register
	volatile u32 reserved_4;
	volatile u32 CAN_FFA1R; // filter FIFO assignment Register
	volatile u32 reserved_5;
	volatile u32 CAN_FA1R; // filter Arbitration Register
	volatile u32 reserved_6[8];
	FILTER_BANK_t FILTER_BANK[28];
} CAN_t;

#define CAN1 ( (volatile CAN_t*)0x40006400 )

#define FINIT_BIT	0

#define WORKING		0
#define FROZEN		1

#define OFF			0
#define ON			1

#define SW_REQUEST	0
#define AUTO_BY_HW	1

#define CONC_MCR(b5,b4,b3,b2,b1,b0) CONC_MCR_HELP(b5,b4,b3,b2,b1,b0)
#define CONC_MCR_HELP(b5,b4,b3,b2,b1,b0) 0b##b5##b4##b3##b2##b1##b0

// in CAN_MCR
#define SLEEP_BIT	1
#define INRQ_BIT	0

// in CAN BTR
#define LBKM_BIT	30
#define SILM_BIT	31

static void CAN_voidSetTiming(void);

#endif /* MCAL_CAN_CAN_PRIVATE_H_ */
