################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
PID/PID.obj: ../PID/PID.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="c:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include" --include_path="D:/DIEN-DIENTU/WORKSPACE/CCS/self_balancing_robot/LCD_LIB" --include_path="D:/DIEN-DIENTU/WORKSPACE/CCS/self_balancing_robot/LCD_data" --include_path="C:/ti/TivaWare_C_Series-2.1.1.71" -g --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=TARGET_IS_BLIZZARD_RB1 --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="PID/PID.pp" --obj_directory="PID" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


