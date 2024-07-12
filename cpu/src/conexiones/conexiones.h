#ifndef CPU_CONEXIONES_H_
#define CPU_CONEXIONES_H_

#include <include/estructuras.h>
#include <inicializar/inicializar.h> 
#include <protocolos/protocoloDispatch.h>
#include <protocolos/protocoloInterrupt.h>
#include <protocolos/protocoloMemoria.h>

#include <utils/hilos/hilos.h>
#include <utils/conexiones/conexiones.h>
#include <utils/serializacion/serializacion.h>


void iniciar_servidores(void);

void iniciar_servidor_dispatch(void);

void iniciar_servidor_interrupt(void);

void conectar_memoria(void);

void enviar_handshake_de_CPU(int fd_conexion, char* modulo, t_log* logger);

void gestionar_conexiones_kernel(void);

void atender_dispatch(void);

void atender_interrupt(void);


#endif