/* Copyright 2017, DSI FCEIA UNR - Sistemas Digitales 2
 *    DSI: http://www.dsi.fceia.unr.edu.ar/
 * Copyright 2017, Diego Alegrechi
 * Copyright 2017, Gustavo Muro
 * Copyright 2023, Guido Cicconi
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
#include "key.h"
#include "../Inits/Gpio.h"

/**********************************************************************
                       MACROS AND TYPEDEFS
**********************************************************************/
#define TIEMPO_ANTIRREBOTE_MS 20

typedef enum
{
	ESPERANDO_ACTIVACION = 0,
	ANTIRREBOTE_ACTIVACION,
	ESPERANDO_DESACTIVACION,
	ANTIRREBOTE_DESACTIVACION
}estPul_enum;

/**********************************************************************
                   INTERNAL VARAIBLES DECLARATION
**********************************************************************/
static estPul_enum estSW[BOARD_SW_ID_TOTAL];
static int32 timerSW[BOARD_SW_ID_TOTAL];
static int32 timerAntibounce[BOARD_SW_ID_TOTAL];
static char pressSW[BOARD_SW_ID_TOTAL];
static char releaseSW[BOARD_SW_ID_TOTAL];

/**********************************************************************
                   EXTERNAL FUNCTIONS DEFINITION
**********************************************************************/

/**********************************************************************
* Function: key_init()
* Description: key driver init function.
*
**********************************************************************/
extern void key_init(void){
    int32 i;

    for (i = 0 ; i < BOARD_SW_ID_TOTAL ; i++)
    {
        estSW[i] = ESPERANDO_ACTIVACION;
        pressSW[i] = 0;
        releaseSW[i] = 0;
        timerSW[i] = 0;
    }
}

/**********************************************************************
* Function: key_periodicTask1ms()
* Description: key driver 1ms periodic function.
*
* Usage: execute this function every 1ms
*
**********************************************************************/
extern void key_periodicTask1ms(void) {
    board_swId_enum i;

	for (i = (board_swId_enum)0; i < BOARD_SW_ID_TOTAL; i++) {

		switch (estSW[i]) {

		case ESPERANDO_ACTIVACION:
			if (gpio_getSw(i)) {
			    timerAntibounce[i] = TIEMPO_ANTIRREBOTE_MS;
				estSW[i] = ANTIRREBOTE_ACTIVACION;
			}
			break;

		case ANTIRREBOTE_ACTIVACION:
		    if(!timerAntibounce[i] && gpio_getSw(i)){
                pressSW[i] = 1;
                estSW[i] = ESPERANDO_DESACTIVACION;
		    }
		    else if(!timerAntibounce[i] && !gpio_getSw(i)){
		        estSW[i] = ESPERANDO_ACTIVACION;
		    }
		    break;

		case ESPERANDO_DESACTIVACION:
			timerSW[i]++;
			if (!gpio_getSw(i)) {
			    timerAntibounce[i] = TIEMPO_ANTIRREBOTE_MS;
				estSW[i] = ANTIRREBOTE_DESACTIVACION;
			}
			break;

		case ANTIRREBOTE_DESACTIVACION:
		    timerSW[i]++;
            if(!timerAntibounce[i] && !gpio_getSw(i)){
                releaseSW[i] = 1;
                timerSW[i]=0;
                estSW[i] = ESPERANDO_ACTIVACION;
            }
            else if(!timerAntibounce[i] && gpio_getSw(i)){
                estSW[i] = ESPERANDO_DESACTIVACION;
            }
		    break;

		default:
			estSW[i] = ESPERANDO_ACTIVACION;
			break;
		}

		if(timerAntibounce[i]) timerAntibounce[i]--;
	}
}

/**********************************************************************
* Function: key_getPressEv()
* Description: function to determine if a switch was pressed.
*
* Usage: swPressEv = key_getPressEv(SW_ID);
*
* Input Parameters: SW_ID = switch ID.
*
* Output value: 1 id the switch "SW_ID" was pressed, 0 if not.
**********************************************************************/
extern char key_getPressEv(board_swId_enum id){
    char ret = 0;

    if (pressSW[id])
    {
        pressSW[id] = 0;
        ret = 1;
    }

    return ret;
}

/**********************************************************************
* Function: key_getReleaseEv()
* Description: function to determine if a switch was released.
*
* Usage: swReleaseEv = key_getReleaseEv(SW_ID);
*
* Input Parameters: SW_ID = switch ID.
*
* Output value: 1 id the switch "SW_ID" was released after being
*               pressed, 0 if not.
**********************************************************************/
extern char key_getReleaseEv(board_swId_enum id){
    char ret = 0;

    if (releaseSW[id])
    {
    	releaseSW[id] = 0;
        ret = 1;
    }

    return ret;
}

/**********************************************************************
* Function: key_getPressed()
* Description: function to determine if a switch was retained for a
*              period of time. To use it, first verify if the switch
*              was pressed with key_getPressEv() function.
*
* Usage: swRetained = key_getPressed(SW_ID, time);
*
* Input Parameters: SW_ID = switch ID.
*                   time = time in ms.
*
* Output value: 1 id the switch "SW_ID" was pressed and retained for
*               "time" miliseconds, 0 if not.
**********************************************************************/
extern char key_getPressed(board_swId_enum id, int32 timeMs){
	if (timerSW[id] >= timeMs) return 1;
	else return 0;
}

/**********************************************************************
* Function: key_clearFlags()
* Description: clears the pressed and released flags for switch "SW_ID"
*
* Usage: key_clearFlags(SW_ID);
*
* Input Parameters: SW_ID = switch ID.
**********************************************************************/
extern void key_clearFlags(board_swId_enum id){
	pressSW[id] = 0;
	releaseSW[id] = 0;
}
