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
#include <protocolos/protocoloKernel.h>


// ---------------- Definiciones de variables globales
t_log* IO_log;
t_log* IO_log_debug;

char* path_config;
char* nombre_Interfaz;//no entiendo el uso
t_config_IO config_IO;

int fd_conexion_memoria; 
int fd_conexion_kernel; 

pthread_mutex_t mutex_log_debug;
pthread_mutex_t mutex_log;

#endif