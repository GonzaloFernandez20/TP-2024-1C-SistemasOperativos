#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED


// ---------------- Librerias Standard
#include <stdlib.h>
#include <stdio.h>

#include <commons/config.h>
#include <commons/log.h>
#include <string.h>
#include <pthread.h>
// ---------------- Librerias Propias
// ---- INTERNAS
#include <include/estructuras.h>
#include <inicializar/inicializar.h>
#include <conexiones/conexiones.h>
#include <protocolos/protocoloES.h>
#include <consola/consola.h>
#include <planificacion/colasEstados.h>


// ---- EXTERNAS
#include <utils/inicializaciones/inicializar.h>

// ---------------- Definiciones de variables globales

t_log* kernel_log;
t_log* kernel_log_debugg;

char *path_config;
t_config_kernel config_kernel;

int fd_server_kernel;
int fd_conexion_memoria;
int fd_conexion_dispatch; 
int fd_conexion_interrupt;

t_dictionary* interfaces_conectadas;
t_dictionary* instrucciones_por_interfaz;
t_dictionary* peticiones_interfaz;

pthread_t manejo_quantum;
int termino_quantum;

t_estado *new;
t_estado *ready;
t_estado *ready_plus;
t_estado *exec;
t_estado *blocked;
t_estado *estado_exit;

pthread_mutex_t mutex_log_debug;
pthread_mutex_t mutex_log;
pthread_mutex_t diccionario_interfaces;
pthread_mutex_t diccionario_peticiones;

sem_t execute_libre;
sem_t proceso_listo;
sem_t proceso_cargado;
sem_t grado_multiprogramacion;
sem_t hay_proceso_exit;
sem_t planificacion_en_pausa;

int planificacion_pausada;
int contador_bloqueados;

#endif