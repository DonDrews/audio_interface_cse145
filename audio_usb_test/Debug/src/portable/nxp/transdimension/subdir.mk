################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src/portable/nxp/transdimension/dcd_transdimension.c \
C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src/portable/nxp/transdimension/hcd_transdimension.c 

OBJS += \
./src/portable/nxp/transdimension/dcd_transdimension.o \
./src/portable/nxp/transdimension/hcd_transdimension.o 

C_DEPS += \
./src/portable/nxp/transdimension/dcd_transdimension.d \
./src/portable/nxp/transdimension/hcd_transdimension.d 


# Each subdirectory must supply rules for building sources it contributes
src/portable/nxp/transdimension/dcd_transdimension.o: C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src/portable/nxp/transdimension/dcd_transdimension.c src/portable/nxp/transdimension/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F042x6 -c -I../Core/Inc -I"C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src" -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/portable/nxp/transdimension/hcd_transdimension.o: C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src/portable/nxp/transdimension/hcd_transdimension.c src/portable/nxp/transdimension/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F042x6 -c -I../Core/Inc -I"C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src" -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-src-2f-portable-2f-nxp-2f-transdimension

clean-src-2f-portable-2f-nxp-2f-transdimension:
	-$(RM) ./src/portable/nxp/transdimension/dcd_transdimension.d ./src/portable/nxp/transdimension/dcd_transdimension.o ./src/portable/nxp/transdimension/dcd_transdimension.su ./src/portable/nxp/transdimension/hcd_transdimension.d ./src/portable/nxp/transdimension/hcd_transdimension.o ./src/portable/nxp/transdimension/hcd_transdimension.su

.PHONY: clean-src-2f-portable-2f-nxp-2f-transdimension

