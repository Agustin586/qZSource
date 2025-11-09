/**
 * @file main.c
 * @brief Punto de entrada principal para el proyecto qZSI-Firmware.
 * @author Zuliani, Agustin
 * 
 * Este archivo contiene la función principal y las rutinas de servicio de interrupción (ISR)
 * para el proyecto qZSI-Firmware. Inicializa los periféricos y maneja el bucle principal de control.
 */

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

/**********************************************************************/

/*
 * NOTA: Este proyecto cuenta con dos linker scripts disponibles: Linker.cmd
 * y Linker_no_flash.cmd. Con el primero de ellos, el programa se carga
 * en la memoria flash del DSP, y permanece ante la falta de energ�a,
 * por lo que se comienza a ejecutar en cada arranque. Con el segundo,
 * el programa se carga en un sector de la memoria RAM, por lo que carece
 * de persistencia ante la falta de energ�a. Se recomienda utilizar este
 * �ltimo en el desarrollo, para evitar arranques indeseados del inversor.
 * Para utilizar uno u otro, basta con excluir de la compilaci�n al no
 * deseado, haciendo click derecho sobre el mismo y marcando la opci�n
 * "Exclude from Build".
 * Adem�s, si se desea utilizar la flash, se debe descomentar la l�nea
 * que define la macro USING_FLASH en el archivo Board_Init.h
 *
 */

/**********************************************************************
                             INCLUDES
**********************************************************************/
#include "DSP2833x_Device.h"
#include "DSP2833x_DefaultIsr.h"
#include "IQmathLib.h"
#include "DelayUs.h"

#include "Inits/Adc.h"
#include "Inits/EPwm.h"
#include "Inits/Dma.h"
#include "Inits/CpuTimers.h"
#include "Inits/Board_Init.h"
#include "Inverter/qZSI.h"
#include "MEFs/mefPrincipal.h"
#include "board.h"
#include "Drivers/digitalPot.h"
#include "Drivers/key.h"
#include "Drivers/LedIndicator.h"
#include "Inverter/inverterConfig.h"
#include "Test/DataLogger.h"

/**********************************************************************
                              DEFINES
**********************************************************************/
#define CORE_CLOCK_FREQ             150E6
#define SYSTICK_PERIOD              1E-3
#define DATA_LOGGER_TRIGGER_VALUE   DC_BUS_VOLTAGE_REFERENCE/3
#define DATA_LOGGER_PRE_SCALAR      0

/**********************************************************************
                      VARIABLES DECLARATION
**********************************************************************/
/*
 * El m�dulo DMA guarda autom�ticamente las mediciones del ADC en el
 * buffer AdcBuff. Con esta instrucci�n se fuerza al compilador a
 * ubicar al buffer en una secci�n de la RAM accesible por DMA,
 * llamada dmaMemBufs y definida en el linker script.
 *
 */
#pragma DATA_SECTION(AdcBuff, "dmaMemBufs");
volatile Uint16 AdcBuff[ADC_total], AdcBuff2[ADC_total];
volatile dataLogger_t dLogValues = {0, 0, 0, 0};

/**********************************************************************
                      FUNCTIONS DEFINITIONS
**********************************************************************/

/**
 * @brief Función principal para el proyecto qZSI-Firmware.
 * 
 * Inicializa la placa, los periféricos y las variables globales. Ejecuta constantemente
 * la máquina de estados principal (mefPrincipal) que contiene submáquinas de estados.
 */
void main(void){

    // -- Buffers init
    int iloop;
    for(iloop = 0; iloop<ADC_total; iloop++){
        AdcBuff[iloop] = 0;
        AdcBuff2[iloop] = 0;
    }

    //--- Some board functions initialization
    BoardInit();

    //--- Peripheral Initialization
    InitAdc();                                      // Initialize the ADC (FILE: Adc.c)
    InitEPwm();                                     // Initialize the EPwm (FILE: EPwm.c)
    InitDma(AdcBuff);                               // Initialize the DMA (FILE: Dma.c)
    InitTimers(CORE_CLOCK_FREQ, SYSTICK_PERIOD);    // Initialize the CPU Timer 0 to interrupt every 1ms (FILE: CpuTimers.c)

    //--- Initialize the shared GPIO pins (FILE: Gpio.c)
    InitGpio();

    //--- Initialize the voltage references for hardware protections
    digitalPot_configureReferences();

    //--- Other Initializations
    key_init();
    qzsi_init();
    mefPrincipal_init();
    ledIndicator_init();
    dataLoggerReset(DATA_LOGGER_TRIGGER_VALUE,
                    DATA_LOGGER_PRE_SCALAR);

    //--- Enable global interrupts
    asm(" CLRC INTM, DBGM");                        // Enable global interrupts and realtime debug

    //--- Main Loop
	while(1)
	{
	    mefPrincipal();
	}
}

/**
 * @brief ISR para la interrupción del temporizador 0 de la CPU (SysTick).
 * 
 * Configura el temporizador para interrumpirse cada 1 ms. Dentro de esta función
 * se monitorean distintos periféricos y se actualizan variables.
 */
void TINT0_ISR(void){                 // PIE1.7 @ 0x000D4C  TINT0 (CPU TIMER 0)
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;     // Must acknowledge the PIE group
    CpuTimer0Regs.TCR.bit.TIF = 1;              // Clear flag

    mefPrincipal_task1ms();
    key_periodicTask1ms();
    qzsi_task1ms();
    ledIndicator_task1ms();
}

/**
 * @brief ISR para la interrupción del EPWM6.
 * 
 * Esta ISR maneja los algoritmos de control para el inversor qZSI. Procesa
 * los datos del ADC, aplica los lazos de control y actualiza las salidas PWM.
 */
void EPWM6_INT_ISR(void){             // PIE3.6 @ 0x000D6A  EPWM6_INT (EPWM6)
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;     // Must acknowledge the PIE group
    EPwm6Regs.ETCLR.bit.INT = 1;                // Clear flag

    // Crea las variables de manera estatica (se mantienen entre cada llamada)
    static activeVars_t activeVars = {0, 0, 0, 0, 0};
    static stVars_t stVars = {0, 0, 0, 0};
    static _iq d0 = 0, D = 0;

    // Se copian a AdcBuff2 las variables muestreadas por el ADC almacenadas por DMA en AdcBuff
    memcpy(&AdcBuff2, &AdcBuff, sizeof(AdcBuff));

    // Calculo de las variables --> Para vector activo y shoot-through
    qzsi_calculateVariables(&activeVars, &stVars, AdcBuff2);

    // Cargamos datos en el Data Logger
    dLogValues.ch_val1 = activeVars.VDC;
    dLogValues.ch_val2 = activeVars.Vac;
    dLogValues.ch_val3 = stVars.IL;
    dLogValues.ch_val4 = stVars.IL1;

    dataLoggerRun(&dLogValues);

    // TODO: Se deben colocar filtros digitales para las entradas de protecci�n por software, para evitar disparos espureos
    #if SOFTWARE_PROTECTIONS == 1
    if((!qzsi_hardwareReady() /*|| qzsi_softProtection(stVars.IL1, stVars.Iin, activeVars.VC1, activeVars.Vac, activeVars.Iac)*/) && !mefPrincipal_getFalla()){
        mefPrincipal_setFalla();
        epwm_softOneShotTZ();
    }
    #endif

    if(mefPrincipal_getArranqueVBUS()){                                                       // En el arranque no se utilizan los lazos de control
        d0 = qzsi_getOpenLoop_d0(OPEN_LOOP_VIN);
    }
    else{
        #if BUS_VOLTAGE_CLOSED_LOOP
        d0 = qzsi_dcBusController(stVars.IL, activeVars.VDC, _IQmpy(activeVars.IBRDG, D), stVars.Vin);   // IDC = IBRDG muestreada en el medio del vector activo * D
        #else
        d0 = qzsi_getOpenLoop_d0(OPEN_LOOP_VIN);
        #endif
    }

    if(mefPrincipal_getArranqueVO()){                                                         // En el arranque no utilizan los lazos de control
        D = qzsi_getOpenLoop_D();
        qzsi_outputVoltageController(activeVars.Vac, activeVars.IBRDG);                       // Se llama al controlador para actualizar los valores iniciales
    }
    else{
        #if OUTPUT_VOLTAGE_CLOSED_LOOP == 1
        D = qzsi_outputVoltageController(activeVars.Vac, activeVars.IBRDG);
        #else
        D = qzsi_getOpenLoop_D();
        #endif
    }

    if(d0 > 1-_IQabs(D)) d0 = 1-_IQabs(D);

    qzsi_updatePWM(d0, D);
    qzsi_updateSineReference();
}

/**
 * @brief ISR para la interrupción de zona de fallo del EPWM6.
 * 
 * Esta ISR se activa cuando se detecta una condición de fallo en el módulo EPWM6.
 * Establece la bandera de fallo y desactiva las salidas del inversor.
 */
void EPWM6_TZINT_ISR(void){                     // PIE2.6 @ 0x000D5A  EPWM6_TZINT (EPWM6)
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;     // Must acknowledge the PIE group
    mefPrincipal_setFalla();
}
