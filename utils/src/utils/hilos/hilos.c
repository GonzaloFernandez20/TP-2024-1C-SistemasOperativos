#include <utils/hilos/hilos.h>


/**
 * La CPU (fd_servidor) espera que Kernel se conecte 
 * y una vez conectado asigna un hilo para el procesamiento de operaciones dispatch.
 * Al procesar la operación dispatch, espera recibir un contexto de ejecución,
 * que contiene la info que necesita CPU para correr un proceso.
 * 
 *  */ 
void atender_cliente(int fd_servidor, void (*procesar_conexion)(void*), t_log* logger, char* modulo){

    int *fd_cliente = malloc(sizeof(int));
        *fd_cliente = esperar_cliente(fd_servidor);
    
    
    log_info(logger, "Se conecto %s", modulo);

    recibir_handshake(*fd_cliente, logger);

    asignar_hilo(fd_cliente, procesar_conexion);
}


void asignar_hilo(int *fd_cliente,  void (*procesar_conexion)(void*)){
    pthread_t thread_cliente;

	pthread_create(&thread_cliente, NULL, (void *)procesar_conexion, (void *)fd_cliente);

	pthread_detach(thread_cliente);
}

int _deshacer_casting(void *fd_cpu_casteado){ 

    int *dato_casteado = (int *)fd_cpu_casteado; // Convierto el puntero a void en un puntero a int con un nuevo casting
    int dato_sin_casting = *dato_casteado; // Desreferencio para obtener el valor original

    return dato_sin_casting; // Devuelvo valor original 
}

