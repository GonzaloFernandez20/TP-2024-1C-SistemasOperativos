#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

// ---------------- Librerias Standard
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <commons/config.h>
#include <stdint.h>
#include <pthread.h>

// ---------------- Librerias Propias
// ---- INTERNAS
#include <include/estructuras.h>
#include <conexiones/conexiones.h> 
#include <ciclo/ciclo.h>
// ---- EXTERNAS
#include <utils/hilos/hilos.h>

// ---------------- Definiciones de variables globales
t_log* cpu_log;
t_log* cpu_log_debug;

// SEMAFOROS
pthread_mutex_t mutex_log;
pthread_mutex_t mutex_log_debug;
pthread_mutex_t mutex_hay_interrupcion;
pthread_mutex_t mutex_lista_interrupciones;

char *path_config;
t_config_cpu config_cpu;

int fd_dispatch_server;
int fd_interrupt_server;
int fd_conexion_memoria; 
int fd_dispatch;
int fd_interrupt;

int hay_interrupcion;

t_registros_cpu registros;

op_code_instruccion instruccion;

int MAX_ENTRADAS;
char* ALGORITMO_TLB;
t_list* tabla_tlb;  //guardará un array de t_entrada_tlb

int TAM_PAGINA;     // se debería recibir el tamaño de página al establecer las conexiones con Memoria.

char** instruccion_ejecutando; // array de strings. Ej: {"SUM", "AX", "BX"}
t_dictionary* opCodes_diccionario;
t_dictionary* registros_diccionario;

int se_devolvio_contexto;

t_list *direcciones;
t_list* lista_interrupciones;

int PID;

#endif