#ifndef PLANIFICADOR_H_INCLUDED
#define PLANIFICADOR_H_INCLUDED

// ---------------- LIBRERIAS 
#include <planificacion/pcb.h>
#include <include/estructuras.h>
#include <protocolos/protocoloMemoria.h>
#include <protocolos/protocoloCPU.h>
#include <planificacion/colasEstados.h>

#include <commons/collections/list.h>

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>



// ---------- DECLARACIONES DE FUNCIONES

void *planificador_corto_plazo(void);

void* crear_proceso(void *path_proceso_void);
void extraer_proceso(int pid);

void inicializar_semaforos(void);
void iniciar_planificacion(void);

int _asignar_PID(void);



#endif

