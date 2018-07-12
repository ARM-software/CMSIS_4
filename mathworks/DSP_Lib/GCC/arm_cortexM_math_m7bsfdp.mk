
# Create CMSIS math library for Cortex-M7 with Soft FPU ABI
TARGET_ARCH := M7bsfdp
BIG_ENDIAN := 1

CFLAGS_TARGET = -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=softfp -DARM_MATH_CM7 -DARM_MATH_MATRIX_CHECK -DARM_MATH_ROUNDING -DUNALIGNED_SUPPORT_DISABLE -D__FPU_PRESENT=1
CPPFLAGS_TARGET = -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=softfp -DARM_MATH_CM7 -DARM_MATH_MATRIX_CHECK -DARM_MATH_ROUNDING -DUNALIGNED_SUPPORT_DISABLE -D__FPU_PRESENT=1
ASFLAGS_TARGET =  -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=softfp -DARM_MATH_CM7 -DARM_MATH_MATRIX_CHECK -DARM_MATH_ROUNDING -DUNALIGNED_SUPPORT_DISABLE -D__FPU_PRESENT=1
ARFLAGS_TARGET = 
CLDFLAGS_TARGET = -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=softfp
CPPLDFLAGS_TARGET = -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=softfp

include ./arm_cortexM_math.mk