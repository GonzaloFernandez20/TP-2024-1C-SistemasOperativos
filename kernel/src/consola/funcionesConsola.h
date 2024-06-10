#ifndef FUNCIONES_CONSOLA_H_INCLUDED
#define FUNCIONES_CONSOLA_H_INCLUDED

// ---------- INCLUSIONES
#include <planificacion/pcb.h>
#include <include/estructuras.h>
#include <protocolos/protocoloMemoria.h>
#include <protocolos/protocoloCPU.h>
#include <planificacion/colasEstados.h>
#include <planificacion/PLPyPCP.h>

#include <commons/collections/list.h>

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// ---------- DECLARACIONES DE FUNCIONES
void crear_proceso(char *path_proceso);
void extraer_proceso(int pid);
void imprimir_cola(t_estado *estado);

void _imprimir_estados_procesos(void);
void imprimir_cola_bloqueados(void);
int _asignar_PID(void);
void _mostrar_pcbs(void *pcbDeLista); 
int _esta_ejecutando(int pid_buscado);



#endif