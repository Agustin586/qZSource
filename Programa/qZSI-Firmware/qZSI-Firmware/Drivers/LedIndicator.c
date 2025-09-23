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
 */

/**********************************************************************
                             INCLUDES
**********************************************************************/
#include "LedIndicator.h"
#include "../Inits/Gpio.h"

/**********************************************************************
                             MACROS
**********************************************************************/
#define LED_ARRANQUE_TOGGLE_MS 500
#define LED_FALLA_TOGGLE_MS 100

/**********************************************************************
                   INTERNAL VARIABLES DECLARATION
**********************************************************************/
static ledIndicator_estado _estado;

/**********************************************************************
                   EXTERNAL FUNCTIONS DEFINITION
**********************************************************************/

/**********************************************************************
* Function: ledIndicator_init()
* Description: led indicator driver init function.
*
**********************************************************************/
extern void ledIndicator_init(void){
    GpioDataRegs.GPASET.bit.GPIO_LED_1 = 1;     // GPIO_LED_1 = 0 (Los LEDs funcionan con lógica inversa)
    GpioDataRegs.GPASET.bit.GPIO_LED_2 = 1;     // GPIO_LED_2 = 0
    GpioDataRegs.GPASET.bit.GPIO_LED_3 = 1;     // GPIO_LED_3 = 0
    GpioDataRegs.GPASET.bit.GPIO_LED_4 = 1;     // GPIO_LED_4 = 0
}

/**********************************************************************
* Function: ledIndicator_setCode()
* Description: set the LEDs 2, 3 and 4 to represent a numeric code.
*
* Usage: ledIndicator_setCode(code);
*
* Input Parameters: code = numeric code to represent with the LEDs.
*
**********************************************************************/
extern void ledIndicator_setCode(unsigned char code){
    switch(code){
        case 0:
            GpioDataRegs.GPASET.bit.GPIO_LED_2 = 1;     // GPIO_LED_2 = 0
            GpioDataRegs.GPASET.bit.GPIO_LED_3 = 1;     // GPIO_LED_3 = 0
            GpioDataRegs.GPASET.bit.GPIO_LED_4 = 1;     // GPIO_LED_4 = 0
            break;

        case 1:
            GpioDataRegs.GPASET.bit.GPIO_LED_2 = 1;     // GPIO_LED_2 = 0
            GpioDataRegs.GPASET.bit.GPIO_LED_3 = 1;     // GPIO_LED_3 = 0
            GpioDataRegs.GPACLEAR.bit.GPIO_LED_4 = 1;   // GPIO_LED_4 = 1
            break;

        case 2:
            GpioDataRegs.GPASET.bit.GPIO_LED_2 = 1;     // GPIO_LED_2 = 0
            GpioDataRegs.GPACLEAR.bit.GPIO_LED_3 = 1;   // GPIO_LED_3 = 1
            GpioDataRegs.GPASET.bit.GPIO_LED_4 = 1;     // GPIO_LED_4 = 0
            break;

        case 3:
            GpioDataRegs.GPASET.bit.GPIO_LED_2 = 1;     // GPIO_LED_2 = 0
            GpioDataRegs.GPACLEAR.bit.GPIO_LED_3 = 1;   // GPIO_LED_3 = 1
            GpioDataRegs.GPACLEAR.bit.GPIO_LED_4 = 1;   // GPIO_LED_4 = 1
            break;

        case 4:
            GpioDataRegs.GPACLEAR.bit.GPIO_LED_2 = 1;   // GPIO_LED_2 = 1
            GpioDataRegs.GPASET.bit.GPIO_LED_3 = 1;     // GPIO_LED_3 = 0
            GpioDataRegs.GPASET.bit.GPIO_LED_4 = 1;     // GPIO_LED_4 = 0
            break;

        case 5:
            GpioDataRegs.GPACLEAR.bit.GPIO_LED_2 = 1;   // GPIO_LED_2 = 1
            GpioDataRegs.GPASET.bit.GPIO_LED_3 = 1;     // GPIO_LED_3 = 1
            GpioDataRegs.GPACLEAR.bit.GPIO_LED_4 = 1;   // GPIO_LED_4 = 0
            break;

        case 6:
            GpioDataRegs.GPACLEAR.bit.GPIO_LED_2 = 1;   // GPIO_LED_2 = 1
            GpioDataRegs.GPACLEAR.bit.GPIO_LED_3 = 1;   // GPIO_LED_3 = 1
            GpioDataRegs.GPASET.bit.GPIO_LED_4 = 1;     // GPIO_LED_4 = 0
            break;

        case 7:
            GpioDataRegs.GPACLEAR.bit.GPIO_LED_2 = 1;   // GPIO_LED_2 = 1
            GpioDataRegs.GPACLEAR.bit.GPIO_LED_3 = 1;   // GPIO_LED_3 = 1
            GpioDataRegs.GPACLEAR.bit.GPIO_LED_4 = 1;   // GPIO_LED_4 = 1
            break;

        default:
            GpioDataRegs.GPASET.bit.GPIO_LED_2 = 1;     // GPIO_LED_2 = 0
            GpioDataRegs.GPASET.bit.GPIO_LED_3 = 1;     // GPIO_LED_3 = 0
            GpioDataRegs.GPASET.bit.GPIO_LED_4 = 1;     // GPIO_LED_4 = 0
            break;
    }
}

/**********************************************************************
* Function: ledIndicator_setStateLed()
* Description: set the LED 4 according to the state of the inverter.
*
* Usage: ledIndicator_setStateLed(state);
*
* Input Parameters: state = state of the inverter.
*
**********************************************************************/
extern void ledIndicator_setStateLed(ledIndicator_estado estado){
    _estado = estado;
    if(_estado == LED_INDICATOR_FUNCIONANDO) GpioDataRegs.GPACLEAR.bit.GPIO_LED_1 = 1;   // GPIO_LED_1 = 1
}

extern void ledIndicator_task1ms(){
    static unsigned char timer = 0;
    if(timer) timer--;
    else{
        if(_estado == LED_INDICATOR_ARRANQUE){
            timer = LED_ARRANQUE_TOGGLE_MS;
            GpioDataRegs.GPATOGGLE.bit.GPIO_LED_1 = 1;     // GPIO_LED_1 TOGGLE
        }
        else if(_estado == LED_INDICATOR_FALLA){
            timer = LED_FALLA_TOGGLE_MS;
            GpioDataRegs.GPATOGGLE.bit.GPIO_LED_1 = 1;     // GPIO_LED_1 TOGGLE
        }
    }
}
