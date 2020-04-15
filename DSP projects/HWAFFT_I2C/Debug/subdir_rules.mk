################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
c5505.out: ../c5505.cmd $(GEN_CMDS)
	@echo 'Building file: $<'
	@echo 'Invoking: Linker'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/bin/cl55" -vcpu:3.3 --symdebug:coff --define="_DEBUG" --define="C55X" --diag_warning=225 --large_memory_model --ptrdiff_size=32 --algebraic --no_mac_expand --memory_model=huge --asm_source=algebraic -z -m"VC5505_FFT_Filter_Demo.map" --stack_size=1000 --warn_sections -i"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/lib" -i"C:/Users/claus/Documents/GitHub/P6-vibration-monitoring/DSP projects/HWAFFT_I2C/I2C_files" -i"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/include" -i"C:/Program Files (x86)/Texas Instruments/bios_5_41_10_36/packages/ti/rtdx/lib/c5500" -i"C:/Program Files (x86)/Texas Instruments/bios_5_41_10_36/packages/ti/bios/lib" -i"C:/Users/claus/Documents/GitHub/P6-vibration-monitoring/DSP projects/HWAFFT_I2C" -i"/lib" --reread_libs --entry_point=reset_isr --rom_model --sys_stacksize=1000 -o "$@" "$<" "../c5505.cmd"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/bin/cl55" -vcpu:3.3 --symdebug:coff --define="_DEBUG" --define="C55X" --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/include" --include_path="C:/Users/claus/Documents/GitHub/P6-vibration-monitoring/DSP projects/HWAFFT_I2C/FFT_files" --include_path="C:/Users/claus/Documents/GitHub/P6-vibration-monitoring/DSP projects/HWAFFT_I2C/I2C_files" --include_path="C:/Users/claus/Documents/GitHub/P6-vibration-monitoring/DSP projects/HWAFFT_I2C/I2C_files/chip_support" --include_path="C:/Users/claus/Documents/GitHub/P6-vibration-monitoring/DSP projects/HWAFFT_I2C/Debug" --include_path="C:/Program Files (x86)/Texas Instruments/bios_5_41_10_36/packages/ti/rtdx/include/c5500" --diag_warning=225 --large_memory_model --ptrdiff_size=32 --algebraic --no_mac_expand --memory_model=huge --asm_source=algebraic --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '


