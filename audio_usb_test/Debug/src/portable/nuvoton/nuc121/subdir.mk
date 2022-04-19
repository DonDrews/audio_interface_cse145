################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src/portable/nuvoton/nuc121/dcd_nuc121.c 

OBJS += \
./src/portable/nuvoton/nuc121/dcd_nuc121.o 

C_DEPS += \
./src/portable/nuvoton/nuc121/dcd_nuc121.d 


# Each subdirectory must supply rules for building sources it contributes
src/portable/nuvoton/nuc121/dcd_nuc121.o: C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src/portable/nuvoton/nuc121/dcd_nuc121.c src/portable/nuvoton/nuc121/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F042x6 -c -I../Core/Inc -I"C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src" -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-src-2f-portable-2f-nuvoton-2f-nuc121

clean-src-2f-portable-2f-nuvoton-2f-nuc121:
	-$(RM) ./src/portable/nuvoton/nuc121/dcd_nuc121.d ./src/portable/nuvoton/nuc121/dcd_nuc121.o ./src/portable/nuvoton/nuc121/dcd_nuc121.su

.PHONY: clean-src-2f-portable-2f-nuvoton-2f-nuc121

