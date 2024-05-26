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

int sistema_encendido;

char *path_config;
t_config_cpu config_cpu;

int fd_dispatch_server;
int fd_interrupt_server;
int fd_conexion_memoria; 
int fd_dispatch;
int fd_interrupt;

int hay_interrupcion;
int tipo_interrupcion;


pthread_mutex_t mutex_dispatch;//locked al inicializarlo
pthread_mutex_t mutex_ciclo;//locked al inicializarlo 
pthread_mutex_t mutexInterrupt;


// SEMAFOROS
pthread_mutex_t mutex_log;
pthread_mutex_t mutex_log_debug;

t_pcb PCB;

t_estado_cpu registrosCPU;   
op_code_instruccion instruccion;


char** instruccion_cpu; // array de strings. Ej: {"SUM", "AX", "BX"}
t_dictionary* opCodes_diccionario;
t_dictionary* registros_diccionario;

int salir_ciclo;


#endif