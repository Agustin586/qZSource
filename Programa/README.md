# 💻 Programa - Firmware qZSI

Este directorio contiene el firmware completo para el control del Inversor Quasi Z-Source implementado en el DSP TMS320F28335 de Texas Instruments.

## 📦 Contenido

- **qZSI-Firmware/**: Proyecto completo de Code Composer Studio con todo el firmware del inversor

## 🏗️ Arquitectura del Firmware

El firmware está organizado en módulos funcionales para facilitar el mantenimiento y la escalabilidad:

### 🧠 Módulos Principales

#### `main.c` - Corazón del Sistema
- **Función principal**: Inicialización y loop principal del sistema
- **Configuración inicial**: Setup de todos los periféricos del DSP
- **Interrupciones**: Manejo de interrupciones de timer y ADC
- **Estado del sistema**: Control del estado global de la aplicación

#### `qZSI.c/h` - Motor de Control del Inversor
- **Algoritmos de control**: Implementación de los controladores PID
- **Cálculo de PWM**: Generación de ciclos de trabajo para MOSFETs
- **Protecciones**: Sistema de protecciones por software
- **Conversiones ADC**: Procesamiento de señales analógicas a valores físicos
- **Funciones clave**:
  - `qzsi_dcBusController()`: Control del bus DC con algoritmo no lineal
  - `qzsi_outputVoltageController()`: Control de tensión de salida con PID cascadeado
  - `qzsi_updatePWM()`: Actualización de registros PWM del DSP
  - `qzsi_getOpenLoop_d0()`: Cálculo de ciclo de trabajo en lazo abierto

#### `mefPrincipal.c/h` - Máquina de Estados Principal
- **Estados del sistema**: INICIO, RUN, FALLA, RESET_FALLA
- **Gestión de fallas**: Detección y manejo de condiciones de error
- **Secuencia de arranque**: Coordinación del arranque del sistema
- **Interfaz de usuario**: Manejo de botones y LEDs indicadores

#### `mefArranque.c/h` - Secuencia de Arranque Inteligente
- **Arranque progresivo**: Encendido suave del inversor
- **Carga de capacitores**: Precarga controlada de la red Z-source
- **Verificaciones**: Chequeos de seguridad antes del arranque
- **Transiciones**: Cambio gradual entre modos de operación

### 🔌 Drivers y Periféricos

- **`main.c`**: Función principal y configuración inicial del sistema
- **`qZSI.c/h`**: Algoritmos de control del inversor quasi Z-source
- **`mefPrincipal.c/h`**: Máquina de estados principal del sistema
- **`mefArranque.c/h`**: Máquina de estados de arranque secuencial

### Drivers y Periféricos

- **`Drivers/`**: Drivers para periféricos externos
  - `digitalPot.c/h`: Control de potenciómetros digitales MCP4461 vía I2C
  - `key.c/h`: Debounce y manejo de botones con detección de flancos
  - `LedIndicator.c/h`: Control de LEDs indicadores con códigos de error
  - `MCP4461.c/h`: Driver específico para potenciómetro digital de 4 canales

### ⚙️ Inicialización de Hardware

- **`Inits/`**: Módulos de inicialización críticos del DSP
  - `Adc.c/h`: ADC de 12 bits, 16 canales, conversión sincronizada con PWM
  - `EPwm.c/h`: 6 módulos PWM para control de MOSFETs y sincronización
  - `Gpio.c/h`: 88 pines GPIO para entradas/salidas digitales
  - `I2C.c/h`: Comunicación I2C a 400 kHz para potenciómetros
  - `SysCtrl.c/h`: PLL, relojes del sistema, configuración a 150MHz
  - `PieCtrl.c/h`: Controlador de interrupciones vectorizadas
  - `Watchdog.c/h`: Perro guardián para detección de bloqueos

### 🛠️ Utilidades y Algoritmos

- **`Utils/`**: Funciones auxiliares matemáticas
  - `ringBuffer.c/h`: Buffers circulares optimizados para filtros digitales IIR/FIR

## 🎯 Características Avanzadas del Control

### 🔄 Algoritmos de Control Implementados

1. **🔋 Control de Tensión DC Bus**: 
   - Algoritmo no lineal basado en modelo matemático
   - Controlador con ganancias variables según punto de operación
   - Compensación automática de variaciones de carga
   
2. **⚡ Control de Tensión de Salida**: 
   - PID cascadeado: tensión externa, corriente interna
   - Filtros digitales pasa-bajos para reducir ruido
   - Generación de referencia senoidal con tabla lookup
   
3. **🎛️ Generación PWM Avanzada**: 
   - 6 señales PWM sincronizadas
   - Tiempos muertos programables
   - Protección por hardware integrada
   
4. **🛡️ Sistema de Protecciones Multinivel**: 
   - Protecciones por hardware (instantáneas)
   - Protecciones por software (configurables)
   - Recuperación automática después de fallas

### ⚙️ Parámetros de Configuración Críticos

El archivo `inverterConfig.h` permite configurar múltiples aspectos:

#### 🎚️ Modos de Operación
- `SALIDA_SENOIDAL`: Habilita salida senoidal vs. continua
- `OUTPUT_VOLTAGE_CLOSED_LOOP`: Control en lazo cerrado de tensión
- `BUS_VOLTAGE_CLOSED_LOOP`: Control en lazo cerrado del bus DC

#### 🛡️ Sistemas de Protección
- `HARDWARE_PROTECTIONS`: Protecciones por hardware del DSP
- `SOFTWARE_PROTECTIONS`: Protecciones por software configurables

#### 📊 Referencias del Sistema
- `OPEN_LOOP_VIN`: Tensión de entrada nominal (300V)
- `DC_BUS_VOLTAGE_REFERENCE`: Tensión del bus DC objetivo (380V)
- `MAX_OUTPUT_VOLTAGE_REFERENCE`: Tensión de salida máxima (300V pico)

### 📚 Librerías Especializadas

- **🧮 IQmath**: Biblioteca de matemática de punto fijo de TI (Q15, Q24)
- **🎯 DSP2833x**: Drivers optimizados y definiciones para el DSP F28335
- **📋 Headers TI**: Archivos de cabecera estándar de Texas Instruments

## 🔧 Detalles Técnicos de Implementación

### ⏱️ Temporización y Sincronización

El proyecto está configurado para **Code Composer Studio** y requiere:

1. Code Composer Studio v6.0 o superior
2. Compilador C2000 de Texas Instruments
3. Biblioteca IQmath instalada

- **⏰ Frecuencia PWM**: 10 kHz (período de 100 µs)
- **📊 Frecuencia de muestreo ADC**: 20 kHz (2 muestras por período PWM)
- **🌊 Frecuencia de salida**: 50 Hz (senoidal de red)
- **🔄 Loop de control**: Ejecutado cada 50 µs (sincronizado con PWM)

### 🧠 Optimizaciones de Rendimiento

#### 💾 Gestión de Memoria
- **Código crítico en RAM**: Rutinas de control para máxima velocidad
- **Datos en SARAM**: Variables de control en memoria rápida
- **Stack optimizado**: Configurado para manejar interrupciones anidadas

#### ⚡ Optimizaciones de Código
- **IQmath**: Aritmética de punto fijo para cálculos rápidos
- **Inline functions**: Funciones críticas declaradas como inline
- **Loop unrolling**: Desenrollado de bucles en filtros digitales

### 🔍 Sistema de Debugging

#### 📊 Variables de Monitoreo
- Variables globales visibles en debugger
- Buffers de datos para análisis en tiempo real
- Contadores de rendimiento y estadísticas

#### 🚨 Códigos de Error
- Sistema de LEDs con códigos específicos
- Registro de fallas con timestamp
- Recuperación automática o manual según tipo de error

## 🛠️ Compilación y Desarrollo
