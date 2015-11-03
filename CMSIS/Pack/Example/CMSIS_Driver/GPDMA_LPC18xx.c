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
 * $Date:        16. June 2015
 * $Revision:    V1.2
 *
 * Project:      GPDMA Driver for NXP LPC18xx
 * -------------------------------------------------------------------------- */

/* History:
 *  Version 1.2
 *    - Added GPDMA_ChannelGetCount function.
 *  Version 1.1
 *    - Updated Initialize and Uninitialize functions
 */

#include "LPC18xx.h"
#include "GPDMA_LPC18xx.h"

// GPDMA Channel register block structure
typedef struct {
  __IO uint32_t  SRCADDR;       // DMA Channel Source Address Register
  __IO uint32_t  DESTADDR;      // DMA Channel Destination Address Register
  __IO uint32_t  LLI;           // DMA Channel Linked List Item Register
  __IO uint32_t  CONTROL;       // DMA Channel Control Register
  __IO uint32_t  CONFIG;        // DMA Channel Configuration Register
  __I  uint32_t  RESERVED1[3];
} GPDMA_CHANNEL_REG;

// GPDMA Callback pointers
static GPDMA_SignalEvent_t GPDMA_cb_event[GPDMA_NUMBER_OF_CHANNELS];

static uint32_t Channel_active = 0;
static uint32_t Init_cnt       = 0;

static uint32_t Channel_transfer_sz[GPDMA_NUMBER_OF_CHANNELS] = {0};

#define GPDMA_CHANNEL(n)  ((GPDMA_CHANNEL_REG *) (&(LPC_GPDMA->C0SRCADDR) + (n * 8)))


/**
  \fn          int32_t Set_Channel_active_flag (uint8_t ch)
  \brief       Protected set of channel active flag
  \param[in]   ch        Channel number (0..7)
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
__inline static int32_t Set_Channel_active_flag (uint8_t ch) {
  uint32_t val;

  do {
    val = __LDREXW (&Channel_active);
    if (val & (1 << ch)) {
      __CLREX (); 
      return -1;
    }
  } while (__STREXW (val | (1 << ch), &Channel_active));

  return 0;
}

/**
  \fn          void Clear_Channel_active_flag (uint8_t ch)
  \brief       Protected clear of channel active flag
  \param[in]   ch        Channel number (0..7)
*/
__inline static void Clear_Channel_active_flag (uint8_t ch) {
  while(__STREXW((__LDREXW(&Channel_active) & ~(1 << ch)), &Channel_active));
}

/**
  \fn          int32_t GPDMA_Initialize (void)
  \brief       Initialize GPDMA peripheral
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t GPDMA_Initialize (void) {
  uint32_t ch_num;

  Init_cnt++;

  // Chech if already initialized
  if (Init_cnt > 1) return 0;

  // Enable DMA clock
  LPC_CCU1->CLK_M3_DMA_CFG |= 1;
  while ((LPC_CCU1->CLK_M3_DMA_STAT & 1) == 0);

  // Reset DMA
  LPC_RGU->RESET_CTRL0 = (1 << 19);

  // Reset all DMA channels
  for (ch_num = 0; ch_num < GPDMA_NUMBER_OF_CHANNELS; ch_num++)
    GPDMA_CHANNEL(ch_num)->CONFIG = 0;

  // Clear all DMA interrupt flags
  LPC_GPDMA->INTTCCLEAR = 0xFF;
  LPC_GPDMA->INTERRCLR = 0xFF;

  // Clear and Enable DMA IRQ
  NVIC_ClearPendingIRQ(DMA_IRQn);
  NVIC_EnableIRQ(DMA_IRQn);

  return 0;
}

/**
  \fn          int32_t GPDMA_Uninitialize (void)
  \brief       De-initialize GPDMA peripheral
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t GPDMA_Uninitialize (void) {

  // Check if DMA is initialized
  if (Init_cnt == 0) return -1;

  Init_cnt--;
  if (Init_cnt) return 0;

  // Disable DMA clock
  LPC_CCU1->CLK_M3_DMA_CFG &= ~1;

  // Disable and Clear DMA IRQ
  NVIC_DisableIRQ(DMA_IRQn);
  NVIC_ClearPendingIRQ(DMA_IRQn);

  return 0;
}

/**
  \fn          int32_t GPDMA_PeripheralSelect (uint8_t peri, uint8_t sel)
  \brief       Configure GPDMA multiplexer
  \param[in]   peri GPDMA peripheral (0..15)
  \param[in]   sel  GPDMA multiplexer peripheral selection (0..3)
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t GPDMA_PeripheralSelect (uint8_t peri, uint8_t sel) {

  if ((peri > 15) || (sel > 3)) return -1;

  LPC_CREG->DMAMUX &= ~(3    << (2 * peri));
  LPC_CREG->DMAMUX |=  (sel  << (2 * peri));

  return 0;
}

/**
  \fn          int32_t GPDMA_ChannelConfigure (uint8_t              ch,
                                               uint32_t             src_addr,
                                               uint32_t             dest_addr,
                                               uint32_t             control,
                                               uint32_t             config,
                                               GPDMA_SignalEvent_t  cb_event)
  \brief       Configure GPDMA channel for next transfer
  \param[in]   ch        Channel number (0..7)
  \param[in]   src_addr  Source address
  \param[in]   dest_addr Destination address
  \param[in]   control   Channel control
  \param[in]   config    Channel configuration
  \param[in]   cb_event  Channel callback pointer
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t GPDMA_ChannelConfigure (uint8_t              ch,
                                uint32_t             src_addr,
                                uint32_t             dest_addr,
                                uint32_t             control,
                                uint32_t             config,
                                GPDMA_SignalEvent_t  cb_event) {
  GPDMA_CHANNEL_REG * dma_ch;

  // Check if channel is valid
  if (ch >= GPDMA_NUMBER_OF_CHANNELS)     return -1;

  // Set Channel active flag
  if (Set_Channel_active_flag (ch) == -1) return -1;

  // Save callback pointer
  GPDMA_cb_event[ch] = cb_event;

  dma_ch = GPDMA_CHANNEL(ch);

  // Reset DMA Channel configuration
  dma_ch->CONFIG  = 0;
  dma_ch->CONTROL = 0;

  // Clear DMA interrupts
  LPC_GPDMA->INTTCCLEAR = (1 << ch);
  LPC_GPDMA->INTERRCLR  = (1 << ch);

  // Linker list not   supported
  dma_ch->LLI = 0;

  // Enable DMA Channels, little endian
  LPC_GPDMA->CONFIG = GPDMA_CONFIG_E;
  while ((LPC_GPDMA->CONFIG & GPDMA_CONFIG_E) == 0);

  // Set Source and destination address
  dma_ch->SRCADDR  = src_addr;
  dma_ch->DESTADDR = dest_addr;

  dma_ch->CONTROL = control;

  // Save transfer size
  Channel_transfer_sz[ch] = control & GPDMA_CH_CONTROL_TRANSFERSIZE_MSK;

  dma_ch->CONFIG  = config;

  if ((config & GPDMA_CONFIG_E) == 0) {
    // Clear Channel active flag
    Clear_Channel_active_flag (ch);
  }

  return 0;
}

/**
  \fn          int32_t GPDMA_ChannelEnable (uint8_t ch)
  \brief       Enable GPDMA channel
  \param[in]   ch Channel number (0..7)
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t GPDMA_ChannelEnable (uint8_t ch) {

  // Check if channel is valid
  if (ch >= GPDMA_NUMBER_OF_CHANNELS)     return -1;

  // Set Channel active flag
  if (Set_Channel_active_flag (ch) == -1) return -1;

  GPDMA_CHANNEL(ch)->CONFIG |= GPDMA_CH_CONFIG_E;
  return 0;
}

/**
  \fn          int32_t GPDMA_ChannelDisable (uint8_t ch)
  \brief       Disable GPDMA channel
  \param[in]   ch Channel number (0..7)
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t GPDMA_ChannelDisable (uint8_t ch) {

  // Check if channel is valid
  if (ch >= GPDMA_NUMBER_OF_CHANNELS) return -1;

  // Clear Channel active flag
  Clear_Channel_active_flag (ch);

  GPDMA_CHANNEL(ch)->CONFIG &= ~GPDMA_CH_CONFIG_E;

  return 0;
}

/**
  \fn          uint32_t GPDMA_ChannelGetStatus (uint8_t ch)
  \brief       Check if GPDMA channel is enabled or disabled
  \param[in]   ch Channel number (0..7)
  \returns     Channel status
   - \b  1: channel enabled
   - \b  0: channel disabled
*/
uint32_t GPDMA_ChannelGetStatus (uint8_t ch) {

  // Check if channel is valid
  if (ch >= GPDMA_NUMBER_OF_CHANNELS) return 0;

  if (Channel_active & (1 << ch)) return 1;
  else                            return 0;
}

/**
  \fn          uint32_t GPDMA_ChannelGetCount (uint8_t ch)
  \brief       Get number of transfered data
  \param[in]   ch Channel number (0..7)
  \returns     Numer of transfered data
*/
uint32_t GPDMA_ChannelGetCount (uint8_t ch) {

  // Check if channel is valid
  if (ch >= GPDMA_NUMBER_OF_CHANNELS) return 0;

  return (Channel_transfer_sz[ch] - (GPDMA_CHANNEL(ch)->CONTROL & GPDMA_CH_CONTROL_TRANSFERSIZE_MSK));
}

/**
  \fn          void DMA_IRQHandler (void)
  \brief       DMA interrupt handler
*/
void DMA_IRQHandler (void) {
  uint32_t ch, event;

  for (ch = 0; ch < GPDMA_NUMBER_OF_CHANNELS; ch++) {
    if (LPC_GPDMA->INTSTAT & (1 << ch)) {
      event = 0;
      // Terminal count request interrupt
      if (LPC_GPDMA->INTTCSTAT & (1 << ch)) {
        event |= GPDMA_EVENT_TERMINAL_COUNT_REQUEST;
        LPC_GPDMA->INTTCCLEAR = (1 << ch);
      }

      // DMA error interrupt
      if (LPC_GPDMA->INTERRSTAT & (1 << ch)) {
        event |= GPDMA_EVENT_ERROR;
        LPC_GPDMA->INTERRCLR = (1 << ch);
      }

      // Clear Channel active flag
      Clear_Channel_active_flag (ch);

      // Signal Event
      if (GPDMA_cb_event[ch])
        GPDMA_cb_event[ch](event);
    }
  }
}
