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
void imprimir_cola(t_estado *estado, void(*_mostrar_pcbs)(void*));
void trasladar(int pid_buscado,  t_estado *origen, t_estado *destino);


t_pcb *pop_estado(t_estado* estado);
void _loggear_ingreso_ready(); 

char* __armar_lista_pids();

// ---------- VARIABLES GLOBALES
extern t_estado *array_estados[];

#endif