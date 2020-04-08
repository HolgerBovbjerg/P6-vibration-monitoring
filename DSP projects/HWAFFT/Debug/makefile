################################################################################
# Automatically-generated file. Do not edit!
################################################################################

EMPTY := 
SPACE := $(EMPTY) $(EMPTY)

ORDERED_OBJS += \
$(GEN_CMDS_QUOTED) \
"../c5505.cmd" \
"./vector.obj" \
"./rtc_routines.obj" \
"./main.obj" \
"./i2s_routines.obj" \
"./i2s_register.obj" \
"./hwafft.obj" \
"./filter_routines.obj" \
"./dma_routines.obj" \
"./configuration.obj" \
"./codec_routines.obj" \
-l"rts55h.lib" \

-include ../makefile.init

RM := DEL /F

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: VC5505_FFT_Filter_Demo.out

# Tool invocations
VC5505_FFT_Filter_Demo.out: $(GEN_CMDS) $(CMD_SRCS) $(OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Linker'
	$(shell echo -vcpu:3.3 > ccsLinker.opt)
	$(shell echo --symdebug:coff >> ccsLinker.opt)
	$(shell echo --define="_DEBUG" --define="C55X" >> ccsLinker.opt)
	$(shell echo --diag_warning=225 >> ccsLinker.opt)
	$(shell echo --ptrdiff_size=32 >> ccsLinker.opt)
	$(shell echo --algebraic >> ccsLinker.opt)
	$(shell echo --no_mac_expand >> ccsLinker.opt)
	$(shell echo --memory_model=huge >> ccsLinker.opt)
	$(shell echo --asm_source=algebraic >> ccsLinker.opt)
	$(shell echo -z >> ccsLinker.opt)
	$(shell echo -m"VC5505_FFT_Filter_Demo.map" >> ccsLinker.opt)
	$(shell echo --stack_size=1000 >> ccsLinker.opt)
	$(shell echo --warn_sections >> ccsLinker.opt)
	$(shell echo -i"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/c5500/lib" -i"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/c5500/include" -i"C:/Program Files/Texas Instruments/bios_5_40_03_23/packages/ti/rtdx/lib/c5500" -i"C:/Program Files/Texas Instruments/bios_5_40_03_23/packages/ti/bios/lib" -i"C:/Documents and Settings/A0272163/My Documents/Projects/2010-01-08 - C5505 FFT Demo/Bug_Fix_2010-05-10/project" -i"C:/CCSTUD~1.3/C5500/csl/lib" >> ccsLinker.opt)
	$(shell echo --reread_libs >> ccsLinker.opt)
	$(shell echo --entry_point=reset_isr >> ccsLinker.opt)
	$(shell echo --rom_model >> ccsLinker.opt)
	$(shell echo --sys_stacksize=1000 >> ccsLinker.opt)
	$(shell type ccsObjs.opt >> ccsLinker.opt)
	"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/c5500/bin/cl55" -@ccsLinker.opt -o "VC5505_FFT_Filter_Demo.out"
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(GEN_CMDS__QTD)$(GEN_SRCS__QTD)$(C5500_EXECUTABLE_OUTPUTS__QTD) "VC5505_FFT_Filter_Demo.out"
	-$(RM) ".\codec_routines.obj" ".\configuration.obj" ".\dma_routines.obj" ".\filter_routines.obj" ".\hwafft.obj" ".\i2s_register.obj" ".\i2s_routines.obj" ".\main.obj" ".\rtc_routines.obj" ".\vector.obj" 
	-$(RM) ".\codec_routines.pp" ".\i2s_register.pp" ".\vector.pp" 
	-$(RM) ".\configuration.pp" ".\dma_routines.pp" ".\filter_routines.pp" ".\i2s_routines.pp" ".\main.pp" ".\rtc_routines.pp" 
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
