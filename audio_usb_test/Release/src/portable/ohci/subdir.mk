################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src/portable/ohci/ohci.c 

OBJS += \
./src/portable/ohci/ohci.o 

C_DEPS += \
./src/portable/ohci/ohci.d 


# Each subdirectory must supply rules for building sources it contributes
src/portable/ohci/ohci.o: C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src/portable/ohci/ohci.c src/portable/ohci/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F042x6 -c -I../Core/Inc -I"C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src" -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/hw/bsp" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-src-2f-portable-2f-ohci

clean-src-2f-portable-2f-ohci:
	-$(RM) ./src/portable/ohci/ohci.d ./src/portable/ohci/ohci.o ./src/portable/ohci/ohci.su

.PHONY: clean-src-2f-portable-2f-ohci

