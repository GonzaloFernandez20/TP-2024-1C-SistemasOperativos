#ifndef PLANIFICADOR_H_INCLUDED
#define PLANIFICADOR_H_INCLUDED

// ---------------- LIBRERIAS 
#include <planificacion/pcb.h>
#include <commons/collections/list.h>
#include <commons/collections/queue.h>

#include <stdio.h>



// ---------- ESTADOS DE UN PROCESO

extern t_list *new;
extern t_list *ready;
extern t_list *exec;
extern t_list *estado_exit;
extern t_list *blocked;

extern t_list *array_colas[];

// ---------- DECLARACIONES DE FUNCIONES

void* crear_proceso(char *path_proceso);
void extraer_proceso(int pid);

void iniciar_colas_planificacion(void);
void imprimir_cola(t_list *cola, void(*_mostrar_pcbs)(void*));

int _asignar_PID(void);
void _mostrar_pcbs(void *pcbDeLista); 
int _buscar_y_eliminar_pid(t_list *cola, int pid_buscado);


// ---------- DECLARACIONES DE ESTRUCTURAS

#endif

