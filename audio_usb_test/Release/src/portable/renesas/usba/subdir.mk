################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src/portable/renesas/usba/dcd_usba.c \
C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src/portable/renesas/usba/hcd_usba.c 

OBJS += \
./src/portable/renesas/usba/dcd_usba.o \
./src/portable/renesas/usba/hcd_usba.o 

C_DEPS += \
./src/portable/renesas/usba/dcd_usba.d \
./src/portable/renesas/usba/hcd_usba.d 


# Each subdirectory must supply rules for building sources it contributes
src/portable/renesas/usba/dcd_usba.o: C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src/portable/renesas/usba/dcd_usba.c src/portable/renesas/usba/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F042x6 -c -I../Core/Inc -I"C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src" -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/hw/bsp" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/portable/renesas/usba/hcd_usba.o: C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src/portable/renesas/usba/hcd_usba.c src/portable/renesas/usba/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F042x6 -c -I../Core/Inc -I"C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src" -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/hw/bsp" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-src-2f-portable-2f-renesas-2f-usba

clean-src-2f-portable-2f-renesas-2f-usba:
	-$(RM) ./src/portable/renesas/usba/dcd_usba.d ./src/portable/renesas/usba/dcd_usba.o ./src/portable/renesas/usba/dcd_usba.su ./src/portable/renesas/usba/hcd_usba.d ./src/portable/renesas/usba/hcd_usba.o ./src/portable/renesas/usba/hcd_usba.su

.PHONY: clean-src-2f-portable-2f-renesas-2f-usba

