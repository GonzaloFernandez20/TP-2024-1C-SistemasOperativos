#ifndef CPU_PROTOCOLO_INTERRUPT_H_
#define CPU_PROTOCOLO_INTERRUPT_H_

#include <include/estructuras.h>

#include <utils/serializacion/serializacion.h>
#include <utils/hilos/hilos.h>
#include <include/ciclo.h>

// Variables globales
uint32_t PID_a_interrumpir;      // según consigna: Identificador del proceso (deberá ser un número entero, único en todo el sistema).
                                        // tomará el valor de 0 si Kernel no pidió interrumpir ningun proceso.
pthread_mutex_t mutex;



// Traemos estas variables globales de ciclo.h
// extern struct_PCB PCB;

void *procesar_operacion_interrupt(void *fd_interrupt_casteado);
int recibir_PID();

#endif
