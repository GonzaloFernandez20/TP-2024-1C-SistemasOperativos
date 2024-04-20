#ifndef MEMORIA_PROTOCOLO_KERNEL_H_
#define MEMORIA_PROTOCOLO_KERNEL_H_

#include <include/estructuras.h>

#include <utils/serializacion/serializacion.h>
#include <utils/hilos/hilos.h>

void *procesar_operacion_kernel(void *fd_kernel_casteado);

#endif