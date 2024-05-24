#ifndef PLANIFICADOR_H_INCLUDED
#define PLANIFICADOR_H_INCLUDED

// ---------------- LIBRERIAS 
#include <planificacion/pcb.h>
#include <include/estructuras.h>
#include <protocolos/protocoloMemoria.h>
#include <protocolos/protocoloCPU.h>
#include <planificacion/colasEstados.h>
#include <consola/funcionesConsola.h>

#include <commons/collections/list.h>

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>



// ---------- DECLARACIONES DE FUNCIONES

void *planificador_corto_plazo(void);
void *planificador_largo_plazo(void);

void inicializar_semaforos(void);
void iniciar_planificacion(void);




#endif

