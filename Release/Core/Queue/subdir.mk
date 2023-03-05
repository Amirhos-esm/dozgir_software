################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Queue/cppQueue.cpp 

OBJS += \
./Core/Queue/cppQueue.o 

CPP_DEPS += \
./Core/Queue/cppQueue.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Queue/cppQueue.o: ../Core/Queue/cppQueue.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m0 -std=gnu++14 -DUSE_HAL_DRIVER -DSTM32F030x6 -c -I../Core/Inc -I../Core/Queue -I../Core/sim800 -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Core/MyInc -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Core/Queue/cppQueue.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

