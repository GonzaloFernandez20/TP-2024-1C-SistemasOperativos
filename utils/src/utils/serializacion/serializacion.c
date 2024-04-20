#include <utils/serializacion/serializacion.h>

///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// ENVIO DE MENSAJES //////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void enviar_handshake(int fd_conexion, char* modulo, t_log* logger){
    
    int respuesta_servidor;

    enviar_presentacion(modulo,fd_conexion);
    recv(fd_conexion, &respuesta_servidor, sizeof(int), MSG_WAITALL);

    if (respuesta_servidor == 0) {
        log_info(logger, "Handshake exitoso: comunicacion con servidor establecida");
    } 
    else {
        log_error(logger, "Error handshake: no se establecio comunicacion con servidor");
    }

}

void enviar_presentacion(char* nombre_modulo, int socket_cliente){
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
	void * magic = malloc(bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;

	return magic;
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
	log_info(logger, "Establecida comunicacion con %s", buffer);
	free(buffer);
}

void* recibir_buffer(int* size, int socket_cliente){
	void * buffer;

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}


