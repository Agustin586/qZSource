# Programa - Firmware qZSI

Este directorio contiene el firmware completo para el control del Inversor Quasi Z-Source implementado en el DSP TMS320F28335 de Texas Instruments.

## Contenido

- **qZSI-Firmware/**: Proyecto completo de Code Composer Studio con todo el firmware del inversor

## Arquitectura del Firmware

El firmware está organizado en módulos funcionales:

### Módulos Principales

- **`main.c`**: Función principal y configuración inicial del sistema
- **`qZSI.c/h`**: Algoritmos de control del inversor quasi Z-source
- **`mefPrincipal.c/h`**: Máquina de estados principal del sistema
- **`mefArranque.c/h`**: Máquina de estados de arranque secuencial

### Drivers y Periféricos

- **`Drivers/`**: Drivers para periféricos externos
  - `digitalPot.c/h`: Control de potenciómetros digitales
  - `key.c/h`: Manejo de botones y entradas digitales
  - `LedIndicator.c/h`: Control de indicadores LED
  - `MCP4461.c/h`: Driver específico para potenciómetro digital MCP4461

### Inicialización de Hardware

- **`Inits/`**: Módulos de inicialización de periféricos del DSP
  - `Adc.c/h`: Configuración del convertidor analógico-digital
  - `EPwm.c/h`: Configuración de PWM para control de MOSFETs
  - `Gpio.c/h`: Configuración de pines de entrada/salida
  - `I2C.c/h`: Comunicación I2C para potenciómetros digitales
  - `SysCtrl.c/h`: Control del sistema y relojes

### Utilidades

- **`Utils/`**: Funciones auxiliares
  - `ringBuffer.c/h`: Implementación de buffers circulares para filtros digitales

## Características del Control

### Algoritmos Implementados

1. **Control de Tensión DC Bus**: Regulación del voltaje del bus DC
2. **Control de Tensión de Salida**: Regulación de la tensión AC de salida
3. **Generación PWM**: Señales de control para MOSFETs
4. **Protecciones**: Sistema de protecciones por hardware y software

### Configuración

El archivo `inverterConfig.h` permite configurar:

- Modo de salida: senoidal o continua
- Habilitación de protecciones
- Modo de control: lazo abierto/cerrado
- Referencias de tensión
- Parámetros del sistema

### Librerías Utilizadas

- **IQmath**: Biblioteca de matemática de punto fijo de TI
- **DSP2833x**: Drivers y definiciones para el DSP F28335
- **Headers TI**: Archivos de cabecera estándar de Texas Instruments

## Compilación

El proyecto está configurado para **Code Composer Studio** y requiere:

1. Code Composer Studio v6.0 o superior
2. Compilador C2000 de Texas Instruments
3. Biblioteca IQmath instalada

## Frecuencia de Operación

- **Frecuencia PWM**: 10 kHz
- **Frecuencia de muestreo ADC**: 20 kHz (2 muestras por período PWM)
- **Frecuencia de salida**: 50 Hz

## Memoria Utilizada

El firmware está optimizado para ejecutarse desde RAM para máximo rendimiento en las rutinas de control críticas.
