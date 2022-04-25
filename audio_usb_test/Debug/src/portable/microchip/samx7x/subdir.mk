################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src/portable/microchip/samx7x/dcd_samx7x.c 

OBJS += \
./src/portable/microchip/samx7x/dcd_samx7x.o 

C_DEPS += \
./src/portable/microchip/samx7x/dcd_samx7x.d 


# Each subdirectory must supply rules for building sources it contributes
src/portable/microchip/samx7x/dcd_samx7x.o: C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src/portable/microchip/samx7x/dcd_samx7x.c src/portable/microchip/samx7x/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g -DDEBUG -DUSE_HAL_DRIVER -DSTM32F042x6 -c -I../Core/Inc -I"C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src" -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-src-2f-portable-2f-microchip-2f-samx7x

clean-src-2f-portable-2f-microchip-2f-samx7x:
	-$(RM) ./src/portable/microchip/samx7x/dcd_samx7x.d ./src/portable/microchip/samx7x/dcd_samx7x.o ./src/portable/microchip/samx7x/dcd_samx7x.su

.PHONY: clean-src-2f-portable-2f-microchip-2f-samx7x

