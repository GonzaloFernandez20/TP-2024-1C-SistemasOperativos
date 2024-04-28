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

void enviar_op_code_y_datos(int op_code,char* info, int socket){
	//revisar char* info, me parece que sería más útil 
	//como un void* por si querés pasar un struct con varios datos
	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = op_code;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = strlen(info) + 1;
	paquete->buffer->stream = malloc(paquete->buffer->size);
	memcpy(paquete->buffer->stream, info, paquete->buffer->size);

	int bytes = paquete->buffer->size + 2*sizeof(int);

	void* a_enviar = serializar_paquete(paquete, bytes);

	send(socket, a_enviar, bytes, 0);

	free(a_enviar);
	eliminar_paquete(paquete);
}


void enviar_presentacion(char* nombre_modulo, int socket){
	enviar_op_code_y_datos(HANDSHAKE,nombre_modulo,socket);
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

void recibir_handshake(int fd_remitente, t_log* logger){
   
    int handshake_ok = 0;
    int handshake_error = -1;
    
    int handshake = recibir_operacion(fd_remitente);

    if(handshake == 1){
        send(fd_remitente, &handshake_ok, sizeof(int), 0);
        recibir_presentacion(fd_remitente, logger);
    }
    else{
        send(fd_remitente, &handshake_error, sizeof(int), 0);
        log_error(logger, "No se puedo establecer comuniciacion con el cliente");
    }
}

int recibir_operacion(int fd_remitente){
	int cod_op;
	if(recv(fd_remitente, &cod_op, sizeof(int), MSG_WAITALL) > 0)
		return cod_op;
	else
	{
		close(fd_remitente);
		return -1;
	}
}

void recibir_presentacion(int fd_remitente, t_log* logger){
	int size;
	char* buffer = recibir_buffer(&size, fd_remitente);
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

t_list* recibir_paquete(int socket_cliente)
{
	int size;
	int desplazamiento = 0;
	void * buffer;
	t_list* valores = list_create();//liberar en algún lado
	int tamanio;

	buffer = recibir_buffer(&size, socket_cliente);
	while(desplazamiento < size)
	{
		memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
		desplazamiento+=sizeof(int);
		char* valor = malloc(tamanio);
		memcpy(valor, buffer+desplazamiento, tamanio);
		desplazamiento+=tamanio;
		list_add(valores, valor);
	}
	free(buffer);
	return valores;
}

