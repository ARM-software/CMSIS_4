/* -----------------------------------------------------------------------------
 * Copyright (c) 2013-2014 ARM Ltd.
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from
 * the use of this software. Permission is granted to anyone to use this
 * software for any purpose, including commercial applications, and to alter
 * it and redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software in
 *    a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 *
 *
 * $Date:        20. January 2015
 * $Revision:    V1.1
 *
 * Project:      USB common (Device and Host) module for NXP LPC18xx
 * -------------------------------------------------------------------------- */

/* History:
 *  Version 1.1
 *    Improved support for Host and Device
 *  Version 1.0
 *    Initial release
 */

#include "LPC18xx.h"
#include "SCU_LPC18xx.h"

#include "Driver_USB.h"

#include "RTE_Device.h"
#include "RTE_Components.h"

volatile uint8_t USB0_role  = ARM_USB_ROLE_NONE;
volatile uint8_t USB0_state = 0U;

#ifdef RTE_Drivers_USBH0
extern void USBH0_IRQ (void);
#endif
#ifdef RTE_Drivers_USBD0
extern void USBD0_IRQ (void);
#endif


// Common IRQ Routine **********************************************************

/**
  \fn          void USB0_IRQHandler (void)
  \brief       USB Interrupt Routine (IRQ).
*/
void USB0_IRQHandler (void) {
#if (defined(RTE_Drivers_USBH0) && defined(RTE_Drivers_USBD0))
  switch (USB0_role) {
#ifdef RTE_Drivers_USBH0
    case ARM_USB_ROLE_HOST:
      USBH0_IRQ ();
      break;
#endif
#ifdef RTE_Drivers_USBD0
    case ARM_USB_ROLE_DEVICE:
      USBD0_IRQ ();
      break;
#endif
    default:
      break;
  }
#else
#ifdef RTE_Drivers_USBH0
  USBH0_IRQ ();
#else
  USBD0_IRQ ();
#endif
#endif

}


// Public Functions ************************************************************

/**
  \fn          void USB0_PinsConfigure (void)
  \brief       Configure USB pins
*/
void USB0_PinsConfigure (void) {

  // Common (Device and Host) Pins
#if (RTE_USB0_IND0_PIN_EN)
  SCU_PinConfigure(RTE_USB0_IND0_PORT, RTE_USB0_IND0_BIT, RTE_USB0_IND0_FUNC);
#endif
#if (RTE_USB0_IND1_PIN_EN)
  SCU_PinConfigure(RTE_USB0_IND1_PORT, RTE_USB0_IND1_BIT, RTE_USB0_IND1_FUNC);
#endif

  // Host Pins
  if (USB0_role == ARM_USB_ROLE_HOST) {
#if (RTE_USB0_PPWR_PIN_EN)
    SCU_PinConfigure(RTE_USB0_PPWR_PORT,      RTE_USB0_PPWR_BIT,      RTE_USB0_PPWR_FUNC);
#endif
#if (RTE_USB0_PWR_FAULT_PIN_EN)
    SCU_PinConfigure(RTE_USB0_PWR_FAULT_PORT, RTE_USB0_PWR_FAULT_BIT, RTE_USB0_PWR_FAULT_FUNC);
#endif
  }
}

/**
  \fn          void USB0_PinsUnconfigure (void)
  \brief       De-configure USB pins
*/
void USB0_PinsUnconfigure (void) {

  // Common (Device and Host) Pins
#if (RTE_USB0_IND0_PIN_EN)
  SCU_PinConfigure(RTE_USB0_IND0_PORT, RTE_USB0_IND0_BIT, 0);
#endif
#if (RTE_USB0_IND1_PIN_EN)
  SCU_PinConfigure(RTE_USB0_IND1_PORT, RTE_USB0_IND1_BIT, 0);
#endif

  // Host Pins
  if (USB0_role == ARM_USB_ROLE_HOST) {
#if (RTE_USB0_PPWR_PIN_EN)
    SCU_PinConfigure(RTE_USB0_PPWR_PORT,      RTE_USB0_PPWR_BIT,      0);
#endif
#if (RTE_USB0_PWR_FAULT_PIN_EN)
    SCU_PinConfigure(RTE_USB0_PWR_FAULT_PORT, RTE_USB0_PWR_FAULT_BIT, 0);
#endif
  }
}
