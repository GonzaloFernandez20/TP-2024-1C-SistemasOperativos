#include <utils/hilos/hilos.h>

void atender_cliente(int fd_servidor, void (*procesar_conexion)(void*), t_log* logger, char* modulo){

    int *fd_cliente = malloc(sizeof(int));
    puts("ESPERANDO CLIENTE. (hilos.c)");
    *fd_cliente = esperar_cliente(fd_servidor);
    puts("CONECTADO CLIENTE. (hilos.c)");
    
    log_info(logger, "Se conecto %s", modulo);

    recibir_handshake(*fd_cliente, logger);
    puts("RECIBIDO HANDSHAKE (hilos.c)");

    asignar_hilo(fd_cliente, procesar_conexion);
}

void asignar_hilo(int *fd_cliente,  void (*procesar_conexion)(void*)){
    pthread_t thread_cliente;

    puts("CREANDO HILO. (hilos.c)");
	pthread_create(&thread_cliente, NULL, (void *)procesar_conexion, (void *)fd_cliente);
    puts("HILO CREADO. (hilos.c)");
	pthread_detach(thread_cliente);
}

int _deshacer_casting(void *fd_cpu_casteado){ 

    int *dato_casteado = (int *)fd_cpu_casteado; // Convierto el puntero a void en un puntero a int con un nuevo casting
    int dato_sin_casting = *dato_casteado; // Desreferencio para obtener el valor original

    return dato_sin_casting; // Devuelvo valor original 
}

void atender_solicitud(void (*procesar_solicitud)){
    pthread_t thread_solicitud;
    pthread_create(&thread_solicitud, NULL, (void *)procesar_solicitud, NULL);
    pthread_detach(thread_solicitud);
}