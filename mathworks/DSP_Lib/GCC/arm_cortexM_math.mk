
# External variables
# TARGET_ARCH
# GNU_ARM_TOOLS_PATH
# BIG_ENDIAN


# Start directory
START_DIR := ../GCC

# CMSIS Directory
CMSIS_PATH := $(START_DIR)/../../../CMSIS
# CMSIS DSP Library source directory
CMSIS_DSP_Lib := $(CMSIS_PATH)/DSP_Lib/Source

# Product directory
PRODUCT_DIR := $(START_DIR)/../../Lib/GCC

# Product name
PRODUCT := libarm_cortex$(TARGET_ARCH)_math.a

# Set shell
ifeq ($(OS),Windows_NT)
SHELL := %SystemRoot%/system32/cmd.exe
endif


# Assembler
AS_PATH := $(GNU_ARM_TOOLS_PATH)/bin
AS := $(AS_PATH)/arm-none-eabi-gcc

# C Compiler
CC_PATH := $(GNU_ARM_TOOLS_PATH)/bin
CC := $(CC_PATH)/arm-none-eabi-gcc

# Linker
LD_PATH := $(GNU_ARM_TOOLS_PATH)/bin
LD := $(LD_PATH)/arm-none-eabi-gcc

# C++ Compiler
CPP_PATH := $(GNU_ARM_TOOLS_PATH)/bin
CPP := $(CPP_PATH)/arm-none-eabi-g++

# C++ Linker
CPP_LD_PATH := $(GNU_ARM_TOOLS_PATH)/bin
CPP_LD := $(CPP_LD_PATH)/arm-none-eabi-gcc

# Archiver
AR_PATH := $(GNU_ARM_TOOLS_PATH)/bin
AR := $(AR_PATH)/arm-none-eabi-ar

#Include paths
INCLUDES := -I$(CMSIS_PATH)/Include

ALL_SRCS := 
ALL_OBJS := 


# Debug mode
ifeq ($(DEBUG),1)
DEBUG_FLAGS := -g -O0
else
DEBUG_FLAGS := -O3
endif

# Compile for Little-endian or Big-endian architectures
ifeq ($(BIG_ENDIAN),1)
ENDIAN_FLAG := -mbig-endian -DARM_MATH_BIG_ENDIAN
else
ENDIAN_FLAG := -mlittle-endian
endif

# Output directory
OUTPUT_O_DIR := $(START_DIR)/IntermediateFiles/$(TARGET_ARCH)


.PHONY : all build clean info

all : build

# C flags
CFLAGS_STD := -c -MMD -MP -gdwarf-2 -mthumb -Wall -fno-strict-aliasing -ffunction-sections -fdata-sections -ffp-contract=off $(ENDIAN_FLAG) $(DEBUG_FLAGS) $(INCLUDES)
# Assembly flags
ASFLAGS_STD := -x assembler-with-cpp $(CFLAGS)
# CPP flags
CPPFLAGS_STD := -c -MMD -MP -gdwarf-2 -mthumb -Wall -fno-strict-aliasing -ffunction-sections -fdata-sections -ffp-contract=off $(ENDIAN_FLAG) $(DEBUG_FLAGS) $(INCLUDES)
# Archiver flags
ARFLAGS_STD := ruvs
# C Linker flags
CLDFLAGS_STD := $(ENDIAN_FLAG) -Wl,--gc-sections
# CPP Linker flags
CPPLDFLAGS_STD := $(ENDIAN_FLAG) -Wl,--gc-sections

# Additional CFLAGS with respect to target
ifdef CFLAGS_TARGET
CFLAGS := $(CFLAGS_STD) $(CFLAGS_TARGET)
else
CFLAGS := $(CFLAGS_STD) 
endif

# Additional ASFLAGS with respect to target
ifdef ASFLAGS_TARGET
ASFLAGS := $(ASFLAGS_STD) $(ASFLAGS_TARGET)
else
ASFLAGS := $(ASFLAGS_STD) 
endif

# Additional CPPFLAGS with respect to target
ifdef CPPFLAGS_TARGET
CPPFLAGS := $(CPPFLAGS_STD) $(CPPFLAGS_TARGET)
else
CPPFLAGS := $(CPPFLAGS_STD) 
endif

# Additional ARFLAGS with respect to target
ifdef ARFLAGS_TARGET
ARFLAGS := $(ARFLAGS_STD) $(ARFLAGS_TARGET)
else
ARFLAGS := $(ARFLAGS_STD) 
endif


# Additional CLDFLAGS with respect to target
ifdef CLDFLAGS_TARGET
CLDFLAGS := $(CLDFLAGS_STD) $(CLDFLAGS_TARGET)
else
CLDFLAGS := $(CLDFLAGS_STD) 
endif

# Additional CPPLDFLAGS with respect to target
ifdef CPPLDFLAGS_TARGET
CPPLDFLAGS := $(CPPLDFLAGS_STD) $(CPPLDFLAGS_TARGET)
else
CPPLDFLAGS := $(CPPLDFLAGS_STD) 
endif

ifeq ($(OS),Windows_NT)
MAKEDIR := mkdir
MAKEDIRFLAGS := 
RM := del
RMFLAGS := /q /f
OUTPUT_O_DIR_T := $(subst /,\,$(OUTPUT_O_DIR))
PRODUCT_DIR_T := $(subst /,\,$(PRODUCT_DIR))
else
MAKEDIR := mkdir
MAKEDIRFLAGS := -p
RM := rm
RMFLAGS := -rf
OUTPUT_O_DIR_T := $(OUTPUT_O_DIR)
PRODUCT_DIR_T := $(PRODUCT_DIR)
endif


# Include CMSIS DSP Library makefile
-include $(START_DIR)/MathLibraryMakefiles/BasicMathFunctionsMakefile.mk
-include $(START_DIR)/MathLibraryMakefiles/CommonTablesMakefile.mk
-include $(START_DIR)/MathLibraryMakefiles/ComplexMathFunctionsMakefile.mk
-include $(START_DIR)/MathLibraryMakefiles/ControllerFunctionsMakefile.mk
-include $(START_DIR)/MathLibraryMakefiles/FastMathFunctionsMakefile.mk
-include $(START_DIR)/MathLibraryMakefiles/FilteringFunctionsMakefile.mk
-include $(START_DIR)/MathLibraryMakefiles/MatrixFunctionsMakefile.mk
-include $(START_DIR)/MathLibraryMakefiles/StatisticsFunctionsMakefile.mk
-include $(START_DIR)/MathLibraryMakefiles/SupportFunctionsMakefile.mk
-include $(START_DIR)/MathLibraryMakefiles/TransformFunctionsMakefile.mk


# Include dependency files to support increamental build
-include $(OUTPUT_O_DIR)/*.d

directories: $(OUTPUT_O_DIR_T) $(PRODUCT_DIR_T)

$(PRODUCT_DIR_T) :
	@echo 	"### Creating $(PRODUCT_DIR) directory"
	$(MAKEDIR) $(MAKEDIRFLAGS) $(PRODUCT_DIR_T)

$(OUTPUT_O_DIR_T):
	@echo 	"### Creating $(OUTPUT_O_DIR_T) directory"
	$(MAKEDIR) $(MAKEDIRFLAGS) $(OUTPUT_O_DIR_T)


$(PRODUCT_DIR)/$(PRODUCT) : directories $(ALL_OBJS) 
	@echo 	"### Building $(PRODUCT)"
	$(AR) $(ARFLAGS) $(PRODUCT_DIR)/$(PRODUCT) $(ALL_OBJS)

build : $(PRODUCT_DIR)/$(PRODUCT)

all : build

clean :
	$(RM) $(RMFLAGS) $(OUTPUT_O_DIR_T)
	$(RM) $(RMFLAGS) $(subst /,\,$(PRODUCT_DIR_T)/$(PRODUCT))
 
info : 
	@echo "### Compiling for target = $(TARGET_ARCH)"
	@echo "### C Compiler flags = $(CFLAGS)"
	@echo "### CPP Compiler flags = $(CPPFLAGS)"
	@echo "### All source files = $(ALL_SRCS)"
	@echo "### Object files = $(ALL_OBJS)"
	@echo "### Make file list $(MAKEFILE_LIST)"
	@echo "### OS = $(OS)"
	@echo "### Delete command = $(RM) $(RMFLAGS)"
	@echo "### Starting directory = $(START_DIR)"
	

