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


// arma el paquete (inicializa sus estructuras) con la informacion de una presentacion y lo envia
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

	t_paquete* paquete = crear_paquete(HANDSHAKE);

	int buffer_size = strlen(nombre_modulo) + 1 + sizeof(int); 
	crear_buffer(paquete, buffer_size);


	buffer_add_int(paquete->buffer, strlen(nombre_modulo) + 1 );
	buffer_add_string(paquete->buffer, nombre_modulo);

	enviar_paquete(paquete, socket_cliente);
	eliminar_paquete(paquete);
} 

// crea un paquete ya indicando su codigo de operacion
t_paquete* crear_paquete(op_code cop){
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = cop;
	return paquete;
}

// Crea un buffer vacío de tamaño size con el offset en 0
void crear_buffer(t_paquete* paquete, int size){
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = size;
	paquete->buffer->offset = 0;
	paquete->buffer->stream = malloc(paquete->buffer->size);
}

void buffer_add_string(t_buffer *buffer, char *string){
	memcpy(buffer->stream + buffer->offset, string, strlen(string)+1);
	buffer->offset +=  strlen(string)+1;
}

void buffer_add_int(t_buffer *buffer, int dato){
	memcpy(buffer->stream + buffer->offset, &dato, sizeof(int));
	buffer->offset +=  sizeof(int);
}


// envia el paquete serializado
void enviar_paquete(t_paquete* paquete, int socket_cliente){
	int bytes = paquete->buffer->size + 2*sizeof(int); 
			// paquete->buffer->size son los bytes del stream
			// 2*sizeof(int) un int es del op_code y el otro del tamanio del buffer

	void* a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
}


void* serializar_paquete(t_paquete* paquete, int bytes){
	void * a_enviar = malloc(bytes);
	int desplazamiento = 0;

	memcpy(a_enviar + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(a_enviar + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(a_enviar + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;

	return a_enviar;

		/**
	 * Quedaría entonces un paquete que sigue la estructura siguiente:
	 * 1) codigo de operacion (paquete) 
	 * 2) tamaño de buffer (para hacer un malloc) 
	 * 3) stream de buffer (los datos en sí)
	 * 
	*/ 
}


void eliminar_paquete(t_paquete* paquete){
	eliminar_buffer(paquete->buffer);
	free(paquete);
}

void eliminar_buffer(t_buffer *buffer){
	free(buffer->stream);
	free(buffer);
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


// char* recibir_mensaje(int socket_cliente)
// {
// 	int size;
// 	char* buffer = recibir_buffer(&size, socket_cliente);  
	
// 	char* mensaje;
// 	strcpy(mensaje, buffer); // mensaje no requiere ser liberado con free()
// 	free(buffer);
	
// 	return mensaje; // devolvemos un literal string así no se necesita liberar nada afuera de esta función
// }

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

	t_buffer *buffer = recibir_buffer(fd_cliente);
	void* stream = buffer->stream;
	
	int length = buffer_read_int(&stream);
	char* modulo = malloc(length);
	strcpy(modulo, buffer_read_string(&stream, length));

	log_info(logger, "Handshake exitoso: Establecida comunicacion con %s\n", modulo);
	
	free(modulo);
	eliminar_buffer(buffer);
}

t_buffer* recibir_buffer(int socket_cliente){
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->offset = 0;
	recv(socket_cliente, &buffer->size, sizeof(int), MSG_WAITALL);
	buffer->stream = malloc(buffer->size);
	recv(socket_cliente, buffer->stream, buffer->size, MSG_WAITALL);

	return buffer;
}

int buffer_read_int(void** stream){
	int dato;
	memcpy(&dato, *stream, sizeof(int));
    *stream += sizeof(int);

	return dato;
}
char* buffer_read_string(void** stream, int length){
	char* string = malloc(length);
	memcpy(string, *stream, length);
	*stream += length;

	return string;
}









/* FUNCIONES DEL BUFFER PARA GUARDAR ESTRUCTURAS DINÁMICAS*/

// t_buffer *buffer_create() {
// 	t_buffer* buffer = malloc(sizeof(t_buffer));
// 	buffer->size = 0;
// 	buffer->offset = 0;		
// 	return buffer;
// }

// // Libera la memoria asociada al buffer
// void buffer_destroy(t_buffer *buffer) {
// 	// free(buffer->size);
// 	// free(buffer->offset);		// creo que no es necesario para estos xq no son punteros
// 	free(buffer->stream);
// 	free(buffer);
// }

// // Agrega un stream al buffer en la posición actual y avanza el offset
// void buffer_add(t_buffer *buffer, void *data, uint32_t size) {
// 	int offset = buffer->size;					// con el offset apuntamos al final del buffer.
// 	buffer->size += size + sizeof(uint32_t);	// calculamos el nuevo tamaño que buffer tomará.
// 												// "size" es el tamaño en bytes de "data".
// 												// "sizeof(uint32_t)" es el tamaño en bytes del "indicador de tamaño" de "data".
// 												// (esto está explicado en la parte de serializacion de estructuras dinámicas).

// 	buffer->stream = realloc(buffer->stream, buffer->size); 		// reasignamos espacio de memoria con el nuevo tamaño. Los datos ya existentes se conservan.
// 	memcpy(buffer->stream + offset, &size, sizeof(uint32_t));		// guardamos en el stream esta especie de mini header de "data" que nos dice su tamaño.
// 	offset += sizeof(uint32_t);										// movemos el offset los bytes que ocupa el anterior dato.
// 	memcpy(buffer->stream + offset, data, size);					// guardamos en el stream el "data" en sí.
// }

// // Guarda "size" bytes del principio del buffer en la dirección "read_data" (dato leido) y avanza el offset
// void buffer_read(t_buffer *buffer, void *read_data) {
// 	uint32_t size;
// 	memcpy(&size, buffer->stream + buffer->offset, sizeof(uint32_t));
// 	memcpy(read_data, buffer->stream + buffer->offset + sizeof(uint32_t), size);		

// 	buffer->offset += sizeof(uint32_t) + size;										// avanza el offset la cantidad de bytes leidos.
// }


/**
 * 
 * IGNORAR POR AHORA, USEN LAS DE ARRIBA.

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
uint8_t buffer_read_uint8(t_buffer *buffer) {
	buffer_read(t_buffer *buffer, void *read_data)
}

// Agrega string al buffer con un uint32_t adelante indicando su longitud
void buffer_add_string(t_buffer *buffer, char *string) {
	buffer_add(buffer, string, strlen(string)+1);
}

// Lee un string y su longitud del buffer y avanza el offset
char *buffer_read_string(t_buffer *buffer, uint32_t *length);

*/
