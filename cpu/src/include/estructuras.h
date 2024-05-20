#ifndef ESTRUCTURAS_H_INCLUDED
#define ESTRUCTURAS_H_INCLUDED

// ---------- INCLUSIONES
#include <stdio.h>
#include <string.h>
#include <commons/log.h>


// ---------- ESTRUCTURAS 

/**
 * Todos estos tipos de dato char* no guardarán String Literals 
 * (que son gestionados por el programa como cualquier otro tipo de dato primitivo; aunque char* no es dato primitivo)
 * sino que van a recibir punteros a bloques de memoria asignadas dinámicamente
 * a través de strdup() que duplicará los valores leidos de los archivos de config.
 * 
 * t_config_cpu es alias de configuraciones_cpu. 
 * 
 * Formas de usar esto para declarar variables:
 *      struct configuraciones_cpu nombre_de_mi_variable;
 *      t_config_cpu nombre_de_mi_variable;
 * Ambas opciones son válidas. Obviamente la segunda es la más práctica.
 * 
 * Including the original structure name (struct configuraciones_cpu) makes it clear what the alias (t_config_cpu) refers to, which can be helpful for others reading your code or for maintaining large codebases.
 * 
*/
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