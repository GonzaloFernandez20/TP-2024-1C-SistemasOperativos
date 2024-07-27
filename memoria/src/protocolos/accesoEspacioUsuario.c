#include <protocolos/accesoEspacioUsuario.h>

void realizar_lectura(int fd_cliente){
    t_buffer *buffer = recibir_buffer(fd_cliente);
	void* stream = buffer->stream;

	int PID = buffer_read_int(&stream);
    int DF = buffer_read_int(&stream);
	int tamanio_bytes = buffer_read_int(&stream);
    
    eliminar_buffer(buffer);

    void* valor = malloc(tamanio_bytes);

    pthread_mutex_lock(&mutex_log);
        log_info(memoria_log, "PID: < %d > - Accion: < LEER > - Direccion fisica: < %d > - Tamaño < %d >", PID, DF, tamanio_bytes);
    pthread_mutex_unlock(&mutex_log);

    pthread_mutex_lock(&mutex_espacio_usuario);
        memcpy(valor, espacio_usuario + DF, tamanio_bytes);
    pthread_mutex_unlock(&mutex_espacio_usuario);

	usleep(1000*config_memoria.RETARDO_RESPUESTA);
    devolver_valor_leido(valor, tamanio_bytes, fd_cliente);
}

void devolver_valor_leido(void* valor, int tamanio_bytes, int fd_cliente){
    t_paquete* paquete = crear_paquete(VALOR_LEIDO);

	int buffer_size = tamanio_bytes + sizeof(int); 
	crear_buffer(paquete, buffer_size);

	buffer_add_int(paquete->buffer, tamanio_bytes );
	buffer_add_valor(paquete->buffer, valor,tamanio_bytes);
    
	enviar_paquete(paquete, fd_cliente);
    
    free(valor);
	eliminar_paquete(paquete);
}

void realizar_escritura(int fd_cliente){
    t_buffer *buffer = recibir_buffer(fd_cliente);
	void* stream = buffer->stream;

	int PID = buffer_read_int(&stream);
    int DF = buffer_read_int(&stream);
	int tamanio_bytes = buffer_read_int(&stream);
    void* valor = malloc(tamanio_bytes);
    valor =  buffer_read_valor(&stream, tamanio_bytes);

    eliminar_buffer(buffer);

    pthread_mutex_lock(&mutex_log);
        log_info(memoria_log, "PID: < %d > - Accion: < ESCRIBIR > - Direccion fisica: < %d > - Tamaño < %d >", PID, DF, tamanio_bytes);
    pthread_mutex_unlock(&mutex_log);

    pthread_mutex_lock(&mutex_espacio_usuario);
        memcpy(espacio_usuario + DF, valor, tamanio_bytes);
    pthread_mutex_unlock(&mutex_espacio_usuario);

    free(valor);

    usleep(1000*config_memoria.RETARDO_RESPUESTA);
    int mensaje_ok = 1;
	send(fd_cliente, &mensaje_ok, sizeof(int), 0);
}

