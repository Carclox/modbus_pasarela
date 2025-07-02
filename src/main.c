#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <modbus.h>     // Librería libmodbus
#include <sys/socket.h> // Para sockets (para close())
#include <netinet/in.h> // Para direcciones de Internet
#include <arpa/inet.h>  // Para inet_ntoa
#include <sys/select.h> // Para select()

#include "rtu_interface.h"
#include "tcp_interface.h"
#include "gateway_logic.h"

int main(void){
    modbus_t *ctx_rtu = NULL;
    modbus_t *ctx_tcp_server = NULL;
    int client_socket = -1;

    printf("Iniciando pasarela Modbus TCP a RTU...\n");

    //Inicializar Modbus RTU
    ctx_rtu = modbus_rtu_init();
    if (ctx_rtu == NULL){
        fprintf(stderr, "Fallo al iniciar Modbus RTU. Saliendo\n");
        return EXIT_FAILURE;
    }

    //Iniciar Modbus TCP (servidor)
    ctx_tcp_server = modbus_tcp_init();
    if (ctx_tcp_server == NULL){
        fprintf(stderr, "Fallo al iniciar Modbus TCP. Saliendo\n");
        modbus_free(ctx_rtu); // Libera RTU si TCP falla
        return EXIT_FAILURE;
    }

    // Bucle principal para aceptar conexiones y procesar solicitudes
    while(1){
        printf("Esperando conexiones de clientes TCP...\n");

        // Aceptar una nueva conexión TCP
        client_socket = modbus_tcp_accept_connection(ctx_tcp_server);
        if (client_socket == -1) {
            fprintf(stderr,"Error al aceptar la conexion TCP, reintentando...\n");
            // añadir un pequeño retardo
            sleep(1);
            continue; // Volver al inicio del bucle para reintentar aceptar
        }
        
        // asociar el socket del cliente aceptado con el contexto TCP del servidor
        // para que las operaciones de modbus_receive/reply/send_raw_request
        // se realicen sobre este socket.
        modbus_set_socket(ctx_tcp_server, client_socket);
        printf("Cliente TCP conectado (socket: %d). Preparado para la comunicacion...\n", client_socket);

        // manejador del cliente
        handle_client_connection(ctx_tcp_server, ctx_rtu);

        // Después de que handle_client_connection termine (el cliente se desconectó o hubo un error)
        // cerrar el socket del cliente. El contexto ctx_tcp_server sigue vivo para futuras conexiones.
        close(client_socket);
        printf("Conexion TCP del cliente (socket: %d) cerrada. Esperando siguiente cliente.\n", client_socket);
    }

    // Limpieza (este código solo se alcanzaría si el bucle while(1) se rompiera)
    if (ctx_tcp_server != NULL){
        modbus_free(ctx_tcp_server); // Liberar el contexto TCP del servidor
    }
    if (ctx_rtu != NULL){
        modbus_free(ctx_rtu); // Liberar el contexto RTU
    }

    return EXIT_SUCCESS;
}