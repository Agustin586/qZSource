# qZSource - Inversor Quasi Z-Source

![License](https://img.shields.io/badge/license-BSD--3--Clause-blue.svg)
![Status](https://img.shields.io/badge/status-Active-green.svg)

## Descripción

Proyecto Final de Ingeniería Electrónica

Este proyecto implementa un **Inversor Quasi Z-Source (qZSI)** completo, incluyendo el diseño de hardware, firmware de control y simulaciones. El inversor qZSI es una topología avanzada que combina las funciones de elevación de tensión y inversión en una sola etapa, ofreciendo ventajas como inmunidad a cortocircuitos y mayor eficiencia.

## Características Principales

- **Topología**: Quasi Z-Source Inverter
- **Control**: DSP TMS320F28335 de Texas Instruments
- **Tensión de entrada**: 300V DC (configurable)
- **Tensión de salida**: 300V AC pico (configurable)
- **Frecuencia de salida**: 50Hz senoidal
- **Protecciones**: Hardware y software configurables
- **Modos de operación**: Lazo abierto y cerrado

## Estructura del Proyecto

```
qZSource/
├── Esquematicos/          # Diseños de PCB y esquemáticos
├── Programa/              # Firmware para DSP TMS320F28335
├── Simulaciones/          # Modelos y simulaciones PLECS
└── README.md             # Este archivo
```

## Tecnologías Utilizadas

- **Hardware**: TMS320F28335 DSP, MOSFETs, Drivers de compuerta
- **Software**: Code Composer Studio, IQmath Library
- **Simulación**: PLECS Blockset, Matlab Simulink
- **Diseño PCB**: Altium Designer (presumiblemente)

## Autores del proyecto 2025
- **Fasolato, Alejandro**
- **Soso, Alvaro**
- **Zuliani, Agustin**

## Autores del proyecto 2024

- **Guido Cicconi**
- **Gustavo García** 
- **Juan Tinazzo**

## Licencia

Este proyecto está licenciado bajo la Licencia BSD de 3 Cláusulas. Ver los archivos de código fuente para más detalles.

## Año

2025 - Proyecto Final de Ingeniería Electrónica - Lazo de Control
2024 - Proyecto Final de Ingeniería Electrónica - Proyecto Base
