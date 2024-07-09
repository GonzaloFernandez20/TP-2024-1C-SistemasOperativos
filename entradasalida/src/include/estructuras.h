#ifndef ESTRUCTURAS_H_INCLUDED
#define ESTRUCTURAS_H_INCLUDED

// ---------- INCLUSIONES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include <pthread.h>
#include <commons/bitarray.h>
#include <sys/mman.h>


// ---------- ESTRUCTURAS PARA EL ARCHIVO CONFIG
typedef struct configuraciones_IO
{
    char* TIPO_INTERFAZ;
    int TIEMPO_UNIDAD_TRABAJO;
    char* IP_KERNEL;
    char* PUERTO_KERNEL;
    char* IP_MEMORIA;
    char* PUERTO_MEMORIA;
    char* PATH_BASE_DIALFS;
    int BLOCK_SIZE;
    int BLOCK_COUNT;
    int RETRASO_COMPACTACION;
    
} t_config_IO;


// ---------- VARIABLES GLOBALES
extern t_log* IO_log;
extern t_log* IO_log_debug;

extern char* path_config;
extern char* nombre_Interfaz;
extern t_config_IO config_IO;

extern char* path_archivo_bloques;
extern char* path_archivo_bitmap;
extern int tamanio_bitmap;
extern FILE* archivo_bloques;
extern FILE* archivo_bitmap;
extern void* memoria_mapeada;
extern t_bitarray* bitarray;

extern int fd_conexion_memoria; 
extern int fd_conexion_kernel; 

extern pthread_mutex_t mutex_log_debug;
extern pthread_mutex_t mutex_log;

#endif