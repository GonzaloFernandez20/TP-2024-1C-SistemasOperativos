#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

// ---------------- Librerias Standard
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <commons/config.h>

// ---------------- Librerias Propias
// ---- INTERNAS
#include <include/estructuras.h>
#include <conexiones/conexiones.h> 
#include <include/ciclo.h>

// ---- EXTERNAS
#include <utils/hilos/hilos.h>

// ---------------- Definiciones de variables globales
t_log* cpu_log;         // Ojito que parece que en todo el módulo CPU no parece que se haya usado el cpu_log. 
t_log* cpu_log_debug;   // Considerar juntar cpu_log con cpu_log_debug.

char *path_config;
t_config_cpu config_cpu;

// int fd_dispatch_server;
int fd_interrupt_server;
int fd_conexion_memoria;

// int dispatch_conectado;
int interrupt_conectado;










//Variables globales de ciclo.c
struct_PCB PCB;
struct_registros registrosCPU;   
char** instruccion_cpu; // array de strings. Ej: {"SUM", "AX", "BX"}
t_dictionary* opCodes_diccionario;
t_dictionary* registros_diccionario;
uint32_t PID_a_interrumpir;      // según consigna: Identificador del proceso (deberá ser un número entero, único en todo el sistema).
                                        // tomará el valor de 0 si Kernel no pidió interrumpir ningun proceso.
pthread_mutex_t mutexInterrupt;

// ---------------- Definiciones de funciones
void liberar_cpu(void);

#endif