################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src/portable/st/stm32_fsdev/dcd_stm32_fsdev.c 

OBJS += \
./src/portable/st/stm32_fsdev/dcd_stm32_fsdev.o 

C_DEPS += \
./src/portable/st/stm32_fsdev/dcd_stm32_fsdev.d 


# Each subdirectory must supply rules for building sources it contributes
src/portable/st/stm32_fsdev/dcd_stm32_fsdev.o: C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src/portable/st/stm32_fsdev/dcd_stm32_fsdev.c src/portable/st/stm32_fsdev/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F042x6 -c -I../Core/Inc -I"C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src" -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-src-2f-portable-2f-st-2f-stm32_fsdev

clean-src-2f-portable-2f-st-2f-stm32_fsdev:
	-$(RM) ./src/portable/st/stm32_fsdev/dcd_stm32_fsdev.d ./src/portable/st/stm32_fsdev/dcd_stm32_fsdev.o ./src/portable/st/stm32_fsdev/dcd_stm32_fsdev.su

.PHONY: clean-src-2f-portable-2f-st-2f-stm32_fsdev

