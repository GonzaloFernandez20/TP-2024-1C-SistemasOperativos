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

// ---- EXTERNAS
#include <utils/hilos/hilos.h>


// ---------------- Definiciones de variables globales
t_log* memoria_log;
t_log* memoria_log_debugg;

char *path_config;
t_config_memoria config_memoria;

int fd_server_memoria;
int fd_kernel;
int fd_cpu;
int fd_IO;


t_list* memoria_de_instrucciones;

//--semaforos

pthread_mutex_t mutex_log_debug;
pthread_mutex_t mutex_log;
pthread_mutex_t mutex_memoria_instrucciones;



#endif