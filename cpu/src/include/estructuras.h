#ifndef ESTRUCTURAS_H_INCLUDED
#define ESTRUCTURAS_H_INCLUDED

// ---------- INCLUSIONES
#include <stdio.h>
#include <string.h>
#include <commons/log.h>
#include <stdint.h>
#include <commons/collections/dictionary.h>


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


// structs de ciclo.c
typedef struct {
    uint32_t PC;    // Program Counter, indica la próxima instrucción a ejecutar
    uint8_t  AX;     // Registro Numérico de propósito general
    uint8_t  BX;     // idem
    uint8_t  CX;     // idem 
    uint8_t  DX;     // idem
    uint32_t EAX;   // idem
    uint32_t EBX;   // idem
    uint32_t ECX;   // idem
    uint32_t EDX;   // idem 
    uint32_t SI;    // Contiene la dirección lógica de memoria de origen desde donde se va a copiar un string.
    uint32_t DI;    // Contiene la dirección lógica de memoria de destino a donde se va a copiar un string.
} struct_registros;

typedef struct {
    int PID;
    int Quantum;
    struct_registros registros; 
} struct_PCB;

typedef struct{
    char* operacion_str;
    char* arg1;
    char* arg2;
    char* arg3;
    char* arg4;
    char* arg5;    
}t_instruccion;


// ---------- VARIABLES GLOBALES

/**
 * El uso del keyword "extern" permite decirle al compilador 
 * que las variables ya fueron definidas en otro archivo.
 * */

extern t_log* cpu_log;
extern t_log* cpu_log_debug;

extern char *path_config;
extern t_config_cpu config_cpu;

extern int fd_interrupt_server;
extern int fd_conexion_memoria;

extern int interrupt_conectado;



extern struct_PCB PCB;
extern struct_registros registrosCPU;   
extern char** instruccion_cpu; // array de strings. Ej: {"SUM", "AX", "BX"}
extern t_dictionary* opCodes_diccionario;
extern t_dictionary* registros_diccionario;
extern uint32_t PID_a_interrumpir;  // CPU recibe, desde Kernel y 
                                    //a través de la conexión Interrupt, PIDs que se quieran interrumpir.
                                    //Si la PID del proceso en ejecución coincide con el PID solicitado a interrumpir,
                                    //entonces seteamos el flag en 1. Luego de procesar la solicitud,
                                    //se debe setear nuevamente el flag en 0 (estado default).
extern pthread_mutex_t mutexInterrupt;

#endif