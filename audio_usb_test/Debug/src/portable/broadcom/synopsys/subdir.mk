################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src/portable/broadcom/synopsys/dcd_synopsys.c 

OBJS += \
./src/portable/broadcom/synopsys/dcd_synopsys.o 

C_DEPS += \
./src/portable/broadcom/synopsys/dcd_synopsys.d 


# Each subdirectory must supply rules for building sources it contributes
src/portable/broadcom/synopsys/dcd_synopsys.o: C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src/portable/broadcom/synopsys/dcd_synopsys.c src/portable/broadcom/synopsys/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F042x6 -c -I../Core/Inc -I"C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src" -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-src-2f-portable-2f-broadcom-2f-synopsys

clean-src-2f-portable-2f-broadcom-2f-synopsys:
	-$(RM) ./src/portable/broadcom/synopsys/dcd_synopsys.d ./src/portable/broadcom/synopsys/dcd_synopsys.o ./src/portable/broadcom/synopsys/dcd_synopsys.su

.PHONY: clean-src-2f-portable-2f-broadcom-2f-synopsys

