#ifndef ESTRUCTURAS_H_INCLUDED
#define ESTRUCTURAS_H_INCLUDED

// ---------- INCLUSIONES
#include <commons/log.h>
#include <commons/collections/list.h>
#include <commons/collections/dictionary.h>
#include <commons/temporal.h>

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

// ---------- ESTRUCTURAS
typedef struct configuraciones_kernel{
    char* IP_KERNEL;
    char* PUERTO_ESCUCHA; 
    char* IP_MEMORIA;
    char* PUERTO_MEMORIA;
    char* IP_CPU;
    char* PUERTO_CPU_DISPATCH;
    char* PUERTO_CPU_INTERRUPT;
    char* ALGORITMO_PLANIFICACION;
    int QUANTUM;
    char** RECURSOS;
    char** INSTANCIAS_RECURSOS;
    int GRADO_MULTIPROGRAMACION;
} t_config_kernel;

typedef struct estado{
    pthread_mutex_t mutex_cola;
    t_list *cola;
    char* nombre;
}t_estado;

typedef struct interfaz_conectada{
    char* nombre;
    char* tipo;
    int fd;
    t_estado* bloqueados;

    pthread_t peticiones;
    sem_t hay_peticiones;
    pthread_mutex_t interfaz_en_uso;
} t_interfaz;

typedef struct t_peticion{
    int unidades_trabajo;
    int tamanio_a_leer;
    int cant_direcciones;
    t_list* direcciones;
    //agregaremos los argumentos que otras funciones necesiten
    void (*funcion)(struct t_peticion*, int, int);
}t_peticion;

typedef struct recurso{
    char* nombre_recurso;
    int instancias_recursos;
    t_estado* cola_recurso;
    pthread_mutex_t mutex_recurso;
    
}t_recurso;

typedef struct recurso_usado{
    int PID;
    char* nombre_recurso;
}t_recurso_usado;


// ---------- VARIABLES GLOBALES

extern t_log* kernel_log;
extern t_log* kernel_log_debugg;

extern char *path_config;
extern t_config_kernel config_kernel;

extern int fd_server_kernel;
extern int fd_conexion_memoria;
extern int fd_conexion_dispatch; 
extern int fd_conexion_interrupt;

extern t_dictionary *interfaces_conectadas;
extern t_dictionary *instrucciones_por_interfaz;
extern t_dictionary *peticiones_interfaz;
extern t_dictionary *recursos_disponibles;

extern pthread_t manejo_quantum;
extern int termino_quantum;
extern t_temporal *quantum_proceso;

extern t_estado *new;
extern t_estado *ready;
extern t_estado *ready_plus;
extern t_estado *exec;
extern t_estado *estado_exit;

extern pthread_mutex_t mutex_log_debug;
extern pthread_mutex_t mutex_log;
extern pthread_mutex_t diccionario_interfaces;
extern pthread_mutex_t diccionario_peticiones;
extern pthread_mutex_t diccionario_recursos;
extern pthread_mutex_t cola_recursos_usados;

extern sem_t proceso_listo;
extern sem_t proceso_cargado;
extern sem_t execute_libre;
extern sem_t grado_multiprogramacion;
extern sem_t hay_proceso_exit;
extern sem_t planificacion_en_pausa;

extern int planificacion_pausada;
extern int contador_bloqueados;

extern t_list *recursos_usados;
#endif