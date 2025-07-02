#include "gateway_logic.h"


void handle_client_connection(modbus_t *ctx_tcp_client, modbus_t *ctx_rtu) {
    // Definimos un buffer
    uint8_t query[MAX_MODBUS_FRAME_LENGTH];
    int rc = 0;      // 
    int slave_id = -1; 
    int rc_rtu = 0;  

    printf("Iniciando manejo de cliente TCP...\n");

    // Bucle para procesar múltiples peticiones del mismo cliente TCP
    while (1) {
        // Recibimos una petición del cliente TCP
        rc = modbus_receive(ctx_tcp_client, query);

        if (rc == -1) {
            // Error en la recepción de la petición Modbus TCP
            if (errno == ETIMEDOUT) {
                fprintf(stderr, "Cliente TCP (socket %d): Tiempo de espera agotado. Esperando siguiente peticion.\n", modbus_get_socket(ctx_tcp_client));
                continue; // Continúa al inicio del bucle para reintentar recibir
            } else if (errno == EPIPE) {
                fprintf(stderr, "Cliente TCP (socket %d): Pipe roto (cliente desconectado). Cerrando conexion.\n", modbus_get_socket(ctx_tcp_client));
                break; // Salir del bucle para cerrar la conexión
            } else {
                // Otro tipo de error grave en la recepción TCP
                fprintf(stderr, "Cliente TCP (socket %d): Error al recibir peticion: %s. Cerrando conexion.\n",
                        modbus_get_socket(ctx_tcp_client), modbus_strerror(errno));
                break; // Salir del bucle para cerrar la conexión
            }
        } else if (rc == 0) {
            // modbus_receive devuelve 0 si el cliente se desconectó limpiamente
            fprintf(stdout, "Cliente TCP (socket %d) desconectado limpiamente. Cerrando conexion.\n", modbus_get_socket(ctx_tcp_client));
            break; // Salir del bucle para cerrar la conexión
        }

        // Si rc > 0, hemos recibido una petición válida
        printf("Cliente TCP (socket %d): Peticion recibida (%d bytes).\n", modbus_get_socket(ctx_tcp_client), rc);

        // Extraer slave_id de la petición Modbus TCP (byte 6 del MBAP header)
        slave_id = query[6]; // Punto y coma añadido

        // Validar slave_id. Los IDs 0 y 255 son especiales y no deben usarse como IDs de esclavo RTU
        if (slave_id == 0 || slave_id == 255) {
            fprintf(stderr, "Cliente TCP (socket %d): ID de esclavo (%d) invalido para RTU. Ignorando peticion.\n",
                    modbus_get_socket(ctx_tcp_client), slave_id);
            continue; // Continúa al siguiente ciclo para esperar otra petición
        }

        // Establecer el slave ID en el contexto RTU
        if (modbus_set_slave(ctx_rtu, slave_id) == -1) { 
            fprintf(stderr, "Error: Slave ID %d no valido para RTU: %s\n", slave_id, modbus_strerror(errno));
            break;
        }
        printf("Configurado RTU para Slave ID: %d\n", slave_id);

        // Enviar la petición Modbus raw al dispositivo RTU
        rc_rtu = modbus_send_raw_request(ctx_rtu, query, rc);
        // La llamada a modbus_receive_confirmation debe estar después de verificar rc_rtu
        if (rc_rtu == -1) {
            fprintf(stderr, "Error al enviar peticion a RTU (Slave ID %d): %s\n", slave_id, modbus_strerror(errno));
            break;
        }
        printf("Peticion enviada a RTU (Slave ID %d). Esperando confirmacion...\n", slave_id);

        // Recibir la confirmación (respuesta) del dispositivo RTU
        // La respuesta se almacenará en el mismo buffer 'query'
        rc = modbus_receive_confirmation(ctx_rtu, query);
        if (rc == -1) {
            fprintf(stderr, "Error al recibir respuesta de RTU (slave ID %d): %s\n", slave_id, modbus_strerror(errno));
            break;
        } else if (rc == 0) {
             fprintf(stderr, "RTU (Slave ID %d): No se recibio respuesta dentro del timeout.\n", slave_id);
             break;
        }
        printf("Respuesta recibida de RTU (Slave ID %d): %d bytes.\n", slave_id, rc);

        // Enviar la respuesta recibida del RTU de vuelta al cliente TCP
        if (modbus_reply(ctx_tcp_client, query, rc, ctx_rtu) == -1) {
            fprintf(stderr, "Error al enviar respuesta al cliente TCP (socket %d): %s \n", modbus_get_socket(ctx_tcp_client), modbus_strerror(errno));
            break;
        }
        printf("Respuesta enviada al cliente TCP (socket %d).\n", modbus_get_socket(ctx_tcp_client));
    }

    printf("Manejo del cliente TCP finalizado.\n");
}