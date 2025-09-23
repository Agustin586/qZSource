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
#include "Gpio.h"

/**********************************************************************
                   EXTERNAL FUNCTIONS DEFINITION
**********************************************************************/

/**********************************************************************
* Function: InitGpio()
*
* Description: Initializes the shared GPIO pins on the F2833x
**********************************************************************/
extern void InitGpio(void){
	asm(" EALLOW");								// Enable EALLOW protected register access

	//--- Group A pins
	GpioCtrlRegs.GPACTRL.all  = 0x00000000;		// QUALPRD = SYSCLKOUT for all group A GPIO
	GpioCtrlRegs.GPAQSEL1.all = 0x00000000;		// No qualification for all group A GPIO 0-15
	GpioCtrlRegs.GPAQSEL2.all = 0x00000000;		// No qualification for all group A GPIO 16-31
	GpioCtrlRegs.GPADIR.all   = 0x00000000;		// All group A GPIO are inputs
	GpioCtrlRegs.GPAPUD.all   = 0xFFFFFFFF;		// All group A pullups disabled

	GpioCtrlRegs.GPAMUX1.bit.GPIO0  = 1;		// 0=GPIO  1=EPWM1A     2=rsvd       3=rsvd
	GpioCtrlRegs.GPAMUX1.bit.GPIO1  = 1;		// 0=GPIO  1=EPWM1B     2=ECAP6      3=rsvd
	GpioCtrlRegs.GPAMUX1.bit.GPIO2  = 1;		// 0=GPIO  1=EPWM2A     2=rsvd       3=rsvd
	GpioCtrlRegs.GPAMUX1.bit.GPIO3  = 1;		// 0=GPIO  1=EPWM2B     2=ECAP5      3=rsvd
	GpioCtrlRegs.GPAMUX1.bit.GPIO4  = 1;		// 0=GPIO  1=EPWM3A     2=rsvd       3=rsvd
	GpioCtrlRegs.GPAMUX1.bit.GPIO5  = 1;		// 0=GPIO  1=EPWM3B     2=MFSRA      3=ECAP1
	GpioCtrlRegs.GPAMUX1.bit.GPIO6  = 1;		// 0=GPIO  1=EPWM4A     2=EPWMSYNCI  3=EPWMSYNCO
	GpioCtrlRegs.GPAMUX1.bit.GPIO7  = 1;		// 0=GPIO  1=EPWM4B     2=MCLKRA     3=ECAP2
	GpioCtrlRegs.GPAMUX1.bit.GPIO8  = 0;		// 0=GPIO  1=EPWM5A     2=CANTXB     3=ADCSOCAO
	GpioCtrlRegs.GPAMUX1.bit.GPIO9  = 1;		// 0=GPIO  1=EPWM5B     2=SCITXDB    3=ECAP3
	GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 0;		// 0=GPIO  1=EPWM6A     2=CANRXB     3=ADCSOCBO
	GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 0;		// 0=GPIO  1=EPWM6B     2=SCIRXDB    3=ECAP4
	GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 0;		// 0=GPIO  1=TZ1        2=CANTXB     3=SPISIMOB
	GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 0;		// 0=GPIO  1=TZ2        2=CANRXB     3=SPISOMIB
	GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 0;		// 0=GPIO  1=TZ3/XHOLD  2=SCITXDB    3=SPICLKB
	GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 0;		// 0=GPIO  1=TZ4/XHOLDA 2=SCIRXDB    3=SPISTEB

	GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 3;		// 0=GPIO  1=SPISIMOA   2=CANTXB     3=TZ5
	GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 0;		// 0=GPIO  1=SPISOMIA   2=CANRXB     3=TZ6
	GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 0;		// 0=GPIO  1=SPICLKA    2=SCITXDB    3=CANRXA
	GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 0;		// 0=GPIO  1=SPISTEA    2=SCIRXDB    3=CANTXA
	GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 0;		// 0=GPIO  1=EQEP1A     2=MDXA       3=CANTXB
	GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 0;		// 0=GPIO  1=EQEP1B     2=MDRA       3=CANRXB
	GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 0;		// 0=GPIO  1=EQEP1S     2=MCLKXA     3=SCITXDB
	GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 0;		// 0=GPIO  1=EQEP1I     2=MFSXA      3=SCIRXDB
	GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 0;		// 0=GPIO  1=ECAP1      2=EQEP2A     3=MDXB
	GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 0;		// 0=GPIO  1=ECAP2      2=EQEP2B     3=MDRB
	GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 0;		// 0=GPIO  1=ECAP3      2=EQEP2I     3=MCLKXB
	GpioCtrlRegs.GPAMUX2.bit.GPIO27 = 0;		// 0=GPIO  1=ECAP4      2=EQEP2S     3=MFSXB  
	GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 1;		// 0=GPIO  1=SCIRXDA    2=XZCS6      3=XZCS6
	GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 1;		// 0=GPIO  1=SCITXDA    2=XA19       3=XA19
	GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 1;		// 0=GPIO  1=CANRXA     2=XA18       3=XA18
	GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 1;		// 0=GPIO  1=CANTXA     2=XA17       3=XA17

	//--- Group B pins
	GpioCtrlRegs.GPBCTRL.all  = 0x00000000;		// QUALPRD = SYSCLKOUT for all group B GPIO
	GpioCtrlRegs.GPBQSEL1.all = 0x00000000;		// No qualification for all group B GPIO 32-47
	GpioCtrlRegs.GPBQSEL2.all = 0x00000000;		// No qualification for all group B GPIO 48-63
	GpioCtrlRegs.GPBDIR.all   = 0x00000000;		// All group B GPIO are inputs
	GpioCtrlRegs.GPBPUD.all   = 0xFFFFFFFF;		// All group B pullups disabled

	GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 1;		// 0=GPIO  1=SDAA      2=EPWMSYNCI  3=ADCSOCAO
	GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 1;		// 0=GPIO  1=SCLA      2=EPWMSYNCO  3=ADCSOCBO
	GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 0;		// 0=GPIO  1=ECAP1     2=XREADY     3=XREADY
	GpioCtrlRegs.GPBMUX1.bit.GPIO35 = 0;		// 0=GPIO  1=SCITXDA   2=XR/W       3=XR/W
	GpioCtrlRegs.GPBMUX1.bit.GPIO36 = 0;		// 0=GPIO  1=SCIRXDA   2=XZCS0      3=XZCS0
	GpioCtrlRegs.GPBMUX1.bit.GPIO37 = 0;		// 0=GPIO  1=ECAP2     2=XZCS7      3=XZCS7
	GpioCtrlRegs.GPBMUX1.bit.GPIO38 = 0;		// 0=GPIO  1=rsvd      2=XWE0       3=XWE0
	GpioCtrlRegs.GPBMUX1.bit.GPIO39 = 0;		// 0=GPIO  1=rsvd      2=XA16       3=XA16
	GpioCtrlRegs.GPBMUX1.bit.GPIO40 = 0;		// 0=GPIO  1=rsvd      2=XA0/XWE1   3=XA0/XWE1
	GpioCtrlRegs.GPBMUX1.bit.GPIO41 = 0;		// 0=GPIO  1=rsvd      2=XA1        3=XA1
	GpioCtrlRegs.GPBMUX1.bit.GPIO42 = 0;		// 0=GPIO  1=rsvd      2=XA2        3=XA2
	GpioCtrlRegs.GPBMUX1.bit.GPIO43 = 0;		// 0=GPIO  1=rsvd      2=XA3        3=XA3
	GpioCtrlRegs.GPBMUX1.bit.GPIO44 = 0;		// 0=GPIO  1=rsvd      2=XA4        3=XA4
	GpioCtrlRegs.GPBMUX1.bit.GPIO45 = 0;		// 0=GPIO  1=rsvd      2=XA5        3=XA5
	GpioCtrlRegs.GPBMUX1.bit.GPIO46 = 0;		// 0=GPIO  1=rsvd      2=XA6        3=XA6
	GpioCtrlRegs.GPBMUX1.bit.GPIO47 = 0;		// 0=GPIO  1=rsvd      2=XA7        3=XA7

	GpioCtrlRegs.GPBMUX2.bit.GPIO48 = 0;		// 0=GPIO  1=ECAP5     2=XD31       3=XD31
	GpioCtrlRegs.GPBMUX2.bit.GPIO49 = 0;		// 0=GPIO  1=ECAP6     2=XD30       3=XD30
	GpioCtrlRegs.GPBMUX2.bit.GPIO50 = 0;		// 0=GPIO  1=EQEP1A    2=XD29       3=XD29
	GpioCtrlRegs.GPBMUX2.bit.GPIO51 = 0;		// 0=GPIO  1=EQEP1B    2=XD28       3=XD28
	GpioCtrlRegs.GPBMUX2.bit.GPIO52 = 0;		// 0=GPIO  1=EQEP1S    2=XD27       3=XD27
	GpioCtrlRegs.GPBMUX2.bit.GPIO53 = 0;		// 0=GPIO  1=EQEP1I    2=XD26       3=XD26
	GpioCtrlRegs.GPBMUX2.bit.GPIO54 = 0;		// 0=GPIO  1=SPISIMOA  2=XD25       3=XD25
	GpioCtrlRegs.GPBMUX2.bit.GPIO55 = 0;		// 0=GPIO  1=SPISOMIA  2=XD24       3=XD24
	GpioCtrlRegs.GPBMUX2.bit.GPIO56 = 0;		// 0=GPIO  1=SPICLKA   2=XD23       3=XD23
	GpioCtrlRegs.GPBMUX2.bit.GPIO57 = 0;		// 0=GPIO  1=SPISTEA   2=XD22       3=XD22
	GpioCtrlRegs.GPBMUX2.bit.GPIO58 = 0;		// 0=GPIO  1=MCLKRA    2=XD21       3=XD21
	GpioCtrlRegs.GPBMUX2.bit.GPIO59 = 0;		// 0=GPIO  1=MFSRA     2=XD20       3=XD20
	GpioCtrlRegs.GPBMUX2.bit.GPIO60 = 0;		// 0=GPIO  1=MCLKRB    2=XD19       3=XD19
	GpioCtrlRegs.GPBMUX2.bit.GPIO61 = 0;		// 0=GPIO  1=MFSRB     2=XD18       3=XD18
	GpioCtrlRegs.GPBMUX2.bit.GPIO62 = 0;		// 0=GPIO  1=SCIRXDC   2=XD17       3=XD17
	GpioCtrlRegs.GPBMUX2.bit.GPIO63 = 0;		// 0=GPIO  1=SCITXDC   2=XD16       3=XD16

	//--- Group C pins
	GpioCtrlRegs.GPCDIR.all = 0x00000000;		// All group C GPIO are inputs
	GpioCtrlRegs.GPCPUD.all = 0xFFFFFFFF;		// All group C pullups disabled

	GpioCtrlRegs.GPCMUX1.bit.GPIO64 = 0;		// 0=GPIO  1=GPIO      2=XD15       3=XD15
	GpioCtrlRegs.GPCMUX1.bit.GPIO65 = 0;		// 0=GPIO  1=GPIO      2=XD14       3=XD14
	GpioCtrlRegs.GPCMUX1.bit.GPIO66 = 0;		// 0=GPIO  1=GPIO      2=XD13       3=XD13
	GpioCtrlRegs.GPCMUX1.bit.GPIO67 = 0;		// 0=GPIO  1=GPIO      2=XD12       3=XD12
	GpioCtrlRegs.GPCMUX1.bit.GPIO68 = 0;		// 0=GPIO  1=GPIO      2=XD11       3=XD11
	GpioCtrlRegs.GPCMUX1.bit.GPIO69 = 0;		// 0=GPIO  1=GPIO      2=XD10       3=XD10
	GpioCtrlRegs.GPCMUX1.bit.GPIO70 = 0;		// 0=GPIO  1=GPIO      2=XD9        3=XD9
	GpioCtrlRegs.GPCMUX1.bit.GPIO71 = 0;		// 0=GPIO  1=GPIO      2=XD8        3=XD8
	GpioCtrlRegs.GPCMUX1.bit.GPIO72 = 0;		// 0=GPIO  1=GPIO      2=XD7        3=XD7
	GpioCtrlRegs.GPCMUX1.bit.GPIO73 = 0;		// 0=GPIO  1=GPIO      2=XD6        3=XD6
	GpioCtrlRegs.GPCMUX1.bit.GPIO74 = 0;		// 0=GPIO  1=GPIO      2=XD5        3=XD5
	GpioCtrlRegs.GPCMUX1.bit.GPIO75 = 0;		// 0=GPIO  1=GPIO      2=XD4        3=XD4
	GpioCtrlRegs.GPCMUX1.bit.GPIO76 = 0;		// 0=GPIO  1=GPIO      2=XD3        3=XD3
	GpioCtrlRegs.GPCMUX1.bit.GPIO77 = 0;		// 0=GPIO  1=GPIO      2=XD2        3=XD2
	GpioCtrlRegs.GPCMUX1.bit.GPIO78 = 0;		// 0=GPIO  1=GPIO      2=XD1        3=XD1
	GpioCtrlRegs.GPCMUX1.bit.GPIO79 = 0;		// 0=GPIO  1=GPIO      2=XD0        3=XD0

	GpioCtrlRegs.GPCMUX2.bit.GPIO80 = 0;		// 0=GPIO  1=GPIO      2=XA8        3=XA8
	GpioCtrlRegs.GPCMUX2.bit.GPIO81 = 0;		// 0=GPIO  1=GPIO      2=XA9        3=XA9
	GpioCtrlRegs.GPCMUX2.bit.GPIO82 = 0;		// 0=GPIO  1=GPIO      2=XA10       3=XA10
	GpioCtrlRegs.GPCMUX2.bit.GPIO83 = 0;		// 0=GPIO  1=GPIO      2=XA11       3=XA11
	GpioCtrlRegs.GPCMUX2.bit.GPIO84 = 0;		// 0=GPIO  1=GPIO      2=XA12       3=XA12
	GpioCtrlRegs.GPCMUX2.bit.GPIO85 = 0;		// 0=GPIO  1=GPIO      2=XA13       3=XA13
	GpioCtrlRegs.GPCMUX2.bit.GPIO86 = 0;		// 0=GPIO  1=GPIO      2=XA14       3=XA14
	GpioCtrlRegs.GPCMUX2.bit.GPIO87 = 0;		// 0=GPIO  1=GPIO      2=XA15       3=XA15

	//--- External interrupt selection
	GpioIntRegs.GPIOXINT1SEL.all = 0x0000;		// GPIO0 is XINT1 source
	GpioIntRegs.GPIOXINT2SEL.all = 0x0000;		// GPIO0 is XINT2 source
	GpioIntRegs.GPIOXINT3SEL.all = 0x0000;		// GPIO32 is XINT3 source
	GpioIntRegs.GPIOXINT4SEL.all = 0x0000;		// GPIO32 is XINT4 source
	GpioIntRegs.GPIOXINT5SEL.all = 0x0000;		// GPIO32 is XINT5 source
	GpioIntRegs.GPIOXINT6SEL.all = 0x0000;		// GPIO32 is XINT6 source
	GpioIntRegs.GPIOXINT7SEL.all = 0x0000;		// GPIO32 is XINT7 source
	GpioIntRegs.GPIOXNMISEL.all = 0x0000;		// GPIO0 is XNMI source

	XIntruptRegs.XINT1CR.all = 0x0000;			// XINT1 disabled
	XIntruptRegs.XINT2CR.all = 0x0000;			// XINT2 disabled
	XIntruptRegs.XINT3CR.all = 0x0000;			// XINT3 disabled
	XIntruptRegs.XINT4CR.all = 0x0000;			// XINT4 disabled
	XIntruptRegs.XINT5CR.all = 0x0000;			// XINT5 disabled
	XIntruptRegs.XINT6CR.all = 0x0000;			// XINT6 disabled
	XIntruptRegs.XINT7CR.all = 0x0000;			// XINT7 disabled
	XIntruptRegs.XNMICR.all = 0x0000;			// XNMI disabled

	//--- Low-power mode selection
	GpioIntRegs.GPIOLPMSEL.all = 0x00000000;	// No pin selected for HALT and STANDBY wakeup (reset default)

	//--- Selected pin configurations
	GpioCtrlRegs.GPADIR.bit.GPIO_SW_IN = 1;         // GPIO11 is an output
    GpioCtrlRegs.GPADIR.bit.GPIO_LVL_OE = 1;        // GPIO13 is an output
	GpioCtrlRegs.GPADIR.bit.GPIO_AUX_OUT_2 = 1;     // GPIO14 is an output
	GpioCtrlRegs.GPADIR.bit.GPIO_VSEL = 1;          // GPIO17 is an output
    GpioCtrlRegs.GPADIR.bit.GPIO_DPOT_RESET = 1;    // GPIO18 is an output
	GpioCtrlRegs.GPADIR.bit.GPIO_LED_2 = 1;         // GPIO19 is an output
	GpioCtrlRegs.GPADIR.bit.GPIO_LED_3 = 1;         // GPIO21 is an output
	GpioCtrlRegs.GPADIR.bit.GPIO_LED_4 = 1;         // GPIO23 is an output
	GpioCtrlRegs.GPADIR.bit.GPIO_AUX_OUT_1 = 1;     // GPIO25 is an output
	GpioCtrlRegs.GPADIR.bit.GPIO_LED_1 = 1;         // GPIO27 is an output

    GpioCtrlRegs.GPBDIR.bit.GPIO_CLR_FAULT = 1;     // GPIO48 is an output
    GpioCtrlRegs.GPBDIR.bit.GPIO_RST = 1;           // GPIO49 is an output

    //--- Initialize outputs states
    GpioDataRegs.GPACLEAR.bit.GPIO_SW_IN = 1;
    GpioDataRegs.GPACLEAR.bit.GPIO_LVL_OE = 1;
    GpioDataRegs.GPACLEAR.bit.GPIO_AUX_OUT_2 = 1;
    GpioDataRegs.GPACLEAR.bit.GPIO_VSEL = 1;
    GpioDataRegs.GPACLEAR.bit.GPIO_DPOT_RESET = 1;
    GpioDataRegs.GPACLEAR.bit.GPIO_LED_2 = 1;
    GpioDataRegs.GPACLEAR.bit.GPIO_LED_3 = 1;
    GpioDataRegs.GPACLEAR.bit.GPIO_LED_4 = 1;
    GpioDataRegs.GPACLEAR.bit.GPIO_AUX_OUT_1 = 1;
    GpioDataRegs.GPACLEAR.bit.GPIO_LED_1 = 1;

    GpioDataRegs.GPBCLEAR.bit.GPIO_CLR_FAULT = 1;
    GpioDataRegs.GPBCLEAR.bit.GPIO_RST = 1;

    //--- Finish up
	asm(" EDIS");								// Disable EALLOW protected register access

}

/**********************************************************************
* Function: gpio_getSw()
* Description: Returns the state of a switch
*
* Usage: gpio_getSw(sw);
* Input Parameters: sw = switch to get the value
*
* Output Value: 1 if the switch is pressed, 0 if it is released
**********************************************************************/
extern Uint16 gpio_getSw(board_swId_enum sw){
    switch(sw){
    case BOARD_SW_CLR_FAULT:
        return GpioDataRegs.GPADAT.bit.GPIO_CLEAR_BTN;

    case BOARD_SW_RST_MANUAL:
        return GpioDataRegs.GPADAT.bit.GPIO_RST_BTN;

    default:
        return 0;
    }
}
