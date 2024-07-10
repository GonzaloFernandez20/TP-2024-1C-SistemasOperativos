#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED


// ---------------- Librerias Standard
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <commons/config.h>
#include <commons/string.h>

// ---------------- Librerias Propias
// ---- INTERNAS
#include <include/estructuras.h>
#include <conexiones/conexiones.h> 
#include <protocolos/protocoloKernel.h>


// ---------------- Definiciones de variables globales
t_log* IO_log;
t_log* IO_log_debug;

char* path_config;
char* nombre_Interfaz;
t_config_IO config_IO;

//estructuras FS
char* path_archivo_bloques;
char* path_archivo_bitmap;
int tamanio_bitmap;
FILE* archivo_bloques;
FILE* archivo_bitmap;
void* memoria_mapeada;
t_bitarray* bitarray;
t_list* archivos_metadata;

int fd_conexion_memoria; 
int fd_conexion_kernel; 

pthread_mutex_t mutex_log_debug;
pthread_mutex_t mutex_log;

#endif