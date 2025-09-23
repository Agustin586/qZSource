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
#include "IQmathLib.h"
#include "digitalPot.h"
#include "MCP4461.h"

/**********************************************************************
                   INTERNAL FUNCTIONS DEFINITION
**********************************************************************/
uint16_t voltageToWiperValue(_iq voltage)
{
    return (uint16_t)(_IQmpy(_IQdiv(_IQ(voltage), _IQ(VCC)), 257.0));
}

/**********************************************************************
                   EXTERNAL FUNCTIONS DEFINITION
**********************************************************************/

/**********************************************************************
* Function: digitalPot_configureReferences()
* Description: Configure the potentiometers references according to
*              the values in digitalPot.h file.
*
* Usage: execute once at the begining of the program.
*
**********************************************************************/
extern void digitalPot_configureReferences(void)
{
    uint16_t i = 0, j = 0;

    MCP4461_init();

    for(i = 0; i<4; i++){
        if(MCP4461_getNonVolatileWiper(DIGPOT0, (MCP4461Wiper_t)i) == MCP4461_DEFAULT_WIPER) j++;
    }
    for(i = 0; i<4; i++){
        if(MCP4461_getNonVolatileWiper(DIGPOT1, (MCP4461Wiper_t)i) == MCP4461_DEFAULT_WIPER) j++;
    }

    MCP4461_setNonVolatileWiper(DIGPOT0, WIPER0, voltageToWiperValue(VREF_Iin));
    MCP4461_setNonVolatileWiper(DIGPOT0, WIPER1, voltageToWiperValue(VREF_IL1p));
    MCP4461_setNonVolatileWiper(DIGPOT0, WIPER2, voltageToWiperValue(VREF_VC12));
    MCP4461_setNonVolatileWiper(DIGPOT0, WIPER3, voltageToWiperValue(VREF_IL1m));
    MCP4461_setNonVolatileWiper(DIGPOT1, WIPER0, voltageToWiperValue(VREF_IBRDGm));
    MCP4461_setNonVolatileWiper(DIGPOT1, WIPER1, voltageToWiperValue(VREF_IACm));
    MCP4461_setNonVolatileWiper(DIGPOT1, WIPER2, voltageToWiperValue(VREF_IBRDGp));
    MCP4461_setNonVolatileWiper(DIGPOT1, WIPER3, voltageToWiperValue(VREF_IACp));

    MCP4461_setVolatileWiper(DIGPOT0, WIPER0, voltageToWiperValue(VREF_Iin));
    MCP4461_setVolatileWiper(DIGPOT0, WIPER1, voltageToWiperValue(VREF_IL1p));
    MCP4461_setVolatileWiper(DIGPOT0, WIPER2, voltageToWiperValue(VREF_VC12));
    MCP4461_setVolatileWiper(DIGPOT0, WIPER3, voltageToWiperValue(VREF_IL1m));
    MCP4461_setVolatileWiper(DIGPOT1, WIPER0, voltageToWiperValue(VREF_IBRDGm));
    MCP4461_setVolatileWiper(DIGPOT1, WIPER1, voltageToWiperValue(VREF_IACm));
    MCP4461_setVolatileWiper(DIGPOT1, WIPER2, voltageToWiperValue(VREF_IBRDGp));
    MCP4461_setVolatileWiper(DIGPOT1, WIPER3, voltageToWiperValue(VREF_IACp));
}
