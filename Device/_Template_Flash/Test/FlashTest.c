/******************************************************************************/
/*  This file is part of the uVision/ARM development tools                    */
/* Copyright (c) 2010 KEIL - An ARM Company. All rights reserved.             */
/******************************************************************************/
/*                                                                            */
/*  TEST.C:  Test for Flash Programming Functions                             */
/*                                                                            */
/******************************************************************************/

#include "..\..\FlashOS.H"                          // FlashOS Structures


#define M8(adr) (*((volatile unsigned char  *) (adr)))

extern struct FlashDevice const FlashDevice;


volatile int  ret;                                  // Return Code
unsigned char buf[1024];                            // Programming Buffer



/*----------------------------------------------------------------------------
  Main Function
 *----------------------------------------------------------------------------*/
int main (void) {
  unsigned long n;

  // Programming Test Pattern
  for (n = 0; n < FlashDevice.szPage; n++) {
    buf[n] = (unsigned char)n;
  }

  /* Test EraseSector Function -----------------------------------------------*/
  ret  = Init(0x08000000, 8000000, 1);              // Initialize Flash Programming Functions
                                                    //   Device Base Address = 0x08000000
                                                    //   Clock Frequency = 8MHz

  for (n = FlashDevice.DevAdr; n < (FlashDevice.DevAdr + FlashDevice.szDev); n += FlashDevice.szPage) {
    ret |= EraseSector(n);                          // Test Sector Erase
  }

                                                    // Verify Erase
  for (n = 0; n < FlashDevice.szDev; n++) {
    if (M8(FlashDevice.DevAdr + n) != FlashDevice.valEmpty) {
      ret = 1;                                      // Error
      break;
    }
  }

  ret |= UnInit (1);                                // Uninitialize Flash Programming Functions


  /* Test ProgramPage Function -----------------------------------------------*/
  ret |= Init(0x08000000, 8000000, 2);              // Initialize Flash Programming Functions

  for (n = FlashDevice.DevAdr; n < (FlashDevice.DevAdr + FlashDevice.szDev); n += FlashDevice.szPage) {
    ret |= ProgramPage(n, FlashDevice.szPage, buf); // Test Page Programming
  }

                                                    // Verify Programm Page
  for (n = 0; n < FlashDevice.szDev; n++) {
    if (M8(FlashDevice.DevAdr + n) != ((unsigned char)n)) {
      ret = 1;                                      // Error
      break;
    }
  }

  ret |= UnInit (2);                                // Uninitialize Flash Programming Functions


  /* Test EraseChip Function -------------------------------------------------*/
  ret |= Init(0x08000000, 8000000, 1);              // Initialize Flash Programming Functions

  ret |= EraseChip();                               // Test Erase Chip
                                                    // Verify Erase
  for (n = 0; n < FlashDevice.szDev; n++) {
    if (M8(FlashDevice.DevAdr + n) != FlashDevice.valEmpty) {
      ret = 1;                                      // Error
      break;
    }
  }

  ret |= UnInit (1);                                // Uninitialize Flash Programming Functions


  while (1);                                        // Wait forever
}
