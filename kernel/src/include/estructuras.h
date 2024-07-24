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
    char* nombre_archivo; 
    int tipo_operacion;
    int registro_tamanio;
    int registro_direccion;
    int reg_puntero_archivo;
    
    void (*funcion)(struct t_peticion*, int, int);
}t_peticion;
/* typedef struct t_peticion {
    void (*funcion)(struct t_peticion*, int, int);
    // Unión para los argumentos adicionales segun la funcion que llamemos
    union argumentos_peticiones{
        struct IO_GEN_SLEEP{
            int unidades_trabajo;
        } IO_GEN_SLEEP;
        struct IO_STDIN_READ{
            int tamanio_a_leer;
            int cant_direcciones;
            t_list* direcciones;
        } IO_STDIN_READ;
        struct IO_STDOUT_WRITE{
            int tamanio_a_leer;
            int cant_direcciones;
            t_list* direcciones;
        } IO_STDOUT_WRITE;
        struct IO_FS_CREATE_DELETE{
            char* nombre_archivo;
            int tipo_operacion;
        } IO_FS_CREATE_DELETE; //
        struct IO_FS_TRUNCATE{
            char* nombre_archivo;
            int registro_tamanio;
        } IO_FS_TRUNCATE;
        struct IO_FS_WRITE_READ{
            char* nombre_archivo;
            int tipo_operacion;
            int registro_tamanio;
            int reg_puntero_archivo;
            int cant_direcciones;
            t_list* direcciones;
        } IO_FS_WRITE_READ;
    }argumentos; 
} t_peticion; */


/*   t_peticion peticion;
    peticion.argumentos.IO_GEN_SLEEP.unidades_trabajo = 10;
    peticion.funcion = funcion_sleep;  */
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
extern t_list *procesos_terminados;
extern char *recurso_que_bloquea;
#endif