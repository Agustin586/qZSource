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

#ifndef QZSI_H_
#define QZSI_H_

/**********************************************************************
                              INCLUDES
**********************************************************************/
#include "DSP2833x_Device.h"
#include "IQmathLib.h"

/*==================[cplusplus]======================================*/
#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************************
                              DEFINES
**********************************************************************/
#define STEP_TIME_MS 50     // Tiempo de delay entre pasos de un setpoint, ya que todo cambio
                            // de setpoint se realiza de forma gradual, de a una unidad
                            // cada STEP_TIME_MS milisegundos.

#define INITIAL_D _IQ(0.4)  // Ciclo de trabajo del vector activo pico inicial en el arranque
                            // Cuando la flag Vo_inicial está en alto, VoMAX = INITIAL_D*Vbus
                            // Esta flag inicia seteada y se limpia en el estado AUMENTANDO_VBUS
                            // de la MEF de arranque.

/**********************************************************************
   VALORES MAXIMOS DE MAGNITUDES PARA LAS PROTECCIONES POR SOFTWARE
**********************************************************************/
#define MAX_IL1 _IQ(22.0)
#define MIN_IL1 _IQ(-4.0)
#define MAX_Iin _IQ(8.5)
#define MAX_VC1 _IQ(500.0)
#define MAX_Vac _IQ(340.0)
#define MAX_Iac _IQ(9.0)
#define MIN_Iac _IQ(-9.0)

/**********************************************************************
                PARÁMETROS DEL CONTROLADOR DEL BUS DC
**********************************************************************/
#define DC_BUS_XI _IQ(0.7071067812)    // Psi del controlador
#define DC_BUS_WN _IQ(942.47)           // Wn del controlador

/**********************************************************************
                              TYPEDEFS
**********************************************************************/
typedef struct {
    _iq IL1;
    _iq Iin;
    _iq Vin;
    _iq IL;
} stVars_t;

typedef struct {
    _iq IBRDG;
    _iq VC1;
    _iq VDC;
    _iq Vac;
    _iq Iac;
} activeVars_t;

/**********************************************************************
                         EXTERNAL FUNCTIONS
**********************************************************************/
extern void qzsi_init(void);
extern void qzsi_calculateVariables(activeVars_t *av, stVars_t *stv, volatile Uint16 *adc);
extern void qzsi_habilitarSalida(void);
extern void qzsi_clearVoInicial(void);
extern _iq qzsi_getOpenLoop_D(void);
extern _iq qzsi_getOpenLoop_d0(_iq vin);
extern _iq qzsi_dcBusController(_iq IL, _iq VDC, _iq IDC, _iq Vin);
extern _iq qzsi_outputVoltageController(_iq Vo, _iq IL);
extern void qzsi_updatePWM(_iq d0, _iq D);
extern char qzsi_hardwareReady(void);
extern char qzsi_softProtection(_iq IL1, _iq Iin, _iq VC1, _iq Vac, _iq Iac);
extern void qzsi_updateSineReference(void);
extern void qzsi_initVBUS(_iq vbus);
extern void qzsi_setVBUS(_iq vbus);
extern _iq qzsi_getVBUS(void);
extern void qzsi_setVo(_iq vo);
extern _iq qzsi_getVo(void);
extern void qzsi_resetReferences(void);
extern void qzsi_task1ms(void);

/*==================[cplusplus]======================================*/
#ifdef __cplusplus
}
#endif

#endif /* QZSI_H_ */
