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
#include "qZSI.h"
#include "../board.h"
#include "../Drivers/LedIndicator.h"
#include "../Inits/Adc.h"
#include "../Inits/EPwm.h"
#include "../Utils/ringBuffer.h"
#include "inverterConfig.h"

/**********************************************************************
                              DEFINES
**********************************************************************/
#define DC_BUS_L _IQ(0.0032)    // Inductancia L de la red Z
#define DC_BUS_C _IQ(0.0005)    // Capacidad C de la red Z

#define PID_V_LENGTH 5          // Longitud del controlador PID de tensión
#define PID_I_LENGTH 3          // Longitud del controlador PID de corriente
#define LPF_LENGTH 3            // Longitud del filtro pasa bajos

/**********************************************************************
         PARAMETROS DE LA INTERFAZ ANALÓGICA DE MEDICIONES
**********************************************************************/
#define IBRDG_OFFSET_VALUE _IQ(0.691)
#define IBRDG_INVERSE_GAIN_VALUE _IQ(23.866634845)

#define IAC_OFFSET_VALUE _IQ(1.487)
#define IAC_INVERSE_GAIN_VALUE _IQ(6.666666666)

#define IL1_OFFSET_VALUE _IQ(0.675)
#define IL1_INVERSE_GAIN_VALUE _IQ(11.36363636)

#define IIN_OFFSET_VALUE _IQ(0.0)
#define IIN_INVERSE_GAIN_VALUE _IQ(3.333333333)

#define VDC_OFFSET_VALUE _IQ(0.0)
#define VDC_INVERSE_GAIN_VALUE _IQ(202.4291498)

#define VC1_OFFSET_VALUE _IQ(0.0)
#define VC1_INVERSE_GAIN_VALUE _IQ(171.2328767)

#define VAC_OFFSET_VALUE _IQ(1.5)
#define VAC_INVERSE_GAIN_VALUE _IQ(253.164557)

#define VIN_OFFSET_VALUE _IQ(0.0)
#define VIN_INVERSE_GAIN_VALUE _IQ(130.3780965)

/**********************************************************************
                      VARIABLES DECLARATION
**********************************************************************/
static _iq currentVo;                                         // Set-point actual de la tensión de salida
static _iq desiredVo;                                         // Set-point deseado de la tensión de salida
static _iq currentVBUS;                                       // Set-point actual de la tensión del bus DC
static _iq desiredVBUS;                                       // Set-point deseado de la tensión del bus DC

static char Vo_inicial;
static char sin_salida;

static _iq DC_BUS_WN2;                                        // Parámetro Wn^2 del controlador de VBUS
static _iq DC_BUS_XI_WN;                                     // Parámetro psi*Wn del controlador de VBUS
static _iq DC_BUS_C_2;                                        // Parámetro C/2 del controlador de Vbus
static _iq PERCENTAGE_SECURITY_TIME;                          // Porcentaje del período PWM dedicado al tiempo de seguridad

_iq pidCurrentInputCoeffs[PID_I_LENGTH];                      // PID of current loop - input coefficients
_iq pidCurrentOutputCoeffs[PID_I_LENGTH-1];                   // PID of current loop - output coefficients

_iq pidVoltageInputCoeffs[PID_V_LENGTH];                      // PID of votage loop - input coefficients
_iq pidVoltageOutputCoeffs[PID_V_LENGTH-1];                   // PID of voltage loop - output coefficients

_iq lpfInputCoeffs[LPF_LENGTH];                               // Low pass filter - input coefficients
_iq lpfOutputCoeffs[LPF_LENGTH-1];                            // Low pass filter - output coefficients

RING_BUF vControlIn, iControlIn, vLpfIn, iLpfIn;              // Ring buffers con valores de entrada a integradores
RING_BUF vControlOut, iControlOut, vLpfOut, iLpfOut;          // Ring buffers con valores de salida de integradores

static long index_seno;

#if SALIDA_SENOIDAL == 1

#define SINE_TABLE_LENGTH 200
static const _iq seno[SINE_TABLE_LENGTH] = { 0.0, 0.0314, 0.0628, 0.0941, 0.1253, 0.1564,
        0.1874, 0.2181, 0.2487, 0.279, 0.309, 0.3387, 0.3681, 0.3971, 0.4258,
        0.454, 0.4818, 0.509, 0.5358, 0.5621, 0.5878, 0.6129, 0.6374, 0.6613,
        0.6845, 0.7071, 0.729, 0.7501, 0.7705, 0.7902, 0.809, 0.8271, 0.8443,
        0.8607, 0.8763, 0.891, 0.9048, 0.9178, 0.9298, 0.9409, 0.9511, 0.9603,
        0.9686, 0.9759, 0.9823, 0.9877, 0.9921, 0.9956, 0.998, 0.9995, 1.0,
        0.9995, 0.998, 0.9956, 0.9921, 0.9877, 0.9823, 0.9759, 0.9686, 0.9603,
        0.9511, 0.9409, 0.9298, 0.9178, 0.9048, 0.891, 0.8763, 0.8607, 0.8443,
        0.8271, 0.809, 0.7902, 0.7705, 0.7501, 0.729, 0.7071, 0.6845, 0.6613,
        0.6374, 0.6129, 0.5878, 0.5621, 0.5358, 0.509, 0.4818, 0.454, 0.4258,
        0.3971, 0.3681, 0.3387, 0.309, 0.279, 0.2487, 0.2181, 0.1874, 0.1564,
        0.1253, 0.0941, 0.0628, 0.0314, 0.0, -0.0314, -0.0628, -0.0941, -0.1253,
        -0.1564, -0.1874, -0.2181, -0.2487, -0.279, -0.309, -0.3387, -0.3681,
        -0.3971, -0.4258, -0.454, -0.4818, -0.509, -0.5358, -0.5621, -0.5878,
        -0.6129, -0.6374, -0.6613, -0.6845, -0.7071, -0.729, -0.7501, -0.7705,
        -0.7902, -0.809, -0.8271, -0.8443, -0.8607, -0.8763, -0.891, -0.9048,
        -0.9178, -0.9298, -0.9409, -0.9511, -0.9603, -0.9686, -0.9759, -0.9823,
        -0.9877, -0.9921, -0.9956, -0.998, -0.9995, -1.0, -0.9995, -0.998,
        -0.9956, -0.9921, -0.9877, -0.9823, -0.9759, -0.9686, -0.9603, -0.9511,
        -0.9409, -0.9298, -0.9178, -0.9048, -0.891, -0.8763, -0.8607, -0.8443,
        -0.8271, -0.809, -0.7902, -0.7705, -0.7501, -0.729, -0.7071, -0.6845,
        -0.6613, -0.6374, -0.6129, -0.5878, -0.5621, -0.5358, -0.509, -0.4818,
        -0.454, -0.4258, -0.3971, -0.3681, -0.3387, -0.309, -0.279, -0.2487,
        -0.2181, -0.1874, -0.1564, -0.1253, -0.0941, -0.0628, -0.0314 };

#else

#define SINE_TABLE_LENGTH 2
static const _iq seno[SINE_TABLE_LENGTH] = { 1.0, 1.0 };

#endif

/**********************************************************************
                   INTERNAL FUNCTIONS DECLARATION
**********************************************************************/
static inline void _IQ_diff_eq(_iq lastData, RING_BUF *in, RING_BUF *out, _iq *inputCoeffs, _iq *outputCoeffs, int inputLen, int outputLen);
static inline void _IQ_PID_CURRENT(_iq lastData, RING_BUF* in, RING_BUF* out);
static inline void _IQ_PID_VOLTAGE(_iq lastData, RING_BUF* in, RING_BUF* out);
static inline void _IQ_LPF(_iq lastData, RING_BUF* in, RING_BUF* out);

/**********************************************************************
                   INTERNAL FUNCTIONS DEFINITION
**********************************************************************/
static inline void _IQ_diff_eq(_iq lastData, RING_BUF *in, RING_BUF *out, _iq *inputCoeffs, _iq *outputCoeffs, int inputLen, int outputLen){
    _iq y = 0;
    int16 i, index;

    ringBuffer_putData(in, lastData);

    index = in->pos;                                            // El índice inicia en el último valor de entrada
    for(i=0; i<inputLen; i++){
        y += _IQmpy(in->buffer[index--], inputCoeffs[i]);       // Se multiplica el valor de entrada (t-i) por el coeficiente i
        if(index<0) index = in->len - 1;                        // Wrap del index
    }

    index = out->pos;                                           // El índice inicia en el último valor de salida
    for(i=0; i<outputLen; i++){
        y += _IQmpy(out->buffer[index--], outputCoeffs[i]);     // Se multiplica el valor de salida (t-1-i) por el coeficiente i
        if(index<0) index = out->len - 1;
    }

    ringBuffer_putData(out, y);                                 // Se guarda el valor de salida en t (actual)
}

static inline void _IQ_PID_CURRENT(_iq lastData, RING_BUF* in, RING_BUF* out){
    _IQ_diff_eq(lastData, in, out, pidCurrentInputCoeffs, pidCurrentOutputCoeffs, PID_I_LENGTH, PID_I_LENGTH-1);
}

static inline void _IQ_PID_VOLTAGE(_iq lastData, RING_BUF* in, RING_BUF* out){
    _IQ_diff_eq(lastData, in, out, pidVoltageInputCoeffs, pidVoltageOutputCoeffs, PID_V_LENGTH, PID_V_LENGTH-1);
}

static inline void _IQ_LPF(_iq lastData, RING_BUF* in, RING_BUF* out){
    _IQ_diff_eq(lastData, in, out, lpfInputCoeffs, lpfOutputCoeffs, LPF_LENGTH, LPF_LENGTH-1);
}

/**********************************************************************
                   EXTERNAL FUNCTIONS DEFINITIONS
**********************************************************************/

/**********************************************************************
* Function: qzsi_init()
* Description: Initialization for qZSI.
*
* Usage: qzsi_init();
**********************************************************************/
extern void qzsi_init(){
    qzsi_resetReferences();
    index_seno = 0;

    // DC bus controller parameters init
    DC_BUS_WN2 = _IQmpy(DC_BUS_WN, DC_BUS_WN);
    DC_BUS_XI_WN = _IQmpy(DC_BUS_XI, DC_BUS_WN);
    DC_BUS_C_2 = _IQdiv(DC_BUS_C, _IQ(2.0));
    PERCENTAGE_SECURITY_TIME = _IQmpy(Q_SECURITY_TIME, ADC_SAMPLE_FRECUENCY);

    // PID current controller parameters init
    pidCurrentInputCoeffs[0] = _IQ(0.08484166);        // Coeficiente para medición en T0
    pidCurrentInputCoeffs[1] = _IQ(-0.14512306);       // Coeficiente para medición en T-1
    pidCurrentInputCoeffs[2] = _IQ(0.06139566);        // Coeficiente para medición en T-2

    pidCurrentOutputCoeffs[0] = _IQ(1.22213691);       // Coeficiente para valor calculado en T-1
    pidCurrentOutputCoeffs[1] = _IQ(-0.22213691);      // Coeficiente para valor calculado en T-2

    // PID voltage controller parameters init
    pidVoltageInputCoeffs[0] = _IQ(0.06086318);        // Coeficiente para medición en T0
    pidVoltageInputCoeffs[1] = _IQ(-0.23473677);       // Coeficiente para medición en T-1
    pidVoltageInputCoeffs[2] = _IQ(0.33921768);        // Coeficiente para medición en T-2
    pidVoltageInputCoeffs[3] = _IQ(-0.21767410);       // Coeficiente para medición en T-3
    pidVoltageInputCoeffs[4] = _IQ(0.05233003);        // Coeficiente para medición en T-4

    pidVoltageOutputCoeffs[0] = _IQ(3.97285449);       // Coeficiente para valor calculado en T-1
    pidVoltageOutputCoeffs[1] = _IQ(-5.91957015);      // Coeficiente para valor calculado en T-2
    pidVoltageOutputCoeffs[2] = _IQ(3.92055145);       // Coeficiente para valor calculado en T-3
    pidVoltageOutputCoeffs[3] = _IQ(-0.97383579);      // Coeficiente para valor calculado en T-4

    // LPF parameters init
    lpfInputCoeffs[0] = _IQ(0.22907918);               // Coeficiente para medición en T0
    lpfInputCoeffs[1] = _IQ(0.45815837);               // Coeficiente para medición en T-1
    lpfInputCoeffs[2] = _IQ(0.22907918);               // Coeficiente para medición en T-2

    lpfOutputCoeffs[0] = _IQ(0.28458000);              // Coeficiente para valor calculado en T-1
    lpfOutputCoeffs[1] = _IQ(-0.20089676);             // Coeficiente para valor calculado en T-2

    // Ring buffers init
    ringBuffer_init(&vControlIn);
    ringBuffer_init(&iControlIn);
    ringBuffer_init(&vLpfIn);
    ringBuffer_init(&iLpfIn);

    ringBuffer_init(&vControlOut);
    ringBuffer_init(&iControlOut);
    ringBuffer_init(&vLpfOut);
    ringBuffer_init(&iLpfOut);
}


/**********************************************************************
* Function: qzsi_calculateVariables()
* Description: Calculates the value in Volts and Amps of measured
* voltages and currents from the raw values sampled by ADC.
*
* Input Parameters: av = pointer to structure where to store calculated
*                        variables sampled in active vector
*                   stv = pointer to structure where to store calculated
*                         variables sampled in shoot-through vector
*                   adc = pointer to array where raw ADC sampled values
*                         are stored
*
* Usage: qzsi_calculateVariables(&activeVars, &stVars, AdcBuffer);
**********************************************************************/
extern void qzsi_calculateVariables(activeVars_t *av, stVars_t *stv, volatile Uint16 *adc){
    av->IBRDG = _IQmpy(Adc_GetVoltage(adc[ADC_IBRDG]) - IBRDG_OFFSET_VALUE, IBRDG_INVERSE_GAIN_VALUE);
    av->VDC   = _IQmpy(Adc_GetVoltage(adc[ADC_VDC])   - VDC_OFFSET_VALUE,   VDC_INVERSE_GAIN_VALUE);
    av->VC1   = _IQmpy(Adc_GetVoltage(adc[ADC_VC1])   - VC1_OFFSET_VALUE,   VC1_INVERSE_GAIN_VALUE);
    av->Vac   = _IQmpy(Adc_GetVoltage(adc[ADC_Vac])   - VAC_OFFSET_VALUE,   VAC_INVERSE_GAIN_VALUE);
    av->Iac   = _IQmpy(Adc_GetVoltage(adc[ADC_Iac])   - IAC_OFFSET_VALUE,   IAC_INVERSE_GAIN_VALUE);
    stv->IL1  = _IQmpy(Adc_GetVoltage(adc[ADC_IL1])   - IL1_OFFSET_VALUE,   IL1_INVERSE_GAIN_VALUE);
    stv->Iin  = _IQmpy(Adc_GetVoltage(adc[ADC_Iin])   - IIN_OFFSET_VALUE,   IIN_INVERSE_GAIN_VALUE);
    stv->Vin  = _IQmpy(Adc_GetVoltage(adc[ADC_Vin])   - VIN_OFFSET_VALUE,   VIN_INVERSE_GAIN_VALUE);
    stv->IL   = _IQmpy(Adc_GetVoltage(adc[ADC_IL])    - IBRDG_OFFSET_VALUE, IBRDG_INVERSE_GAIN_VALUE); //IL se mide del mismo canal que IBRDG
}

/**********************************************************************
* Function: qzsi_dcBusController()
* Description: IQmath DC voltage bus control algorithm for qZSI.
*
* Usage: _iq d0 = qzsi_dcBusController(_iq IL, _iq VDC, _iq IDC, _iq Vin);
* Input Parameters: IL = last sampled inductor current.
*                   VDC = last sampled DC Bus voltage.
*                   IDC = last sampled DC Bus average in active vector current.
*                   Vin = last sampled input voltage.
*
* Output Value: Shoot-Through vector duty cycle d0 value
**********************************************************************/
extern _iq qzsi_dcBusController(_iq IL, _iq VDC, _iq IDC, _iq Vin){
    _iq D0, IL_ref, d0;
    _iq Ki, Kv, W02, delta;

    D0 = _IQmpy(_IQ(0.5),_IQ(1.0)-_IQdiv(Vin, currentVBUS));
    if(D0<0) D0=0;
    IL_ref = _IQdiv(_IQmpy(_IQ(2.0),IDC),_IQ(1.0)-_IQmpy(_IQ(2.0),D0));

    if(Vin<1) Vin=1;
    delta = _IQ(1.0)-_IQmpy(_IQ(2.0),D0);
    W02 = _IQdiv(_IQmpy(delta, delta), _IQmpy(DC_BUS_L, DC_BUS_C));
    Ki = -_IQmpy(DC_BUS_L, _IQdiv(_IQmpy(IDC, DC_BUS_WN2-W02)+_IQdiv(_IQmpy(Vin, _IQmpy(delta, DC_BUS_XI_WN)), DC_BUS_L), _IQdiv(_IQmpy(_IQ(4.0), _IQmpy(IDC, IDC)), DC_BUS_C)+ _IQdiv(_IQmpy(Vin, Vin), DC_BUS_L)));
    Kv = -_IQmpy(DC_BUS_C_2, _IQdiv(_IQmpy(Vin, DC_BUS_WN2-W02)-_IQdiv(_IQmpy(_IQmpy(_IQ(4.0),IDC), _IQmpy(delta, DC_BUS_XI_WN)), DC_BUS_C), _IQdiv(_IQmpy(_IQ(4.0), _IQmpy(IDC, IDC)), DC_BUS_C)+ _IQdiv(_IQmpy(Vin, Vin), DC_BUS_L)));

    d0 = (D0 + _IQmpy(Ki, IL-IL_ref) + _IQmpy(Kv, VDC-currentVBUS));

    if(d0>_IQ(0.35)) d0=_IQ(0.35);
    else if(d0<_IQ(0.05)) d0=_IQ(0.05);

    return d0;
}

/**********************************************************************
* Function: qzsi_outputVoltageController()
* Description: IQmath AC output voltage control algorithm for qZSI.
*
* Usage: _iq d = qzsi_outputVoltageController(_iq Vo, _iq IL);
* Input Parameters: Vo = last sampled AC output voltage.
*                   IL = last sampled output inductor current.
*
* Output Value: Active vector duty cycle d value
**********************************************************************/
extern _iq qzsi_outputVoltageController(_iq Vo, _iq IL){
    static _iq d;
    _iq Vref = _IQmpy(currentVo, seno[index_seno]);

    if(d < 0) IL=-IL;               // Como IBRDG = |IL| en el vector activo, se pasa como IL la medición de IBRDG
                                    // y luego se ajusta su signo según el signo del ciclo de trabajo

    _IQ_LPF(Vo, &vLpfIn, &vLpfOut);
    _IQ_LPF(IL, &iLpfIn, &iLpfOut);
    _IQ_PID_VOLTAGE(Vref - vLpfOut.buffer[vLpfOut.pos], &vControlIn, &vControlOut);
    _IQ_PID_CURRENT(vControlOut.buffer[vControlOut.pos] - iLpfOut.buffer[iLpfOut.pos], &iControlIn, &iControlOut);

    d = iControlOut.buffer[iControlOut.pos];

    if(d<_IQ(-0.9)) d = _IQ(-0.9);
    if(d>_IQ(0.9)) d = _IQ(0.9);

    return d;
}

/**********************************************************************
* Function: qzsi_updatePWM()
* Description: PWM values update for qZSI.
*
* Usage: qzsi_updatePWM(_iq d0, _iq D, char semiperiod);
* Input Parameters: d0 = Shoot-Through duty cycle.
*                   D = Active duty cycle.
*                   semiperiod = 1 if is positive semiperiod, 0 if is negative
*
**********************************************************************/
extern void qzsi_updatePWM(_iq d0, _iq D){
    if(D >= 0){                                                                                     // EPWM 1 y 2 manejan el puente
        EPwm1Regs.CMPA.half.CMPA = (int)(_IQmpy(d0, PWM_PERIOD));
        EPwm1Regs.CMPB = (int)(_IQmpy(_IQ(1.0)-D, PWM_PERIOD));
        EPwm2Regs.CMPA.half.CMPA = (int)(_IQmpy(d0, PWM_PERIOD));
        EPwm2Regs.CMPB = (int)(_IQmpy(_IQ(1.1), PWM_PERIOD));
    }
    else{
        D = -D;
        EPwm1Regs.CMPA.half.CMPA = (int)(_IQmpy(_IQ(1.0)-D, PWM_PERIOD));
        EPwm1Regs.CMPB = (int)(_IQmpy(d0, PWM_PERIOD));
        EPwm2Regs.CMPA.half.CMPA = (int)(_IQmpy(_IQ(1.1), PWM_PERIOD));
        EPwm2Regs.CMPB = (int)(_IQmpy(d0, PWM_PERIOD));
    }

    EPwm3Regs.CMPA.half.CMPA = (int)(_IQmpy(_IQdiv(d0,_IQ(2.0)), PWM_PERIOD));                      // EPWM3 muestrea las stVars en la mitad del ST (d0/2)

    EPwm4Regs.CMPA.half.CMPA = (int)(_IQmpy(_IQ(1.0)-d0+PERCENTAGE_SECURITY_TIME, PWM_PERIOD));     // EPWM4 maneja los Snubbers, que se activan 1uS despues del ST
    EPwm4Regs.CMPB = (int)(_IQmpy(_IQ(1.0)-PERCENTAGE_SECURITY_TIME, PWM_PERIOD));                  // Los Snubbers se desactivan 1uS antes del ST

    if(D>=0.03){                                                                                    // Si el vector activo es mayor a 0.03, QDZ se enciende
        EPwm5Regs.CMPA.half.CMPA = (int)(_IQmpy(_IQ(1.0)-D+PERCENTAGE_SECURITY_TIME, PWM_PERIOD));  // EPWM5 maneja QDZ, que se enciende 1uS despues del vector nulo
        EPwm5Regs.CMPB = (int)(_IQmpy(_IQ(1.0)-PERCENTAGE_SECURITY_TIME, PWM_PERIOD));              // El QDZ se apaga 1uS antes del ST
    }
    else{                                                                                           // Si el vector activo es muy pequeño, QDZ no se enciende
        EPwm5Regs.CMPA.half.CMPA = (int)(_IQmpy(_IQ(1.1), PWM_PERIOD));                             // En este caso, QDZ no se enciende
    }

    EPwm6Regs.CMPB = (int)(_IQmpy(_IQ(1.0)-_IQdiv(D,_IQ(2.0)), PWM_PERIOD));                        // EPWM6 muestrea las activeVars en la mitad del vector activo (T-D/2)
}


/**********************************************************************
* Function: qzsi_getOpenLoop_D()
* Description: Get the value of duty cycle D for open-loop operation
*
* Usage: D = qzsi_getDfromVoltage();
*
* Output Value: Active vector duty cycle D value
**********************************************************************/
extern _iq qzsi_getOpenLoop_D(){
    _iq D;
    if(sin_salida) return _IQ(0.0);
    if(Vo_inicial){
        D = _IQmpy(INITIAL_D, seno[index_seno]);
    }
    else{
        if(currentVBUS != 0) D = _IQmpy(_IQdiv(currentVo, currentVBUS), seno[index_seno]);
        else D = _IQ(0.0);
    }
    return (D < _IQ(0.9)) ? D : _IQ(0.9);
}


/**********************************************************************
* Function: qzsi_getOpenLoop_d0()
* Description: Get the value of duty cycle d0 for open-loop operation
*
* Usage: d0 = qzsi_getOpenLoop_d0(_iq vin);
* Input Parameters: vin = DC input voltage.
*
* Output Value: Shoot-Through vector duty cycle d0 value
**********************************************************************/
extern _iq qzsi_getOpenLoop_d0(_iq vin){
    _iq d0;
    if(currentVBUS != 0){
        d0 = _IQdiv(currentVBUS-vin, _IQmpy(2, currentVBUS));
        if(d0>_IQ(0.45)) d0=_IQ(0.45);
        else if(d0<_IQ(0.02)) d0=_IQ(0.02);
        return d0;
    }
    else return _IQ(0.0);
}

/**********************************************************************
* Function: qzsi_hardwareReady()
* Description: qZSI hardware check.
*
* Usage: char ready = qzsi_hardwareReady();
*
* Output Value: returns 1 if hardware is ok, 0 if it's not
**********************************************************************/
extern char qzsi_hardwareReady(){
    if(!GpioDataRegs.GPADAT.bit.GPIO_FLT){
        return 0;
    }
    // TODO: Analizar el uso de la entrada de RDY para reportar una falla,
    // dado que no se restaura con el reset y esto trae problemas
    /*
    else if(!GpioDataRegs.GPADAT.bit.GPIO_RDY){
        ledIndicator_setCode(1);
        return 0;
    }*/
    else if(!GpioDataRegs.GPBDAT.bit.GPIO_OC){
        ledIndicator_setCode(1);
        return 0;
    }
    else if(GpioDataRegs.GPADAT.bit.GPIO_OCP){
        ledIndicator_setCode(1);
        return 0;
    }

   return 1;
}

/**********************************************************************
* Function: qzsi_softProtection()
* Description: qZSI software protection for overvalues.
*
* Usage: char falla = qzsi_softProtection(_iq IL1, _iq Iin, _iq VC1, _iq Vac, _iq Iac);
* Input Parameters: IL1 = last sampled DC Bus L1 current.
*                   Vin = last sampled DC input current.
*                   VC1 = last sampled DC Bus C1 voltage.
*                   Vac = last sampled AC output voltage.
*                   Iac = last sampled AC output current.
*
* Output Value: returns 0 if values are correct, 1 if it's not
**********************************************************************/
extern char qzsi_softProtection(_iq IL1, _iq Iin, _iq VC1, _iq Vac, _iq Iac){
    if(IL1 > MAX_IL1 || IL1 < MIN_IL1){
        ledIndicator_setCode(2);
        return 1;
    }
    else if(Iin > MAX_Iin){
        ledIndicator_setCode(3);
        return 1;
    }
    else if(VC1 > MAX_VC1){
        ledIndicator_setCode(4);
        return 1;
    }
    else if(Vac > MAX_Vac){
        ledIndicator_setCode(5);
        return 1;
    }
    else if(Iac > MAX_Iac || Iac < MIN_Iac){
        ledIndicator_setCode(6);
        return 1;
    }

    return 0;
}

/**********************************************************************
* Function: void qzsi_updateSineReference()
* Description: update sine reference.
*
**********************************************************************/
extern void qzsi_updateSineReference(){
    index_seno++;
    if(index_seno >= SINE_TABLE_LENGTH) index_seno = 0;
}

/**********************************************************************
* Function: void qzsi_initVBUS(_iq vbus)
* Description: init VBUS at desired set-point vbus
*
**********************************************************************/
extern void qzsi_initVBUS(_iq vbus){
    currentVBUS = OPEN_LOOP_VIN;
    desiredVBUS = vbus;
}

/**********************************************************************
* Function: void qzsi_setVBUS(_iq vbus)
* Description: set desired VBUS set-point
*
**********************************************************************/
extern void qzsi_setVBUS(_iq vbus){
    desiredVBUS = vbus;
}

/**********************************************************************
* Function: _iq qzsi_getVBUS()
* Description: get current VBUS set-point
*
**********************************************************************/
extern _iq qzsi_getVBUS(){
    return currentVBUS;
}

/**********************************************************************
* Function: void qzsi_setVo(_iq vo)
* Description: set desired Vo set-point
*
**********************************************************************/
extern void qzsi_setVo(_iq vo){
    desiredVo = vo;
}


/**********************************************************************
* Function: _iq qzsi_getVo()
* Description: get current Vo set-point
*
**********************************************************************/
extern _iq qzsi_getVo(){
    return currentVo;
}

/**********************************************************************
* Function: void qzsi_clearVoInicial()
* Description: clears Vo_inicial flag
*
**********************************************************************/
extern void qzsi_clearVoInicial(){
    Vo_inicial = 0;
}

/**********************************************************************
* Function: void qzsi_HabilitarSalida()
* Description: clears sin_salida flag
*
**********************************************************************/
extern void qzsi_habilitarSalida(){
    sin_salida = 0;
}

/**********************************************************************
* Function: void qzsi_resetReferences()
* Description: reset Vo and VBUS voltage references to initial values
*
**********************************************************************/
extern void qzsi_resetReferences(){
    Vo_inicial = 1;
    sin_salida = 1;
    currentVo = _IQmpy(INITIAL_D, DC_BUS_VOLTAGE_REFERENCE);
    desiredVo = _IQmpy(INITIAL_D, DC_BUS_VOLTAGE_REFERENCE);
    currentVBUS = 0;
    desiredVBUS = 0;
}

/**********************************************************************
* Function: void qzsi_task1ms()
* Description: qZSI 1ms task
*
**********************************************************************/
extern void qzsi_task1ms(){
    static unsigned char timer_step = STEP_TIME_MS;

    if(timer_step > 0) timer_step--;

    if(timer_step == 0){
        timer_step = STEP_TIME_MS;
        if(desiredVBUS > currentVBUS) currentVBUS++;
        if(desiredVBUS < currentVBUS) currentVBUS--;
        if(_IQabs(desiredVBUS-currentVBUS) < 1) currentVBUS = desiredVBUS;

        if(desiredVo > currentVo) currentVo++;
        if(desiredVo < currentVo) currentVo--;
        if(_IQabs(desiredVo-currentVo) < 1) currentVo = desiredVo;
    }
}
