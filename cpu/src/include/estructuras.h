#ifndef ESTRUCTURAS_H_INCLUDED
#define ESTRUCTURAS_H_INCLUDED

// ---------- INCLUSIONES
#include <stdio.h>
#include <string.h>
#include <commons/log.h>


// ---------- ESTRUCTURAS 

/** 
 * Todos estos tipos de dato char* no guardarán String Literals 
 * (que son gestionados por el programa como cualquier int, bool o char; es decir, no tenemos que preocuparnos de liberar memoria manualmente)
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
 * Incluir el nombre original de la estructura (struct configuraciones_cpu) aclara el significado del alias (t_config_cpu), 
 * es útil para la legibilidad y mantenibilidad.
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

/**
 * El uso del keyword "extern" permite decirle al compilador 
 * que las variables ya fueron definidas en otro archivo.
 * */

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