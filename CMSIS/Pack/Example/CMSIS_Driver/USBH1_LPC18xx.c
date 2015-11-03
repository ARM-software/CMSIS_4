/* -----------------------------------------------------------------------------
 * Copyright (c) 2013-2015 ARM Ltd.
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
 * $Date:        17. June 2015
 * $Revision:    V2.3
 *
 * Driver:       Driver_USBH1_HCI
 * Configured:   via RTE_Device.h configuration file
 * Project:      USB Host 1 HCI Controller (EHCI) Driver for NXP LPC18xx
 * --------------------------------------------------------------------------
 * Use the following configuration settings in the middleware component
 * to connect to this driver.
 *
 *   Configuration Setting                  Value
 *   ---------------------                  -----
 *   Connect to hardware via Driver_USBH# = 1
 *   USB Host controller interface        = EHCI
 * -------------------------------------------------------------------------- */

/* History:
 *  Version 2.3
 *    PowerControl for Power OFF and Uninitialize functions made unconditional
 *  Version 2.2
 *    Updated in accordance with USB Device Driver
 *  Version 2.1
 *    Moved register initialization and uninitialization to PowerControl
 *    function and removed from Initialize/Uninitialize functions
 *    Pin configuration moved to USB_LPC18xx_USB0.c
 *  Version 2.0
 *    Initial release for USB Host EHCI Driver API v2.0
 *  Version 1.0
 *    Initial release
 */


#include "Driver_USBH.h"

#include "LPC18xx.h"
#include "SCU_LPC18xx.h"
#include "USB_LPC18xx.h"

#include "RTE_Device.h"
#include "RTE_Components.h"

#if      (RTE_USB_USB1 == 0)
#error   "USB1 is not enabled in the RTE_Device.h!"
#endif
#if      (RTE_USB_USB1_FS_PHY_EN && RTE_USB_USB1_HS_PHY_EN)
#error   "Both full-speed and high-speed PHY can not be selected at the same time!"
#endif

extern uint8_t USB1_role;
extern uint8_t USB1_state;

extern void USB1_PinsConfigure   (void);
extern void USB1_PinsUnconfigure (void);


// USBH EHCI Driver ************************************************************

#define ARM_USBH_EHCI_DRIVER_VERSION ARM_DRIVER_VERSION_MAJOR_MINOR(2,3)

// Driver Version
static const ARM_DRIVER_VERSION usbh_ehci_driver_version = { ARM_USBH_API_VERSION, ARM_USBH_EHCI_DRIVER_VERSION };

// Driver Capabilities
static const ARM_USBH_HCI_CAPABILITIES usbh_ehci_driver_capabilities = {
  0x0001U       // Root HUB available Ports Mask
};

static ARM_USBH_HCI_Interrupt_t EHCI_IRQ;

// USBH EHCI Driver functions

/**
  \fn          ARM_DRIVER_VERSION USBH_HCI_GetVersion (void)
  \brief       Get USB Host HCI (OHCI/EHCI) driver version.
  \return      \ref ARM_DRIVER_VERSION
*/
static ARM_DRIVER_VERSION USBH_HCI_GetVersion (void) { return usbh_ehci_driver_version; }

/**
  \fn          ARM_USBH_HCI_CAPABILITIES USBH_HCI_GetCapabilities (void)
  \brief       Get driver capabilities.
  \return      \ref ARM_USBH_HCI_CAPABILITIES
*/
static ARM_USBH_HCI_CAPABILITIES USBH_HCI_GetCapabilities (void) { return usbh_ehci_driver_capabilities; }

/**
  \fn          int32_t USBH_HCI_Initialize (ARM_USBH_HCI_Interrupt_t *cb_interrupt)
  \brief       Initialize USB Host HCI (OHCI/EHCI) Interface.
  \param[in]   cb_interrupt Pointer to Interrupt Handler Routine
  \return      \ref execution_status
*/
static int32_t USBH_HCI_Initialize (ARM_USBH_HCI_Interrupt_t cb_interrupt) {

  if ((USB1_state & USBH_DRIVER_INITIALIZED) != 0U) { return ARM_DRIVER_OK; }

  EHCI_IRQ = cb_interrupt;

  USB1_role   =  ARM_USB_ROLE_HOST;

  USB1_PinsConfigure ();

  USB1_state  =  USBH_DRIVER_INITIALIZED;

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USBH_HCI_Uninitialize (void)
  \brief       De-initialize USB Host HCI (OHCI/EHCI) Interface.
  \return      \ref execution_status
*/
static int32_t USBH_HCI_Uninitialize (void) {

  USB1_PinsUnconfigure ();
  USB1_role   =  ARM_USB_ROLE_NONE;
  USB1_state &= ~USBH_DRIVER_INITIALIZED;

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USBH_HCI_PowerControl (ARM_POWER_STATE state)
  \brief       Control USB Host HCI (OHCI/EHCI) Interface Power.
  \param[in]   state Power state
  \return      \ref execution_status
*/
static int32_t USBH_HCI_PowerControl (ARM_POWER_STATE state) {

  switch (state) {
    case ARM_POWER_OFF:
      NVIC_DisableIRQ      (USB1_IRQn);                 // Disable interrupt
      NVIC_ClearPendingIRQ (USB1_IRQn);                 // Clear pending interrupt
      USB1_state &= ~USBH_DRIVER_POWERED;               // Clear powered flag
#if (!RTE_USB_USB1_HS_PHY_EN)
      SCU_USB1_PinConfigure (SCU_USB1_PIN_CFG_ESEA);    // Reset SCU Register
#endif
      LPC_CCU1->CLK_USB1_CFG    &= ~1U;                 // Disable USB1 Base Clock
      while (LPC_CCU1->CLK_USB1_STAT    & 1U);
      LPC_CCU1->CLK_M3_USB1_CFG &= ~1U;                 // Disable USB1 Register Interface Clock
      while (LPC_CCU1->CLK_M3_USB1_STAT & 1U);
      LPC_CGU->BASE_USB1_CLK     =  0U;                 // Disable Base Clock
      break;

    case ARM_POWER_FULL:
      if ((USB1_state & USBH_DRIVER_POWERED) != 0U) { return ARM_DRIVER_OK; }

      LPC_CGU->BASE_USB1_CLK     = (0x01U << 11) |      // Auto-block Enable
                                   (0x0CU << 24) ;      // Clock source: IDIVA
      LPC_CCU1->CLK_M3_USB1_CFG |=  1U;                 // Enable USB1 Register Interface Clock
      while (!(LPC_CCU1->CLK_M3_USB1_STAT & 1U));
      LPC_CCU1->CLK_USB1_CFG    |=  1U;                 // Enable USB1 Base Clock
      while (!(LPC_CCU1->CLK_USB1_STAT    & 1U));

      // Clear Transceiver Selection
      LPC_USB1->PORTSC1_H &= ~(USB_PORTSC1_H_PTS_MSK | USB_PORTSC1_H_PFSC | USB_PORTSC1_H_PHCD);
#if  (RTE_USB_USB1_HS_PHY_EN)
      // ULPI Selected
      LPC_USB1->PORTSC1_H |=   USB_PORTSC1_H_PTS(2U);   // Activate ULPI
#else
      // Serial/1.1 PHY selected and Full-speed forced
      LPC_USB1->PORTSC1_H |=   USB_PORTSC1_H_PTS(3UL);
      SCU_USB1_PinConfigure (SCU_USB1_PIN_CFG_AIM  |
                             SCU_USB1_PIN_CFG_ESEA |
                             SCU_USB1_PIN_CFG_EPD  |
                             SCU_USB1_PIN_CFG_EPWR);
#endif

      USB1_state |=  USBH_DRIVER_POWERED;               // Set powered flag
      NVIC_EnableIRQ   (USB1_IRQn);                     // Enable interrupt
      break;

    default:
      return ARM_DRIVER_ERROR_UNSUPPORTED;
  }

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USBH_HCI_PortVbusOnOff (uint8_t port, bool vbus)
  \brief       USB Host HCI (OHCI/EHCI) Root HUB Port VBUS on/off.
  \param[in]   port  Root HUB Port Number
  \param[in]   vbus
                - \b false VBUS off
                - \b true  VBUS on
  \return      \ref execution_status
*/
static int32_t USBH_HCI_PortVbusOnOff (uint8_t port, bool power) {
  // No GPIO pins used for VBUS control it is controlled by EHCI Controller

  if (((1U << port) & usbh_ehci_driver_capabilities.port_mask) == 0U) { return ARM_DRIVER_ERROR; }
  return ARM_DRIVER_OK;
}

/**
  \fn          void USBH1_IRQ (void)
  \brief       USB1 Host Interrupt Routine (IRQ).
*/
void USBH1_IRQ (void) {
  EHCI_IRQ();
}

ARM_DRIVER_USBH_HCI Driver_USBH1_HCI = {
  USBH_HCI_GetVersion,
  USBH_HCI_GetCapabilities,
  USBH_HCI_Initialize,
  USBH_HCI_Uninitialize,
  USBH_HCI_PowerControl,
  USBH_HCI_PortVbusOnOff
};
