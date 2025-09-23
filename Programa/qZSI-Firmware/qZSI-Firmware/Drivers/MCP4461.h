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

#ifndef MCP4461_H
#define MCP4461_H

/**********************************************************************
                             INCLUDES
**********************************************************************/
#include "stdint.h"

/*==================[cplusplus]======================================*/
#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************************
                       MACROS AND TYPEDEFS
**********************************************************************/
#define MCP4461_DEFAULT_WIPER 0x80

typedef enum{
    DIGPOT0 = 0b0101100,
    DIGPOT1 = 0b0101101,
    DIGPOT2 = 0b0101110,
    DIGPOT3 = 0b0101111
}MCP4461Address_t;

typedef enum{
    WIPER0 = 0,
    WIPER1,
    WIPER2,
    WIPER3
}MCP4461Wiper_t;

/**********************************************************************
                   EXTERNAL FUNCTIONS DECLARATION
**********************************************************************/
extern void MCP4461_init(void);
extern void MCP4461_setVolatileWiper(MCP4461Address_t pot, MCP4461Wiper_t wiper, uint16_t wiper_value);
extern void MCP4461_setNonVolatileWiper(MCP4461Address_t pot, MCP4461Wiper_t wiper, uint16_t wiper_value);
extern uint16_t MCP4461_getVolatileWiper(MCP4461Address_t pot, MCP4461Wiper_t wiper);
extern uint16_t MCP4461_getNonVolatileWiper(MCP4461Address_t pot, MCP4461Wiper_t wiper);
extern uint16_t MCP4461_getStatus(MCP4461Address_t pot);

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* MCP4461_H */
