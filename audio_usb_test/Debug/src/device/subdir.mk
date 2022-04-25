################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src/device/usbd.c \
C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src/device/usbd_control.c 

OBJS += \
./src/device/usbd.o \
./src/device/usbd_control.o 

C_DEPS += \
./src/device/usbd.d \
./src/device/usbd_control.d 


# Each subdirectory must supply rules for building sources it contributes
src/device/usbd.o: C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src/device/usbd.c src/device/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g -DDEBUG -DUSE_HAL_DRIVER -DSTM32F042x6 -c -I../Core/Inc -I"C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src" -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/device/usbd_control.o: C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src/device/usbd_control.c src/device/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g -DDEBUG -DUSE_HAL_DRIVER -DSTM32F042x6 -c -I../Core/Inc -I"C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src" -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-src-2f-device

clean-src-2f-device:
	-$(RM) ./src/device/usbd.d ./src/device/usbd.o ./src/device/usbd.su ./src/device/usbd_control.d ./src/device/usbd_control.o ./src/device/usbd_control.su

.PHONY: clean-src-2f-device

