#ifndef MEMORIA_PROTOCOLO_CPU_H_
#define MEMORIA_PROTOCOLO_CPU_H_

#include <include/estructuras.h>

#include <utils/serializacion/serializacion.h>
#include <utils/hilos/hilos.h>

void *procesar_operacion_cpu(void *fd_cpu_casteado);

#endif