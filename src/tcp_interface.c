#include "tcp_interface.h"

modbus_t* modbus_tcp_init(void){
    // creacion del contexto TCP
    modbus_t *ctx = NULL; // inicializar contexto TCP como NULL

    ctx = modbus_new_tcp(TCP_HOST, TCP_PORT);
    if (ctx == NULL) {
        fprintf(stderr, "Unable to allocate libmodbus context\n");
        return NULL;
    }

    // poner el contexto en modo escucha
    // En caso de requerir configurar un timeout para escuchar se hace con modbus_set_response_timeout
    //int modbus_set_response_timeout(modbus_t *ctx, uint32_t to_sec, uint32_t to_usec);

    int server_socket = modbus_tcp_listen(ctx, 1); // 1 es el backlog de conexiones pendientes
    if (server_socket == -1) {
        fprintf(stderr, "Error: fallo al escuchar TCP en %s, %d: %s\n",TCP_HOST, TCP_PORT, modbus_strerror(errno));
        modbus_free(ctx);
        return NULL;
    }

    printf("Modbus TCP configurado para escuchar en %s, %d, (Socket: %d)",TCP_HOST, TCP_PORT, server_socket);
    return ctx; // Devuelve el contexto creado
}



int modbus_tcp_accept_connection(modbus_t *ctx_tcp_listen){

    int client_socket = -1;

    if (ctx_tcp_listen == NULL){
        fprintf(stderr, "Error: contexto TCP nulo en modbus_tcp_accept_conection. \n");
        return -1;
    }

    // modbus_accept bloquea hasta que una nueva conexion es aceptada
    //retorna el descriptor del archivo del socket del cliente
    client_socket = modbus_accept(ctx_tcp_listen);
    if (client_socket == -1){
        fprintf(stderr, "Error: fallo al aceptar conexion TCP: %s\n",modbus_strerror(errno));
        return -1;
    }
    
    printf("Nueva conexion TCP aceptada (Socket cliente: %d)\n", client_socket);
    return client_socket;
}

