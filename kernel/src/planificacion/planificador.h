#ifndef PLANIFICADOR_H_INCLUDED
#define PLANIFICADOR_H_INCLUDED

// ---------------- LIBRERIAS 
#include <planificacion/pcb.h>
#include <include/estructuras.h>
#include <protocolos/protocoloMemoria.h>
#include <protocolos/protocoloCPU.h>


#include <commons/collections/list.h>

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

extern t_list *array_colas[];

// ---------- DECLARACIONES DE FUNCIONES

void *planificador_corto_plazo(void);

void* crear_proceso(void *path_proceso_void);
void extraer_proceso(int pid);

void iniciar_colas_planificacion(void);
void imprimir_cola(t_list *cola, void(*_mostrar_pcbs)(void*));
t_pcb *pop_estado(t_list* lista);
void trasladar(int pid_buscado,  t_list *cola_origen, t_list *cola_destino);
void inicializar_semaforos(void);
void iniciar_planificacion(void);

int _asignar_PID(void);
void _mostrar_pcbs(void *pcbDeLista); 
void _enviar_ready(t_pcb *pcb);

#endif

