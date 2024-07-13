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

t_dictionary *interfaces_conectadas;
pthread_mutex_t diccionario_interfaces;

t_list* memoria_de_instrucciones;
t_list* tablas_de_paginas;
void* espacio_usuario;
int cantidad_marcos;
t_bitarray* bitarray;

//--semaforos

pthread_mutex_t mutex_log_debug;
pthread_mutex_t mutex_log;
pthread_mutex_t mutex_memoria_instrucciones;
pthread_mutex_t mutex_espacio_usuario;
pthread_mutex_t mutex_tablas_de_paginas;
pthread_mutex_t mutex_bitarray;



#endif