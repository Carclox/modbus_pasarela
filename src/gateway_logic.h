#ifndef GATEWAY_LOGIC_H
#define GATEWAY_LOGIC_H

//---
// Encabezados
//---

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

// Tamaño del buffer para las tramas Modbus
#define MAX_MODBUS_FRAME_LENGTH MODBUS_TCP_MAX_ADU_LENGTH 

//---
// Prototipado de funciones
//---

/**
 * @brief Maneja la comunicación de un cliente Modbus TCP, retransmitiendo peticiones a RTU.
 *
 * Esta función se encarga de leer las peticiones Modbus de un cliente TCP,
 * reenviarlas al dispositivo RTU especificado por el slave ID,
 * y luego enviar la respuesta del RTU de vuelta al cliente TCP.
 * El bucle interno continúa hasta que el cliente se desconecta o ocurre un error.
 *
 * @param ctx_tcp_client Un puntero al contexto Modbus TCP asociado al cliente entrante.
 * @param ctx_rtu Un puntero al contexto Modbus RTU para la comunicación con el esclavo.
 */
void handle_client_connection(modbus_t *ctx_tcp_client, modbus_t *ctx_rtu);

#endif // GATEWAY_LOGIC_H