################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src/portable/raspberrypi/rp2040/dcd_rp2040.c \
C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src/portable/raspberrypi/rp2040/hcd_rp2040.c \
C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src/portable/raspberrypi/rp2040/rp2040_usb.c 

OBJS += \
./src/portable/raspberrypi/rp2040/dcd_rp2040.o \
./src/portable/raspberrypi/rp2040/hcd_rp2040.o \
./src/portable/raspberrypi/rp2040/rp2040_usb.o 

C_DEPS += \
./src/portable/raspberrypi/rp2040/dcd_rp2040.d \
./src/portable/raspberrypi/rp2040/hcd_rp2040.d \
./src/portable/raspberrypi/rp2040/rp2040_usb.d 


# Each subdirectory must supply rules for building sources it contributes
src/portable/raspberrypi/rp2040/dcd_rp2040.o: C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src/portable/raspberrypi/rp2040/dcd_rp2040.c src/portable/raspberrypi/rp2040/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F042x6 -c -I../Core/Inc -I"C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src" -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/hw/bsp" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/portable/raspberrypi/rp2040/hcd_rp2040.o: C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src/portable/raspberrypi/rp2040/hcd_rp2040.c src/portable/raspberrypi/rp2040/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F042x6 -c -I../Core/Inc -I"C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src" -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/hw/bsp" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/portable/raspberrypi/rp2040/rp2040_usb.o: C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src/portable/raspberrypi/rp2040/rp2040_usb.c src/portable/raspberrypi/rp2040/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F042x6 -c -I../Core/Inc -I"C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src" -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/hw/bsp" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-src-2f-portable-2f-raspberrypi-2f-rp2040

clean-src-2f-portable-2f-raspberrypi-2f-rp2040:
	-$(RM) ./src/portable/raspberrypi/rp2040/dcd_rp2040.d ./src/portable/raspberrypi/rp2040/dcd_rp2040.o ./src/portable/raspberrypi/rp2040/dcd_rp2040.su ./src/portable/raspberrypi/rp2040/hcd_rp2040.d ./src/portable/raspberrypi/rp2040/hcd_rp2040.o ./src/portable/raspberrypi/rp2040/hcd_rp2040.su ./src/portable/raspberrypi/rp2040/rp2040_usb.d ./src/portable/raspberrypi/rp2040/rp2040_usb.o ./src/portable/raspberrypi/rp2040/rp2040_usb.su

.PHONY: clean-src-2f-portable-2f-raspberrypi-2f-rp2040

