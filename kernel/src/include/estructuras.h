#ifndef ESTRUCTURAS_H_INCLUDED
#define ESTRUCTURAS_H_INCLUDED
// ---------- INCLUSIONES

#include <commons/log.h>

// ---------- ESTRUCTURAS PARA EL ARCHIVO CONFIG
typedef struct config_kernel
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



// ---------- DEFINICIONES DE LAS ESTRUCTURAS
extern t_config_kernel config_kernel;

// ---------- DEFINICIONES DE LAS VARIABLES 
extern t_log* kernel_log;
extern t_log* kernel_log_debugg;

extern char *path_config;

extern int fd_server_kernel;
extern int fd_conexion_memoria;

extern int fd_conexion_dispatch; 
extern int fd_conexion_interrupt;

#endif