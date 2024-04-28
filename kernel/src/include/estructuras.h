#ifndef ESTRUCTURAS_H_INCLUDED
#define ESTRUCTURAS_H_INCLUDED

// ---------- INCLUSIONES
#include <stdio.h>
#include <string.h>
#include <commons/log.h>

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
typedef enum{
    IO_GEN_SLEEP = 0,
    IO_STDIN_READ = 1,
    IO_STDOUT_WRITE = 2,
    EJECUTAR_SCRIPT = 3,//no checkpoint 2
    INICIAR_PROCESO = 4,
    FINALIZAR_PROCESO = 5,
    INICIAR_PLANIFICACION = 6,
    DETENER_PLANIFICACION = 7,
    LISTAR_PROCESOS = 8//no checkpoint 2
}que_hace_kernel;


// ---------- VARIABLES GLOBALES
extern t_log* kernel_log;
extern t_log* kernel_log_debugg;

extern char *path_config;
extern t_config_kernel config_kernel;

extern int fd_server_kernel_interface;
extern int fd_conexion_memoria;
extern int fd_conexion_dispatch; 
extern int fd_conexion_interrupt;

extern int entrada_salida_conectada;

#endif