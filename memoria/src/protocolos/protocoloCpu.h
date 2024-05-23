#ifndef MEMORIA_PROTOCOLO_CPU_H_
#define MEMORIA_PROTOCOLO_CPU_H_

#include <include/estructuras.h>
#include <esquemaEstructuras/memoriaInstrucciones.h>

#include <utils/serializacion/serializacion.h>
#include <utils/hilos/hilos.h>

void *procesar_operacion_cpu(void *fd_cpu_casteado);

void devolver_instruccion(void);

void enviar_instruccion_serializada(char* instruccion);

#endif