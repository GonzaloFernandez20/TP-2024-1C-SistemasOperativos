#include <conexiones/conexiones.h>

void iniciar_servidores(void){
    iniciar_servidor_dispatch();
    iniciar_servidor_interrupt();
}

void iniciar_servidor_dispatch(void){
    char* IP = strdup(config_cpu.IP_CPU);
    char* PUERTO = strdup(config_cpu.PUERTO_ESCUCHA_DISPATCH);

    fd_dispatch_server = iniciar_servidor(IP , PUERTO);

    if(fd_dispatch_server == -1){
        log_error(cpu_log_debug,"No se pudo iniciar servidor DISPATCH");
    }
    else{
        log_info(cpu_log_debug, "Servidor iniciado! CPU DISPATCH escuchando en %s:%s\n", IP, PUERTO);
    }


    free(IP);
    free(PUERTO);
}

void iniciar_servidor_interrupt(void){
    char* IP = strdup(config_cpu.IP_CPU);
    char* PUERTO = strdup(config_cpu.PUERTO_ESCUCHA_INTERRUPT);

    fd_interrupt_server = iniciar_servidor(IP , PUERTO);

    if(fd_interrupt_server == -1){
        log_error(cpu_log_debug,"No se pudo iniciar servidor INTERRUPT");
    }
    else{
        log_info(cpu_log_debug, "Servidor iniciado! CPU INTERRUPT escuchando en %s:%s\n", IP, PUERTO);
    }


    free(IP);
    free(PUERTO);
}

void conectar_memoria(void){ 
    char* IP = strdup(config_cpu.IP_MEMORIA);
    char* PUERTO = strdup(config_cpu.PUERTO_MEMORIA);

    fd_conexion_memoria = crear_conexion(IP, PUERTO);

    if(fd_conexion_memoria == -1){
        log_error(cpu_log_debug,"CPU no se conecto a MEMORIA");
    }
    else{
        log_info(cpu_log_debug, "CPU conectado a MEMORIA en %s:%s", IP, PUERTO);
        enviar_handshake(fd_conexion_memoria, "CPU", cpu_log_debug);
        
    }

    free(IP);
    free(PUERTO);

}

void gestionar_conexiones_kernel(void){
    atender_dispatch();
    atender_interrupt();

    while(dispatch_conectado || interrupt_conectado){
        // CONTINGENCIA: se borra en cuanto veamos señales y semaforos
        // es temporal para que el hilo main no finalice el programa
    }
}

void atender_dispatch(void){
    atender_cliente(fd_dispatch_server, (void *)procesar_operacion_dispatch, cpu_log_debug, "KERNEL-DISPATCH");
    dispatch_conectado = 1;
}

void atender_interrupt(void){
    atender_cliente(fd_interrupt_server, (void *)procesar_operacion_interrupt, cpu_log_debug,"KERNEL-INTERRUPT");
    interrupt_conectado = 1;
}
