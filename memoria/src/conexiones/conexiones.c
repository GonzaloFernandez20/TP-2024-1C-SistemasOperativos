#include <conexiones/conexiones.h>

void iniciar_servidor_memoria(void){
    char* IP = strdup(config_memoria.IP_MEMORIA);
    char* PUERTO = strdup(config_memoria.PUERTO_ESCUCHA);

    fd_server_memoria = iniciar_servidor(IP , PUERTO);

    if(fd_server_memoria == -1){
        log_error(memoria_log_debugg,"No se pudo iniciar servidor MEMORIA");
    }
    else{
        log_info(memoria_log_debugg, "Servidor iniciado! MEMORIA escuchando en %s:%s\n", IP, PUERTO);
    }

    free(IP);
    free(PUERTO);
}

void gestionar_conexiones_clientes(void){
   atender_cpu();
   atender_kernel();
   atender_entradaSalida();
}

void atender_cpu(void){
    atender_cliente(fd_server_memoria, (void *)procesar_operacion_cpu, memoria_log_debugg, "CPU");
}
void atender_kernel(void){
    atender_cliente(fd_server_memoria, (void *)procesar_operacion_kernel, memoria_log_debugg, "KERNEL");
}

// la conexion de E/S la atendemos con el hilo main
void atender_entradaSalida(void){
    int fd_cliente = esperar_cliente(fd_server_memoria);
    
    log_info(memoria_log_debugg, "Se conecto E/S");

    recibir_handshake(fd_cliente, memoria_log_debugg);
    procesar_operacion_entradaSalida(fd_cliente);
}