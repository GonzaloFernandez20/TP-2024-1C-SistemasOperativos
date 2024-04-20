#include <utils/hilos/hilos.h>

void atender_cliente(int *fd_servidor, void (*procesar_conexion)(void*), t_log* logger, char* modulo){

    int *fd_cliente = malloc(sizeof(int));
        *fd_cliente = esperar_cliente(*fd_servidor);
    
    log_info(logger, "Se conecto %s", modulo);

    recibir_handshake(int fd_cliente, t_log* logger);

    _asignar_hilo(fd_cliente, procesar_conexion);
}

void _asignar_hilo(int *fd_cliente,  void (*procesar_conexion)(void*)){
    pthread_t thread_cliente;

	pthread_create(&thread_cliente, NULL, procesar_conexion, (void *)fd_cliente);

	pthread_detach(thread_cliente);
}

int _deshacer_casting(void *fd_cpu_casteado){ //! Esta funcion deberia ir en utils, porque es generica, si es que se va a usar en otro lado

    int *dato_casteado = (int *)fd_cpu_casteado; //? Convierto el puntero a void en un puntero a int con un nuevo casting
    int dato_sin_casting = *dato_casteado; //? Desreferencio para obtener el valor original

    return dato_sin_casting; //? Devuelvo dicho valor original 
}

// el hilo esta declarado localmente y cuando termine asignar hilo nada hace referencia
// por ahi probar un return del hilo
