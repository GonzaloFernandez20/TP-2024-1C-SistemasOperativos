#ifndef ESTADOS_H_INCLUDED
#define ESTADOS_H_INCLUDED

// ---------- INCLUSIONES
#include <commons/collections/list.h>
#include <include/estructuras.h>
#include <planificacion/pcb.h>

#include <pthread.h>
#include <semaphore.h>


// ---------- DECLARACIONES DE FUNCIONES
void iniciar_colas_planificacion(void);
void imprimir_cola(t_list *cola, void(*_mostrar_pcbs)(void*));
void trasladar(int pid_buscado,  t_list *cola_origen, t_list *cola_destino);


t_pcb *pop_estado(t_list* lista);
void _enviar_ready(t_pcb *pcb);
void _mostrar_pcbs(void *pcbDeLista); 


// ---------- VARIABLES GLOBALES
extern t_list *array_colas[];


#endif