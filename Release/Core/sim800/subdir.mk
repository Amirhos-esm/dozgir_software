################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/sim800/BT.c \
../Core/sim800/sim800c.c \
../Core/sim800/sim800c_call.c \
../Core/sim800/sim800c_general.c \
../Core/sim800/sim800c_interface.c \
../Core/sim800/sim800c_sms.c 

C_DEPS += \
./Core/sim800/BT.d \
./Core/sim800/sim800c.d \
./Core/sim800/sim800c_call.d \
./Core/sim800/sim800c_general.d \
./Core/sim800/sim800c_interface.d \
./Core/sim800/sim800c_sms.d 

OBJS += \
./Core/sim800/BT.o \
./Core/sim800/sim800c.o \
./Core/sim800/sim800c_call.o \
./Core/sim800/sim800c_general.o \
./Core/sim800/sim800c_interface.o \
./Core/sim800/sim800c_sms.o 


# Each subdirectory must supply rules for building sources it contributes
Core/sim800/BT.o: ../Core/sim800/BT.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F030x6 -c -I../Core/Inc -I../Core/Queue -I../Core/sim800 -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I../Core/MyInc -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/sim800/BT.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Core/sim800/sim800c.o: ../Core/sim800/sim800c.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F030x6 -c -I../Core/Inc -I../Core/Queue -I../Core/sim800 -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I../Core/MyInc -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/sim800/sim800c.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Core/sim800/sim800c_call.o: ../Core/sim800/sim800c_call.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F030x6 -c -I../Core/Inc -I../Core/Queue -I../Core/sim800 -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I../Core/MyInc -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/sim800/sim800c_call.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Core/sim800/sim800c_general.o: ../Core/sim800/sim800c_general.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F030x6 -c -I../Core/Inc -I../Core/Queue -I../Core/sim800 -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I../Core/MyInc -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/sim800/sim800c_general.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Core/sim800/sim800c_interface.o: ../Core/sim800/sim800c_interface.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F030x6 -c -I../Core/Inc -I../Core/Queue -I../Core/sim800 -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I../Core/MyInc -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/sim800/sim800c_interface.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Core/sim800/sim800c_sms.o: ../Core/sim800/sim800c_sms.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F030x6 -c -I../Core/Inc -I../Core/Queue -I../Core/sim800 -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I../Core/MyInc -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/sim800/sim800c_sms.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

