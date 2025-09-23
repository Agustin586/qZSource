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
#include "Dma.h"

/**********************************************************************
                   EXTERNAL FUNCTIONS DEFINITION
**********************************************************************/

/**********************************************************************
* Function: InitDma()
*
* Description: Initializes the DMA on the F2833x
**********************************************************************/
extern void InitDma(volatile Uint16 *dest_buffer)
{
	asm(" EALLOW");								// Enable EALLOW protected register access

    //---------------------------------------------------------------------
    //--- Overall DMA setup
    //---------------------------------------------------------------------
	DmaRegs.DMACTRL.bit.HARDRESET = 1;			// Reset entire DMA module
	asm(" NOP");								// 1 cycle delay for HARDRESET to take effect

	DmaRegs.DEBUGCTRL.bit.FREE = 1;				// 1 = DMA unaffected by emulation halt
	DmaRegs.PRIORITYCTRL1.bit.CH1PRIORITY = 0;	// Not using CH1 Priority mode

    //---------------------------------------------------------------------
    //--- Configure DMA channel 1 to read the ADC active vars results
    //---------------------------------------------------------------------
	DmaRegs.CH1.MODE.all = 0x0D01;
    // bit 15        0:      CHINTE, 0=interrupt disabled, 1=interrupt enabled
    // bit 14        0:      DATASIZE, 0=16-bit, 1=32-bit
    // bit 13        0:      SYNCSEL, sync select, don't care
    // bit 12        0:      SYNCE, ADC sync enable, 0=disabled, 1=enabled
    // bit 11        1:      CONTINUOUS, 0=stop, 1=re-init after transfer complete
    // bit 10        1:      ONESHOT, 0=one burst on trigger, 1=all bursts on trigger
    // bit 9         0:      CHINTMODE, 0=start of transfer, 1=end of transfer
    // bit 8         1:      PERINTE, peripheral interrupt trigger enable, 0=disabled, 1=enabled
    // bit 7         0:      OVRINTE, overflow interrupt enable, 0=disabled, 1=enabled
    // bit 6-5       00:     reserved
    // bit 4-0       00001:  PERINTSEL, 1=ADC SEQ1INT

	DmaRegs.CH1.BURST_SIZE.bit.BURSTSIZE = 0;							    // 0 means 1 word per burst
	DmaRegs.CH1.TRANSFER_SIZE = 4;							                // 5 bursts per transfer

	DmaRegs.CH1.SRC_TRANSFER_STEP = 1;				                        // 1 means add 1 to pointer each burst in a transfer
	DmaRegs.CH1.SRC_WRAP_SIZE = 0xFF;						                // make greater than TRANSFER_SIZE to not use wrap feature
	DmaRegs.CH1.SRC_WRAP_STEP = 0;							                // 0 means no change to wrap address on wrap
	DmaRegs.CH1.SRC_ADDR_SHADOW = (Uint32)&AdcMirror.ADCRESULT0;		    // SRC start address
	DmaRegs.CH1.SRC_BEG_ADDR_SHADOW = (Uint32)&AdcMirror.ADCRESULT0;	    // SRC wrap start address

	DmaRegs.CH1.DST_TRANSFER_STEP = 1;						                // 1 = add 1 to pointer each burst in a transfer
	DmaRegs.CH1.DST_WRAP_SIZE = 0xFF;									    // make greater than TRANSFER_SIZE to not use wrap feature
	DmaRegs.CH1.DST_ADDR_SHADOW = (Uint32)dest_buffer;					    // DST start address


	DmaRegs.CH1.CONTROL.all = 0x00D1;
    // bit 15        0:      reserved
    // bit 14        0;      OVRFLG, overflow flag, read-only
    // bit 13        0;      RUNSTS, run status, read-only
    // bit 12        0;      BURSTSTS, burst status, read-only
    // bit 11        0;      TRANSFERSTS, transfer status, read-only
    // bit 10        0;      SYNCERR, sync error, read-only
    // bit 9         0;      SYNCFLG, sync flag, read-only
    // bit 8         0;      PERINTFLG, read-only
    // bit 7         1;      ERRCLR, error clear, 0=no action, 1=clear SYNCERR bit
    // bit 6         1;      SYNCCLR, sync clear, 0=no action, 1=clear SYNCFLG bit
    // bit 5         0;      SYNCFRC, sync force, 0=no action, 1=force sync event
    // bit 4         1;      PERINTCLR, periph event clear, 0=no action, 1=clear periph event
    // bit 3         0;      PERINTFRC, periph event force, 0=no action, 1=force periph event
    // bit 2         0;      SOFTRESET, 0=no action, 1=soft reset the channel
    // bit 1         0;      HALT, 0=no action, 1=halt the channel
    // bit 0         1;      RUN, 0=no action, 1=enable the channel

    //---------------------------------------------------------------------
    //--- Configure DMA channel 2 to read the ADC ST vars results
    //---------------------------------------------------------------------
    DmaRegs.CH2.MODE.all = 0x0D02;
    // bit 15        0:      CHINTE, 0=interrupt disabled, 1=interrupt enabled
    // bit 14        0:      DATASIZE, 0=16-bit, 1=32-bit
    // bit 13        0:      SYNCSEL, sync select, don't care
    // bit 12        0:      SYNCE, ADC sync enable, 0=disabled, 1=enabled
    // bit 11        1:      CONTINUOUS, 0=stop, 1=re-init after transfer complete
    // bit 10        1:      ONESHOT, 0=one burst on trigger, 1=all bursts on trigger
    // bit 9         0:      CHINTMODE, 0=start of transfer, 1=end of transfer
    // bit 8         1:      PERINTE, peripheral interrupt trigger enable, 0=disabled, 1=enabled
    // bit 7         0:      OVRINTE, overflow interrupt enable, 0=disabled, 1=enabled
    // bit 6-5       00:     reserved
    // bit 4-0       00010:  PERINTSEL, 1=ADC SEQ2INT

    DmaRegs.CH2.BURST_SIZE.bit.BURSTSIZE = 0;                               // 0 means 1 word per burst
    DmaRegs.CH2.TRANSFER_SIZE = 3;                                          // 4 bursts per transfer

    DmaRegs.CH2.SRC_TRANSFER_STEP = 1;                                      // 1 means add 1 to pointer each burst in a transfer
    DmaRegs.CH2.SRC_WRAP_SIZE = 0xFF;                                       // make greater than TRANSFER_SIZE to not use wrap feature
    DmaRegs.CH2.SRC_WRAP_STEP = 0;                                          // 0 means no change to wrap address on wrap
    DmaRegs.CH2.SRC_ADDR_SHADOW = (Uint32)&AdcMirror.ADCRESULT8;            // SRC start address
    DmaRegs.CH2.SRC_BEG_ADDR_SHADOW = (Uint32)&AdcMirror.ADCRESULT8;        // SRC wrap start address

    DmaRegs.CH2.DST_TRANSFER_STEP = 1;                                      // 1 = add 1 to pointer each burst in a transfer
    DmaRegs.CH2.DST_WRAP_SIZE = 0xFF;                                       // make greater than TRANSFER_SIZE to not use wrap feature
    DmaRegs.CH2.DST_ADDR_SHADOW = (Uint32)(dest_buffer+5);                  // DST start address


    DmaRegs.CH2.CONTROL.all = 0x00D1;
    // bit 15        0:      reserved
    // bit 14        0;      OVRFLG, overflow flag, read-only
    // bit 13        0;      RUNSTS, run status, read-only
    // bit 12        0;      BURSTSTS, burst status, read-only
    // bit 11        0;      TRANSFERSTS, transfer status, read-only
    // bit 10        0;      SYNCERR, sync error, read-only
    // bit 9         0;      SYNCFLG, sync flag, read-only
    // bit 8         0;      PERINTFLG, read-only
    // bit 7         1;      ERRCLR, error clear, 0=no action, 1=clear SYNCERR bit
    // bit 6         1;      SYNCCLR, sync clear, 0=no action, 1=clear SYNCFLG bit
    // bit 5         0;      SYNCFRC, sync force, 0=no action, 1=force sync event
    // bit 4         1;      PERINTCLR, periph event clear, 0=no action, 1=clear periph event
    // bit 3         0;      PERINTFRC, periph event force, 0=no action, 1=force periph event
    // bit 2         0;      SOFTRESET, 0=no action, 1=soft reset the channel
    // bit 1         0;      HALT, 0=no action, 1=halt the channel
    // bit 0         1;      RUN, 0=no action, 1=enable the channel

    //--- Finish up
	asm(" EDIS");						// Disable EALLOW protected register access

    //--- Enable the DMA interrupt
    //	PieCtrlRegs.PIEIER7.bit.INTx1 = 1;	// Enable DINTCH1 in PIE group 7
    //	IER |= 0x0040;						// Enable INT7 in IER to enable PIE group

}
