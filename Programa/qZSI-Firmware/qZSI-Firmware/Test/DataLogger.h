/*
 * DataLogger.h
 *
 * Autor: Zuliani, Agustín
 * Fecha: 06/11/25
 *
 * Descripción: Consiste en un archivo que se encarga de manejar
 * el data logger para permitir almacenar la información de leida
 * para posteriormente mostrarla y analizarla.
 */

#ifndef TEST_DATALOGGER_H_
#define TEST_DATALOGGER_H_

#include "dlog_4ch.h"
#include "IQmathLib.h"

typedef struct {
    _iq ch_val1;
    _iq ch_val2;
    _iq ch_val3;
    _iq ch_val4;
} dataLogger_t;

/**
 * @brief Resetea e inicializa el data logger para mostrar los datos.
 * @param [in] triggerValue Valor del trigger para comenzar a tomar datos.
 * @param [in] preScalar Permite saltear muestras o actualizaciones.
 *
 * @note El valor del trigger tiene que ser en referencia al puntero 1.
 */
void dataLoggerReset(float32_t triggerValue,
                     float32_t preScalar);
/**
 * @brief Se encarga de muestrear el valor de las variables correspondientes.
 * Esto se realiza de acuerdo al tiempo asignado del trigger.
 *
 * @param dlogValues Valores actualizados de los canales a mostrar.
 */
void dataLoggerRun(volatile dataLogger_t *dlogValues);

//void dataLoggerConvertToFloatTask1ms();

#endif /* TEST_DATALOGGER_H_ */
