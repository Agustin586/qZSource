# 🔌 Esquemáticos - Hardware qZSI

Este directorio contiene todos los diseños de hardware para el Inversor Quasi Z-Source, incluyendo esquemáticos, layouts de PCB y documentación de diseño.

## 📦 Contenido

### 🧩 Módulos de Diseño

- **`ControlBoard/`**: 🧠 Placa de control principal con DSP TMS320F28335
- **`Design_AC_Output/`**: 🌊 Circuito de salida AC y filtros
- **`Design_DC_AC_Converter/`**: ⚡ Etapa inversora DC-AC principal
- **`Design_DC_Input/`**: 🔋 Circuito de entrada DC y red quasi Z-source
- **`Design_Drivers/`**: 🚗 Drivers de compuerta para MOSFETs
- **`Driver_MOSFET_DC_Input/`**: 🔌 Driver específico para MOSFETs de entrada
- **`MultiBoard/`**: 🏗️ Diseño integrado de múltiples placas
- **`Reformas/`**: 🔄 Modificaciones y actualizaciones del diseño

## 🏗️ Arquitectura del Hardware

### 🧠 Placa de Control (ControlBoard)
- **Procesador**: DSP TMS320F28335 de Texas Instruments
- **Interfaces**: 
  - 📊 Entradas analógicas para sensado de corriente y tensión
  - 🎛️ Salidas PWM para control de MOSFETs
  - 📡 Comunicación I2C para potenciómetros digitales
  - 🔍 Entradas digitales para protecciones
- **Alimentación**: 🔌 Fuentes aisladas para control y potencia

### ⚡ Etapa de Potencia

#### 🔋 Red Quasi Z-Source (Design_DC_Input)
- **Inductores**: 🧲 L1, L2 para almacenamiento de energía
- **Capacitores**: ⚡ C1, C2 para filtrado y elevación de tensión
- **Sensores**: 📏 Medición de corrientes y tensiones

#### 🔄 Puente Inversor (Design_DC_AC_Converter)
- **MOSFETs**: 🔌 Semiconductores de potencia para conmutación
- **Drivers**: 🚗 Circuitos de excitación aislados
- **Protecciones**: 🛡️ Detección de sobrecorriente y fallas

#### 🌊 Salida AC (Design_AC_Output)
- **Filtro**: 📊 Filtro LC para obtener forma de onda senoidal
- **Sensores**: 📏 Medición de tensión y corriente de salida
- **Protecciones**: 🛡️ Monitoreo de parámetros de salida

### 🚗 Drivers de Compuerta (Design_Drivers)
- **Aislamiento**: 🔒 Aislamiento galvánico entre control y potencia
- **Protecciones**: 
  - 🔍 Detección de desaturación (DESAT)
  - ⚡ Protección por sobrecorriente
  - 🌡️ Monitoreo de temperatura
- **Señalización**: 💡 LEDs indicadores y señales de estado

## 📊 Especificaciones Técnicas

### ⚡ Parámetros Eléctricos
- **Tensión de entrada**: 🔋 200-400V DC
- **Tensión de salida**: 🌊 220V AC RMS (311V pico)
- **Corriente máxima**: ⚡ Depende del diseño específico
- **Frecuencia de conmutación**: ⏱️ 10 kHz
- **Frecuencia de salida**: 🌊 50 Hz

### 🛡️ Protecciones Implementadas
- ⚡ Sobrecorriente en inductores L1, L2
- 🔋 Sobretensión en capacitores C1, C2
- 🌊 Sobrecorriente en la salida AC
- 🌡️ Protección térmica en drivers
- 🚨 Fallas de hardware (FAULT, READY, OC, OCP)

## 🛠️ Herramientas de Diseño

Los esquemáticos fueron diseñados utilizando herramientas profesionales de diseño de PCB (probablemente Altium Designer o similar).

## 🎯 Consideraciones de Diseño

### 🔒 Aislamiento
- 🔒 Aislamiento galvánico entre etapas de control y potencia
- 🚧 Barreras de aislamiento en drivers de compuerta
- 🔌 Fuentes de alimentación aisladas

### 📡 EMI/EMC
- 📐 Layout optimizado para minimizar interferencias
- 🌍 Planos de masa apropiados
- 🔍 Filtros de entrada y salida

### 🌡️ Térmico
- 🌡️ Disipadores para semiconductores de potencia
- 📊 Monitoreo de temperatura en puntos críticos
- 💨 Ventilación adecuada

## 🧩 Estructura Modular

El diseño está organizado de forma modular para facilitar:
- 🔬 Pruebas individuales de cada etapa
- 🔧 Mantenimiento y reparación
- 📈 Escalabilidad del sistema
- ♻️ Reutilización de módulos

## 📚 Documentación

Cada módulo incluye:
- 📋 Esquemáticos detallados
- 📊 Lista de materiales (BOM)
- 🔧 Consideraciones de montaje
- 🧪 Procedimientos de prueba
