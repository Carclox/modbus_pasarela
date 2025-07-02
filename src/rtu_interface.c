#include "rtu_interface.h"


modbus_t modbus_rtu_init(void){

    modbus_t *ctx = NULL; // inicializar ctx como null porque es un PUNTERO
    //Crear el contexto modbus RTU
    ctx = modbus_new_rtu(RTU_DEVICE), RTU_BAUDRATE, RTU_PARITY, RTU_DATABITS, RTU_STOPBITS);
    if (ctx == NULL) {
        fprintf(stderr, "Unable to create the libmodbus context\n");
        return NULL;
    }
    // Configurar modo serial (modbus_new_rtu ya lo hace implicitamente), pero no esta de mas dejarlo claro
    modbus_set_serial_mode(ctx, MODBUS_RTU_RS485);

    // Configuar el timeout de respuesta
    modbus_set_response_timeout(ctx, RTU_TIMEOUT_SEC, RTU_TIMEOUT_USEC);

    // Conectar el dispositivo RTU
    if (modbus_connect(ctx) == -1) {
        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
        modbus_free(ctx); //en caso de falla se libera la conexion
        return NULL;
    }


    printf("Modbus RTU iniciado y conectado con exito en %s\n", RTU_DEVICE);
    return ctx; // si todo va bien se devuelve el contexto creado
}



