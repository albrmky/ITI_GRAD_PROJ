################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/MCAL/CAN_program.c \
../Src/MCAL/ESP8266_PROG.c \
../Src/MCAL/GPIO_program.c \
../Src/MCAL/NVIC_program.c \
../Src/MCAL/RCC_program.c \
../Src/MCAL/SCB_program.c \
../Src/MCAL/SYSTICK_program.c \
../Src/MCAL/USART_PROG.c 

OBJS += \
./Src/MCAL/CAN_program.o \
./Src/MCAL/ESP8266_PROG.o \
./Src/MCAL/GPIO_program.o \
./Src/MCAL/NVIC_program.o \
./Src/MCAL/RCC_program.o \
./Src/MCAL/SCB_program.o \
./Src/MCAL/SYSTICK_program.o \
./Src/MCAL/USART_PROG.o 

C_DEPS += \
./Src/MCAL/CAN_program.d \
./Src/MCAL/ESP8266_PROG.d \
./Src/MCAL/GPIO_program.d \
./Src/MCAL/NVIC_program.d \
./Src/MCAL/RCC_program.d \
./Src/MCAL/SCB_program.d \
./Src/MCAL/SYSTICK_program.d \
./Src/MCAL/USART_PROG.d 


# Each subdirectory must supply rules for building sources it contributes
Src/MCAL/CAN_program.o: ../Src/MCAL/CAN_program.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/MCAL/CAN_program.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Src/MCAL/ESP8266_PROG.o: ../Src/MCAL/ESP8266_PROG.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/MCAL/ESP8266_PROG.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Src/MCAL/GPIO_program.o: ../Src/MCAL/GPIO_program.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/MCAL/GPIO_program.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Src/MCAL/NVIC_program.o: ../Src/MCAL/NVIC_program.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/MCAL/NVIC_program.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Src/MCAL/RCC_program.o: ../Src/MCAL/RCC_program.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/MCAL/RCC_program.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Src/MCAL/SCB_program.o: ../Src/MCAL/SCB_program.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/MCAL/SCB_program.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Src/MCAL/SYSTICK_program.o: ../Src/MCAL/SYSTICK_program.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/MCAL/SYSTICK_program.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Src/MCAL/USART_PROG.o: ../Src/MCAL/USART_PROG.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/MCAL/USART_PROG.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

