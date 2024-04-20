#ifndef KERNEL_PROTOCOLO_H_
#define KERNEL_PROTOCOLO_H_

#include <include/estructuras.h>

#include <utils/hilos/hilos.h>
#include <utils/serializacion/serializacion.h>

void *procesar_conexion_es(void *fd_cpu_casteado);

#endif