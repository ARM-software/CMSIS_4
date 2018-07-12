
TARGET_ARCH := M4lsf
BIG_ENDIAN := 0

CFLAGS_TARGET = -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -DARM_MATH_CM4 -DARM_MATH_MATRIX_CHECK -DARM_MATH_ROUNDING -DUNALIGNED_SUPPORT_DISABLE -D__FPU_PRESENT=1
CPPFLAGS_TARGET = -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -DARM_MATH_CM4 -DARM_MATH_MATRIX_CHECK -DARM_MATH_ROUNDING -DUNALIGNED_SUPPORT_DISABLE -D__FPU_PRESENT=1
ASFLAGS_TARGET =  -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -DARM_MATH_CM4 -DARM_MATH_MATRIX_CHECK -DARM_MATH_ROUNDING -DUNALIGNED_SUPPORT_DISABLE -D__FPU_PRESENT=1
ARFLAGS_TARGET = 
CLDFLAGS_TARGET = -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp
CPPLDFLAGS_TARGET = -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp

include ./arm_cortexM_math.mk