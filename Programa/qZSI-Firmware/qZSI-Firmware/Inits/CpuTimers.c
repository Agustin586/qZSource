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
#include "CpuTimers.h"
#include "../DelayUs.h"

/**********************************************************************
                   EXTERNAL FUNCTIONS DEFINITION
**********************************************************************/

/**********************************************************************
* Function: InitCpuTimers()
*
* Description: Initializes the CPU Timers on the F2833x
**********************************************************************/
extern void InitTimers(_iq cpuFreq, _iq interruptPeriod)
{
    //---------------------------------------------------------------------
    //--- Configure CPU Timer 0
    //---------------------------------------------------------------------

    // Initialize pre-scale counter to divide by 1 (Timer freq. = SYSCLKOUT):
    CpuTimer0Regs.TPR.all  = 0;
    CpuTimer0Regs.TPRH.all = 0;

    // Configure CpuTimer0 PRD to interrupt at desired period
    CpuTimer0Regs.PRD.all  = (long)_IQmpy(cpuFreq, interruptPeriod);

    CpuTimer0Regs.TCR.all = 0xC030;
    // bit 15        1:      TIF, interrupt flag, write 1 to clear
    // bit 14        1:      TIE, 1 = timer interrupt enabled
    // bit 13-12     0's:    reserved
    // bit 11-10     00:     FREE/SOFT, 00 = hard stop in emulation
    // bit 9-6       0's:    reserved
    // bit 5         1:      TRB, 1 = reload all counter register with period value
    // bit 4         1:      TSS, 1 = timer stopped
    // bit 3-0       0's:    reserved

    // Start CpuTimer0
    CpuTimer0Regs.TCR.bit.TSS = 0;

    //--- Enable the CpuTimer0 interrupt ---
	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;	// Enable TINT0 in PIE group 1
	IER |= M_INT1;						// Enable INT1 in IER to enable PIE group
}
