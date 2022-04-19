################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/portable/valentyusb/eptri/subdir.mk
-include src/portable/ti/msp430x5xx/subdir.mk
-include src/portable/template/subdir.mk
-include src/portable/synopsys/dwc2/subdir.mk
-include src/portable/sunxi/subdir.mk
-include src/portable/st/synopsys/subdir.mk
-include src/portable/st/stm32_fsdev/subdir.mk
-include src/portable/sony/cxd56/subdir.mk
-include src/portable/renesas/usba/subdir.mk
-include src/portable/raspberrypi/rp2040/subdir.mk
-include src/portable/ohci/subdir.mk
-include src/portable/nxp/transdimension/subdir.mk
-include src/portable/nxp/lpc_ip3511/subdir.mk
-include src/portable/nxp/lpc17_40/subdir.mk
-include src/portable/nxp/khci/subdir.mk
-include src/portable/nuvoton/nuc505/subdir.mk
-include src/portable/nuvoton/nuc121/subdir.mk
-include src/portable/nuvoton/nuc120/subdir.mk
-include src/portable/nordic/nrf5x/subdir.mk
-include src/portable/mindmotion/mm32/subdir.mk
-include src/portable/microchip/samx7x/subdir.mk
-include src/portable/microchip/samg/subdir.mk
-include src/portable/microchip/samd/subdir.mk
-include src/portable/microchip/pic32mz/subdir.mk
-include src/portable/mentor/musb/subdir.mk
-include src/portable/espressif/esp32sx/subdir.mk
-include src/portable/ehci/subdir.mk
-include src/portable/dialog/da146xx/subdir.mk
-include src/portable/chipidea/ci_hs/subdir.mk
-include src/portable/broadcom/synopsys/subdir.mk
-include src/portable/bridgetek/ft9xx/subdir.mk
-include src/host/subdir.mk
-include src/device/subdir.mk
-include src/common/subdir.mk
-include src/class/video/subdir.mk
-include src/class/vendor/subdir.mk
-include src/class/usbtmc/subdir.mk
-include src/class/net/subdir.mk
-include src/class/msc/subdir.mk
-include src/class/midi/subdir.mk
-include src/class/hid/subdir.mk
-include src/class/dfu/subdir.mk
-include src/class/cdc/subdir.mk
-include src/class/bth/subdir.mk
-include src/class/audio/subdir.mk
-include src/subdir.mk
-include Drivers/STM32F0xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := audio_usb_test
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
audio_usb_test.elf \

MAP_FILES += \
audio_usb_test.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
audio_usb_test.list \


# All Target
all: main-build

# Main-build Target
main-build: audio_usb_test.elf secondary-outputs

# Tool invocations
audio_usb_test.elf audio_usb_test.map: $(OBJS) $(USER_OBJS) C:\Users\donsp\Documents\GitHub\audio_interface\audio_usb_test\STM32F042K6TX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "audio_usb_test.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m0 -T"C:\Users\donsp\Documents\GitHub\audio_interface\audio_usb_test\STM32F042K6TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="audio_usb_test.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

audio_usb_test.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "audio_usb_test.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) audio_usb_test.elf audio_usb_test.list audio_usb_test.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
