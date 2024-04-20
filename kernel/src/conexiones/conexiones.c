#include <conexiones/conexiones.h>

void iniciar_servidor_kernel(void){
    char* IP = strdup(config_kernel.IP_MEMORIA);
    char* PUERTO = strdup(config_kernel.PUERTO_ESCUCHA);

    fd_server_kernel = iniciar_servidor(IP , PUERTO);

    log_info(kernel_log_debugg, "Servidor iniciado! KERNEL escuchando en %s:%s\n", IP, PUERTO);

    free(IP);
    free(PUERTO);
}

void atender_entradasalida(){
    atender_cliente(fd_server_kernel, (void *)procesar_conexion_es, kernel_log_debugg, "ENTRADA/SALIDA");
}


// ---------------------- CONEXIONES CON LOS CLIENTES ---------------------------

void gestionar_conexiones(void){
    conectar_memoria();
    conectar_dispatch();
    conectar_interrupt();
}

void conectar_memoria(void){ //? Conexion con memoria
    char* IP = strdup(config_kernel.IP_MEMORIA);
    char* PUERTO = strdup(config_kernel.PUERTO_MEMORIA);

    fd_conexion_memoria = crear_conexion(config_kernel.IP_MEMORIA, config_kernel.PUERTO_MEMORIA);

    enviar_handshake(fd_conexion_memoria, "KERNEL", kernel_log_debugg);
    
    free(IP);
    free(PUERTO);
}

void conectar_dispatch(void){
    char* IP = strdup(config_kernel.IP_CPU);
    char* PUERTO = strdup(config_kernel.PUERTO_CPU_INTERRUPT);

    fd_conexion_dispatch = crear_conexion(IP, PUERTO);

    enviar_handshake(fd_conexion_dispatch, "KERNEL", kernel_log_debugg);

    log_trace(kernel_log_debugg, "KERNEL conectado a CPU DISPATCH %s en %s:%s\n", IP, PUERTO);
    
    free(IP);
    free(PUERTO);
}

void conectar_interrupt(void){
    char* IP = strdup(config_kernel.IP_CPU);
    char* PUERTO = strdup(config_kernel.PUERTO_CPU_INTERRUPT);

    fd_conexion_interrupt = crear_conexion(IP, PUERTO);

    enviar_handshake(fd_conexion_interrupt, "KERNEL", kernel_log_debugg);

    log_trace(kernel_log_debugg, "KERNEL conectado a CPU INTERRUPT %s en %s:%s\n", IP, PUERTO);

    free(IP);
    free(PUERTO);
}

// -------------------------------------------------------------------------------