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
#include "Adc.h"
#include "../DelayUs.h"

/**********************************************************************
                              DEFINES
**********************************************************************/
#if MATH_TYPE == IQ_MATH
    #define ADC_FS_VOLTAGE  _IQ(3.0)        // ADC full scale voltage
#else
    #define ADC_FS_VOLTAGE  _IQ(3.0/4095.0) // ADC full scale voltage, scaled for 12 bit ADC
#endif
#define REFERENCE_VOLTAGE _IQ(3.3)
#define REFERENCE_DIVIDER_FACTOR _IQ(0.6802721088)

/**********************************************************************
                        INTERNAL VARIABLES
**********************************************************************/
static _iq AdcScale = _IQ(ADC_FS_VOLTAGE);

/**********************************************************************
                   INTERNAL FUNCTIONS DECLARATION
**********************************************************************/
static void calibrateAdcScale(void);

/**********************************************************************
                   INTERNAL FUNCTIONS DEFINITION
**********************************************************************/

/**********************************************************************
* Function: calibrateAdcScale()
*
* Description: Take a sample of a known voltage to compensate ADC
* scale error
**********************************************************************/
static void calibrateAdcScale(void)
{
    //--- Reset the ADC module
    AdcRegs.ADCTRL1.bit.RESET = 1;      // Reset the ADC

    // Must wait 2 ADCCLK periods for the reset to take effect.  The ADC is
    // already reset after a DSP reset, but this example is just showing good
    // coding practice to reset the peripheral before configuring it (as you
    // never know why the DSP has started the code over again from the
    // beginning).  Assuming a 12.5 MHz ADCCLK was previously configured, and
    // a 150 MHz SYSCLKOUT, the wait period of 2 ADCCLK periods equates to 24
    // CPU clocks.  This is the example being used below.

    asm(" RPT #22 || NOP");             // Must wait for ADC reset to take effect

    // Call the ADC_cal() function located in the Boot ROM.
    // ADC_cal_func_ptr is a macro defined in the file Adc.h.  This
    // macro simply defines ADC_cal_func_ptr to be a function pointer to
    // the correct address in the boot ROM.
    (*ADC_cal_func_ptr)();

    //--- Select the ADC reference
    AdcRegs.ADCREFSEL.bit.REF_SEL = 0;  // 0=internal, 1=external

    //--- Power-up the ADC
    AdcRegs.ADCTRL3.all = 0x00E8;       // Power-up reference and main ADC
    // bit 15-8      0's:    reserved
    // bit 7-6       11:     ADCBGRFDN, reference power, 00=off, 11=on
    // bit 5         1:      ADCPWDN, main ADC power, 0=off, 1=on
    // bit 4-1       0100:   ADCCLKPS, clock prescaler, FCLK=HSPCLK/(2*ADCCLKPS)=150MHz/8=25MHz
    // bit 0         0:      SMODE_SEL, 0=sequential sampling, 1=simultaneous sampling

    DelayUs(5000);                      // Wait 5ms before using the ADC

    //--- Configure the other ADC register

    AdcRegs.ADCMAXCONV.all = 0x0000;
    // bit 15-7      0's:    reserved
    // bit 6-4       011:    MAX_CONV2 value (0 means 1 conversion)
    // bit 3-0       0100:   MAX_CONV1 value (0 means 1 conversion)


    // Configure channel selection sequence
    AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0xB;    // Convert Channel 11 - B3

    AdcRegs.ADCTRL1.all = 0x0000;
    // bit 15        0:      reserved
    // bit 14        0:      RESET, 0=no action, 1=reset ADC
    // bit 13-12     00:     SUSMOD, 00=ignore emulation suspend
    // bit 11-8      0000:   ACQ_PS (Acquisition), 0000 = ADCCLK
    // bit 7         0:      CPS (Core clock), 0: ADCCLK=FCLK/1, 1: ADCCLK=FCLK/2
    // bit 6         0:      CONT_RUN, 0=start/stop mode, 1=continuous run
    // bit 5         0:      SEQ_OVRD, 0=disabled, 1=enabled
    // bit 4         0:      SEQ_CASC, 0=dual sequencer, 1=cascaded sequencer
    // bit 3-0       0000:   reserved

    AdcRegs.ADCTRL2.all = 0x0000;
    // bit 15        0:      ePWM_SOCB_SEQ, 0=no action
    // bit 14        0:      RST_SEQ1, 0=no action
    // bit 13        0:      SOC_SEQ1, 0=clear any pending SOCs
    // bit 12        0:      reserved
    // bit 11        0:      INT_ENA_SEQ1, 1=enable interrupt
    // bit 10        0:      INT_MOD_SEQ1, 0=int on every SEQ1 conv
    // bit 9         0:      reserved
    // bit 8         0:      ePWM_SOCA_SEQ1, 0=no action
    // bit 7         0:      EXT_SOC_SEQ1, 0=no action
    // bit 6         0:      RST_SEQ2, 0=no action
    // bit 5         0:      SOC_SEQ2, 0=clear any pending SOCs
    // bit 4         0:      reserved
    // bit 3         0:      INT_ENA_SEQ2, 0=int disabled
    // bit 2         0:      INT_MOD_SEQ2, 0=int on every other SEQ2 conv
    // bit 1         0:      reserved
    // bit 0         0:      ePWM_SOCB_SEQ2, 0=no action

    //Reset ADC sequencers
    AdcRegs.ADCTRL2.bit.RST_SEQ1 = 0x1;
    AdcRegs.ADCTRL2.bit.RST_SEQ2 = 0x1;

    AdcScale = 0;
    //Do ADC conversions in channel of reference
    int i;
    for(i = 0; i<16; i++){
        AdcRegs.ADCTRL2.bit.SOC_SEQ1 = 1;       // Start ADC conversion
        while(!AdcRegs.ADCST.bit.INT_SEQ1);     // Wait for conversion
        AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;     // Clear flag
        AdcScale += AdcRegs.ADCRESULT0 >> 4;    // Get the sample
    }

    AdcScale = _IQdiv(AdcScale, _IQ(16.0));                                             // Calculate the average measured counts
    AdcScale = _IQmpy(AdcScale, _IQ(ADC_FS_VOLTAGE));                                   // Calculate the average measured voltage
    AdcScale = _IQdiv(_IQmpy(REFERENCE_VOLTAGE, REFERENCE_DIVIDER_FACTOR), AdcScale);   // Calculate the scale correction factor
    AdcScale = _IQmpy(AdcScale, _IQ(ADC_FS_VOLTAGE));                                   // Calculate the counts-to-voltage factor
}

/**********************************************************************
                   EXTERNAL FUNCTIONS DEFINITION
**********************************************************************/

/**********************************************************************
* Function: InitAdc()
*
* Description: Initializes the ADC on the F2833x
**********************************************************************/
extern void InitAdc(void)
{
    //--- Calibrates the ADC Scale
    calibrateAdcScale();

    //--- Reset the ADC module
	AdcRegs.ADCTRL1.bit.RESET = 1;		// Reset the ADC

    // Must wait 2 ADCCLK periods for the reset to take effect.  The ADC is
    // already reset after a DSP reset, but this example is just showing good
    // coding practice to reset the peripheral before configuring it (as you
    // never know why the DSP has started the code over again from the
    // beginning).  Assuming a 12.5 MHz ADCCLK was previously configured, and
    // a 150 MHz SYSCLKOUT, the wait period of 2 ADCCLK periods equates to 24
    // CPU clocks.  This is the example being used below.
 
	asm(" RPT #22 || NOP");				// Must wait for ADC reset to take effect

    // Call the ADC_cal() function located in the Boot ROM.
    // ADC_cal_func_ptr is a macro defined in the file example_nonBios.h or
    // example_BIOS.h (as may be the case for the example being used).  This
    // macro simply defines ADC_cal_func_ptr to be a function pointer to
    // the correct address in the boot ROM.
	(*ADC_cal_func_ptr)();

    //--- Select the ADC reference
	AdcRegs.ADCREFSEL.bit.REF_SEL = 0;	// 0=internal, 1=external
	
	//--- Power-up the ADC
	AdcRegs.ADCTRL3.all = 0x00EC;		// Power-up reference and main ADC
    // bit 15-8      0's:    reserved
    // bit 7-6       11:     ADCBGRFDN, reference power, 00=off, 11=on
    // bit 5         1:      ADCPWDN, main ADC power, 0=off, 1=on
    // bit 4-1       0110:   ADCCLKPS, clock prescaler, FCLK=HSPCLK/(2*ADCCLKPS)
    // bit 0         0:      SMODE_SEL, 0=sequential sampling, 1=simultaneous sampling

	DelayUs(5000);						// Wait 5ms before using the ADC

    //--- Configure the other ADC register

	AdcRegs.ADCMAXCONV.all = 0x0034;
    // bit 15-7      0's:    reserved
    // bit 6-4       011:    MAX_CONV2 value (0 means 1 conversion)
    // bit 3-0       0100:   MAX_CONV1 value (0 means 1 conversion)


	// Configure channel selection sequence
	AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0x0;    // Convert Channel 0 - A0
	AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 0x1;    // Convert Channel 1 - A1
	AdcRegs.ADCCHSELSEQ1.bit.CONV02 = 0x2;    // Convert Channel 2 - A2
    AdcRegs.ADCCHSELSEQ1.bit.CONV03 = 0x3;    // Convert Channel 3 - A3
    AdcRegs.ADCCHSELSEQ2.bit.CONV04 = 0x4;    // Convert Channel 4 - A4

	AdcRegs.ADCCHSELSEQ3.bit.CONV08 = 0x8;    // Convert Channel 8 - B0
	AdcRegs.ADCCHSELSEQ3.bit.CONV09 = 0x9;    // Convert Channel 9 - B1
	AdcRegs.ADCCHSELSEQ3.bit.CONV10 = 0xA;    // Convert Channel 10 - B2
	AdcRegs.ADCCHSELSEQ3.bit.CONV11 = 0x0;    // Convert Channel 0 - A0

	AdcRegs.ADCTRL1.all = 0x0700;
    // bit 15        0:      reserved
    // bit 14        0:      RESET, 0=no action, 1=reset ADC
    // bit 13-12     00:     SUSMOD, 00=ignore emulation suspend
    // bit 11-8      0111:   ACQ_PS (Acquisition), 0000 = 8 x ADCCLK
    // bit 7         0:      CPS (Core clock), 0: ADCCLK=FCLK/1, 1: ADCCLK=FCLK/2
    // bit 6         0:      CONT_RUN, 0=start/stop mode, 1=continuous run
    // bit 5         0:      SEQ_OVRD, 0=disabled, 1=enabled
    // bit 4         0:      SEQ_CASC, 0=dual sequencer, 1=cascaded sequencer
    // bit 3-0       0000:   reserved

	AdcRegs.ADCTRL2.all = 0x0909;
    // bit 15        0:      ePWM_SOCB_SEQ, 0=no action
    // bit 14        0:      RST_SEQ1, 0=no action
    // bit 13        0:      SOC_SEQ1, 0=clear any pending SOCs
    // bit 12        0:      reserved
    // bit 11        1:      INT_ENA_SEQ1, 1=enable interrupt
    // bit 10        0:      INT_MOD_SEQ1, 0=INT_SEQ1 is set at the end of every SEQ1 sequence.
    // bit 9         0:      reserved
    // bit 8         1:      ePWM_SOCA_SEQ1, 1=SEQ1 start from ePWM_SOCA trigger
    // bit 7         0:      EXT_SOC_SEQ1, 0=no action
    // bit 6         0:      RST_SEQ2, 0=no action
    // bit 5         0:      SOC_SEQ2, 0=clear any pending SOCs
    // bit 4         0:      reserved
    // bit 3         1:      INT_ENA_SEQ2, 1=enable interrupt
    // bit 2         0:      INT_MOD_SEQ2, 0=INT_SEQ2 is set at the end of every SEQ2 sequence.
    // bit 1         0:      reserved
    // bit 0         1:      ePWM_SOCB_SEQ2, 1=SEQ2 start from ePWM_SOCB trigger

	//Reset ADC sequencers
	AdcRegs.ADCTRL2.bit.RST_SEQ1 = 0x1;
	AdcRegs.ADCTRL2.bit.RST_SEQ2 = 0x1;

	//--- Enable the ADC interrupt ---
	//PieCtrlRegs.PIEIER1.bit.INTx1 = 1;	// Enable SEQ1INT in PIE group 1
	//PieCtrlRegs.PIEIER1.bit.INTx2 = 1;  // Enable SEQ2INT in PIE group 1
	//IER |= 0x0001;						// Enable INT1 in IER to enable PIE group
}

/**********************************************************************
* Function: Adc_GetVoltage(Uint16 adc_raw)
*
* Description: converts from ADC counts to voltage
**********************************************************************/
extern _iq Adc_GetVoltage(Uint16 adc_raw){
    return _IQmpy(_IQ(adc_raw), AdcScale);
}
