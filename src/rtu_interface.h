#ifndef RTU_INTERFACE_H
#define RTU_INTERFACE_H

#include <stdio.h>   // Para FILE y funciones como fopen, fclose, perror, fprintf
#include <modbus.h> // Libreria para el protocolo modbus
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>


//---
// Definiciones
//---

#define RTU_DEVICE "/dev/ttyUSB0" // Puerto serial para el adaptador USB a RS485
#define RTU_BAUDRATE 9600        // Velocidad en baudios para Modbus RTU
#define RTU_PARITY 'N'           // Paridad (N: None, E: Even, O: Odd)
#define RTU_DATABITS 8           // Bits de datos
#define RTU_STOPBITS 1           // Bits de parada
#define TIMEOUT_SEC 3            // Timeout en segundos para operaciones Modbus RTU
#define TIMEOUT_USEC 0


//---
// Declaración de funciones
//---

/**
 @brief inicializa y conecta el contexto Modbus RTU RS485

 Esta funcion crea un nuevo contexto Modbus RTU, lo configura con los parametros
 definidos y establece la conexion serial. tambien configura el timeout de respuesta

 @return Un puntero al contexto modbus_t si la inicializacion y la conexion son exitosas
 o un 'NULL' en caso de error.
*/
modbus_t* modbus_rtu_init(void);


#endif // RTU_INTERFACE_H