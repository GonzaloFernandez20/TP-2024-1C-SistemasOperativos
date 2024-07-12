#ifndef MEMORIA_CONEXIONES_H_
#define MEMORIA_CONEXIONES_H_

#include <include/estructuras.h>
#include <inicializar/inicializar.h> 
#include <protocolos/protocoloCpu.h>
#include <protocolos/protocoloKernel.h>
#include <protocolos/protocoloIO.h>

#include <utils/hilos/hilos.h>
#include <utils/conexiones/conexiones.h>
#include <utils/serializacion/serializacion.h>


void iniciar_servidor_memoria(void);

void gestionar_conexiones_clientes(void);

void atender_cpu(void);
void recibir_handshake_cpu(int fd_cliente, t_log* logger);

void atender_kernel(void);

void atender_entradaSalida(void);

void atender_interfaz(void (*procesar_conexion)(void*));

void recibir_handshake_IO(int fd_cliente);

void recibir_presentacion_IO(int fd_cliente);

#endif