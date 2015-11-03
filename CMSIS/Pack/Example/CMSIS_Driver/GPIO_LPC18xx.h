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
 * $Date:        14. April 2014
 * $Revision:    V1.0
 *
 * Project:      GPIO Driver Definitions for NXP LPC18xx
 * -------------------------------------------------------------------------- */

#ifndef __GPIO_LPC18XX_H
#define __GPIO_LPC18XX_H

#include <stdint.h>

// GPIO identifier
typedef struct _GPIO_ID {
  uint8_t       port;
  uint8_t       num;
} GPIO_ID;

// GPIO Direction
#define GPIO_DIR_INPUT     (0)
#define GPIO_DIR_OUTPUT    (1)


/**
  \fn          void GPIO_PortClock (uint32_t clock)
  \brief       Port Clock Control
  \param[in]   clock  Enable or disable clock
*/
extern void GPIO_PortClock (uint32_t clock);

/**
  \fn          void GPIO_SetDir (uint32_t port_num,
                                 uint32_t pin_num,
                                 uint32_t dir)
  \brief       Configure GPIO pin direction
  \param[in]   port_num   GPIO number (0..7)
  \param[in]   pin_num    Port pin number
  \param[in]   dir        GPIO_DIR_INPUT, GPIO_DIR_OUTPUT
*/
extern void GPIO_SetDir (uint32_t port_num, uint32_t pin_num, uint32_t dir);

/**
  \fn          void GPIO_PinWrite (uint32_t port_num,
                                   uint32_t pin_num,
                                   uint32_t val)
  \brief       Write port pin
  \param[in]   port_num   GPIO number (0..7)
  \param[in]   pin_num    Port pin number
  \param[in]   val        Port pin value (0 or 1)
*/
extern void GPIO_PinWrite (uint32_t port_num,
                           uint32_t pin_num,
                           uint32_t val);

/**
  \fn          uint32_t  GPIO_PinRead (uint32_t port_num, uint32_t pin_num)
  \brief       Read port pin
  \param[in]   port_num   GPIO number (0..7)
  \param[in]   pin_num    Port pin number
  \return      pin value (0 or 1)
*/
extern uint32_t GPIO_PinRead (uint32_t port_num, uint32_t pin_num);

/**
  \fn          void GPIO_PortWrite (uint32_t port_num,
                                    uint32_t mask,
                                    uint32_t val)
  \brief       Write port pins
  \param[in]   port_num   GPIO number (0..7)
  \param[in]   mask       Selected pins
  \param[in]   val        Pin values
*/
extern void GPIO_PortWrite (uint32_t port_num, uint32_t mask, uint32_t val);

/**
  \fn          uint32_t  GPIO_PortRead (uint32_t port_num)
  \brief       Read port pins
  \param[in]   port_num   GPIO number (0..7)
  \return      port pin inputs
*/
extern uint32_t GPIO_PortRead (uint32_t port_num);

#endif /* __GPIO_LPC18XX_H */
