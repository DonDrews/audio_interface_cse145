################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src/portable/chipidea/ci_hs/dcd_ci_hs.c \
C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src/portable/chipidea/ci_hs/hcd_ci_hs.c 

OBJS += \
./src/portable/chipidea/ci_hs/dcd_ci_hs.o \
./src/portable/chipidea/ci_hs/hcd_ci_hs.o 

C_DEPS += \
./src/portable/chipidea/ci_hs/dcd_ci_hs.d \
./src/portable/chipidea/ci_hs/hcd_ci_hs.d 


# Each subdirectory must supply rules for building sources it contributes
src/portable/chipidea/ci_hs/dcd_ci_hs.o: C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src/portable/chipidea/ci_hs/dcd_ci_hs.c src/portable/chipidea/ci_hs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F042x6 -c -I../Core/Inc -I"C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src" -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/portable/chipidea/ci_hs/hcd_ci_hs.o: C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src/portable/chipidea/ci_hs/hcd_ci_hs.c src/portable/chipidea/ci_hs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F042x6 -c -I../Core/Inc -I"C:/Users/donsp/Documents/GitHub/audio_interface/tinyusb/src" -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-src-2f-portable-2f-chipidea-2f-ci_hs

clean-src-2f-portable-2f-chipidea-2f-ci_hs:
	-$(RM) ./src/portable/chipidea/ci_hs/dcd_ci_hs.d ./src/portable/chipidea/ci_hs/dcd_ci_hs.o ./src/portable/chipidea/ci_hs/dcd_ci_hs.su ./src/portable/chipidea/ci_hs/hcd_ci_hs.d ./src/portable/chipidea/ci_hs/hcd_ci_hs.o ./src/portable/chipidea/ci_hs/hcd_ci_hs.su

.PHONY: clean-src-2f-portable-2f-chipidea-2f-ci_hs

