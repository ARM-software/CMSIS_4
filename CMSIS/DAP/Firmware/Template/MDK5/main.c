/******************************************************************************
 * @file     main.c
 * @brief    CMSIS-DAP Main module
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

#include "cmsis_os.h"
#include "osObjects.h"
#include "rl_usb.h"
#include "DAP_config.h"
#include "DAP.h"

// Main program
int main (void) {

  DAP_Setup();                          // DAP Setup 

  USBD_Initialize(0U);                  // USB Device Initialization
  USBD_Connect(0U);                     // USB Device Connect

  while (!USBD_Configured(0U));         // Wait for USB Device to configure

  LED_CONNECTED_OUT(1U);                // Turn on  Debugger Connected LED
  LED_RUNNING_OUT(1U);                  // Turn on  Target Running LED
  Delayms(500U);                        // Wait for 500ms
  LED_RUNNING_OUT(0U);                  // Turn off Target Running LED
  LED_CONNECTED_OUT(0U);                // Turn off Debugger Connected LED

  // Create HID Thread
  HID0_ThreadId = osThreadCreate(osThread(HID0_Thread), NULL);

  osThreadSetPriority(osThreadGetId(), osPriorityIdle);
  for (;;);                             // Endless Loop
}
