#ifndef CPU_PROTOCOLO_INTERRUPT_H_
#define CPU_PROTOCOLO_INTERRUPT_H_

#include <include/estructuras.h>

#include <utils/serializacion/serializacion.h>
#include <utils/hilos/hilos.h>
#include <include/ciclo.h>


void *procesar_operacion_interrupt(void *fd_interrupt_casteado);
int recibir_PID(int fd_interrupt);

#endif
