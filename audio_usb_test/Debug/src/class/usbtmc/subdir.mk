################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src/class/usbtmc/usbtmc_device.c 

OBJS += \
./src/class/usbtmc/usbtmc_device.o 

C_DEPS += \
./src/class/usbtmc/usbtmc_device.d 


# Each subdirectory must supply rules for building sources it contributes
src/class/usbtmc/usbtmc_device.o: C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src/class/usbtmc/usbtmc_device.c src/class/usbtmc/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g -DDEBUG -DUSE_HAL_DRIVER -DSTM32F042x6 -c -I../Core/Inc -I"C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src" -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-src-2f-class-2f-usbtmc

clean-src-2f-class-2f-usbtmc:
	-$(RM) ./src/class/usbtmc/usbtmc_device.d ./src/class/usbtmc/usbtmc_device.o ./src/class/usbtmc/usbtmc_device.su

.PHONY: clean-src-2f-class-2f-usbtmc

