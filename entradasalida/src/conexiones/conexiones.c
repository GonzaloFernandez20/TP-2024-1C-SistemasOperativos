#include <conexiones/conexiones.h>

void establecer_conexiones(void){
    conectar_memoria();
    conectar_kernel();
}

void conectar_memoria(void){ 
    char* IP = strdup(config_IO.IP_MEMORIA);
    char* PUERTO = strdup(config_IO.PUERTO_MEMORIA);

    fd_conexion_memoria = crear_conexion(IP, PUERTO);

    if(fd_conexion_memoria == -1){
        log_error(IO_log_debug,"E/S no se conecto a MEMORIA");
    }
    else{
        log_info(IO_log_debug, "E/S conectado a MEMORIA en %s:%s", IP, PUERTO);
        enviar_handshake(fd_conexion_memoria, "E/S", IO_log_debug);
    }

    free(IP);
    free(PUERTO);
}

void conectar_kernel(void){ 
    char* IP = strdup(config_IO.IP_KERNEL);
    char* PUERTO = strdup(config_IO.PUERTO_KERNEL);

    fd_conexion_kernel = crear_conexion(IP, PUERTO);

    if(fd_conexion_kernel == -1){
        log_error(IO_log_debug,"E/S no se conecto a KERNEL");
    }
    else{
        log_info(IO_log_debug, "E/S conectado a KERNEL en %s:%s", IP, PUERTO);
        enviar_handshake(fd_conexion_kernel, "E/S", IO_log_debug);
    }

    free(IP);
    free(PUERTO);
}