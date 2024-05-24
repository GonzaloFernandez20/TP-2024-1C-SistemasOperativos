#ifndef IO_CONEXIONES_H_
#define IO_CONEXIONES_H_

#include <include/estructuras.h>
#include <inicializar/inicializar.h> 

#include <utils/conexiones/conexiones.h>
#include <utils/serializacion/serializacion.h>

void establecer_conexiones(void);

void conectar_memoria(void);

void conectar_kernel(void);

void enviar_handshake_IO(int fd_conexion, char* nombre_Interfaz, char* tipo_Interfaz);

void enviar_presentacion_IO(char* nombre_Interfaz, char* tipo_Interfaz, int socket_cliente);

#endif