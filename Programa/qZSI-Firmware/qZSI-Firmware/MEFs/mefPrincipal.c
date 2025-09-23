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
#include "mefPrincipal.h"
#include "mefArranque.h"
#include "../board.h"
#include "../Drivers/key.h"
#include "../Drivers/LedIndicator.h"
#include "../Inverter/inverterConfig.h"

/**********************************************************************
                       MACROS AND TYPEDEFS
**********************************************************************/
#define LED_TOGGLE_100_MS 100
#define CLEAR_FAULT_TIME 100

typedef enum {
    INICIO = 0,
    CLEAR_INICIAL,
    RUN,
    FALLA,
    REST_FALLA,
    WAIT_HW_READY
} estados_mef_principal_t;


/**********************************************************************
                  INTERNAL VARIABLES DECLARATION
**********************************************************************/
static estados_mef_principal_t estadoMefPrincipal;
static char falla;
static int timer;

/**********************************************************************
                  EXTERNAL FUNCTIONS DEFINITION
**********************************************************************/
void mefPrincipal_init(){
    estadoMefPrincipal = INICIO;
    timer = CLEAR_FAULT_TIME;
    GpioDataRegs.GPBCLEAR.bit.GPIO_RST = 1;          // Pone en bajo el pin de reset de falla de los drivers (lógica inversa)
    GpioDataRegs.GPBCLEAR.bit.GPIO_CLR_FAULT = 1;    // Pone en bajo el pin de reset de falla en flip-flop (lógica inversa)
    GpioDataRegs.GPASET.bit.GPIO_LVL_OE = 1;         // Pone en alto el output enable de los level shifters
    mefArranque_init();
}

void mefPrincipal(void){

    switch (estadoMefPrincipal){

    case INICIO:
        if(!timer){
            GpioDataRegs.GPBSET.bit.GPIO_RST = 1;          // Pone en alto el pin de reset de falla de los drivers (lógica inversa)
            #if HARDWARE_PROTECTIONS == 1
            GpioDataRegs.GPBSET.bit.GPIO_CLR_FAULT = 1;    // Pone en alto el pin de reset de falla en flip-flop (lógica inversa)
            #endif
            estadoMefPrincipal = CLEAR_INICIAL;
        }
        break;


    case CLEAR_INICIAL:
        if(qzsi_hardwareReady()){
            key_clearFlags(BOARD_SW_RST_MANUAL);            // Limpio memoria de posibles pulsadas anteriores
            epwm_rstTripZone();
            falla = 0;
            estadoMefPrincipal = RUN;
        }
        break;

    case RUN:
        mefArranque();
        if(falla){
            ledIndicator_setStateLed(LED_INDICATOR_FALLA);
            key_clearFlags(BOARD_SW_CLR_FAULT);             // Limpio memoria de pulsadas anteriores
            estadoMefPrincipal = FALLA;
        }
        if(key_getPressEv(BOARD_SW_RST_MANUAL)){
            mefArranque_init();
        }
        break;

    case FALLA:
        if(key_getPressEv(BOARD_SW_CLR_FAULT)){             // Al pulsar el botón, se pone en bajo el reset de falla, luego, 100ms después
            timer = CLEAR_FAULT_TIME;                       // se pone en alto. Esto hace que qzsi_hardwareReady() devuelva true cuando todos los
            GpioDataRegs.GPBCLEAR.bit.GPIO_RST = 1;         // dispositivos ya se hayan restaurado. Cuando esto pasa, se resetea el trip zone y
            GpioDataRegs.GPBCLEAR.bit.GPIO_CLR_FAULT = 1;   // se vuelve a ejecutar desde el inicio la secuencia de arranque.
            estadoMefPrincipal = REST_FALLA;
        }
        break;

    case REST_FALLA:
        if(!timer){
            GpioDataRegs.GPBSET.bit.GPIO_RST = 1;
            #if HARDWARE_PROTECTIONS == 1
            GpioDataRegs.GPBSET.bit.GPIO_CLR_FAULT = 1;    // Pone en alto el pin de reset de falla en flip-flop (lógica inversa)
            #endif
            estadoMefPrincipal = WAIT_HW_READY;
        }
        break;

    case WAIT_HW_READY:
        if(qzsi_hardwareReady()){
            mefArranque_init();
            epwm_rstTripZone();
            falla = 0;
            key_clearFlags(BOARD_SW_RST_MANUAL);            // Limpio memoria de pulsadas anteriores
            estadoMefPrincipal = RUN;
        }
        break;

    default:
        mefPrincipal_init();
        break;
    }

}


char mefPrincipal_getArranqueVBUS(){
    return mefArranque_getArranqueVBUS();
}

char mefPrincipal_getArranqueVO(){
    return mefArranque_getArranqueVO();
}

void mefPrincipal_setFalla(){
    falla = 1;
}

char mefPrincipal_getFalla(){
    return falla;
}

void mefPrincipal_task1ms(){
    if (timer) timer--;
    mefArranque_task1ms();
}
