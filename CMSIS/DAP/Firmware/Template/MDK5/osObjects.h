/******************************************************************************
 * @file     osObjects.h
 * @brief    CMSIS-DAP RTOS Objects
 * @version  V1.10
 * @date     20. Jan 2015
 *
 * @note
 * Copyright (C) 2012-2015 ARM Limited. All rights reserved.
 *
 * @par
 * ARM Limited (ARM) is supplying this software for use with Cortex-M
 * processor based microcontrollers.
 *
 * @par
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 ******************************************************************************/

#ifndef __osObjects_h__
#define __osObjects_h__

#include "cmsis_os.h"

#ifdef osObjectsExternal
extern osThreadId HID0_ThreadId;
#else
       osThreadId HID0_ThreadId;
#endif

extern void HID0_Thread (void const *arg);
osThreadDef(HID0_Thread, osPriorityNormal, 1U, 512U);

#endif  /* __osObjects_h__ */
