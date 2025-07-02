
**Carlos S. Rangel**

# 🛠️ Pasarela Modbus TCP ↔ RTU (Gateway)

Este proyecto implementa una pasarela bidireccional entre un cliente **Modbus TCP** y un esclavo **Modbus RTU**. Está diseñado para ejecutarse en entornos **Linux**, como parte de sistemas embebidos o gateways industriales, permitiendo integrar dispositivos serie (RS-485) en redes Ethernet/IP sin necesidad de hardware adicional.

---

## 🔌 ¿Qué hace esta pasarela?

- Acepta conexiones entrantes de clientes Modbus TCP.
- Traduce las peticiones y las reenvía a un dispositivo esclavo RTU.
- Recibe las respuestas del dispositivo RTU.
- Las reenvía de nuevo al cliente TCP.
- Todo esto de forma transparente y eficiente.

---

## 📦 Estructura del Proyecto

```txt
.
├── src/
│   ├── main.c                 # Lógica principal del programa (entrypoint)
│   ├── rtu_interface.c        # Inicialización y gestión de interfaz RTU
│   ├── tcp_interface.c        # Inicialización y gestión de interfaz TCP
│   ├── gateway_logic.c        # Lógica de pasarela y reenvío Modbus
│   ├── *.h                    # Archivos de cabecera asociados
│   └── README.md              # Documentación interna por módulo
├── include/                   # (Opcional) Encabezados compartidos
├── build/                     # Archivos compilados y binarios
└── README.md                  # ← Este archivo
└── Makefile 
```

## ⚙️ Requisitos del sistema
Sistema operativo: Linux (Debian, Ubuntu, Yocto, etc.)

Dependencias:

libmodbus (v3.1.4 o superior)

make, gcc, build-essential

Hardware recomendado:

Puerto serie RS-485 funcional (ej. /dev/ttyUSB0)

Puerto Ethernet libre para clientes TCP

