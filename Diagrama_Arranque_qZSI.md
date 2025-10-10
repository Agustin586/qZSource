# 🔄 Diagrama de Flujo - Arranque del Convertidor qZSI

## 📊 Flujo Principal de Arranque

```mermaid
graph TD
    A[🔌 POWER ON] --> B[⚙️ INICIALIZACIÓN SISTEMA]
    B --> C[🛡️ VERIFICACIÓN HARDWARE]
    C --> D{🔍 Hardware OK?}
    
    D -->|❌ NO| E[🚨 ESTADO FALLA]
    D -->|✅ SÍ| F[🚀 INICIO ARRANQUE]
    
    F --> G[⏱️ Esperar 5s]
    G --> H[🔌 SW_IN = 1<br/>Conectar entrada resistiva]
    
    H --> I[⏱️ Esperar 10s<br/>Carga suave capacitores]
    I --> J[⚡ VSEL = 1<br/>Entrada directa]
    
    J --> K[🔋 Habilitar salida]
    K --> L[📈 Aumentar VBUS<br/>Objetivo: 380V]
    
    L --> M{🎯 VBUS = 380V?}
    M -->|❌ NO| L
    M -->|✅ SÍ| N[✅ VBUS Enganchado]
    
    N --> O[⏱️ Esperar 5s]
    O --> P[🌊 Aumentar VO<br/>Objetivo: 300V]
    
    P --> Q{🎯 VO = 300V?}
    Q -->|❌ NO| P
    Q -->|✅ SÍ| R[✅ VO Enganchado]
    
    R --> S[⏱️ Esperar 5s]
    S --> T[🟢 FUNCIONAMIENTO NORMAL]
    
    E --> U[💡 LED Código Error]
    E --> V{🔧 Reset Manual?}
    V -->|❌ NO| E
    V -->|✅ SÍ| C
    
    T --> W{🚨 Falla?}
    W -->|❌ NO| T
    W -->|✅ SÍ| E
```

## 🏗️ Diagrama Detallado de Estados

```mermaid
stateDiagram-v2
    [*] --> POWER_ON : Sistema energizado
    
    POWER_ON --> INICIO : Inicialización hardware
    INICIO --> CLEAR_INICIAL : Timer 100ms
    
    CLEAR_INICIAL --> RUN : Hardware Ready
    CLEAR_INICIAL --> WAIT_HW_READY : Hardware NOT Ready
    
    RUN --> INICIO_ARRANQUE : Ejecutar MEF Arranque
    
    state RUN {
        INICIO_ARRANQUE --> SW_IN_RESISTIVO : 5s delay
        SW_IN_RESISTIVO --> AUMENTANDO_VBUS : 10s delay + VSEL=1
        AUMENTANDO_VBUS --> CONTROLADOR_VBUS_ENGANCHADO : VBUS = 380V
        CONTROLADOR_VBUS_ENGANCHADO --> AUMENTANDO_VO : 5s delay
        AUMENTANDO_VO --> CONTROLADOR_VO_ENGANCHADO : VO = 300V  
        CONTROLADOR_VO_ENGANCHADO --> RUN_NORMAL : 5s delay
        RUN_NORMAL --> RUN_NORMAL : Operación continua
    }
    
    RUN --> FALLA : Error detectado
    FALLA --> REST_FALLA : Button CLR_FAULT
    REST_FALLA --> WAIT_HW_READY : 100ms delay
    WAIT_HW_READY --> RUN : Hardware Ready
    
    FALLA --> FALLA : Sin acción usuario
```

## ⚙️ Secuencia de Señales de Control

```mermaid
gantt
    title Secuencia Temporal de Arranque qZSI
    dateFormat X
    axisFormat %s
    
    section Hardware Init
    Reset Drivers    :done, reset, 0, 100ms
    Enable Level Shifters :done, enable, 0, 100ms
    
    section Arranque Suave
    SW_IN = 0 (Resistivo)  :active, sw0, 0, 5000ms
    SW_IN = 1 (Directo)    :sw1, after sw0, 10000ms
    VSEL = 1 (Bypass)      :vsel, 5000ms, 10000ms
    
    section Control VBUS
    Habilitar Salida       :vo_enable, 15000ms, 100ms
    Rampa VBUS 0→380V      :vbus_ramp, after vo_enable, 3000ms
    VBUS Estabilizado      :vbus_stable, after vbus_ramp, 5000ms
    
    section Control VO
    Clear Vo Inicial       :vo_clear, after vbus_stable, 100ms
    Rampa VO 0→300V        :vo_ramp, after vo_clear, 2000ms
    VO Estabilizado        :vo_stable, after vo_ramp, 5000ms
    
    section Operación
    Funcionamiento Normal  :normal, after vo_stable, 10000ms
```

## 📋 Descripción de Cada Etapa

### 🔧 **1. INICIALIZACIÓN (0-100ms)**
- Reset de drivers de compuerta
- Habilitación de level shifters
- Configuración inicial de periféricos
- Verificación de protecciones hardware

### 🔌 **2. ENTRADA RESISTIVA (5s)**
- `SW_IN = 0`: Conexión a través de resistor limitador
- Carga controlada de capacitores de entrada
- Protección contra corrientes de irrupción

### ⚡ **3. ENTRADA DIRECTA (10s)**
- `SW_IN = 1`: Bypass del resistor limitador
- `VSEL = 1`: Conexión directa a fuente
- Estabilización de tensión de entrada

### 🔋 **4. CONTROL VBUS (Variable)**
- Habilitación de salida PWM
- Rampa gradual de VBUS: 0V → 380V
- Control de lazo abierto inicial
- Verificación de alcance de referencia

### 🌊 **5. CONTROL VO (Variable)**
- Liberación de restricción de Vo inicial
- Rampa gradual de VO: 0V → 300V
- Transición a control normal
- Verificación de alcance de referencia

### 🟢 **6. FUNCIONAMIENTO NORMAL**
- Control automático de VBUS y VO
- Monitoreo continuo de protecciones
- Operación estable del inversor

## 🛡️ Protecciones Durante el Arranque

### Hardware
- Trip Zone activo en todo momento
- Monitoreo de sobrecorriente instantáneo
- Protección térmica de drivers

### Software
- Verificación de límites de tensión/corriente
- Timeouts en cada etapa
- Recuperación automática ante fallas menores

## ⏱️ Tiempos Típicos de Arranque

| Etapa | Tiempo | Descripción |
|-------|--------|-------------|
| Inicialización | 100ms | Setup hardware |
| Entrada Resistiva | 5s | Carga suave |
| Entrada Directa | 10s | Estabilización |
| Rampa VBUS | 2-5s | Dependiente de carga |
| Rampa VO | 1-3s | Control progresivo |
| **TOTAL** | **~20s** | Arranque completo |

Este arranque gradual asegura que no haya transitorios bruscos que puedan dañar los componentes del sistema. 🚀