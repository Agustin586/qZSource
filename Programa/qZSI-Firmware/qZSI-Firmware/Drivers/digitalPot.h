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

#ifndef DIGITALPOT_H
#define DIGITALPOT_H

/**********************************************************************
                             INCLUDES
**********************************************************************/
#include "stdint.h"

/*==================[cplusplus]======================================*/
#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************************
       TENSIÓN DE ALIMENTACIÓN DE LOS POTENCIÓMETROS DIGITALES
**********************************************************************/
#define VCC 3.3

/**********************************************************************
           VALORES DE SALIDA DE LOS POTENCIÓMETROS DIGITALES
**********************************************************************/
#define VREF_VC12   2.6
#define VREF_Iin    2.55
#define VREF_IL1m   0.32
#define VREF_IL1p   2.61
#define VREF_IBRDGm 0.147
#define VREF_IBRDGp 2.57
#define VREF_IACm   0       // TODO: Las referencias de IAC se encuentran en sus valores máximos
#define VREF_IACp   3.3     // debido a disparos espúreos de esta protección. A solucionar.

/**********************************************************************
                   EXTERNAL FUNCTIONS DECLARATION
**********************************************************************/
extern void digitalPot_configureReferences(void);

/*==================[cplusplus]======================================*/
#ifdef __cplusplus
}
#endif

#endif /* DIGITALPOT_H */
