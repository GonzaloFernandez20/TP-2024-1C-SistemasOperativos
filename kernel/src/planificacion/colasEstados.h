#ifndef ESTADOS_H_INCLUDED
#define ESTADOS_H_INCLUDED

// ---------- INCLUSIONES
#include <commons/collections/list.h>
#include <commons/string.h>
#include <include/estructuras.h>
#include <planificacion/pcb.h>
#include <consola/funcionesConsola.h>

#include <pthread.h>
#include <semaphore.h>


// ---------- DECLARACIONES DE FUNCIONES
void iniciar_colas_planificacion(void);
void trasladar(int pid_buscado,  t_estado *origen, t_estado *destino);
int algoritmo_es_VRR();

t_pcb *pop_estado(t_estado* estado);
void push_estado(t_estado* estado, t_pcb* pcb);
void _loggear_ingreso_ready(t_estado* estado); 
void *limpieza_cola_exit();

char* __armar_lista_pids(t_estado* estado);

// ---------- VARIABLES GLOBALES
extern t_estado *array_estados[];

#endif