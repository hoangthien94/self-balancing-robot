################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
Kalman/Kalman.obj: ../Kalman/Kalman.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.9.0.STS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.9.0.STS/include" --preinclude="G:/ti/TivaWare_C_Series-2.1.1.71" --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --display_error_number --diag_wrap=off --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="Kalman/Kalman.pp" --obj_directory="Kalman" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


