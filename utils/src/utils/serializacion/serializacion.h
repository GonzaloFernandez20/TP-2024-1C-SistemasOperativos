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
	MOTIVO_DESALOJO,
	MENSAJE

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

/**
 * @brief Envía un mensaje de saludo (handshake) a un servidor y espera su respuesta.
 * 
 * Esta función envía un mensaje de saludo al servidor identificado por el descriptor de archivo 'fd_conexion'.
 * Espera la respuesta del servidor y registra el resultado utilizando el logger proporcionado.
 * 
 * @param fd_conexion El file descriptor de la conexión de socket al servidor.
 * @param modulo Un string del nombre del módulo que realiza la solicitud de saludo.
 * @param logger Un puntero al logger
 */
void enviar_handshake(int fd_conexion, char* modulo, t_log* logger);

void enviar_presentacion(char* nombre_modulo, int socket_cliente);

t_paquete* crear_paquete(op_code cop);

void crear_buffer(t_paquete* paquete, int size);

void buffer_add_string(t_buffer *buffer, char *string);

void buffer_add_int(t_buffer *buffer, int dato);

void enviar_paquete(t_paquete* paquete, int socket_cliente);

void* serializar_paquete(t_paquete* paquete, int bytes);

void eliminar_paquete(t_paquete* paquete);

void eliminar_buffer(t_buffer *buffer);

/**
 * @brief Recibe y procesa un mensaje de handshake de un cliente.
 * 
 * Esta función recibe el mensaje de handshake del cliente identificado por el descriptor de archivo 'fd_cliente'.
 * Basado en el mensaje recibido, envía una respuesta al cliente y procesa la presentación del cliente si el handshake es exitoso.
 * Los resultados de la operación se registran utilizando el logger proporcionado.
 * 
 * @param fd_cliente El descriptor de archivo del socket del cliente.
 * @param logger Un puntero al objeto logger utilizado para registrar mensajes.
 * 
 * @note La función asume que `recibir_operacion` y `recibir_presentacion` están definidas en otro lugar y se utilizan para recibir la operación y la presentación del cliente respectivamente.
 * 
 * @note Si el mensaje de handshake indica un éxito (valor 1), se envía una respuesta de handshake exitoso al cliente y se procesa la presentación del cliente utilizando la función `recibir_presentacion`.
 *       Si el mensaje de handshake indica un error (valor distinto de 1), se envía una respuesta de handshake fallido al cliente y se registra un error utilizando el logger.
 */
void eliminar_buffer(t_buffer *buffer);

/**
 * @brief Recibe y procesa un mensaje de handshake de un cliente.
 * 
 * Esta función recibe el mensaje de handshake del cliente identificado por el descriptor de archivo 'fd_cliente'.
 * Basado en el mensaje recibido, envía una respuesta al cliente y procesa la presentación del cliente si el handshake es exitoso.
 * Los resultados de la operación se registran utilizando el logger proporcionado.
 * 
 * @param fd_cliente El descriptor de archivo del socket del cliente.
 * @param logger Un puntero al objeto logger utilizado para registrar mensajes.
 * 
 * @note La función asume que `recibir_operacion` y `recibir_presentacion` están definidas en otro lugar y se utilizan para recibir la operación y la presentación del cliente respectivamente.
 * 
 * @note Si el mensaje de handshake indica un éxito (valor 1), se envía una respuesta de handshake exitoso al cliente y se procesa la presentación del cliente utilizando la función `recibir_presentacion`.
 *       Si el mensaje de handshake indica un error (valor distinto de 1), se envía una respuesta de handshake fallido al cliente y se registra un error utilizando el logger.
 */
void recibir_handshake(int fd_cliente, t_log* logger);

char* recibir_mensaje(int socket_cliente);

/**
 * @brief Recibe el código de operación desde un cliente.
 *
 * Esta función recibe un entero (código de operación) desde un socket
 * conectado al cliente especificado por el descriptor de archivo `fd_cliente`.
 *
 * @param fd_cliente El descriptor de archivo del cliente.
 * @return El código de operación recibido si la operación tiene éxito, -1 si hay un error.
 *
 * @note Esta función utiliza la función recv() de sockets.
 * @note La conexión con el cliente debe estar establecida antes de llamar a esta función.
 */
int recibir_operacion(int fd_cliente);

void recibir_presentacion(int fd_cliente, t_log* logger);

t_buffer* recibir_buffer(int socket_cliente);

int buffer_read_int(void** stream);

char* buffer_read_string(void** stream, int length);


#endif