/*
 * FPEC_private.h
 *
 *  Created on: Mar 10, 2022
 *      Author: noureldeen albrmky
 */

#ifndef MCAL_FPEC_FPEC_PRIVATE_H_
#define MCAL_FPEC_FPEC_PRIVATE_H_

typedef struct {
	volatile u32 FLASH_ACR;
	volatile u32 FLASH_KEYR;
	volatile u32 FLASH_OPTKEYR;
	volatile u32 FLASH_SR;
	volatile u32 FLASH_CR;
	volatile u32 FLASH_AR;
	volatile u32 RESERVED;
	volatile u32 FLASH_OBR;
	volatile u32 FLASH_WRPR;
} FPEC_t;

#define FPEC ( (volatile FPEC_t*)0x40022000 )

#define FLASH_MAIN_MEM_START	0x08000000
#define FLASH_SYS_MEM_START		0x1FFFF000
#define FLASH_OPT_BYTES			0x1FFFF800

#define FLASH_OPTION_BYTE_DATA_0	2
#define FLASH_OPTION_BYTE_DATA_1	3

#define FLASH_KEY1	0x45670123
#define FLASH_KEY2	0xCDEF89AB
#define FLASH_RDPRT	0x00A5

// status register
#define BUSY_FLAG			0
#define EOP_FLAG			5

// control register
#define PAGE_PROGRAM		0
#define PAGE_ERASE			1
#define PAGE_MASS_ERASE		2
#define OPT_BYTE_PROGRAM	4
#define OPT_BYTE_ERASE		5
#define START_BIT			6
#define LOCK_BIT			7
#define OPT_WRE				9
#define ERRIE_BIT			10
#define EOPIE_BIT			12

#endif /* MCAL_FPEC_FPEC_PRIVATE_H_ */
