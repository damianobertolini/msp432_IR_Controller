################################################################################
# Automatically-generated file. Do not edit!
################################################################################

CG_TOOL_ROOT := /Applications/ti/ccs1210/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./main.obj" \
"./startup_msp432p401r_ccs.obj" \
"./system_msp432p401r.obj" \
"./Hardware/Bluetooth/Controller/UART_IO.obj" \
"./Hardware/Bluetooth/Hardware/UART_Driver.obj" \
"./Hardware/Clock/clock.obj" \
"./Hardware/Graphics/direction_graphics.obj" \
"./Hardware/Infrared/command_matrices.obj" \
"./Hardware/Infrared/send_codes.obj" \
"./Hardware/Lcd/Crystalfontz128x128_ST7735.obj" \
"./Hardware/Lcd/HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.obj" \
"./Hardware/hardware_init.obj" \
"./Logic/Interrupts/adc_interrupt.obj" \
"./Logic/Interrupts/buttons_interrupts.obj" \
"./Logic/Interrupts/timers_interrupts.obj" \
"./Logic/project_logic.obj" \
"../msp432p401r.cmd" \
$(GEN_CMDS__FLAG) \
-llibc.a \
-l"/Applications/simplelink_msp432p4_sdk_3_40_01_02/source/ti/grlib/lib/ccs/m4f/grlib.a" \
-l"/Applications/simplelink_msp432p4_sdk_3_40_01_02/source/ti/devices/msp432p4xx/driverlib/ccs/msp432p4xx_driverlib.lib" \

-include ../makefile.init

RM := rm -rf
RMDIR := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include Hardware/Bluetooth/Controller/subdir_vars.mk
-include Hardware/Bluetooth/Hardware/subdir_vars.mk
-include Hardware/Clock/subdir_vars.mk
-include Hardware/Graphics/subdir_vars.mk
-include Hardware/Infrared/subdir_vars.mk
-include Hardware/Lcd/subdir_vars.mk
-include Hardware/subdir_vars.mk
-include Logic/Interrupts/subdir_vars.mk
-include Logic/subdir_vars.mk
-include subdir_rules.mk
-include Hardware/Bluetooth/Controller/subdir_rules.mk
-include Hardware/Bluetooth/Hardware/subdir_rules.mk
-include Hardware/Clock/subdir_rules.mk
-include Hardware/Graphics/subdir_rules.mk
-include Hardware/Infrared/subdir_rules.mk
-include Hardware/Lcd/subdir_rules.mk
-include Hardware/subdir_rules.mk
-include Logic/Interrupts/subdir_rules.mk
-include Logic/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
MSP432_Project.out \

EXE_OUTPUTS__QUOTED += \
"MSP432_Project.out" \

BIN_OUTPUTS += \
MSP432_Project.hex \

BIN_OUTPUTS__QUOTED += \
"MSP432_Project.hex" \


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "MSP432_Project.out"

# Tool invocations
MSP432_Project.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: Arm Linker'
	"/Applications/ti/ccs1210/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --advice:power=all --define=__MSP432P401R__ --define=ccs -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi -z -m"MSP432_Project.map" --heap_size=1024 --stack_size=512 -i"/Applications/ti/ccs1210/ccs/ccs_base/arm/include" -i"/Applications/ti/ccs1210/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/lib" -i"/Applications/ti/ccs1210/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/include" --reread_libs --diag_wrap=off --display_error_number --warn_sections --xml_link_info="MSP432_Project_linkInfo.xml" --rom_model -o "MSP432_Project.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

MSP432_Project.hex: $(EXE_OUTPUTS)
	@echo 'Building secondary target: "$@"'
	@echo 'Invoking: Arm Hex Utility'
	"/Applications/ti/ccs1210/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/bin/armhex" --memwidth=8 --romwidth=8 --diag_wrap=off -o "MSP432_Project.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building secondary target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "main.obj" "startup_msp432p401r_ccs.obj" "system_msp432p401r.obj" "Hardware/Bluetooth/Controller/UART_IO.obj" "Hardware/Bluetooth/Hardware/UART_Driver.obj" "Hardware/Clock/clock.obj" "Hardware/Graphics/direction_graphics.obj" "Hardware/Infrared/command_matrices.obj" "Hardware/Infrared/send_codes.obj" "Hardware/Lcd/Crystalfontz128x128_ST7735.obj" "Hardware/Lcd/HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.obj" "Hardware/hardware_init.obj" "Logic/Interrupts/adc_interrupt.obj" "Logic/Interrupts/buttons_interrupts.obj" "Logic/Interrupts/timers_interrupts.obj" "Logic/project_logic.obj" 
	-$(RM) "main.d" "startup_msp432p401r_ccs.d" "system_msp432p401r.d" "Hardware/Bluetooth/Controller/UART_IO.d" "Hardware/Bluetooth/Hardware/UART_Driver.d" "Hardware/Clock/clock.d" "Hardware/Graphics/direction_graphics.d" "Hardware/Infrared/command_matrices.d" "Hardware/Infrared/send_codes.d" "Hardware/Lcd/Crystalfontz128x128_ST7735.d" "Hardware/Lcd/HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.d" "Hardware/hardware_init.d" "Logic/Interrupts/adc_interrupt.d" "Logic/Interrupts/buttons_interrupts.d" "Logic/Interrupts/timers_interrupts.d" "Logic/project_logic.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

