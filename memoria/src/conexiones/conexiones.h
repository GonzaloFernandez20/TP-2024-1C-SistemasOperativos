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

void atender_kernel(void);

void atender_entradaSalida(void);

#endif