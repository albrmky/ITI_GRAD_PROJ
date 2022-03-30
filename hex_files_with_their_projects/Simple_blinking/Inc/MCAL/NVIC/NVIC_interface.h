/*
 * NVIC_interface.h
 *
 *  Created on: Feb 23, 2022
 *      Author: noureldeen albrmky
 */

#ifndef MCAL_NVIC_NVIC_INTERFACE_H_
#define MCAL_NVIC_NVIC_INTERFACE_H_

typedef enum {
	NVIC_16_GROUP_ZERO_SUB = 3,
	NVIC_8_GROUP_2_SUB,
	NVIC_4_GROUP_4_SUB,
	NVIC_2_GROUP_8_SUB,
	NVIC_ZERO_GROUP_16_SUB

} PRIORITY_GROUPING_t;

void MNVIC_voidEnableInterrupt(u8 copy_ISRNumber);
void MNVIC_voidDisableInterrupt(u8 copy_ISRNumber);
void MNVIC_voidSetPendingInterrupt(u8 copy_ISRNumber);
void MNVIC_voidClearPendingInterrupt(u8 copy_ISRNumber);
u8 MNVIC_u8IsINTActive(u8 copy_ISRNumber);

void MNVIC_voidSetPriorityGrouping(PRIORITY_GROUPING_t copy_value);

void MNVIC_voidSetISRPriority(s8 copy_ISRNumber,
		PRIORITY_GROUPING_t copy_GroupingType, u8 copy_Group, u8 copy_Sub);

#define NVIC_MEM_MANG 		-4
#define NVIC_BUS_FAULT		-5
#define NVIC_USAGE_FAULT	-6

#define NVIC_SVCALL			-11
#define NVIC_PENDSV			-14
#define NVIC_SYSTICK		-15

#define NVIC_WWDG			0
#define NVIC_PVD			1
#define NVIC_TAMPER			2
#define NVIC_RTC			3
#define NVIC_FLASH			4
#define NVIC_RCC			5
#define NVIC_EXTI0			6
#define NVIC_EXTI1			7
#define NVIC_EXTI2			8
#define NVIC_EXTI3			9
#define NVIC_EXTI4			10
#define NVIC_DMA1_CH_1			11
#define NVIC_DMA1_CH_2			12
#define NVIC_DMA1_CH_3			13
#define NVIC_DMA1_CH_4			14
#define NVIC_DMA1_CH_5			15
#define NVIC_DMA1_CH_6			16
#define NVIC_DMA1_CH_7			17
#define NVIC_ADC1_2				18
#define NVIC_USB_HP_CAN_TX		19
#define NVIC_USB_LP_CAN_RX0		20
#define NVIC_CAN_RX1				21
#define NVIC_CAN_SCE				22
#define NVIC_NVIC_EXTI5_9		23
#define NVIC_TIM1_BRK			24
#define NVIC_TIM1_UP				25
#define NVIC_TIM1_TRG_COM		26
#define NVIC_TIM1_CC				27
#define NVIC_TIM2				28
#define NVIC_TIM3				29
#define NVIC_TIM4				30
#define NVIC_I2C1_EV				31
#define NVIC_I2C1_ER				32
#define NVIC_I2C2_EV				33
#define NVIC_I2C2_ER				34
#define NVIC_SPI1				35
#define NVIC_SPI2				36
#define NVIC_USART1				37
#define NVIC_USART2				38
#define NVIC_USART3				39
#define NVIC_NVIC_EXTI10_15		40
#define NVIC_RTCALARM			41
#define NVIC_USBWakeup			42
#define NVIC_TIM8_BRK			43
#define NVIC_TIM8_UP				44
#define NVIC_TIM8_TRG_COM        45
#define NVIC_TIM8_CC             46
#define NVIC_ADC3                47
#define NVIC_FSMC                48
#define NVIC_SDIO                49
#define NVIC_TIM5                50
#define NVIC_SPI3                51
#define NVIC_UART4               52
#define NVIC_UART5               53
#define NVIC_TIM6                54
#define NVIC_TIM7                55
#define NVIC_DMA2_Channel1       56
#define NVIC_DMA2_Channel2       57
#define NVIC_DMA2_Channel3       58
#define NVIC_DMA2_Channel4_5     59

#endif /* MCAL_NVIC_NVIC_INTERFACE_H_ */
