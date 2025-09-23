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

#ifndef INVERTER_INVERTERCONFIG_H_
#define INVERTER_INVERTERCONFIG_H_

/*==================[cplusplus]======================================*/
#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************************
              DEFINES PARA CONFIGURACIÓN DEL INVERSOR
**********************************************************************/
#define SALIDA_SENOIDAL 1                              // Si vale 1, la salida será senoidal. Si no, será continua.

#define HARDWARE_PROTECTIONS 1                         // Si vale 1, habilita las protecciones por hardware. En caso contrario, están deshabilitadas.
#define SOFTWARE_PROTECTIONS 0                         // Si vale 1, habilita las protecciones por software. En caso contrario, están deshabilitadas.

#define OUTPUT_VOLTAGE_CLOSED_LOOP 0                   // Si vale 1, se cierra el lazo de control de la tensión de salida. En caso contrario, funciona a lazo abierto.
#define BUS_VOLTAGE_CLOSED_LOOP 0                      // Si vale 1, se cierra el lazo de control de la tensión del bus DC. En caso contrario, funciona a lazo abierto.

#define OPEN_LOOP_VIN _IQ(300.0)                       // Valor de la tensión de entrada, que se utiliza para calcular Vbus a lazo abierto.
#define DC_BUS_VOLTAGE_REFERENCE _IQ(380.0)            // Valor deseado de la tensión del bus DC.
#define MAX_OUTPUT_VOLTAGE_REFERENCE _IQ(300.0)        // Valor deseado (pico) de la tensión de salida.

/*==================[cplusplus]======================================*/
#ifdef __cplusplus
}
#endif

#endif /* INVERTER_INVERTERCONFIG_H_ */
