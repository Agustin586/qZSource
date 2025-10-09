# Simulaciones - Modelado qZSI

Este directorio contiene las simulaciones y modelos matemáticos del Inversor Quasi Z-Source desarrollados para validar el diseño y optimizar los algoritmos de control.

## Contenido

- **`simulacion/`**: Modelos de simulación y archivos de proyecto
- **`licencia/`**: Archivos de licencia del software PLECS

## Herramienta de Simulación

### PLECS (Piecewise Linear Electrical Circuit Simulation)

- **Software**: PLECS Blockset para MATLAB/Simulink
- **Versión**: Licencia estudiantil 2022
- **Fabricante**: Plexim GmbH

### Archivos de Licencia

El directorio `licencia/` contiene:
- `license.lic`: Archivo de licencia PLECS
- `General_Terms_and_Conditions_Plexim_2018.pdf`: Términos y condiciones generales
- `PLECS_Student_Licensing_Terms_and_Conditions_11_2022.pdf`: Términos específicos de licencia estudiantil

## Modelos Implementados

### Modelo Completo del Sistema

El modelo de simulación incluye:

#### Red Quasi Z-Source
- **Inductores L1, L2**: Modelado con resistencias parásitas
- **Capacitores C1, C2**: Modelado con ESR (resistencia serie equivalente)
- **Comportamiento dinámico**: Respuesta transitoria y en estado estacionario

#### Puente Inversor
- **MOSFETs**: Modelo de conmutación ideal con pérdidas configurables
- **Drivers**: Retardos y limitaciones de slew rate
- **Dead-time**: Tiempo muerto entre conmutaciones

#### Control Digital
- **DSP**: Modelado de los algoritmos de control implementados
- **PWM**: Generación de señales moduladas
- **ADC**: Muestreo y cuantización de señales analógicas

#### Filtro de Salida
- **Filtro LC**: Filtrado de armónicos de conmutación
- **Carga**: Diferentes tipos de carga (resistiva, RL, no lineal)

### Algoritmos de Control Simulados

#### Control de Lazo Abierto
- Cálculo de ciclo de trabajo `d0` para shoot-through
- Cálculo de ciclo de trabajo `d` para vector activo
- Generación de referencia senoidal

#### Control de Lazo Cerrado
- **PID de Corriente**: Control de corriente de inductor
- **PID de Tensión**: Control de tensión de salida
- **Filtros Digitales**: Filtros pasa-bajos para mediciones

### Protecciones Simuladas
- Límites de corriente en inductores
- Límites de tensión en capacitores
- Protección por sobrecorriente de salida
- Detección de fallas de hardware

## Casos de Estudio

### Análisis Realizados

1. **Arranque del Sistema**
   - Secuencia de encendido progresivo
   - Carga de capacitores de la red Z
   - Establización de tensiones

2. **Respuesta ante Escalones**
   - Cambios en la carga
   - Variaciones de tensión de entrada
   - Cambios en referencias

3. **Análisis de Estabilidad**
   - Márgenes de fase y ganancia
   - Respuesta en frecuencia
   - Robustez del control

4. **Eficiencia Energética**
   - Pérdidas de conmutación
   - Pérdidas de conducción
   - Eficiencia total del sistema

### Validación de Diseño

Las simulaciones se utilizaron para:

- **Dimensionamiento de componentes**: L, C, MOSFETs
- **Ajuste de controladores**: Parámetros PID
- **Verificación de protecciones**: Umbrales y tiempos
- **Optimización de rendimiento**: THD, eficiencia

## Parámetros de Simulación

### Condiciones Nominales
- **Tensión de entrada**: 300V DC
- **Tensión de salida**: 220V AC RMS
- **Frecuencia de salida**: 50 Hz
- **Frecuencia de conmutación**: 10 kHz
- **Carga nominal**: Resistiva, RL

### Componentes de la Red Z
- **L1 = L2**: 3.2 mH
- **C1 = C2**: 500 µF
- **Frecuencia de resonancia**: Optimizada para la aplicación

## Resultados y Validación

### Métricas Evaluadas
- **THD (Total Harmonic Distortion)**: Distorsión armónica total < 5%
- **Eficiencia**: > 90% en condiciones nominales
- **Regulación de tensión**: ±2% para variaciones de carga
- **Factor de potencia**: > 0.95 con carga resistiva

### Comparación con Resultados Experimentales
- Validación de formas de onda
- Verificación de eficiencia
- Confirmación de estabilidad

## Software Requerido

Para ejecutar las simulaciones:
- MATLAB R2020b o superior
- Simulink
- PLECS Blockset (licencia válida)
- Conocimientos básicos de electrónica de potencia

## Archivos de Salida

Las simulaciones generan:
- Formas de onda de tensiones y corrientes
- Análisis espectral (FFT)
- Gráficos de eficiencia
- Reportes de THD
- Datos para validación experimental
