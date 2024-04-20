#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED


// ---------------- Librerias Standard
#include <stdlib.h>
#include <stdio.h>
#include <commons/config.h>

#include <string.h>


// ---------------- Librerias Propias

#include <include/estructuras.h>
#include <conexiones/conexiones.h>

#include <utils/hilos/hilos.h>




// ---------------- Definiciones de variables globales
t_log* memoria_log;
t_log* memoria_log_debugg;
char *path_config;
int fd_server_memoria;
t_config_memoria config_memoria;

pthread_t thread_cpu;
pthread_t thread_kernel;

// ---------------- Definiciones de funciones



void liberar_memoria(void);

#endif