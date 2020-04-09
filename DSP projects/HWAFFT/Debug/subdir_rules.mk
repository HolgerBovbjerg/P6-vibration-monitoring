################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
codec_routines.obj: ../codec_routines.asm $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/bin/cl55" -vcpu:3.3 --symdebug:coff --define="_DEBUG" --define="C55X" --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/include" --include_path="C:/Users/claus/Documents/GitHub/P6-vibration-monitoring/DSP projects/HWAFFT/Debug" --include_path="C:/Program Files (x86)/Texas Instruments/bios_5_41_10_36/packages/ti/rtdx/include/c5500" --diag_warning=225 --ptrdiff_size=32 --algebraic --no_mac_expand --memory_model=huge --asm_source=algebraic --preproc_with_compile --preproc_dependency="codec_routines.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

configuration.obj: ../configuration.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/bin/cl55" -vcpu:3.3 --symdebug:coff --define="_DEBUG" --define="C55X" --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/include" --include_path="C:/Users/claus/Documents/GitHub/P6-vibration-monitoring/DSP projects/HWAFFT/Debug" --include_path="C:/Program Files (x86)/Texas Instruments/bios_5_41_10_36/packages/ti/rtdx/include/c5500" --diag_warning=225 --ptrdiff_size=32 --algebraic --no_mac_expand --memory_model=huge --asm_source=algebraic --preproc_with_compile --preproc_dependency="configuration.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

dma_routines.obj: ../dma_routines.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/bin/cl55" -vcpu:3.3 --symdebug:coff --define="_DEBUG" --define="C55X" --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/include" --include_path="C:/Users/claus/Documents/GitHub/P6-vibration-monitoring/DSP projects/HWAFFT/Debug" --include_path="C:/Program Files (x86)/Texas Instruments/bios_5_41_10_36/packages/ti/rtdx/include/c5500" --diag_warning=225 --ptrdiff_size=32 --algebraic --no_mac_expand --memory_model=huge --asm_source=algebraic --preproc_with_compile --preproc_dependency="dma_routines.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

filter_routines.obj: ../filter_routines.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/bin/cl55" -vcpu:3.3 --symdebug:coff --define="_DEBUG" --define="C55X" --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/include" --include_path="C:/Users/claus/Documents/GitHub/P6-vibration-monitoring/DSP projects/HWAFFT/Debug" --include_path="C:/Program Files (x86)/Texas Instruments/bios_5_41_10_36/packages/ti/rtdx/include/c5500" --diag_warning=225 --ptrdiff_size=32 --algebraic --no_mac_expand --memory_model=huge --asm_source=algebraic --preproc_with_compile --preproc_dependency="filter_routines.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

hwafft.obj: ../hwafft.asm $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/bin/cl55" -vcpu:3.3 --symdebug:coff --define="_DEBUG" --define="C55X" --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/include" --include_path="C:/Users/claus/Documents/GitHub/P6-vibration-monitoring/DSP projects/HWAFFT/Debug" --include_path="C:/Program Files (x86)/Texas Instruments/bios_5_41_10_36/packages/ti/rtdx/include/c5500" --diag_warning=225 --ptrdiff_size=32 --algebraic --no_mac_expand --memory_model=huge --asm_source=algebraic $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

i2s_register.obj: ../i2s_register.asm $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/bin/cl55" -vcpu:3.3 --symdebug:coff --define="_DEBUG" --define="C55X" --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/include" --include_path="C:/Users/claus/Documents/GitHub/P6-vibration-monitoring/DSP projects/HWAFFT/Debug" --include_path="C:/Program Files (x86)/Texas Instruments/bios_5_41_10_36/packages/ti/rtdx/include/c5500" --diag_warning=225 --ptrdiff_size=32 --algebraic --no_mac_expand --memory_model=huge --asm_source=algebraic --preproc_with_compile --preproc_dependency="i2s_register.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

i2s_routines.obj: ../i2s_routines.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/bin/cl55" -vcpu:3.3 --symdebug:coff --define="_DEBUG" --define="C55X" --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/include" --include_path="C:/Users/claus/Documents/GitHub/P6-vibration-monitoring/DSP projects/HWAFFT/Debug" --include_path="C:/Program Files (x86)/Texas Instruments/bios_5_41_10_36/packages/ti/rtdx/include/c5500" --diag_warning=225 --ptrdiff_size=32 --algebraic --no_mac_expand --memory_model=huge --asm_source=algebraic --preproc_with_compile --preproc_dependency="i2s_routines.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/bin/cl55" -vcpu:3.3 --symdebug:coff --define="_DEBUG" --define="C55X" --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/include" --include_path="C:/Users/claus/Documents/GitHub/P6-vibration-monitoring/DSP projects/HWAFFT/Debug" --include_path="C:/Program Files (x86)/Texas Instruments/bios_5_41_10_36/packages/ti/rtdx/include/c5500" --diag_warning=225 --ptrdiff_size=32 --algebraic --no_mac_expand --memory_model=huge --asm_source=algebraic --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

rtc_routines.obj: ../rtc_routines.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/bin/cl55" -vcpu:3.3 --symdebug:coff --define="_DEBUG" --define="C55X" --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/include" --include_path="C:/Users/claus/Documents/GitHub/P6-vibration-monitoring/DSP projects/HWAFFT/Debug" --include_path="C:/Program Files (x86)/Texas Instruments/bios_5_41_10_36/packages/ti/rtdx/include/c5500" --diag_warning=225 --ptrdiff_size=32 --algebraic --no_mac_expand --memory_model=huge --asm_source=algebraic --preproc_with_compile --preproc_dependency="rtc_routines.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

vector.obj: ../vector.asm $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/bin/cl55" -vcpu:3.3 --symdebug:coff --define="_DEBUG" --define="C55X" --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/include" --include_path="C:/Users/claus/Documents/GitHub/P6-vibration-monitoring/DSP projects/HWAFFT/Debug" --include_path="C:/Program Files (x86)/Texas Instruments/bios_5_41_10_36/packages/ti/rtdx/include/c5500" --diag_warning=225 --ptrdiff_size=32 --algebraic --no_mac_expand --memory_model=huge --asm_source=algebraic --preproc_with_compile --preproc_dependency="vector.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '


