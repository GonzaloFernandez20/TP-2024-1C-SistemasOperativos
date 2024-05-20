#include <utils/serializacion/serializacion.h>

///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// ENVIO DE MENSAJES //////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void enviar_handshake(int fd_conexion, char* modulo, t_log* logger){
    
    int respuesta_servidor;

    enviar_presentacion(modulo,fd_conexion);
    recv(fd_conexion, &respuesta_servidor, sizeof(int), MSG_WAITALL);

    if (respuesta_servidor == 0) {
        log_info(logger, "Handshake exitoso: comunicacion con servidor establecida\n");
    } 
    else {
        log_error(logger, "Error handshake: no se establecio comunicacion con servidor");
    }

}

/**
 * @brief Envía un mensaje de presentación al servidor.
 * 
 * Esta función crea un paquete que contiene el nombre del módulo y lo envía al servidor
 * identificado por el file descriptor 'socket_cliente'.
 * 
 * @param nombre_modulo El nombre del módulo que se está presentando.
 * @param socket_cliente El descriptor de archivo del socket de cliente conectado al servidor.
 * 
 * @note La función crea un paquete que incluye el código de operación para el handshake y el nombre del módulo.
 *       Este paquete se serializa antes de ser enviado al servidor.
 */
void enviar_presentacion(char* nombre_modulo, int socket_cliente){

	// es el crear_paquete, agregar_paquete() y enviar_paquete()del tp0 todo fusionado.

	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = HANDSHAKE;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = strlen(nombre_modulo) + 1;
	paquete->buffer->stream = malloc(paquete->buffer->size);
	memcpy(paquete->buffer->stream, nombre_modulo, paquete->buffer->size);

	int bytes = paquete->buffer->size + 2*sizeof(int);

	void* a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
	eliminar_paquete(paquete);
} 

void* serializar_paquete(t_paquete* paquete, int bytes){
	void* paquete_serializado = malloc(bytes); 	// Reservamos espacio de memoria para armar un paquete serializado. 
											   	// Este puntero comienza (obviamente) apuntando a la primera dirección (en bytes) del espacio reservado.
	int desplazamiento = 0;						// Lo utilizamos para movernos dentro del espacio de memoria asignado.

	memcpy(paquete_serializado + desplazamiento, &(paquete->codigo_operacion), sizeof(int)); // copiamos los datos de un lado al otro
	desplazamiento+= sizeof(int); // nos movemos sizeof(int) bytes (es decir lo que ocupa el tipo de dato de "codigo_operacion")
	memcpy(paquete_serializado + desplazamiento, &(paquete->buffer->size), sizeof(int)); 
	desplazamiento+= sizeof(int); // nos movemos sizeof(int) bytes (es decir lo que ocupa el tipo de dato de "size")
	memcpy(paquete_serializado + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;

	return paquete_serializado;
}

void eliminar_paquete(t_paquete* paquete){
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// RECEPCION DE MENSAJES ////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void recibir_handshake(int fd_cliente, t_log* logger){
    
    int handshake_ok = 0;
    int handshake_error = -1;
    
    int handshake = recibir_operacion(fd_cliente);

    if(handshake == 1){
        send(fd_cliente, &handshake_ok, sizeof(int), 0);
        recibir_presentacion(fd_cliente, logger);
    }
    else{
        send(fd_cliente, &handshake_error, sizeof(int), 0);
        log_error(logger, "No se puedo establecer comuniciacion con el cliente");
    }
}

int recibir_operacion(int fd_cliente){
	int cod_op;
	if(recv(fd_cliente, &cod_op, sizeof(int), MSG_WAITALL) > 0)
		return cod_op;
	else
	{
		close(fd_cliente);
		return -1;
	}
}

void recibir_presentacion(int fd_cliente, t_log* logger){
	int size;
	char* buffer = recibir_buffer(&size, fd_cliente);
	log_info(logger, "Handshake exitoso: Establecida comunicacion con %s\n", buffer);
	free(buffer);
}

void* recibir_buffer(int* size, int socket_cliente){
	void * buffer;

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}



t_buffer *buffer_create(uint32_t size) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = size;
	buffer->offset = 0;		// creo que no va a ser necesario.
	return buffer;
}

// Libera la memoria asociada al buffer
void buffer_destroy(t_buffer *buffer) {
	free(buffer->size);
	free(buffer->stream);
	free(buffer->offset);
	free(buffer);
}

// Agrega un stream al buffer en la posición actual y avanza el offset
void buffer_add(t_buffer *buffer, void *data, uint32_t size) {
	int offset = buffer->size;					// con el offset apuntamos al final del buffer.
	buffer->size += size + sizeof(uint32_t);	// calculamos el nuevo tamaño que buffer tomará.
												// "size" es el tamaño en bytes de "data".
												// "sizeof(uint32_t)" es el tamaño en bytes del "indicador de tamaño" de "data".
												// (esto está explicado en la parte de serializacion de estructuras dinámicas).

	buffer->stream = realloc(buffer->stream, buffer->size); 		// reasignamos espacio de memoria con el nuevo tamaño. Los datos ya existentes se conservan.
	memcpy(buffer->stream + offset, &size, sizeof(uint32_t));		// guardamos en el stream esta especie de mini header de "data" que nos dice su tamaño.
	offset += sizeof(uint32_t)										// movemos el offset los bytes que ocupa el anterior dato.
	memcpy(buffer->stream + offset, data, size);					// guardamos en el stream el "data" en sí.
}

// Guarda "size" bytes del principio del buffer en la dirección "read_data" (dato leido) y avanza el offset
void buffer_read(t_buffer *buffer, void *read_data, uint32_t size) {
	memcpy(read_data, buffer->stream + buffer->offset, size);	//	
	buffer->offset += size;										// avanza el offset la cantidad de bytes leidos.
}

// Agrega un uint32_t al buffer
void buffer_add_uint32(t_buffer *buffer, uint32_t data) {
	buffer_add(buffer, data, sizeof(uint32_t));
}

// Lee un uint32_t del buffer y avanza el offset
uint32_t buffer_read_uint32(t_buffer *buffer);

// Agrega un uint8_t al buffer
void buffer_add_uint8(t_buffer *buffer, uint8_t data) {
	buffer_add(buffer, data, sizeof(uint8_t));
}

// Lee un uint8_t del buffer y avanza el offset
uint8_t buffer_read_uint8(t_buffer *buffer);

// Agrega string al buffer con un uint32_t adelante indicando su longitud
void buffer_add_string(t_buffer *buffer, char *string) {
	buffer_add(buffer, data, strlen(string)+1);
}

// Lee un string y su longitud del buffer y avanza el offset
char *buffer_read_string(t_buffer *buffer, uint32_t *length);
