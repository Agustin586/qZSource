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
#include "EPwm.h"

/**********************************************************************
                   EXTERNAL FUNCTIONS DEFINITION
**********************************************************************/

/**********************************************************************
* Function: InitEPwm()
*
* Description: Initializes the Enhanced PWM modules on the F2833x
**********************************************************************/
extern void InitEPwm(void)
{
    //---------------------------------------------------------------------
    //--- Must disable the clock to the ePWM modules if you
    //--- want all ePMW modules synchronized.
    //---------------------------------------------------------------------
    asm(" EALLOW");						// Enable EALLOW protected register access
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
    asm(" EDIS");						// Disable EALLOW protected register access

    //---------------------------------------------------------------------
    //--- Configure ePWM1
    //---------------------------------------------------------------------
    EPwm1Regs.TBCTL.bit.CTRMODE = 0x3;		// Disable the timer

    EPwm1Regs.TBCTL.all = 0xC013;			// Configure timer control register
    // bit 15-14     11:     FREE/SOFT, 11 = ignore emulation suspend
    // bit 13        0:      PHSDIR, 0 = count down after sync event
    // bit 12-10     000:    CLKDIV, 000 => TBCLK = HSPCLK/1
    // bit 9-7       000:    HSPCLKDIV, 000 => HSPCLK = SYSCLKOUT/1
    // bit 6         0:      SWFSYNC, 0 = no software sync produced
    // bit 5-4       01:     SYNCOSEL, 01 = EPWMxSYNCO signal source is CTR=zero
    // bit 3         0:      PRDLD, 0 = reload PRD on counter=0
    // bit 2         0:      PHSEN, 0 = phase control disabled
    // bit 1-0       11:     CTRMODE, 11 = timer stopped (disabled)

    EPwm1Regs.TBCTR = 0x0000;				// Clear timer counter
    EPwm1Regs.TBPRD = PWM_PERIOD;		        // Set timer period
    EPwm1Regs.TBPHS.half.TBPHS = 0x0000;	// Set timer phase

    EPwm1Regs.CMPA.half.CMPA = 0;
    EPwm1Regs.CMPB = (int)(PWM_PERIOD);

    EPwm1Regs.CMPCTL.all = 0x0000;			// Compare control register
    // bit 15-10     0's:    reserved
    // bit 9         0:      SHDWBFULL, read-only
    // bit 8         0:      SHDWAFULL, read-only
    // bit 7         0:      reserved
    // bit 6         0:      SHDWBMODE, don't care
    // bit 5         0:      reserved
    // bit 4         0:      SHDWAMODE, 0 = shadow mode
    // bit 3-2       00:     LOADBMODE, 00 = load on zero
    // bit 1-0       00:     LOADAMODE, 00 = load on zero

    EPwm1Regs.AQCTLA.all = 0x0212;		// Action-qualifier control register A
    // bit 15-12     0000:   reserved
    // bit 11-10     00:     CBD, 00 = do nothing
    // bit 9-8       10:     CBU, 10 = set
    // bit 7-6       00:     CAD, 00 = do nothing
    // bit 5-4       01:     CAU, 01 = clear
    // bit 3-2       00:     PRD, 00 = do nothing
    // bit 1-0       10:     ZRO, 10 = set

    EPwm1Regs.AQCTLB.all = 0x0122;      // Action-qualifier control register B
    // bit 15-12     0000:   reserved
    // bit 11-10     00:     CBD, 00 = do nothing
    // bit 9-8       01:     CBU, 01 = clear
    // bit 7-6       00:     CAD, 00 = do nothing
    // bit 5-4       10:     CAU, 10 = set
    // bit 3-2       00:     PRD, 00 = do nothing
    // bit 1-0       10:     ZRO, 10 = set

    EPwm1Regs.AQSFRC.all = 0x0000;		// Action-qualifier s/w force register
    // bit 15-8      0's:    reserved
    // bit 7-6       00:     RLDCSF, 00 = reload AQCSFRC on zero
    // bit 5         0:      OTSFB, 0 = do not initiate a s/w forced event on output B
    // bit 4-3       00:     ACTSFB, don't care
    // bit 2         0:      OTSFA, 0 = do not initiate a s/w forced event on output A
    // bit 1-0       00:     ACTSFA, don't care

    EPwm1Regs.AQCSFRC.all = 0x0000;		// Action-qualifier continuous s/w force register
    // bit 15-4      0's:    reserved
    // bit 3-2       00:     CSFB, 00 = forcing disabled
    // bit 1-0       00:     CSFA, 00 = forcing disabled

    EPwm1Regs.DBCTL.all = 0x0020;       //	Dead-Band Generator Control Register
    // bit 15-6      0's:    reserved
    // bit 5-4       10:     IN_MODE, 10 = EPWMxA In (from the action-qualifier) is the source for rising-edge delayed signal
    // bit 3-2       00:     POLSEL, 00 = active high mode
    // bit 1-0       00:     OUT_MODE, 00 = dead band generation bypassed

    EPwm1Regs.DBRED = 0;                //  Dead-Band Generator Rising Edge Delay Register
    EPwm1Regs.DBFED = 0;                //  Dead-Band Generator Falling Edge Delay Register


    EPwm1Regs.ETSEL.all = 0x1202;       //  Event-Trigger Selection Register
    // bit 15        0:      SOCBEN, 0 = Enable EPWMxSOCB.
    // bit 14-12     001:    SOCBSEL, 001 = Enable event time-base counter equal to zero
    // bit 11        0:      SOCAEN, 0 = Disable EPWMxSOCA.
    // bit 10-8      010:    SOCASEL, 010 = Enable event time-base counter equal to period
    // bit 7-4       0's:    reserved
    // bit 3         0:      INTEN, 0 = Disable EPWMx_INT generation
    // bit 2-0       010:    INTSEL, 010 = Enable event time-base counter equal to period

    EPwm1Regs.ETPS.all = 0x1101;       //  Event-Trigger Prescale Register
    // bit 15-14     00:     SOCBCNT, ePWM ADC Start-of-Conversion B Event (EPWMxSOCB) Counter Register
    // bit 13-12     01:     SOCBPRD, 01 = Generate the EPWMxSOCB pulse on the first event: ETPS[SOCBCNT] = 0,1
    // bit 11-10     00:     SOCACNT, ePWM ADC Start-of-Conversion A Event (EPWMxSOCA) Counter Register
    // bit 9-8       01:     SOCAPRD, 01 =  Generate the EPWMxSOCA pulse on the first event: ETPS[SOCACNT] = 0,1
    // bit 7-4       0's:    reserved
    // bit 3-2       00:     INTCNT, ePWM Interrupt Event (EPWMx_INT) Counter Register
    // bit 1-0       01:     INTPRD, 01 = Generate an interrupt on the first event INTCNT = 0,1 (first event)

    EPwm1Regs.PCCTL.all = 0x0000;       // PWM-Chopper Control Register
    // bit 15-11     0's:    reserved
    // bit 10-8      000:    CHPDUTY
    // bit 7-5       000:    CHPFREQ
    // bit 4-1       0000:   OSHTWTH
    // bit 0         0:      CHPEN, 0 = Disable (bypass) PWM chopping function

    asm(" EALLOW");

    EPwm1Regs.TZSEL.all = 0x1000;       // Trip-Zone Select Register
    // bit 15-14     0's:    reserved
    // bit 13        0:      OSHT6
    // bit 12        1:      OSHT5
    // bit 11        0:      OSHT4
    // bit 10        0:      OSHT3
    // bit 9         0:      OSHT2
    // bit 8         0:      OSHT1
    // bit 7-6       0's:    reserved
    // bit 5         0:      CBC6
    // bit 4         0:      CBC5
    // bit 3         0:      CBC4
    // bit 2         0:      CBC3
    // bit 1         0:      CBC2
    // bit 0         0:      CBC1

    EPwm1Regs.TZCTL.all = 0x000A;       // Trip-Zone Control Register
    // bit 15-4      0's:    reserved
    // bit 3-2       10:     TZB, 10 = Force EPWMxB to a low state.
    // bit 1-0       10:     TZA, 10 = Force EPWMxA to a low state

    EPwm1Regs.TZEINT.all = 0x0000;       // Trip-Zone Enable Interrupt Register
    // bit 15-3      0's:    reserved
    // bit 2         0:      OST, 0 = Disable one-shot trip event interrupt generation.
    // bit 1         0:      CBC, 0 = Disable cycle-by-cycle interrupt generation.
    // bit 0         0:      reserved

    asm(" EDIS");

    EPwm1Regs.TBCTL.bit.CTRMODE = 0x0;	// Enable the timer in count up mode


    //------------------------------------------------------------------------------------------------
    //--- Configure ePWM2
    //------------------------------------------------------------------------------------------------
    EPwm2Regs.TBCTL.bit.CTRMODE = 0x3;      // Disable the timer

    EPwm2Regs.TBCTL.all = 0xC007;           // Configure timer control register
    // bit 15-14     11:     FREE/SOFT, 11 = ignore emulation suspend
    // bit 13        0:      PHSDIR, 0 = count down after sync event
    // bit 12-10     000:    CLKDIV, 000 => TBCLK = HSPCLK/1
    // bit 9-7       000:    HSPCLKDIV, 000 => HSPCLK = SYSCLKOUT/1
    // bit 6         0:      SWFSYNC, 0 = no software sync produced
    // bit 5-4       00:     SYNCOSEL, 00 = EPWMxSYNCO signal source is EPWMxSYNC
    // bit 3         0:      PRDLD, 0 = reload PRD on counter=0
    // bit 2         1:      PHSEN, 1 = phase control enabled
    // bit 1-0       11:     CTRMODE, 11 = timer stopped (disabled)

    EPwm2Regs.TBCTR = 0x0000;               // Clear timer counter
    EPwm2Regs.TBPRD = PWM_PERIOD;                // Set timer period
    EPwm2Regs.TBPHS.half.TBPHS = 0x0000;    // Set timer phase

    EPwm2Regs.CMPA.half.CMPA = 0;
    EPwm2Regs.CMPB = (int)(_IQmpy(_IQ(1.1), PWM_PERIOD));

    EPwm2Regs.CMPCTL.all = 0x0000;          // Compare control register
    // bit 15-10     0's:    reserved
    // bit 9         0:      SHDWBFULL, read-only
    // bit 8         0:      SHDWAFULL, read-only
    // bit 7         0:      reserved
    // bit 6         0:      SHDWBMODE, don't care
    // bit 5         0:      reserved
    // bit 4         0:      SHDWAMODE, 0 = shadow mode
    // bit 3-2       00:     LOADBMODE, 00 = load on zero
    // bit 1-0       00:     LOADAMODE, 00 = load on zero

    EPwm2Regs.AQCTLA.all = 0x0012;      // Action-qualifier control register A
    // bit 15-12     0000:   reserved
    // bit 11-10     00:     CBD, 00 = do nothing
    // bit 9-8       00:     CBU, 00 = do nothing
    // bit 7-6       00:     CAD, 00 = do nothing
    // bit 5-4       01:     CAU, 01 = clear
    // bit 3-2       00:     PRD, 00 = do nothing
    // bit 1-0       10:     ZRO, 10 = set

    EPwm2Regs.AQCTLB.all = 0x0102;      // Action-qualifier control register B
    // bit 15-12     0000:   reserved
    // bit 11-10     00:     CBD, 00 = do nothing
    // bit 9-8       01:     CBU, 01 = clear
    // bit 7-6       00:     CAD, 00 = do nothing
    // bit 5-4       00:     CAU, 00 = do nothing
    // bit 3-2       00:     PRD, 00 = do nothing
    // bit 1-0       10:     ZRO, 10 = set

    EPwm2Regs.AQSFRC.all = 0x0000;      // Action-qualifier s/w force register
    // bit 15-8      0's:    reserved
    // bit 7-6       00:     RLDCSF, 00 = reload AQCSFRC on zero
    // bit 5         0:      OTSFB, 0 = do not initiate a s/w forced event on output B
    // bit 4-3       00:     ACTSFB, don't care
    // bit 2         0:      OTSFA, 0 = do not initiate a s/w forced event on output A
    // bit 1-0       00:     ACTSFA, don't care

    EPwm2Regs.AQCSFRC.all = 0x0000;     // Action-qualifier continuous s/w force register
    // bit 15-4      0's:    reserved
    // bit 3-2       00:     CSFB, 00 = forcing disabled
    // bit 1-0       00:     CSFA, 00 = forcing disabled

    EPwm2Regs.DBCTL.all = 0x0020;       //  Dead-Band Generator Control Register
    // bit 15-6      0's:    reserved
    // bit 5-4       10:     IN_MODE, 10 = EPWMxA In (from the action-qualifier) is the source for rising-edge delayed signal
    // bit 3-2       00:     POLSEL, 00 = active high mode
    // bit 1-0       00:     OUT_MODE, 00 = dead band generation bypassed

    EPwm2Regs.DBRED = 0;                //  Dead-Band Generator Rising Edge Delay Register
    EPwm2Regs.DBFED = 0;                //  Dead-Band Generator Falling Edge Delay Register


    EPwm2Regs.ETSEL.all = 0x1201;       //  Event-Trigger Selection Register
    // bit 15        0:      SOCBEN, 0 = Enable EPWMxSOCB.
    // bit 14-12     001:    SOCBSEL, 001 = Enable event time-base counter equal to zero
    // bit 11        0:      SOCAEN, 0 = Disable EPWMxSOCA.
    // bit 10-8      010:    SOCASEL, 010 = Enable event time-base counter equal to period
    // bit 7-4       0's:    reserved
    // bit 3         0:      INTEN, 0 = Disable EPWMx_INT generation
    // bit 2-0       001:    INTSEL, 001 = Enable event time-base counter equal to zero

    EPwm2Regs.ETPS.all = 0x1101;       //  Event-Trigger Prescale Register
    // bit 15-14     00:     SOCBCNT, ePWM ADC Start-of-Conversion B Event (EPWMxSOCB) Counter Register
    // bit 13-12     01:     SOCBPRD, 01 = Generate the EPWMxSOCB pulse on the first event: ETPS[SOCBCNT] = 0,1
    // bit 11-10     00:     SOCACNT, ePWM ADC Start-of-Conversion A Event (EPWMxSOCA) Counter Register
    // bit 9-8       01:     SOCAPRD, 01 =  Generate the EPWMxSOCA pulse on the first event: ETPS[SOCACNT] = 0,1
    // bit 7-4       0's:    reserved
    // bit 3-2       00:     INTCNT, ePWM Interrupt Event (EPWMx_INT) Counter Register
    // bit 1-0       01:     INTPRD, 01 = Generate an interrupt on the first event INTCNT = 0,1 (first event)

    EPwm2Regs.PCCTL.all = 0x0000;       // PWM-Chopper Control Register
    // bit 15-11     0's:    reserved
    // bit 10-8      000:    CHPDUTY
    // bit 7-5       000:    CHPFREQ
    // bit 4-1       0000:   OSHTWTH
    // bit 0         0:      CHPEN, 0 = Disable (bypass) PWM chopping function

    asm(" EALLOW");

    EPwm2Regs.TZSEL.all = 0x1000;       // Trip-Zone Select Register
    // bit 15-14     0's:    reserved
    // bit 13        0:      OSHT6
    // bit 12        1:      OSHT5
    // bit 11        0:      OSHT4
    // bit 10        0:      OSHT3
    // bit 9         0:      OSHT2
    // bit 8         0:      OSHT1
    // bit 7-6       0's:    reserved
    // bit 5         0:      CBC6
    // bit 4         0:      CBC5
    // bit 3         0:      CBC4
    // bit 2         0:      CBC3
    // bit 1         0:      CBC2
    // bit 0         0:      CBC1

    EPwm2Regs.TZCTL.all = 0x000A;       // Trip-Zone Control Register
    // bit 15-4      0's:    reserved
    // bit 3-2       10:     TZB, 10 = Force EPWMxB to a low state.
    // bit 1-0       10:     TZA, 10 = Force EPWMxA to a low state

    EPwm2Regs.TZEINT.all = 0x0000;       // Trip-Zone Enable Interrupt Register
    // bit 15-3      0's:    reserved
    // bit 2         0:      OST, 0 = Disable one-shot trip event interrupt generation.
    // bit 1         0:      CBC, 0 = Disable cycle-by-cycle interrupt generation.
    // bit 0         0:      reserved

    asm(" EDIS");

    EPwm2Regs.TBCTL.bit.CTRMODE = 0x0;  // Enable the timer in count up mode

    //---------------------------------------------------------------------
    //--- Configure ePWM3
    //---------------------------------------------------------------------
    EPwm3Regs.TBCTL.bit.CTRMODE = 0x3;      // Disable the timer

    EPwm3Regs.TBCTL.all = 0xC007;           // Configure timer control register
    // bit 15-14     11:     FREE/SOFT, 11 = ignore emulation suspend
    // bit 13        0:      PHSDIR, 0 = count down after sync event
    // bit 12-10     000:    CLKDIV, 000 => TBCLK = HSPCLK/1
    // bit 9-7       000:    HSPCLKDIV, 000 => HSPCLK = SYSCLKOUT/1
    // bit 6         0:      SWFSYNC, 0 = no software sync produced
    // bit 5-4       00:     SYNCOSEL, 00 = EPWMxSYNCO signal source is EPWMxSYNC
    // bit 3         0:      PRDLD, 0 = reload PRD on counter=0
    // bit 2         1:      PHSEN, 1 = phase control enabled
    // bit 1-0       11:     CTRMODE, 11 = timer stopped (disabled)

    EPwm3Regs.TBCTR = 0x0000;               // Clear timer counter
    EPwm3Regs.TBPRD = PWM_PERIOD;                // Set timer period
    EPwm3Regs.TBPHS.half.TBPHS = 0x0000;    // Set timer phase

    EPwm3Regs.CMPA.half.CMPA = 0;
    EPwm3Regs.CMPB = (int)(_IQmpy(_IQ(0.975), PWM_PERIOD));

    EPwm3Regs.CMPCTL.all = 0x0000;          // Compare control register
    // bit 15-10     0's:    reserved
    // bit 9         0:      SHDWBFULL, read-only
    // bit 8         0:      SHDWAFULL, read-only
    // bit 7         0:      reserved
    // bit 6         0:      SHDWBMODE, don't care
    // bit 5         0:      reserved
    // bit 4         0:      SHDWAMODE, 0 = shadow mode
    // bit 3-2       00:     LOADBMODE, 00 = load on zero
    // bit 1-0       00:     LOADAMODE, 00 = load on zero

    EPwm3Regs.AQCTLA.all = 0x0001;      // Action-qualifier control register A
    // bit 15-12     0000:   reserved
    // bit 11-10     00:     CBD, 00 = do nothing
    // bit 9-8       00:     CBU, 00 = do nothing
    // bit 7-6       00:     CAD, 00 = do nothing
    // bit 5-4       00:     CAU, 00 = do nothing
    // bit 3-2       00:     PRD, 00 = do nothing
    // bit 1-0       01:     ZRO, 01 = clear

    EPwm3Regs.AQCTLB.all = 0x0001;      // Action-qualifier control register B
    // bit 15-12     0000:   reserved
    // bit 11-10     00:     CBD, 00 = do nothing
    // bit 9-8       00:     CBU, 00 = do nothing
    // bit 7-6       00:     CAD, 00 = do nothing
    // bit 5-4       00:     CAU, 00 = do nothing
    // bit 3-2       00:     PRD, 00 = do nothing
    // bit 1-0       01:     ZRO, 01 = clear

    EPwm3Regs.AQSFRC.all = 0x0000;      // Action-qualifier s/w force register
    // bit 15-8      0's:    reserved
    // bit 7-6       00:     RLDCSF, 00 = reload AQCSFRC on zero
    // bit 5         0:      OTSFB, 0 = do not initiate a s/w forced event on output B
    // bit 4-3       00:     ACTSFB, don't care
    // bit 2         0:      OTSFA, 0 = do not initiate a s/w forced event on output A
    // bit 1-0       00:     ACTSFA, don't care

    EPwm3Regs.AQCSFRC.all = 0x0000;     // Action-qualifier continuous s/w force register
    // bit 15-4      0's:    reserved
    // bit 3-2       00:     CSFB, 00 = forcing disabled
    // bit 1-0       00:     CSFA, 00 = forcing disabled

    EPwm3Regs.DBCTL.all = 0x0000;       //  Dead-Band Generator Control Register
    // bit 15-6      0's:    reserved
    // bit 5-4       00:     IN_MODE, 10 = EPWMxA In (from the action-qualifier) is the source for rising-edge delayed signal
    // bit 3-2       00:     POLSEL, 00 = active high mode
    // bit 1-0       00:     OUT_MODE, 00 = dead band generation bypassed

    EPwm3Regs.DBRED = 0;                //  Dead-Band Generator Rising Edge Delay Register
    EPwm3Regs.DBFED = 0;                //  Dead-Band Generator Falling Edge Delay Register


    EPwm3Regs.ETSEL.all = 0xC404;       //  Event-Trigger Selection Register
    // bit 15        1:      SOCBEN, 0 = Enable EPWMxSOCB.
    // bit 14-12     100:    SOCBSEL, 100 = Enable event time-base counter equal to CMPA when the timer is incrementing.
    // bit 11        0:      SOCAEN, 0 = Disable EPWMxSOCA.
    // bit 10-8      100:    SOCASEL, 100 = Enable event time-base counter equal to CMPA when the timer is incrementing.
    // bit 7-4       0's:    reserved
    // bit 3         0:      INTEN, 0 = Disable EPWMx_INT generation
    // bit 2-0       100:    INTSEL, 100 = Enable event time-base counter equal to CMPA when the timer is incrementing.

    EPwm3Regs.ETPS.all = 0x1101;       //  Event-Trigger Prescale Register
    // bit 15-14     00:     SOCBCNT, ePWM ADC Start-of-Conversion B Event (EPWMxSOCB) Counter Register
    // bit 13-12     01:     SOCBPRD, 01 = Generate the EPWMxSOCB pulse on the first event: ETPS[SOCBCNT] = 0,1
    // bit 11-10     00:     SOCACNT, ePWM ADC Start-of-Conversion A Event (EPWMxSOCA) Counter Register
    // bit 9-8       01:     SOCAPRD, 01 =  Generate the EPWMxSOCA pulse on the first event: ETPS[SOCACNT] = 0,1
    // bit 7-4       0's:    reserved
    // bit 3-2       00:     INTCNT, ePWM Interrupt Event (EPWMx_INT) Counter Register
    // bit 1-0       01:     INTPRD, 01 = Generate an interrupt on the first event INTCNT = 0,1 (first event)

    EPwm3Regs.PCCTL.all = 0x0000;       // PWM-Chopper Control Register
    // bit 15-11     0's:    reserved
    // bit 10-8      000:    CHPDUTY
    // bit 7-5       000:    CHPFREQ
    // bit 4-1       0000:   OSHTWTH
    // bit 0         0:      CHPEN, 0 = Disable (bypass) PWM chopping function

    asm(" EALLOW");

    EPwm3Regs.TZSEL.all = 0x1000;       // Trip-Zone Select Register
    // bit 15-14     0's:    reserved
    // bit 13        0:      OSHT6
    // bit 12        1:      OSHT5
    // bit 11        0:      OSHT4
    // bit 10        0:      OSHT3
    // bit 9         0:      OSHT2
    // bit 8         0:      OSHT1
    // bit 7-6       0's:    reserved
    // bit 5         0:      CBC6
    // bit 4         0:      CBC5
    // bit 3         0:      CBC4
    // bit 2         0:      CBC3
    // bit 1         0:      CBC2
    // bit 0         0:      CBC1

    EPwm3Regs.TZCTL.all = 0x000A;       // Trip-Zone Control Register
    // bit 15-4      0's:    reserved
    // bit 3-2       10:     TZB, 10 = Force EPWMxB to a low state.
    // bit 1-0       10:     TZA, 10 = Force EPWMxA to a low state

    EPwm3Regs.TZEINT.all = 0x0000;       // Trip-Zone Enable Interrupt Register
    // bit 15-3      0's:    reserved
    // bit 2         0:      OST, 0 = Disable one-shot trip event interrupt generation.
    // bit 1         0:      CBC, 0 = Disable cycle-by-cycle interrupt generation.
    // bit 0         0:      reserved

    asm(" EDIS");

    EPwm3Regs.TBCTL.bit.CTRMODE = 0x0;  // Enable the timer in count up mode

    //---------------------------------------------------------------------
    //--- Configure ePWM4
    //---------------------------------------------------------------------
    EPwm4Regs.TBCTL.bit.CTRMODE = 0x3;      // Disable the timer

    EPwm4Regs.TBCTL.all = 0xC007;           // Configure timer control register
    // bit 15-14     11:     FREE/SOFT, 11 = ignore emulation suspend
    // bit 13        0:      PHSDIR, 0 = count down after sync event
    // bit 12-10     000:    CLKDIV, 000 => TBCLK = HSPCLK/1
    // bit 9-7       000:    HSPCLKDIV, 000 => HSPCLK = SYSCLKOUT/1
    // bit 6         0:      SWFSYNC, 0 = no software sync produced
    // bit 5-4       00:     SYNCOSEL, 00 = EPWMxSYNCO signal source is EPWMxSYNC
    // bit 3         0:      PRDLD, 0 = reload PRD on counter=0
    // bit 2         1:      PHSEN, 1 = phase control enabled
    // bit 1-0       11:     CTRMODE, 11 = timer stopped (disabled)

    EPwm4Regs.TBCTR = 0x0000;               // Clear timer counter
    EPwm4Regs.TBPRD = PWM_PERIOD;           // Set timer period
    EPwm4Regs.TBPHS.half.TBPHS = 0x0000;    // Set timer phase

    EPwm4Regs.CMPA.half.CMPA = (int)(_IQmpy(_IQ(1.1), PWM_PERIOD));
    EPwm4Regs.CMPB = (int)(_IQmpy(_IQ(1.0)-_IQmpy(Q_SECURITY_TIME, ADC_SAMPLE_FRECUENCY), PWM_PERIOD));

    EPwm4Regs.CMPCTL.all = 0x0000;          // Compare control register
    // bit 15-10     0's:    reserved
    // bit 9         0:      SHDWBFULL, read-only
    // bit 8         0:      SHDWAFULL, read-only
    // bit 7         0:      reserved
    // bit 6         0:      SHDWBMODE, don't care
    // bit 5         0:      reserved
    // bit 4         0:      SHDWAMODE, 0 = shadow mode
    // bit 3-2       00:     LOADBMODE, 00 = load on zero
    // bit 1-0       00:     LOADAMODE, 00 = load on zero

    EPwm4Regs.AQCTLA.all = 0x0121;      // Action-qualifier control register A
    // bit 15-12     0000:   reserved
    // bit 11-10     00:     CBD, 00 = do nothing
    // bit 9-8       01:     CBU, 01 = clear
    // bit 7-6       00:     CAD, 00 = do nothing
    // bit 5-4       10:     CAU, 10 = set
    // bit 3-2       00:     PRD, 00 = do nothing
    // bit 1-0       01:     ZRO, 01 = clear

    EPwm4Regs.AQCTLB.all = 0x0121;      // Action-qualifier control register B
    // bit 15-12     0000:   reserved
    // bit 11-10     00:     CBD, 00 = do nothing
    // bit 9-8       01:     CBU, 01 = clear
    // bit 7-6       00:     CAD, 00 = do nothing
    // bit 5-4       10:     CAU, 10 = set
    // bit 3-2       00:     PRD, 00 = do nothing
    // bit 1-0       01:     ZRO, 01 = clear

    EPwm4Regs.AQSFRC.all = 0x0000;      // Action-qualifier s/w force register
    // bit 15-8      0's:    reserved
    // bit 7-6       00:     RLDCSF, 00 = reload AQCSFRC on zero
    // bit 5         0:      OTSFB, 0 = do not initiate a s/w forced event on output B
    // bit 4-3       00:     ACTSFB, don't care
    // bit 2         0:      OTSFA, 0 = do not initiate a s/w forced event on output A
    // bit 1-0       00:     ACTSFA, don't care

    EPwm4Regs.AQCSFRC.all = 0x0000;     // Action-qualifier continuous s/w force register
    // bit 15-4      0's:    reserved
    // bit 3-2       00:     CSFB, 00 = forcing disabled
    // bit 1-0       00:     CSFA, 00 = forcing disabled

    EPwm4Regs.DBCTL.all = 0x0000;       //  Dead-Band Generator Control Register
    // bit 15-6      0's:    reserved
    // bit 5-4       00:     IN_MODE, 10 = EPWMxA In (from the action-qualifier) is the source for rising-edge delayed signal
    // bit 3-2       00:     POLSEL, 00 = active high mode
    // bit 1-0       00:     OUT_MODE, 00 = dead band generation bypassed

    EPwm4Regs.DBRED = 0;                //  Dead-Band Generator Rising Edge Delay Register
    EPwm4Regs.DBFED = 0;                //  Dead-Band Generator Falling Edge Delay Register


    EPwm4Regs.ETSEL.all = 0x6602;       //  Event-Trigger Selection Register
    // bit 15        0:      SOCBEN, 0 = Enable EPWMxSOCB.
    // bit 14-12     110:    SOCBSEL, 110 = Enable event: time-base counter equal to CMPB when the timer is incrementing.
    // bit 11        0:      SOCAEN, 0 = Disable EPWMxSOCA.
    // bit 10-8      110:    SOCASEL, 110 = Enable event: time-base counter equal to CMPB when the timer is incrementing.
    // bit 7-4       0's:    reserved
    // bit 3         0:      INTEN, 0 = Disable EPWMx_INT generation
    // bit 2-0       010:    INTSEL, 010 = Enable event time-base counter equal to period (TBCTR = TBPRD)


    EPwm4Regs.ETPS.all = 0x1101;       //  Event-Trigger Prescale Register
    // bit 15-14     00:     SOCBCNT, ePWM ADC Start-of-Conversion B Event (EPWMxSOCB) Counter Register
    // bit 13-12     01:     SOCBPRD, 01 = Generate the EPWMxSOCB pulse on the first event: ETPS[SOCBCNT] = 0,1
    // bit 11-10     00:     SOCACNT, ePWM ADC Start-of-Conversion A Event (EPWMxSOCA) Counter Register
    // bit 9-8       01:     SOCAPRD, 01 =  Generate the EPWMxSOCA pulse on the first event: ETPS[SOCACNT] = 0,1
    // bit 7-4       0's:    reserved
    // bit 3-2       00:     INTCNT, ePWM Interrupt Event (EPWMx_INT) Counter Register
    // bit 1-0       01:     INTPRD, 01 = Generate an interrupt on the first event INTCNT = 0,1 (first event)

    EPwm4Regs.PCCTL.all = 0x0000;       // PWM-Chopper Control Register
    // bit 15-11     0's:    reserved
    // bit 10-8      000:    CHPDUTY
    // bit 7-5       000:    CHPFREQ
    // bit 4-1       0000:   OSHTWTH
    // bit 0         0:      CHPEN, 0 = Disable (bypass) PWM chopping function

    asm(" EALLOW");

    EPwm4Regs.TZSEL.all = 0x1000;       // Trip-Zone Select Register
    // bit 15-14     0's:    reserved
    // bit 13        0:      OSHT6
    // bit 12        1:      OSHT5
    // bit 11        0:      OSHT4
    // bit 10        0:      OSHT3
    // bit 9         0:      OSHT2
    // bit 8         0:      OSHT1
    // bit 7-6       0's:    reserved
    // bit 5         0:      CBC6
    // bit 4         0:      CBC5
    // bit 3         0:      CBC4
    // bit 2         0:      CBC3
    // bit 1         0:      CBC2
    // bit 0         0:      CBC1

    EPwm4Regs.TZCTL.all = 0x000A;       // Trip-Zone Control Register
    // bit 15-4      0's:    reserved
    // bit 3-2       10:     TZB, 10 = Force EPWMxB to a low state.
    // bit 1-0       10:     TZA, 10 = Force EPWMxA to a low state

    EPwm4Regs.TZEINT.all = 0x0000;       // Trip-Zone Enable Interrupt Register
    // bit 15-3      0's:    reserved
    // bit 2         0:      OST, 0 = Disable one-shot trip event interrupt generation.
    // bit 1         0:      CBC, 0 = Disable cycle-by-cycle interrupt generation.
    // bit 0         0:      reserved

    asm(" EDIS");

    EPwm4Regs.TBCTL.bit.CTRMODE = 0x0;  // Enable the timer in count up mode

    //---------------------------------------------------------------------
    //--- Configure ePWM5
    //---------------------------------------------------------------------
    EPwm5Regs.TBCTL.bit.CTRMODE = 0x3;      // Disable the timer

    EPwm5Regs.TBCTL.all = 0xC007;           // Configure timer control register
    // bit 15-14     11:     FREE/SOFT, 11 = ignore emulation suspend
    // bit 13        0:      PHSDIR, 0 = count down after sync event
    // bit 12-10     000:    CLKDIV, 000 => TBCLK = HSPCLK/1
    // bit 9-7       000:    HSPCLKDIV, 000 => HSPCLK = SYSCLKOUT/1
    // bit 6         0:      SWFSYNC, 0 = no software sync produced
    // bit 5-4       00:     SYNCOSEL, 00 = EPWMxSYNCO signal source is EPWMxSYNC
    // bit 3         0:      PRDLD, 0 = reload PRD on counter=0
    // bit 2         1:      PHSEN, 1 = phase control enabled
    // bit 1-0       11:     CTRMODE, 11 = timer stopped (disabled)

    EPwm5Regs.TBCTR = 0x0000;               // Clear timer counter
    EPwm5Regs.TBPRD = PWM_PERIOD;           // Set timer period
    EPwm5Regs.TBPHS.half.TBPHS = 0x0000;    // Set timer phase

    EPwm5Regs.CMPA.half.CMPA = (int)(_IQmpy(_IQ(1.1), PWM_PERIOD));
    EPwm5Regs.CMPB = (int)(_IQmpy(_IQ(1.0)-_IQmpy(Q_SECURITY_TIME, ADC_SAMPLE_FRECUENCY), PWM_PERIOD));

    EPwm5Regs.CMPCTL.all = 0x0000;          // Compare control register
    // bit 15-10     0's:    reserved
    // bit 9         0:      SHDWBFULL, read-only
    // bit 8         0:      SHDWAFULL, read-only
    // bit 7         0:      reserved
    // bit 6         0:      SHDWBMODE, don't care
    // bit 5         0:      reserved
    // bit 4         0:      SHDWAMODE, 0 = shadow mode
    // bit 3-2       00:     LOADBMODE, 00 = load on zero
    // bit 1-0       00:     LOADAMODE, 00 = load on zero

    EPwm5Regs.AQCTLA.all = 0x0024;      // Action-qualifier control register A
    // bit 15-12     0000:   reserved
    // bit 11-10     00:     CBD, 00 = do nothing
    // bit 9-8       00:     CBU, 01 = do nothing
    // bit 7-6       00:     CAD, 00 = do nothing
    // bit 5-4       10:     CAU, 10 = set
    // bit 3-2       01:     PRD, 01 = clear
    // bit 1-0       00:     ZRO, 00 = do nothing

    EPwm5Regs.AQCTLB.all = 0x0121;      // Action-qualifier control register B
    // bit 15-12     0000:   reserved
    // bit 11-10     00:     CBD, 00 = do nothing
    // bit 9-8       01:     CBU, 01 = clear
    // bit 7-6       00:     CAD, 00 = do nothing
    // bit 5-4       10:     CAU, 10 = set
    // bit 3-2       00:     PRD, 00 = do nothing
    // bit 1-0       01:     ZRO, 01 = clear

    EPwm5Regs.AQSFRC.all = 0x0000;      // Action-qualifier s/w force register
    // bit 15-8      0's:    reserved
    // bit 7-6       00:     RLDCSF, 00 = reload AQCSFRC on zero
    // bit 5         0:      OTSFB, 0 = do not initiate a s/w forced event on output B
    // bit 4-3       00:     ACTSFB, don't care
    // bit 2         0:      OTSFA, 0 = do not initiate a s/w forced event on output A
    // bit 1-0       00:     ACTSFA, don't care

    EPwm5Regs.AQCSFRC.all = 0x0000;     // Action-qualifier continuous s/w force register
    // bit 15-4      0's:    reserved
    // bit 3-2       00:     CSFB, 00 = forcing disabled
    // bit 1-0       00:     CSFA, 00 = forcing disabled

    EPwm5Regs.DBCTL.all = 0x0000;       //  Dead-Band Generator Control Register
    // bit 15-6      0's:    reserved
    // bit 5-4       00:     IN_MODE, 10 = EPWMxA In (from the action-qualifier) is the source for rising-edge delayed signal
    // bit 3-2       00:     POLSEL, 00 = active high mode
    // bit 1-0       00:     OUT_MODE, 00 = dead band generation bypassed

    EPwm5Regs.DBRED = 0;                //  Dead-Band Generator Rising Edge Delay Register
    EPwm5Regs.DBFED = 0;                //  Dead-Band Generator Falling Edge Delay Register


    EPwm5Regs.ETSEL.all = 0x1201;       //  Event-Trigger Selection Register
    // bit 15        0:      SOCBEN, 0 = Enable EPWMxSOCB.
    // bit 14-12     001:    SOCBSEL, 110 = Enable event time-base counter equal to zero. (TBCTR = 0x0000)
    // bit 11        0:      SOCAEN, 0 = Disable EPWMxSOCA.
    // bit 10-8      010:    SOCASEL, 010 = Enable event time-base counter equal to period (TBCTR = TBPRD)
    // bit 7-4       0's:    reserved
    // bit 3         0:      INTEN, 0 = Disable EPWMx_INT generation
    // bit 2-0       001:    INTSEL, 001 = Enable event time-base counter equal to zero. (TBCTR = 0x0000)


    EPwm5Regs.ETPS.all = 0x1101;       //  Event-Trigger Prescale Register
    // bit 15-14     00:     SOCBCNT, ePWM ADC Start-of-Conversion B Event (EPWMxSOCB) Counter Register
    // bit 13-12     01:     SOCBPRD, 01 = Generate the EPWMxSOCB pulse on the first event: ETPS[SOCBCNT] = 0,1
    // bit 11-10     00:     SOCACNT, ePWM ADC Start-of-Conversion A Event (EPWMxSOCA) Counter Register
    // bit 9-8       01:     SOCAPRD, 01 =  Generate the EPWMxSOCA pulse on the first event: ETPS[SOCACNT] = 0,1
    // bit 7-4       0's:    reserved
    // bit 3-2       00:     INTCNT, ePWM Interrupt Event (EPWMx_INT) Counter Register
    // bit 1-0       01:     INTPRD, 01 = Generate an interrupt on the first event INTCNT = 0,1 (first event)

    EPwm5Regs.PCCTL.all = 0x0000;       // PWM-Chopper Control Register
    // bit 15-11     0's:    reserved
    // bit 10-8      000:    CHPDUTY
    // bit 7-5       000:    CHPFREQ
    // bit 4-1       0000:   OSHTWTH
    // bit 0         0:      CHPEN, 0 = Disable (bypass) PWM chopping function

    asm(" EALLOW");

    EPwm5Regs.TZSEL.all = 0x1000;       // Trip-Zone Select Register
    // bit 15-14     0's:    reserved
    // bit 13        0:      OSHT6
    // bit 12        1:      OSHT5
    // bit 11        0:      OSHT4
    // bit 10        0:      OSHT3
    // bit 9         0:      OSHT2
    // bit 8         0:      OSHT1
    // bit 7-6       0's:    reserved
    // bit 5         0:      CBC6
    // bit 4         0:      CBC5
    // bit 3         0:      CBC4
    // bit 2         0:      CBC3
    // bit 1         0:      CBC2
    // bit 0         0:      CBC1

    EPwm5Regs.TZCTL.all = 0x000A;       // Trip-Zone Control Register
    // bit 15-4      0's:    reserved
    // bit 3-2       10:     TZB, 10 = Force EPWMxB to a low state.
    // bit 1-0       10:     TZA, 10 = Force EPWMxA to a low state

    EPwm5Regs.TZEINT.all = 0x0000;       // Trip-Zone Enable Interrupt Register
    // bit 15-3      0's:    reserved
    // bit 2         0:      OST, 0 = Disable one-shot trip event interrupt generation.
    // bit 1         0:      CBC, 0 = Disable cycle-by-cycle interrupt generation.
    // bit 0         0:      reserved

    asm(" EDIS");

    EPwm5Regs.TBCTL.bit.CTRMODE = 0x0;  // Enable the timer in count up mode

    //---------------------------------------------------------------------
    //--- Configure ePWM6
    //---------------------------------------------------------------------
    EPwm6Regs.TBCTL.bit.CTRMODE = 0x3;      // Disable the timer

    EPwm6Regs.TBCTL.all = 0xC007;           // Configure timer control register
    // bit 15-14     11:     FREE/SOFT, 11 = ignore emulation suspend
    // bit 13        0:      PHSDIR, 0 = count down after sync event
    // bit 12-10     000:    CLKDIV, 000 => TBCLK = HSPCLK/1
    // bit 9-7       000:    HSPCLKDIV, 000 => HSPCLK = SYSCLKOUT/1
    // bit 6         0:      SWFSYNC, 0 = no software sync produced
    // bit 5-4       00:     SYNCOSEL, 00 = EPWMxSYNCO signal source is EPWMxSYNC
    // bit 3         0:      PRDLD, 0 = reload PRD on counter=0
    // bit 2         1:      PHSEN, 1 = phase control enabled
    // bit 1-0       11:     CTRMODE, 11 = timer stopped (disabled)

    EPwm6Regs.TBCTR = 0x0000;               // Clear timer counter
    EPwm6Regs.TBPRD = PWM_PERIOD;                // Set timer period
    EPwm6Regs.TBPHS.half.TBPHS = 0x0000;    // Set timer phase

    EPwm6Regs.CMPA.half.CMPA = (int)(_IQmpy(_IQ(0.5), PWM_PERIOD));
    EPwm6Regs.CMPB = (int)(PWM_PERIOD);

    EPwm6Regs.CMPCTL.all = 0x0000;          // Compare control register
    // bit 15-10     0's:    reserved
    // bit 9         0:      SHDWBFULL, read-only
    // bit 8         0:      SHDWAFULL, read-only
    // bit 7         0:      reserved
    // bit 6         0:      SHDWBMODE, don't care
    // bit 5         0:      reserved
    // bit 4         0:      SHDWAMODE, 0 = shadow mode
    // bit 3-2       00:     LOADBMODE, 00 = load on zero
    // bit 1-0       00:     LOADAMODE, 00 = load on zero

    EPwm6Regs.AQCTLA.all = 0x0024;          // Action-qualifier control register A
    // bit 15-12     0000:   reserved
    // bit 11-10     00:     CBD, 00 = do nothing
    // bit 9-8       00:     CBU, 00 = do nothing
    // bit 7-6       00:     CAD, 00 = do nothing
    // bit 5-4       10:     CAU, 10 = set
    // bit 3-2       01:     PRD, 01 = clear
    // bit 1-0       00:     ZRO, 00 = do nothing

    EPwm6Regs.AQCTLB.all = 0x0121;      // Action-qualifier control register B
    // bit 15-12     0000:   reserved
    // bit 11-10     00:     CBD, 00 = do nothing
    // bit 9-8       00:     CBU, 00 = do nothing
    // bit 7-6       00:     CAD, 00 = do nothing
    // bit 5-4       01:     CAU, 01 = clear
    // bit 3-2       00:     PRD, 00 = do nothing
    // bit 1-0       10:     ZRO, 10 = set

    EPwm6Regs.AQSFRC.all = 0x0000;      // Action-qualifier s/w force register
    // bit 15-8      0's:    reserved
    // bit 7-6       00:     RLDCSF, 00 = reload AQCSFRC on zero
    // bit 5         0:      OTSFB, 0 = do not initiate a s/w forced event on output B
    // bit 4-3       00:     ACTSFB, don't care
    // bit 2         0:      OTSFA, 0 = do not initiate a s/w forced event on output A
    // bit 1-0       00:     ACTSFA, don't care

    EPwm6Regs.AQCSFRC.all = 0x0000;     // Action-qualifier continuous s/w force register
    // bit 15-4      0's:    reserved
    // bit 3-2       00:     CSFB, 00 = forcing disabled
    // bit 1-0       00:     CSFA, 00 = forcing disabled

    EPwm6Regs.DBCTL.all = 0x0000;       //  Dead-Band Generator Control Register
    // bit 15-6      0's:    reserved
    // bit 5-4       00:     IN_MODE, 10 = EPWMxA In (from the action-qualifier) is the source for rising-edge delayed signal
    // bit 3-2       00:     POLSEL, 00 = active high mode
    // bit 1-0       00:     OUT_MODE, 00 = dead band generation bypassed

    EPwm6Regs.DBRED = 0;                //  Dead-Band Generator Rising Edge Delay Register
    EPwm6Regs.DBFED = 0;                //  Dead-Band Generator Falling Edge Delay Register


    EPwm6Regs.ETSEL.all = 0x6E0C;       //  Event-Trigger Selection Register
    // bit 15        0:      SOCBEN, 0 = Enable EPWMxSOCB.
    // bit 14-12     110:    SOCBSEL, 110 = Enable event: time-base counter equal to CMPB when the timer is incrementing.
    // bit 11        1:      SOCAEN, 1 = Enable EPWMxSOCA.
    // bit 10-8      110:    SOCASEL, 110 = Enable event: time-base counter equal to CMPB when the timer is incrementing.
    // bit 7-4       0's:    reserved
    // bit 3         1:      INTEN, 0 = Disable EPWMx_INT generation
    // bit 2-0       100:    INTSEL, 100 = Enable event: time-base counter equal to CMPA when the timer is incrementing.


    EPwm6Regs.ETPS.all = 0x1101;       //  Event-Trigger Prescale Register
    // bit 15-14     00:     SOCBCNT, ePWM ADC Start-of-Conversion B Event (EPWMxSOCB) Counter Register
    // bit 13-12     01:     SOCBPRD, 01 = Generate the EPWMxSOCB pulse on the first event: ETPS[SOCBCNT] = 0,1
    // bit 11-10     00:     SOCACNT, ePWM ADC Start-of-Conversion A Event (EPWMxSOCA) Counter Register
    // bit 9-8       01:     SOCAPRD, 01 =  Generate the EPWMxSOCA pulse on the first event: ETPS[SOCACNT] = 0,1
    // bit 7-4       0's:    reserved
    // bit 3-2       00:     INTCNT, ePWM Interrupt Event (EPWMx_INT) Counter Register
    // bit 1-0       01:     INTPRD, 01 = Generate an interrupt on the first event INTCNT = 0,1 (first event)

    EPwm6Regs.PCCTL.all = 0x0000;       // PWM-Chopper Control Register
    // bit 15-11     0's:    reserved
    // bit 10-8      000:    CHPDUTY
    // bit 7-5       000:    CHPFREQ
    // bit 4-1       0000:   OSHTWTH
    // bit 0         0:      CHPEN, 0 = Disable (bypass) PWM chopping function

    asm(" EALLOW");

    EPwm6Regs.TZSEL.all = 0x1000;       // Trip-Zone Select Register
    // bit 15-14     0's:    reserved
    // bit 13        0:      OSHT6
    // bit 12        1:      OSHT5
    // bit 11        0:      OSHT4
    // bit 10        0:      OSHT3
    // bit 9         0:      OSHT2
    // bit 8         0:      OSHT1
    // bit 7-6       0's:    reserved
    // bit 5         0:      CBC6
    // bit 4         0:      CBC5
    // bit 3         0:      CBC4
    // bit 2         0:      CBC3
    // bit 1         0:      CBC2
    // bit 0         0:      CBC1

    EPwm6Regs.TZCTL.all = 0x000A;       // Trip-Zone Control Register
    // bit 15-4      0's:    reserved
    // bit 3-2       10:     TZB, 10 = Force EPWMxB to a low state.
    // bit 1-0       10:     TZA, 10 = Force EPWMxA to a low state

    EPwm6Regs.TZEINT.all = 0x0004;       // Trip-Zone Enable Interrupt Register
    // bit 15-3      0's:    reserved
    // bit 2         1:      OST, 1 = Enable Interrupt generation; a one-shot trip event will cause a EPWMx_TZINT PIE interrupt.
    // bit 1         0:      CBC, 0 = Disable cycle-by-cycle interrupt generation.
    // bit 0         0:      reserved

    asm(" EDIS");

    EPwm6Regs.TBCTL.bit.CTRMODE = 0x0;  // Enable the timer in count up mode

    //---------------------------------------------------------------------
    //--- Enable ePWM interrupts
    //---------------------------------------------------------------------
    //PieCtrlRegs.PIEIER3.bit.INTx3 = 1;  // Enable EPWM3_INT in PIE group 1
    //PieCtrlRegs.PIEIER3.bit.INTx5 = 1;  // Enable EPWM5_INT in PIE group 1
    PieCtrlRegs.PIEIER3.bit.INTx6 = 1;  // Enable EPWM6_INT in PIE group 1
    IER |= M_INT3;                      // Enable INT3 in IER to enable PIE group

    PieCtrlRegs.PIEIER2.bit.INTx6 = 1;  // Enable EPWM6_TZINT in PIE group 1
    IER |= M_INT2;                      // Enable INT2 in IER to enable PIE group

    //---------------------------------------------------------------------
    //--- Enable the clocks to the ePWM module.
    //--- Note: this should be done after all ePWM modules are configured
    //--- to ensure synchronization between the ePWM modules.
    //---------------------------------------------------------------------
    asm(" EALLOW");							// Enable EALLOW protected register access
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;	// HSPCLK to ePWM modules enabled
    asm(" EDIS");							// Disable EALLOW protected register access

} // end InitEPwm()


/**********************************************************************
* Function: epwm_rstTripZone()
*
* Description: Clear all EPWM modules trip-zone flags register
**********************************************************************/
extern void epwm_rstTripZone(){
    EALLOW; // Allow writing to protected registers
    EPwm1Regs.TZCLR.all = 0x7;
    EPwm2Regs.TZCLR.all = 0x7;
    EPwm3Regs.TZCLR.all = 0x7;
    EPwm4Regs.TZCLR.all = 0x7;
    EPwm5Regs.TZCLR.all = 0x7;
    EPwm6Regs.TZCLR.all = 0x7;
    EDIS;   // Lock registers against writing
}

/**********************************************************************
* Function: epwm_softOneShotTZ()
*
* Description: generates a software One Shot TZ event in all EPWM modules
**********************************************************************/
extern void epwm_softOneShotTZ(){
    EALLOW; // Allow writing to protected registers
    EPwm1Regs.TZFRC.bit.OST = 1;
    EPwm2Regs.TZFRC.bit.OST = 1;
    EPwm3Regs.TZFRC.bit.OST = 1;
    EPwm4Regs.TZFRC.bit.OST = 1;
    EPwm5Regs.TZFRC.bit.OST = 1;
    EDIS;   // Lock registers against writing
}
