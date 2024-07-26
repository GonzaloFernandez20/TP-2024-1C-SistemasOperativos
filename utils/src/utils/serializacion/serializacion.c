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
void enviar_presentacion(char* nombre_modulo, int socket_cliente){
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

void buffer_add_valor(t_buffer *buffer, void* valor, int bytes){
	memcpy(buffer->stream + buffer->offset, valor, bytes);
	buffer->offset +=  bytes;
}

void buffer_add_int(t_buffer *buffer, int dato){
	memcpy(buffer->stream + buffer->offset, &dato, sizeof(int));
	buffer->offset +=  sizeof(int);
}

void buffer_add_uint8(t_buffer *buffer, uint8_t dato){
	memcpy(buffer->stream + buffer->offset, &dato, sizeof(uint8_t));
	buffer->offset +=  sizeof(uint8_t);
}

void buffer_add_uint32(t_buffer *buffer, uint32_t dato){
	memcpy(buffer->stream + buffer->offset, &dato, sizeof(uint32_t));
	buffer->offset +=  sizeof(uint32_t);
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
	char* modulo = buffer_read_string(&stream, length);

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

uint8_t buffer_read_uint8(void** stream){
	uint8_t dato;
	memcpy(&dato, *stream, sizeof(uint8_t));
    *stream += sizeof(uint8_t);

	return dato;
}

uint32_t buffer_read_uint32(void** stream){
	uint32_t dato;
	memcpy(&dato, *stream, sizeof(uint32_t));
    *stream += sizeof(uint32_t);

	return dato;
}

char* buffer_read_string(void** stream, int length){
	char* string = malloc(length);
	memcpy(string, *stream, length);
	*stream += length;

	return string;
}

void* buffer_read_valor(void** stream, int bytes){
	void* valor = malloc(bytes);
	memcpy(valor, *stream, bytes);
	*stream += bytes;

	return valor;
}