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

#ifndef BOARD_H_
#define BOARD_H_

/*==================[cplusplus]======================================*/
#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************************
                     ENTRADAS/SALIDAS DIGITALES
**********************************************************************/
// PORT A
#define GPIO_SW_IN      GPIO11  // Salida para activar gate del switch de entrada
#define GPIO_RST_BTN    GPIO12  // Entrada para leer el botón de rst
#define GPIO_LVL_OE     GPIO13  // Salida para activar los level shifters
#define GPIO_AUX_OUT_2  GPIO14  // Salida auxiliar para uso general
#define GPIO_CLEAR_BTN  GPIO15  // Entrada para leer el botón de clear de fallas
#define GPIO_VSEL       GPIO17  // Salida para cambiar tensión VGS del switch de entrada
#define GPIO_DPOT_RESET GPIO18  // Salida para controlar el reset de los potes digitales
#define GPIO_LED_2      GPIO19  // Salida para controlar LED indicador de estado
#define GPIO_RDY        GPIO20  // Entrada para leer estado ready de drivers
#define GPIO_LED_3      GPIO21  // Salida para controlar LED indicador de estado
#define GPIO_FLT        GPIO22  // Entrada para leer estado de fallas en los drivers
#define GPIO_LED_4      GPIO23  // Salida para controlar LED indicador de estado
#define GPIO_AUX_OUT_1  GPIO25  // Salida auxiliar para uso general
#define GPIO_OCP        GPIO26  // Entrada para detectar en GPIO falla de hardware
#define GPIO_LED_1      GPIO27  // Salida para controlar LED indicador de estado

// PORT B
#define GPIO_OC         GPIO34  // Entrada para detectar falla en sensor de corriente AC de efecto hall
#define GPIO_CLR_FAULT  GPIO48  // Salida para resetear flip-flop de detección de falla por hardware
#define GPIO_RST        GPIO49  // Salida para resetear fallas en los drivers
#define GPIO_EXT_IN_1   GPIO59  // Entrada digital externa 1
#define GPIO_EXT_IN_2   GPIO61  // Entrada digital externa 2

/**********************************************************************
                       ENTRADAS ANALÓGICAS
**********************************************************************/
enum adc_variables{
    ADC_IBRDG = 0,          // ADCRESULT0
    ADC_VDC,                // ADCRESULT1
    ADC_VC1,                // ADCRESULT2
    ADC_Vac,                // ADCRESULT3
    ADC_Iac,                // ADCRESULT4
    ADC_IL1,                // ADCRESULT8
    ADC_Iin,                // ADCRESULT9
    ADC_Vin,                // ADCRESULT10
    ADC_IL,                 // ADCRESULT11   Se muestrea dos veces Ibrdg: en el vector activo sirve para calcular IDC y durante el ST, la corriente del puente es la suma de IL1 e IL2 = IL
    ADC_total
};

/**********************************************************************
                             SWITCH IDs
**********************************************************************/
typedef enum{
    BOARD_SW_RST_MANUAL = 0,
    BOARD_SW_CLR_FAULT,
    BOARD_SW_ID_TOTAL
}board_swId_enum;

/*==================[cplusplus]======================================*/
#ifdef __cplusplus
}
#endif

#endif /* BOARD_H_ */
