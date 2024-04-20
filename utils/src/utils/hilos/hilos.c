#include <utils/hilos/hilos.h>

void atender_cliente(int *socket_servidor, void (*procesar_conexion)(void*), t_log* logger){

    int *fd_cliente = malloc(sizeof(int));
        *fd_cliente = esperar_cliente(*socket_servidor);

    recibir_handshake(int fd_cliente, t_log* logger);

    asignar_hilo(fd_cliente, procesar_conexion);
}

void asignar_hilo(int *fd_cliente,  void (*procesar_conexion)(void*)){
    pthread_t thread_cliente;

	pthread_create(&thread_cliente, NULL, procesar_conexion, (void *)fd_cliente);

	pthread_detach(thread_cliente);
}
