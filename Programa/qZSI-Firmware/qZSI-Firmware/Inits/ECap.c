/*
 * Proyecto Final de Ing. Electronica - TM
 *
 * Copyright 2024, Guido Cicconi - Gustavo Garcia - Juan Tinazzo
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**********************************************************************
                             INCLUDES
**********************************************************************/
#include "ECap.h"

/**********************************************************************
                   EXTERNAL FUNCTIONS DEFINITION
**********************************************************************/

/**********************************************************************
* Function: InitECap()
*
* Description: Initializes the Enhanced Capture modules on the F2833x.
**********************************************************************/
extern void InitECap(void)
{
    //---------------------------------------------------------------------
    //--- Configure eCAP1 unit for capture
    //---------------------------------------------------------------------
	ECap1Regs.ECEINT.all = 0;					// Disable all eCAP interrupts
	ECap1Regs.ECCTL1.bit.CAPLDEN = 0;			// Disabled loading of capture results
	ECap1Regs.ECCTL2.bit.TSCTRSTOP = 0;			// Stop the counter

	ECap1Regs.TSCTR = 0;						// Clear the counter
	ECap1Regs.CTRPHS = 0;						// Clear the counter phase register

	ECap1Regs.ECCTL2.all = 0x0096;				// ECAP control register 2
    // bit 15-11     00000:  reserved
    // bit 10        0:      APWMPOL, don't care
    // bit 9         0:      CAP/APWM, 0 = capture mode, 1 = APWM mode
    // bit 8         0:      SWSYNC, 0 = no action (no s/w synch)
    // bit 7-6       10:     SYNCO_SEL, 10 = disable sync out signal
    // bit 5         0:      SYNCI_EN, 0 = disable Sync-In
    // bit 4         1:      TSCTRSTOP, 1 = enable counter
    // bit 3         0:      RE-ARM, 0 = don't re-arm, 1 = re-arm
    // bit 2-1       11:     STOP_WRAP, 11 = wrap after 4 captures
    // bit 0         0:      CONT/ONESHT, 0 = continuous mode

	ECap1Regs.ECCTL1.all = 0xC144;				// ECAP control register 1
    // bit 15-14     11:     FREE/SOFT, 11 = ignore emulation suspend
    // bit 13-9      00000:  PRESCALE, 00000 = divide by 1
    // bit 8         1:      CAPLDEN, 1 = enable capture results load
    // bit 7         0:      CTRRST4, 0 = do not reset counter on CAP4 event
    // bit 6         1:      CAP4POL, 0 = rising edge, 1 = falling edge
    // bit 5         0:      CTRRST3, 0 = do not reset counter on CAP3 event
    // bit 4         0:      CAP3POL, 0 = rising edge, 1 = falling edge
    // bit 3         0:      CTRRST2, 0 = do not reset counter on CAP2 event
    // bit 2         1:      CAP2POL, 0 = rising edge, 1 = falling edge
    // bit 1         0:      CTRRST1, 0 = do not reset counter on CAP1 event
    // bit 0         0:      CAP1POL, 0 = rising edge, 1 = falling edge

	ECap1Regs.ECEINT.all = 0x0008;				// Enable desired eCAP interrupts
    // bit 15-8      0's:    reserved
    // bit 7         0:      CTR=CMP, 0 = compare interrupt disabled
    // bit 6         0:      CTR=PRD, 0 = period interrupt disabled
    // bit 5         0:      CTROVF, 0 = overflow interrupt disabled
    // bit 4         0:      CEVT4, 0 = event 4 interrupt disabled
    // bit 3         1:      CEVT3, 1 = event 3 interrupt enabled
    // bit 2         0:      CEVT2, 0 = event 2 interrupt disabled
    // bit 1         0:      CEVT1, 0 = event 1 interrupt disabled
    // bit 0         0:      reserved

	PieCtrlRegs.PIEIER4.bit.INTx1 = 1;	// Enable ECAP1_INT in PIE group 4
	IER |= 0x0008;						// Enable INT4 in IER to enable PIE group 4
}
