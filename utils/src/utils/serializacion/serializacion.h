#ifndef UTILS_SERIALIZACION_H_
#define UTILS_SERIALIZACION_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <netdb.h>
#include <unistd.h>
//#include <errno.h>
#include <sys/socket.h>
#include <commons/log.h>


// Protocolos de comunicacion
typedef enum
{
	HANDSHAKE = 1
	MENSAJE

}op_code; 


typedef struct
{
	int size;
	void* stream;
} t_buffer;

typedef struct
{
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;


void enviar_handshake(int fd_conexion, char* modulo, t_log* logger);

void enviar_presentacion(char* nombre_modulo, int socket_cliente);

void* serializar_paquete(t_paquete* paquete, int bytes);

void eliminar_paquete(t_paquete* paquete);

void recibir_handshake(int fd_cliente, t_log* logger);

int recibir_operacion(int fd_cliente);

void recibir_presentacion(int fd_cliente, t_log* logger);

void* recibir_buffer(int* size, int socket_cliente);

#endif