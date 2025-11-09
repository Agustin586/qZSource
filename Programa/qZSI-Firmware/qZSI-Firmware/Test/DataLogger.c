/*
 * DataLogger.c
 *
 * Autor: Zuliani, Agustín
 * Fecha: 06/11/25
 *
 * Descripción: Consiste en un archivo que se encarga de manejar
 * el data logger para permitir almacenar la información de leida
 * para posteriormente mostrarla y analizarla.
 */

#include "DataLogger.h"

#define DBUFF_SIZE 200

DLOG_4CH dLog1;

float32_t input_p1 = 0;
float32_t input_p2 = 0;
float32_t input_p3 = 0;
float32_t input_p4 = 0;

float32_t dBuff1[DBUFF_SIZE];
float32_t dBuff2[DBUFF_SIZE];
float32_t dBuff3[DBUFF_SIZE];
float32_t dBuff4[DBUFF_SIZE];

static volatile int32_t dBuffFix1[DBUFF_SIZE];
static volatile int32_t dBuffFix2[DBUFF_SIZE];
static volatile int32_t dBuffFix3[DBUFF_SIZE];
static volatile int32_t dBuffFix4[DBUFF_SIZE];

void dataLoggerReset(float32_t triggerValue,
                     float32_t preScalar)
{
    DLOG_4CH_reset(&dLog1);

    DLOG_4CH_config(&dLog1,         // Tipo Data Log 4 Ch
                    &input_p1,      // Valor a graficar 1
                    &input_p2,      // ...
                    &input_p3,      // ...
                    &input_p4,      // ...
                    dBuff1,         // Valor de salida 1
                    dBuff2,         // ...
                    dBuff3,         // ...
                    dBuff4,         // ...
                    DBUFF_SIZE,     // Tamaño del buffer
                    triggerValue,   // Valor de Trigger (a partir de este umbral se toman muestras)
                    preScalar       // PreScalar (saltea muestras)
                    );
}

void dataLoggerRun(volatile dataLogger_t *dlogValues)
{
    input_p1 = _IQtoF(dlogValues->ch_val1);
    input_p2 = _IQtoF(dlogValues->ch_val2);
    input_p3 = _IQtoF(dlogValues->ch_val3);
    input_p4 = _IQtoF(dlogValues->ch_val4);

    DLOG_4CH_run(&dLog1);
}






