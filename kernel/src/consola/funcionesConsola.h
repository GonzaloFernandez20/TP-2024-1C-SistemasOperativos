#ifndef FUNCIONES_CONSOLA_H_INCLUDED
#define FUNCIONES_CONSOLA_H_INCLUDED

// ---------- INCLUSIONES
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
void crear_proceso(char *path_proceso);
void extraer_proceso(int pid);
void _imprimir_estados_procesos(void);

int _asignar_PID(void);
void _mostrar_pcbs(void *pcbDeLista); 
// ---------- VARIABLES GLOBALES


#endif