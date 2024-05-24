#ifndef KERNEL_CONEXIONES_H_
#define KERNEL_CONEXIONES_H_

#include <include/estructuras.h>
#include <inicializar/inicializar.h> 
#include <protocolos/protocoloES.h>

#include <utils/hilos/hilos.h>
#include <utils/conexiones/conexiones.h>
#include <utils/serializacion/serializacion.h>


void iniciar_servidor_kernel(void);

void establecer_conexiones(void);

void conectar_memoria(void);

void conectar_dispatch(void);

void conectar_interrupt(void);

void atender_entradasalida(void);

void atender_interfaz(void (*procesar_conexion)(void*));

void recibir_handshake_IO(int fd_cliente);

void recibir_presentacion_IO(int fd_cliente);

#endif