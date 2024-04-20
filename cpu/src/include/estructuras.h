#ifndef ESTRUCTURAS_H_INCLUDED
#define ESTRUCTURAS_H_INCLUDED

// ---------- INCLUSIONES
#include <stdio.h>
#include <string.h>
#include <commons/log.h>


// ---------- ESTRUCTURAS 
typedef struct configuraciones_cpu
{   
    char* IP_CPU;
    char* IP_MEMORIA;
    char* PUERTO_MEMORIA;
    char* PUERTO_ESCUCHA_DISPATCH;
    char* PUERTO_ESCUCHA_INTERRUPT;
    int   CANTIDAD_ENTRADAS_TLB;
    char* ALGORITMO_TLB;
    
}t_config_cpu;

// ---------- VARIABLES GLOBALES
extern t_log* cpu_log;
extern t_log* cpu_log_debug;

extern char *path_config;
extern t_config_cpu config_cpu;

extern int fd_dispatch_server;
extern int fd_interrupt_server;
extern int fd_conexion_memoria;

extern int dispatch_conectado;
extern int interrupt_conectado;

#endif