
**Carlos S. Rangel**

# 🛠️ Pasarela Modbus TCP ↔ RTU (Gateway)

La aplicación es una pasarela (gateway) **Modbus TCP** a **RTU** diseñada para operar en sistemas Linux embebidos, como la Raspberry Pi. Su propósito principal es permitir que los clientes Modbus TCP se comuniquen con dispositivos Modbus RTU (esclavos) conectados a un puerto serie. Esto es particularmente útil para leer información de medidores eléctricos que utilizan el protocolo Modbus RTU, permitiendo su acceso a través de una red Ethernet o Wi-Fi (incluyendo Wi-Fi HaLow).

---

## 🔌 Caracteristicas principales

- Pasarela Modbus TCP a RTU: Traduce peticiones y respuestas entre los protocolos Modbus TCP y Modbus RTU.


- Lenguaje: Implementado en C.

- Librería Principal: Utiliza la librería libmodbus para el manejo de la comunicación Modbus.

- Despliegue: Orientado a plataformas Linux embebidas, con la Raspberry Pi como dispositivo de prototipado inicial.

- Conectividad de Red: Soporta tramas TCP recibidas vía Ethernet o Wi-Fi (incluyendo Wi-Fi HaLow).Acepta conexiones entrantes de clientes Modbus TCP.


---

## 📦 Estructura del Proyecto

* El proyecto está organizado modularmente para facilitar la comprensión, el mantenimiento y la escalabilidad.

* ```main.c```: Contiene el bucle principal de la aplicación. Se encarga de inicializar los contextos Modbus (TCP y RTU) y de gestionar la aceptación de nuevas conexiones de clientes TCP. Es el orquestador central de la pasarela.

* ```rtu_interface.h``` / ```rtu_interface.c```: Módulo dedicado a la inicialización y configuración del contexto Modbus RTU. Maneja los detalles de la comunicación serial (puerto, velocidad, paridad, etc.).

* ```tcp_interface.h``` / ```tcp_interface.c```: Módulo responsable de la inicialización del contexto Modbus TCP en modo servidor. Configura el servidor para que escuche y acepte conexiones entrantes de clientes Modbus TCP.

* ```gateway_logic.h``` / ```gateway_logic.c```: Este módulo encapsula la lógica central de la pasarela. Se encarga de recibir las peticiones de los clientes Modbus TCP, extraer el ID de la unidad (slave ID), retransmitir la petición al dispositivo Modbus RTU correspondiente, recibir la respuesta del RTU y, finalmente, enviarla de vuelta al cliente TCP.

## ⚙️ Requisitos del sistema
Sistema operativo: ```Linux``` (Debian, Ubuntu, Yocto, etc.)

**Dependencias:**<br>
libmodbus (v3.1.4 o superior)<br>
make, gcc, build-essential

**Hardware recomendado:**<br>
Puerto serie RS-485 funcional (ej. /dev/ttyUSB0)<br>
Puerto Ethernet libre para clientes TCP

## :key: probar la pasarela.:

Asegurarte de que tu Raspberry Pi (o el dispositivo embebido) esté configurado con el puerto serial correcto (/dev/ttyUSB0 o similar).

Tener un dispositivo Modbus RTU conectado al puerto serial.

Usar un cliente Modbus TCP (como ```Modbus Poll```, ```Modscan```, o incluso un script Python simple con pymodbus) en tu máquina de desarrollo o en otra máquina en la red, para enviar peticiones Modbus TCP a la IP de tu Raspberry Pi y al puerto 502.
