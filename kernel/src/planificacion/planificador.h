#ifndef PLANIFICADOR_H_INCLUDED
#define PLANIFICADOR_H_INCLUDED

// ---------------- LIBRERIAS 
#include <planificacion/pcb.h>
#include <commons/collections/list.h>

#include <stdio.h>



// ---------- ESTADOS DE UN PROCESO

extern t_list *new;
extern t_list *ready;
extern t_list *exec;
extern t_list *estado_exit;
extern t_list *blocked;

// ---------- DECLARACIONES DE FUNCIONES

void* crear_proceso(char *path_proceso);
int _asignar_PID(void);
void iniciar_colas_planificacion(void);
void _mostrar_pcbs(void *pcbDeLista); 

#endif

