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
#include "mefArranque.h"
#include "../board.h"
#include "../Inverter/qZSI.h"
#include "../Drivers/LedIndicator.h"
#include "../Inverter/inverterConfig.h"

/**********************************************************************
                       MACROS AND TYPEDEFS
**********************************************************************/
#define START_DELAY_TIME_MS 5000
#define VO_LOW_VOLTAGE _IQ(20.0)

typedef enum {
    INICIO_ARRANQUE = 0,
    SW_IN_RESISTIVO,
    AUMENTANDO_VBUS,
    CONTROLADOR_VBUS_ENGANCHADO,
    AUMENTANDO_VO,
    CONTROLADOR_VO_ENGANCHADO,
    RUN
} estados_mef_arranque_t;


/**********************************************************************
                  INTERNAL VARIABLES DECLARATION
**********************************************************************/
static estados_mef_arranque_t estadoMefArranque;
static int timer;
static char arranqueVBUS, arranqueVO;

/**********************************************************************
                  EXTERNAL FUNCTIONS DEFINITION
**********************************************************************/
void mefArranque_init(){
    estadoMefArranque = INICIO_ARRANQUE;
    timer = START_DELAY_TIME_MS;
    arranqueVBUS = 1;
    arranqueVO = 1;
    ledIndicator_setStateLed(LED_INDICATOR_ARRANQUE);
    qzsi_resetReferences();
    GpioDataRegs.GPACLEAR.bit.GPIO_VSEL = 1;    // VSEL = 0
    GpioDataRegs.GPACLEAR.bit.GPIO_SW_IN = 1;   // SW_IN = 0
}

void mefArranque(){

    switch (estadoMefArranque){

    case INICIO_ARRANQUE:
        if(!timer){
            GpioDataRegs.GPASET.bit.GPIO_SW_IN = 1;        // SW_IN = 1
            timer = _IQmpy(START_DELAY_TIME_MS, 2);
            ledIndicator_setCode(1);
            estadoMefArranque = SW_IN_RESISTIVO;
        }
        break;

    case SW_IN_RESISTIVO:
        if(!timer){
            GpioDataRegs.GPASET.bit.GPIO_VSEL = 1;         // VSEL = 1
            qzsi_habilitarSalida();                        // Habilita la salida de tensión con D fijo inicialmente
            qzsi_initVBUS(DC_BUS_VOLTAGE_REFERENCE);
            ledIndicator_setCode(2);
            estadoMefArranque = AUMENTANDO_VBUS;
        }
        break;

    case AUMENTANDO_VBUS:
        if(qzsi_getVBUS() == DC_BUS_VOLTAGE_REFERENCE){
            arranqueVBUS = 0;
            timer = START_DELAY_TIME_MS;
            ledIndicator_setCode(3);
            estadoMefArranque = CONTROLADOR_VBUS_ENGANCHADO;
        }
        break;

    case CONTROLADOR_VBUS_ENGANCHADO:
        if(!timer){
            qzsi_clearVoInicial();                         // Vo deja de tener un Dmax fijo y se ajusta según desiredVo y VBUS
            qzsi_setVo(MAX_OUTPUT_VOLTAGE_REFERENCE);
            ledIndicator_setCode(4);
            estadoMefArranque = AUMENTANDO_VO;
        }
        break;

    case AUMENTANDO_VO:
        if(qzsi_getVo() == MAX_OUTPUT_VOLTAGE_REFERENCE){
            arranqueVO = 0;
            timer = START_DELAY_TIME_MS;
            ledIndicator_setCode(5);
            estadoMefArranque = CONTROLADOR_VO_ENGANCHADO;
        }
        break;

    case CONTROLADOR_VO_ENGANCHADO:
        if(!timer){
            ledIndicator_setCode(0);
            ledIndicator_setStateLed(LED_INDICATOR_FUNCIONANDO);
            estadoMefArranque = RUN;
        }
        break;

    case RUN:
        break;

    default:
        mefArranque_init();
        break;
    }
}


char mefArranque_getArranqueVBUS(){
    return arranqueVBUS;
}

char mefArranque_getArranqueVO(){
    return arranqueVO;
}

void mefArranque_task1ms(){
    if (timer) timer--;
}
