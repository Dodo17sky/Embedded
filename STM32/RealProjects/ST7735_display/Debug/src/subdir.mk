################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/main.c \
../src/st7735.c \
../src/syscalls.c \
../src/system_stm32f10x.c \
../src/uart.c 

OBJS += \
./src/main.o \
./src/st7735.o \
./src/syscalls.o \
./src/system_stm32f10x.o \
./src/uart.o 

C_DEPS += \
./src/main.d \
./src/st7735.d \
./src/syscalls.d \
./src/system_stm32f10x.d \
./src/uart.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -DDEBUG -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -I"D:/Dorin/Projects/C_embedded/Eclipse/STM32/ST7735_display/StdPeriph_Driver/inc" -I"D:/Dorin/Projects/C_embedded/Eclipse/STM32/ST7735_display/inc" -I"D:/Dorin/Projects/C_embedded/Eclipse/STM32/ST7735_display/CMSIS/device" -I"D:/Dorin/Projects/C_embedded/Eclipse/STM32/ST7735_display/CMSIS/core" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


