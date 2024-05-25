#ifndef CPU_PROTOCOLO_DISPATCH_H_
#define CPU_PROTOCOLO_DISPATCH_H_

#include <include/estructuras.h>

#include <utils/serializacion/serializacion.h>
#include <utils/hilos/hilos.h>

void *procesar_operacion_dispatch(void *fd_dispatch_casteado);

#endif 