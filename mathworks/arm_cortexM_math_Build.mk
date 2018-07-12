
ifeq ($(OS),Windows_NT)
MAKEDIR := mkdir
MAKEDIRFLAGS := 
CP := xcopy
CPFLAGS := /e /f /r /y /q
CPWILDCARD := 
else
MAKEDIR := mkdir
MAKEDIRFLAGS := -p
CP := cp
CPFLAGS := -r
CPWILDCARD := *
endif


.PHONY : all build clean info

all : build
	@echo Creating CMSIS package
	$(MAKEDIR) $(MAKEDIRFLAGS) ./CMSIS
	@echo	Copying DSP library from actual CMSIS
	$(CP) $(CPFLAGS) ../CMSIS/Lib ./CMSIS/Lib
	@echo	Copying DSP library from actual Mathworks CMSIS
	$(CP) $(CPFLAGS) ./Lib/$(CPWILDCARD) ./CMSIS/Lib
	@echo	Copying Include directory
	$(CP) $(CPFLAGS) ../CMSIS/Include ./CMSIS/Include
	@echo	Copying RTOS
	$(CP) $(CPFLAGS) ../CMSIS/RTOS ./CMSIS/RTOS

build :
	@echo   ----------------------------------------------------------------------------
	@echo   "Building DSP Library for Cortex-M4 with FPU (Soft ABI) Little Endian"
	@echo   ----------------------------------------------------------------------------
	make -s -C ./DSP_Lib/GCC -f arm_cortexM_math_m4lsf.mk build
	@echo   ----------------------------------------------------------------------------
	@echo   "Building DSP Library for Cortex-M4 with FPU (Soft ABI) Big Endian"
	@echo   ----------------------------------------------------------------------------
	make -s -C ./DSP_Lib/GCC -f arm_cortexM_math_m4bsf.mk build
	@echo   ----------------------------------------------------------------------------
	@echo   "Building DSP Library for Cortex-M7 with single precision FPU (Soft ABI) Little Endian"
	@echo   ----------------------------------------------------------------------------
	make -s -C ./DSP_Lib/GCC -f arm_cortexM_math_m7lsfsp.mk build
	@echo   ----------------------------------------------------------------------------
	@echo   "Building DSP Library for Cortex-M7 with single precision FPU (Soft ABI) Big Endian"
	@echo   ----------------------------------------------------------------------------
	make -s -C ./DSP_Lib/GCC -f arm_cortexM_math_m7bsfsp.mk build
	@echo   ----------------------------------------------------------------------------
	@echo   "Building DSP Library for Cortex-M7 with double precision FPU (Soft ABI) Little Endian"
	@echo   ----------------------------------------------------------------------------
	make -s -C ./DSP_Lib/GCC -f arm_cortexM_math_m7lsfdp.mk build
	@echo   ----------------------------------------------------------------------------
	@echo   "Building DSP Library for Cortex-M7 with double precision FPU (Soft ABI) Big Endian"
	@echo   ----------------------------------------------------------------------------
	make -s -C ./DSP_Lib/GCC -f arm_cortexM_math_m7bsfdp.mk build

clean :
	make -s -C ./DSP_Lib/GCC -f arm_cortexM_math_m4lsf.mk clean
	make -s -C ./DSP_Lib/GCC -f arm_cortexM_math_m4bsf.mk clean
	make -s -C ./DSP_Lib/GCC -f arm_cortexM_math_m7lsfsp.mk clean
	make -s -C ./DSP_Lib/GCC -f arm_cortexM_math_m7bsfsp.mk clean
	make -s -C ./DSP_Lib/GCC -f arm_cortexM_math_m7lsfdp.mk clean
	make -s -C ./DSP_Lib/GCC -f arm_cortexM_math_m7bsfdp.mk clean
	rm -rf ./CMSIS
    
info :
	make -s -C ./DSP_Lib/GCC -f arm_cortexM_math_m4lsf.mk info


