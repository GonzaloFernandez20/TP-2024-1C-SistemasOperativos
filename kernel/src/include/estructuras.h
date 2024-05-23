#ifndef ESTRUCTURAS_H_INCLUDED
#define ESTRUCTURAS_H_INCLUDED

// ---------- INCLUSIONES
#include <commons/log.h>
#include <commons/collections/list.h>

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
// ---------- ESTRUCTURAS PARA EL ARCHIVO CONFIG
typedef struct configuraciones_kernel
{
    char* IP_KERNEL;
    char* PUERTO_ESCUCHA; 
    char* IP_MEMORIA;
    char* PUERTO_MEMORIA;
    char* IP_CPU;
    char* PUERTO_CPU_DISPATCH;
    char* PUERTO_CPU_INTERRUPT;
    char* ALGORITMO_PLANIFICACION;
    int QUANTUM;
    char* RECURSOS;
    char* INSTANCIAS_RECURSOS;
    int GRADO_MULTIPROGRAMACION;

} t_config_kernel;




/* struct estado //? ESTO TODAVIA NO SE PARA QUE SIRVE
{
    t_nombre_estado nombreEstado;
    t_list *listaProcesos;
    sem_t *semaforoEstado;
    pthread_mutex_t *mutexEstado;
};
typedef struct estado t_estado; */

//t_nombre_estado estado_de_proceso;

// ---------- VARIABLES GLOBALES
extern t_log* kernel_log;
extern t_log* kernel_log_debugg;

extern char *path_config;
extern t_config_kernel config_kernel;

extern int fd_server_kernel;
extern int fd_conexion_memoria;
extern int fd_conexion_dispatch; 
extern int fd_conexion_interrupt;

extern int entrada_salida_conectada;


// Estados
extern t_list *new;
extern t_list *ready;
extern t_list *exec;
extern t_list *estado_exit;
extern t_list *blocked;


extern pthread_mutex_t mutexBlocked;
extern pthread_mutex_t mutexNew;
extern pthread_mutex_t mutexReady;
extern pthread_mutex_t mutexExec;
extern pthread_mutex_t mutexExit;
extern sem_t proceso_listo;
extern sem_t execute_libre;

extern pthread_mutex_t mutex_log_debug;
extern pthread_mutex_t mutex_log;

#endif