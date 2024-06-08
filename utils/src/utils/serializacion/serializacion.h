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
typedef enum CODIGOS_DE_OPERACION
{
	HANDSHAKE = 1,
	CREAR_PROCESO,
	ELIMINAR_PROCESO,
	OBTENER_INSTRUCCION,
	INSTRUCCION,            
	CONTEXTO_EJECUCION,
	OPERACION_COMPLETADA,
    CAMBIO_TAMANIO,         
    ESTADO,                 // Respuesta a una solicitud. Estos paquetes informarán si hubo un error en la solicitud o si fue exitoso o sobre cualquier otro estado.
	MENSAJE,
    WRITE_DIRECCION_FISICA,
    READ_DIRECCION_FISICA,
    SUCCESFULL_READ,
    SUCCESFULL_WRITE,
    FAILED_WRITE,
    FAILED_READ
}op_code; 


typedef struct
{
	int size;
	int offset;
	void* stream;
} t_buffer;

typedef struct
{
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;


typedef enum motivo_desalojo{
    FIN_DE_QUANTUM,
    INTERRUPCION,
    LLAMADA_IO,
    OUT_OF_MEMORY,
    EXIT
}t_motivo_desalojo;

typedef enum code_instruccion{
    SET,
    MOV_IN,
    MOV_OUT, 
    SUM,
    SUB,
    JNZ,
    RESIZE,
    COPY_STRING,
    WAIT,
    SIGNAL,
    IO_GEN_SLEEP,
    IO_STDIN_READ,
    IO_STDOUT_WRITE,
    IO_FS_CREATE,
    IO_FS_DELETE,
    IO_FS_TRUNCATE, 
    IO_FS_WRITE,
    IO_FS_READ,
    EXIT_OS,
    X
} op_code_instruccion;

void enviar_handshake(int fd_conexion, char* modulo, t_log* logger);

void enviar_presentacion(char* nombre_modulo, int socket_cliente);

t_paquete* crear_paquete(op_code cop);

void crear_buffer(t_paquete* paquete, int size);

void buffer_add_string(t_buffer *buffer, char *string);

void buffer_add_int(t_buffer *buffer, int dato);

void buffer_add_uint8(t_buffer *buffer, uint8_t dato);

void buffer_add_uint32(t_buffer *buffer, uint32_t dato);

void enviar_paquete(t_paquete* paquete, int socket_cliente);

void* serializar_paquete(t_paquete* paquete, int bytes);

void eliminar_paquete(t_paquete* paquete);

void eliminar_buffer(t_buffer *buffer);

void recibir_handshake(int fd_cliente, t_log* logger);

int recibir_operacion(int fd_cliente);

void recibir_presentacion(int fd_cliente, t_log* logger);

t_buffer* recibir_buffer(int socket_cliente);

int buffer_read_int(void** stream);

uint8_t buffer_read_uint8(void** stream);

uint32_t buffer_read_uint32(void** stream);

char* buffer_read_string(void** stream, int length);


#endif