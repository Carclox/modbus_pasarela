#ifndef TCP_INTERFACE_H
#define TCP_INTERFACE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <modbus.h>     // Librería libmodbus
#include <sys/socket.h> // Para sockets
#include <netinet/in.h> // Para direcciones de Internet
#include <arpa/inet.h>  // Para inet_ntoa
#include <sys/select.h> // Para select()

//---
// Definiciones
//---

#define TCP_PORT 502               // Puerto Modbus TCP estándar
#define TCP_HOST "0.0.0.0"         // HOST TCP para escuchar en todas las interfaces

//---
//Declaracion de funciones
//---

/**
 * @brief Inicializa el contexto Modbus TCP para escuchar conexiones entrantes.
 *
 * Esta función crea un nuevo contexto Modbus TCP, lo configura y lo pone a la escucha
 * de conexiones de clientes Modbus TCP.
 *
 * @return Un puntero al contexto `modbus_t` si la inicialización y escucha son exitosas,
 * o `NULL` en caso de error.
 */
modbus_t* modbus_tcp_init(void);



 /**
  * @brief Aceptar conexion entrante
  * 
  * Esta funcion debe ser llamada despues de que el contexto TCP haya sido inicializado,
  * y esta en modo de escucha, bloquea hasta que haya una nueva conexion
  * 
  * @param *ctx_tcp_listen Puntero al contexto tcp configuarad para escuchar
  * 
  * @return descriptor del archivo del socket de cliente si es exitoso o -1 en caso de error
  */
int modbus_tcp_accept_connection(modbus_t *ctx_tcp_listen);


#endif // TCP_INTERFACE_H

