################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/MyInc/Player.c \
../Core/MyInc/Player_ll.c \
../Core/MyInc/debug.c \
../Core/MyInc/toPort.c \
../Core/MyInc/w25qxx.c 

CPP_SRCS += \
../Core/MyInc/event.cpp \
../Core/MyInc/mainCodes.cpp 

C_DEPS += \
./Core/MyInc/Player.d \
./Core/MyInc/Player_ll.d \
./Core/MyInc/debug.d \
./Core/MyInc/toPort.d \
./Core/MyInc/w25qxx.d 

OBJS += \
./Core/MyInc/Player.o \
./Core/MyInc/Player_ll.o \
./Core/MyInc/debug.o \
./Core/MyInc/event.o \
./Core/MyInc/mainCodes.o \
./Core/MyInc/toPort.o \
./Core/MyInc/w25qxx.o 

CPP_DEPS += \
./Core/MyInc/event.d \
./Core/MyInc/mainCodes.d 


# Each subdirectory must supply rules for building sources it contributes
Core/MyInc/Player.o: ../Core/MyInc/Player.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F030x6 -c -I../Core/Inc -I../Core/Queue -I../Core/sim800 -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I../Core/MyInc -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/MyInc/Player.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Core/MyInc/Player_ll.o: ../Core/MyInc/Player_ll.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F030x6 -c -I../Core/Inc -I../Core/Queue -I../Core/sim800 -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I../Core/MyInc -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/MyInc/Player_ll.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Core/MyInc/debug.o: ../Core/MyInc/debug.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F030x6 -c -I../Core/Inc -I../Core/Queue -I../Core/sim800 -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I../Core/MyInc -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/MyInc/debug.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Core/MyInc/event.o: ../Core/MyInc/event.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m0 -std=gnu++14 -DUSE_HAL_DRIVER -DSTM32F030x6 -c -I../Core/Inc -I../Core/Queue -I../Core/sim800 -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Core/MyInc -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Core/MyInc/event.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Core/MyInc/mainCodes.o: ../Core/MyInc/mainCodes.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m0 -std=gnu++14 -DUSE_HAL_DRIVER -DSTM32F030x6 -c -I../Core/Inc -I../Core/Queue -I../Core/sim800 -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Core/MyInc -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Core/MyInc/mainCodes.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Core/MyInc/toPort.o: ../Core/MyInc/toPort.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F030x6 -c -I../Core/Inc -I../Core/Queue -I../Core/sim800 -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I../Core/MyInc -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/MyInc/toPort.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Core/MyInc/w25qxx.o: ../Core/MyInc/w25qxx.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F030x6 -c -I../Core/Inc -I../Core/Queue -I../Core/sim800 -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I../Core/MyInc -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/MyInc/w25qxx.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

