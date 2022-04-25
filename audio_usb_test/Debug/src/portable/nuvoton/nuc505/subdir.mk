################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src/portable/nuvoton/nuc505/dcd_nuc505.c 

OBJS += \
./src/portable/nuvoton/nuc505/dcd_nuc505.o 

C_DEPS += \
./src/portable/nuvoton/nuc505/dcd_nuc505.d 


# Each subdirectory must supply rules for building sources it contributes
src/portable/nuvoton/nuc505/dcd_nuc505.o: C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src/portable/nuvoton/nuc505/dcd_nuc505.c src/portable/nuvoton/nuc505/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g -DDEBUG -DUSE_HAL_DRIVER -DSTM32F042x6 -c -I../Core/Inc -I"C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src" -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-src-2f-portable-2f-nuvoton-2f-nuc505

clean-src-2f-portable-2f-nuvoton-2f-nuc505:
	-$(RM) ./src/portable/nuvoton/nuc505/dcd_nuc505.d ./src/portable/nuvoton/nuc505/dcd_nuc505.o ./src/portable/nuvoton/nuc505/dcd_nuc505.su

.PHONY: clean-src-2f-portable-2f-nuvoton-2f-nuc505

