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

#ifndef EPWM_H_
#define EPWM_H_

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
                             MACROS
**********************************************************************/
#define Q_SECURITY_TIME _IQ(1000E-9)    //Tiempo de seguridad entre conmutaciones
#define ADC_SAMPLE_FRECUENCY 10000
#define PWM_CLOCK_FREQ 150000000
#define PWM_PERIOD (_IQdiv(PWM_CLOCK_FREQ, ADC_SAMPLE_FRECUENCY) - 1)

/**********************************************************************
                   EXTERNAL FUNCTIONS DECLARATION
**********************************************************************/
extern void InitEPwm(void);
extern void epwm_rstTripZone(void);
extern void epwm_softOneShotTZ(void);

/*==================[cplusplus]======================================*/
#ifdef __cplusplus
}
#endif

#endif /* EPWM_H_ */
