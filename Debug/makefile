################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.9.0.STS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./main.obj" \
"./tm4c123gh6pm_startup_ccs.obj" \
"./motorControl/motorControl.obj" \
"./UART/UART.obj" \
"./UART/uartstdio.obj" \
"./UART/ustdlib.obj" \
"./Timer/Timer.obj" \
"./QEI/QEI.obj" \
"./Processs/Process.obj" \
"./Processs/sine.obj" \
"./PWM/PWM.obj" \
"./PID/PID.obj" \
"./MPU6050/MPU6050.obj" \
"./LED/LED.obj" \
"./Kalman/Kalman.obj" \
"./I2C/I2C.obj" \
"./Button/Button.obj" \
"../tm4c123gh6pm.cmd" \
$(GEN_CMDS__FLAG) \
-l"libc.a" \
-l"G:\ti\TivaWare_C_Series-2.1.1.71\driverlib\ccs\Debug\driverlib.lib" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include motorControl/subdir_vars.mk
-include UART/subdir_vars.mk
-include Timer/subdir_vars.mk
-include QEI/subdir_vars.mk
-include Processs/subdir_vars.mk
-include PWM/subdir_vars.mk
-include PID/subdir_vars.mk
-include MPU6050/subdir_vars.mk
-include LED/subdir_vars.mk
-include Kalman/subdir_vars.mk
-include I2C/subdir_vars.mk
-include Button/subdir_vars.mk
-include subdir_rules.mk
-include motorControl/subdir_rules.mk
-include UART/subdir_rules.mk
-include Timer/subdir_rules.mk
-include QEI/subdir_rules.mk
-include Processs/subdir_rules.mk
-include PWM/subdir_rules.mk
-include PID/subdir_rules.mk
-include MPU6050/subdir_rules.mk
-include LED/subdir_rules.mk
-include Kalman/subdir_rules.mk
-include I2C/subdir_rules.mk
-include Button/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
self_balancing_robot.out \

EXE_OUTPUTS__QUOTED += \
"self_balancing_robot.out" \

BIN_OUTPUTS += \
self_balancing_robot.hex \

BIN_OUTPUTS__QUOTED += \
"self_balancing_robot.hex" \


# All Target
all: self_balancing_robot.out

# Tool invocations
self_balancing_robot.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Linker'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.9.0.STS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -g --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=TARGET_IS_BLIZZARD_RB1 --display_error_number --diag_wrap=off --diag_warning=225 --abi=eabi -z -m"self_balancing_robot.map" --heap_size=0 --stack_size=512 -i"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.9.0.STS/lib" -i"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.9.0.STS/include" --reread_libs --display_error_number --diag_wrap=off --warn_sections --xml_link_info="self_balancing_robot_linkInfo.xml" --rom_model -o "self_balancing_robot.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

self_balancing_robot.hex: $(EXE_OUTPUTS)
	@echo 'Invoking: ARM Hex Utility'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.9.0.STS/bin/armhex"  -o "self_balancing_robot.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)$(BIN_OUTPUTS__QUOTED)
	-$(RM) "main.pp" "tm4c123gh6pm_startup_ccs.pp" "motorControl\motorControl.pp" "UART\UART.pp" "UART\uartstdio.pp" "UART\ustdlib.pp" "Timer\Timer.pp" "QEI\QEI.pp" "Processs\Process.pp" "Processs\sine.pp" "PWM\PWM.pp" "PID\PID.pp" "MPU6050\MPU6050.pp" "LED\LED.pp" "Kalman\Kalman.pp" "I2C\I2C.pp" "Button\Button.pp" 
	-$(RM) "main.obj" "tm4c123gh6pm_startup_ccs.obj" "motorControl\motorControl.obj" "UART\UART.obj" "UART\uartstdio.obj" "UART\ustdlib.obj" "Timer\Timer.obj" "QEI\QEI.obj" "Processs\Process.obj" "Processs\sine.obj" "PWM\PWM.obj" "PID\PID.obj" "MPU6050\MPU6050.obj" "LED\LED.obj" "Kalman\Kalman.obj" "I2C\I2C.obj" "Button\Button.obj" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

