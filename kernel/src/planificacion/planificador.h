#ifndef PLANIFICADOR_H_INCLUDED
#define PLANIFICADOR_H_INCLUDED

// ---------------- LIBRERIAS 
#include <planificacion/pcb.h>
//#include <protocolos/protocoloMemoria.h>
#include <include/estructuras.h>


#include <commons/collections/list.h>

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

extern t_list *array_colas[];

// ---------- DECLARACIONES DE FUNCIONES

void* crear_proceso(void *path_proceso_void);
void extraer_proceso(int pid);

void iniciar_colas_planificacion(void);
void imprimir_cola(t_list *cola, void(*_mostrar_pcbs)(void*));

int _asignar_PID(void);
void _mostrar_pcbs(void *pcbDeLista); 
void trasladar(int pid_buscado,  t_list *cola_origen, t_list *cola_destino);


// ---------- DECLARACIONES DE ESTRUCTURAS

#endif

