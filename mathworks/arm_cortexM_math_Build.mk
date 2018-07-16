
ifeq ($(OS),Windows_NT)
MAKEDIR := mkdir
MAKEDIRFLAGS := 
RM := del
RMFLAGS := /q /f
CP := xcopy
CPFLAGS := /e /r /y /q /i
CPWILDCARD := 
else
MAKEDIR := mkdir
MAKEDIRFLAGS := -p
RM := rm
RMFLAGS := -rf
CP := cp
CPFLAGS := -r
CPWILDCARD := /*
endif


# CMSIS Packaging directory list
CMSISPKG := ./CMSIS
CMSIS := ../CMSIS
CMSISPKGINCLUDE := $(CMSISPKG)/Include
CMSISPKGLIB := $(CMSISPKG)/Lib
CMSISPKGRTOS := $(CMSISPKG)/RTOS
CMSISLIB := $(CMSIS)/Lib
CMSISMWLIB := ./Lib$(CPWILDCARD)
CMSISINCLUDE := $(CMSIS)/Include
CMSISRTOS := $(CMSIS)/RTOS
CMSISEULAPDF := $(CMSIS)/CMSIS_END_USER_LICENCE_AGREEMENT.pdf
CMSISEULARTF := $(CMSIS)/CMSIS_END_USER_LICENCE_AGREEMENT.rtf
CMSISPKGEULAPDF := $(CMSISPKG)/CMSIS_END_USER_LICENCE_AGREEMENT.pdf
CMSISPKGEULARTF := $(CMSISPKG)/CMSIS_END_USER_LICENCE_AGREEMENT.rtf

ifeq ($(OS),Windows_NT)
CMSISPKG := $(subst /,\,$(CMSISPKG))
CMSISPKGINCLUDE := $(subst /,\,$(CMSISPKGINCLUDE))
CMSISPKGLIB := $(subst /,\,$(CMSISPKGLIB))
CMSISPKGRTOS := $(subst /,\,$(CMSISPKGRTOS))
CMSISLIB := $(subst /,\,$(CMSISLIB))
CMSISMWLIB := $(subst /,\,$(CMSISMWLIB))
CMSISINCLUDE := $(subst /,\,$(CMSISINCLUDE))
CMSISRTOS := $(subst /,\,$(CMSISRTOS))
CMSISEULAPDF := $(subst /,\,$(CMSISEULAPDF))
CMSISEULARTF := $(subst /,\,$(CMSISEULARTF))
CMSISPKGEULAPDF := $(subst /,\,$(CMSISPKGEULAPDF))
CMSISPKGEULARTF := $(subst /,\,$(CMSISPKGEULARTF))
endif


.PHONY : all build clean info

$(CMSISPKGEULAPDF) : 
	$(CP) $(CMSISEULAPDF) $(CMSISPKG)

$(CMSISPKGEULARTF) : 
	$(CP) $(CMSISEULARTF) $(CMSISPKG)

$(CMSISPKG) : 
	@echo Creating CMSIS package
	$(MAKEDIR) $(MAKEDIRFLAGS) $(CMSISPKG)

$(CMSISPKGINCLUDE) : 
	@ echo	Copying Include directory
	$(CP) $(CPFLAGS) $(CMSISINCLUDE) $(CMSISPKGINCLUDE)

$(CMSISPKGLIB) : 
	@echo	Copying DSP library from CMSIS package
	$(CP) $(CPFLAGS) $(CMSISLIB) $(CMSISPKGLIB)
	@echo	Copying DSP library compiled with Soft ABI FPU
	$(CP) $(CPFLAGS) $(CMSISMWLIB) $(CMSISPKGLIB)

$(CMSISPKGRTOS) : 
	@echo	Copying RTOS
	$(CP) $(CPFLAGS) $(CMSISRTOS) $(CMSISPKGRTOS)
	
all : build $(CMSISPKG) $(CMSISPKGEULAPDF) $(CMSISPKGEULARTF) $(CMSISPKGINCLUDE) $(CMSISPKGLIB) $(CMSISPKGRTOS)

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
	$(RM) $(RMFLAGS) $(CMSISPKG)
    
info :
	make -s -C ./DSP_Lib/GCC -f arm_cortexM_math_m4lsf.mk info


