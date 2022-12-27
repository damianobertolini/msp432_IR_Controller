################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
BluetoothDriver/Devices/%.obj: ../BluetoothDriver/Devices/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/Applications/ti/ccs1210/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --include_path="/Applications/ti/ccs1210/ccs/ccs_base/arm/include" --include_path="/Applications/ti/ccs1210/ccs/ccs_base/arm/include/CMSIS" --include_path="/Users/damiano/Desktop/UNIVERSITA'/TERZO ANNO TRENTO/EMBEDDED SYSTEMS FOR IOT/msp432_IR_Controller/MSP432_Project" --include_path="/Applications/ti/ccs1210/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/include" --advice:power=all --define=__MSP432P401R__ --define=ccs --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="BluetoothDriver/Devices/$(basename $(<F)).d_raw" --obj_directory="BluetoothDriver/Devices" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


