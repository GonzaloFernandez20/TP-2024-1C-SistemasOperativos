#include <conexiones/conexiones.h>

void iniciar_servidor_kernel(void){
    char* IP = strdup(config_kernel.IP_KERNEL);
    char* PUERTO = strdup(config_kernel.PUERTO_ESCUCHA);

    fd_server_kernel = iniciar_servidor(IP , PUERTO);

    if(fd_server_kernel == -1){
        log_error(kernel_log_debugg,"No se pudo iniciar servidor KERNEL");
    }
    else{
        log_info(kernel_log_debugg, "Servidor iniciado! KERNEL escuchando en %s:%s\n", IP, PUERTO);
    }

    free(IP);
    free(PUERTO);
}

void atender_entradasalida(){
    atender_cliente(fd_server_kernel, (void *)procesar_conexion_es, kernel_log_debugg, "ENTRADA/SALIDA");
    
    entrada_salida_conectada = 1;
    while(entrada_salida_conectada){
        // CONTINGENCIA: se borra en cuanto veamos señales y semaforos
        // es temporal para que el hilo main no finalice el programa
    }
}


void establecer_conexiones(void){
    conectar_memoria();
    conectar_dispatch();
    conectar_interrupt();
}

void conectar_memoria(void){ 
    char* IP = strdup(config_kernel.IP_MEMORIA);
    char* PUERTO = strdup(config_kernel.PUERTO_MEMORIA);

    fd_conexion_memoria = crear_conexion(config_kernel.IP_MEMORIA, config_kernel.PUERTO_MEMORIA);

    if(fd_conexion_memoria == -1){
        log_error(kernel_log_debugg,"KERNEL no se conecto a MEMORIA\n");
    }
    else{
        log_info(kernel_log_debugg, "KERNEL conectado a MEMORIA en %s:%s", IP, PUERTO);
        enviar_handshake(fd_conexion_memoria, "KERNEL", kernel_log_debugg);
    }
    
    free(IP);
    free(PUERTO);
}

void conectar_dispatch(void){
    char* IP = strdup(config_kernel.IP_CPU);
    char* PUERTO = strdup(config_kernel.PUERTO_CPU_DISPATCH);

    fd_conexion_dispatch = crear_conexion(IP, PUERTO);

    if(fd_conexion_dispatch == -1){
        log_error(kernel_log_debugg,"KERNEL no se conecto a CPU DISPATCH\n");
    }
    else{
        log_info(kernel_log_debugg, "KERNEL conectado a CPU DISPATCH en %s:%s", IP, PUERTO);
        enviar_handshake(fd_conexion_dispatch, "KERNEL", kernel_log_debugg);
    }
    
    free(IP);
    free(PUERTO);
}

void conectar_interrupt(void){
    char* IP = strdup(config_kernel.IP_CPU);
    char* PUERTO = strdup(config_kernel.PUERTO_CPU_INTERRUPT);

    fd_conexion_interrupt = crear_conexion(IP, PUERTO);

    if(fd_conexion_interrupt == -1){
        log_error(kernel_log_debugg,"KERNEL no se conecto a CPU INTERRUPT\n");
    }
    else{
        log_info(kernel_log_debugg, "KERNEL conectado a CPU INTERRUPT  en %s:%s", IP, PUERTO);
        enviar_handshake(fd_conexion_interrupt, "KERNEL", kernel_log_debugg);
    }

    free(IP);
    free(PUERTO);
}

