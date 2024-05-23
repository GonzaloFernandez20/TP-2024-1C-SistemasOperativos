#ifndef ESTRUCTURAS_H_INCLUDED
#define ESTRUCTURAS_H_INCLUDED

// ---------- INCLUSIONES

#include <stdio.h>
#include <string.h>
#include <commons/log.h>
#include <commons/list.h>
#include <commons/node.h>

#include <pthread.h>


// ---------- ESTRUCTURAS 
typedef struct configuraciones_memoria
{
    char* IP_MEMORIA;
    char* PUERTO_ESCUCHA;
    int TAM_MEMORIA;
    int TAM_PAGINA;
    char* PATH_INSTRUCCIONES;
    int RETARDO_RESPUESTA;
    
} t_config_memoria;

//asocia PID con el archivo de seudocofigo
typedef struct proceso_cargado
{
    int PID;

    int cant_instrucciones;
    char** CODE_segmento;
}t_proceso;



// ---------- VARIABLES GLOBALES
extern t_log* memoria_log;
extern t_log* memoria_log_debugg;

extern char *path_config;
extern t_config_memoria config_memoria;

extern int fd_server_memoria;
extern int fd_kernel;
extern int fd_cpu;

extern t_list* memoria_de_instrucciones;

extern pthread_mutex_t mutex_log_debug;
extern pthread_mutex_t mutex_log;
extern pthread_mutex_t mutex_memoria_instrucciones;

#endif