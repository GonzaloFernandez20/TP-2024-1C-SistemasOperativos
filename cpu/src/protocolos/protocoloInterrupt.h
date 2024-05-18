#ifndef CPU_PROTOCOLO_INTERRUPT_H_
#define CPU_PROTOCOLO_INTERRUPT_H_

#include <include/estructuras.h>

#include <utils/serializacion/serializacion.h>
#include <utils/hilos/hilos.h>


void *procesar_operacion_interrupt(void *fd_interrupt_casteado);

#endif