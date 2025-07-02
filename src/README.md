## `main.c` - Entrada principal de la pasarela Modbus TCP ↔ RTU

Este archivo representa el núcleo del sistema de pasarela. Es responsable de:

- Inicializar las interfaces Modbus RTU y TCP.
- Escuchar conexiones TCP entrantes desde clientes Modbus.
- Delegar la comunicación al módulo de lógica de pasarela (`gateway_logic.c`).
- Mantener un bucle de servicio continuo, gestionando múltiples conexiones TCP sucesivas.

### 🛠️ Funcionalidades clave

- **Inicialización robusta**: Maneja errores en la configuración de RTU o TCP de forma segura, asegurando limpieza de recursos.
- **Gestión de clientes TCP**: Acepta conexiones Modbus TCP entrantes, asociándolas dinámicamente con el contexto de servidor.
- **Comunicación transparente**: Delegación a `handle_client_connection()` para gestión completa del ciclo de vida de la sesión Modbus.
- **Reintento automático**: Si la aceptación de una conexión falla, el sistema reintenta tras un breve retardo (`sleep(1)`).

### 🔁 Flujo de ejecución

1. Inicialización de RTU y TCP.
2. En bucle:
   - Espera cliente TCP.
   - Acepta la conexión.
   - Enlaza el socket con el contexto Modbus.
   - Procesa la solicitud del cliente.
   - Cierra el socket al finalizar.
3. (Teóricamente) limpieza de recursos al salir del bucle.

> Este archivo utiliza las interfaces definidas en `rtu_interface.h`, `tcp_interface.h`, y la lógica de enrutamiento en `gateway_logic.h`.


## 🧩 Módulo: `rtu_interface.c` - Interfaz para Modbus RTU

Este archivo contiene la implementación de la inicialización de la interfaz **Modbus RTU** utilizando la biblioteca [libmodbus](https://libmodbus.org/).

Su objetivo principal es encapsular la creación, configuración y conexión del contexto RTU para ser utilizado por la pasarela en la comunicación con dispositivos esclavos Modbus RTU.

---

### ✅ Función principal

```c
modbus_t *modbus_rtu_init(void);
```

## 🌐 Módulo: `tcp_interface.c` - Interfaz para Modbus TCP

Este archivo implementa la capa de inicialización y gestión de conexiones para la parte **Modbus TCP** de la pasarela. Utiliza la biblioteca [libmodbus](https://libmodbus.org/) para crear el servidor, escuchar conexiones entrantes y aceptar clientes.

---

### ✅ Funciones principales

#### `modbus_t *modbus_tcp_init(void)`

- Crea y configura un contexto Modbus TCP usando `modbus_new_tcp()`.
- Inicia el modo de escucha con `modbus_tcp_listen()` y un backlog de `1`.
- Devuelve el puntero al contexto TCP creado, o `NULL` en caso de error.
- En caso de error al crear o escuchar, imprime mensajes de depuración y libera recursos.

---

#### `int modbus_tcp_accept_connection(modbus_t *ctx_tcp_listen)`

- Espera (bloqueante) una conexión entrante en el contexto de servidor TCP.
- Si la conexión se acepta correctamente, devuelve el descriptor del socket del cliente.
- En caso de error, imprime mensajes y retorna `-1`.

---

### ⚙️ Características clave

- **Inicialización segura del servidor**:
  - Se asegura que el contexto TCP es creado correctamente.
  - Se maneja apropiadamente cualquier error en el *binding* o *listening*.

- **Modo bloqueante**:
  - La función `modbus_accept()` es bloqueante: detiene la ejecución hasta que un cliente TCP se conecta.

- **Separación de responsabilidades**:
  - `modbus_tcp_init()` configura el servidor.
  - `modbus_tcp_accept_connection()` se encarga de aceptar nuevos clientes.

---

### 📌 Dependencias

Este módulo requiere los siguientes macros definidos en `tcp_interface.h`:

- `TCP_HOST` — IP o interfaz donde escuchar (ej. `"0.0.0.0"`).
- `TCP_PORT` — Puerto TCP donde se abrirá el servidor Modbus.

---

### 🧠 Notas técnicas

- **Backlog**: el backlog de conexiones en espera está fijo en `1`, lo cual es adecuado para entornos embebidos o aplicaciones de un solo cliente simultáneo.
- **Timeouts**: aunque no están configurados aquí, pueden añadirse usando `modbus_set_response_timeout()`.

---


## 🔁 Módulo: `gateway_logic.c` - Lógica de pasarela TCP ↔ RTU

Este archivo implementa el núcleo de la pasarela Modbus, encargándose de recibir solicitudes Modbus TCP desde un cliente y reenviarlas hacia un dispositivo esclavo Modbus RTU. Luego, reenvía la respuesta del RTU nuevamente al cliente TCP.

---

### ✅ Función principal

```c
void handle_client_connection(modbus_t *ctx_tcp_client, modbus_t *ctx_rtu);
```

