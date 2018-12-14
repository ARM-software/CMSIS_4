
# External defines

# CMSIS_PATH
# CMSIS_DSP_Lib
# CFLAGS
# CPPFLAGS
# ASFLAGS
# CC
# CPP
# AS
# START_DIR
# OUTPUT_O_DIR


MathLibraryDir = $(CMSIS_DSP_Lib)/ComplexMathFunctions

# Find all c files
ALL_L_SRCS := $(wildcard $(MathLibraryDir)/*.c)
COBJS := $(patsubst %.c,%.c.o,$(ALL_L_SRCS))
ALL_SRCS += $(ALL_L_SRCS)

# Find all cpp files
ALL_L_SRCS := $(wildcard $(MathLibraryDir)/*.cpp)
CPPOBJS := $(patsubst %.cpp,%.cpp.o,$(ALL_L_SRCS))
ALL_SRCS += $(ALL_L_SRCS)

# Find all cpp files
ALL_L_SRCS := $(wildcard $(MathLibraryDir)/*.s)
ASOBJS := $(patsubst %.s,%.s.o,$(ALL_L_SRCS))
ALL_SRCS += $(ALL_L_SRCS)


ALL_OBJS += $(addprefix $(OUTPUT_O_DIR)/,$(notdir $(COBJS))) $(addprefix $(OUTPUT_O_DIR)/,$(notdir $(CPPOBJS))) $(addprefix $(OUTPUT_O_DIR)/,$(notdir $(ASOBJS)))

include $(START_DIR)/MathLibraryMakefiles/arm_cortex_mathLib.mk




