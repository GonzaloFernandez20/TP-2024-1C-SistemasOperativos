#ifndef ESTRUCTURAS_H_INCLUDED
#define ESTRUCTURAS_H_INCLUDED

// ---------- INCLUSIONES
#include <stdio.h>
#include <string.h>
#include <commons/log.h>
#include <stdint.h>
#include <utils/serializacion/serializacion.h>
#include <commons/collections/dictionary.h>
#include <commons/collections/list.h>

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

typedef enum RESPUESTA_DE_CONSULTA_A_TLB 
{
    HIT,
    MISS
}tlb_respuesta;


//REGISTROS GENERALES
typedef struct contexto_ejecucion{
    uint32_t PC;// Program Counter, indica la próxima instrucción a ejecutar
    uint8_t  AX;    // Registro Numérico de propósito general
    uint8_t  BX;    // idem
    uint8_t  CX;    // idem 
    uint8_t  DX;    // idem
    uint32_t EAX;   // idem
    uint32_t EBX;   // idem
    uint32_t ECX;   // idem
    uint32_t EDX;   // idem 
    uint32_t SI;    // Contiene la dirección lógica de memoria de origen desde donde se va a copiar un string.
    uint32_t DI;    // Contiene la dirección lógica de memoria de destino a donde se va a copiar un string.
}t_registros_cpu;

//VARIABLES GLOBALES

extern t_log* cpu_log;
extern t_log* cpu_log_debug;

// SEMAFOROS
extern pthread_mutex_t mutex_log;
extern pthread_mutex_t mutex_log_debug;
extern pthread_mutex_t mutex_hay_interrupcion;
extern pthread_mutex_t mutex_lista_interrupciones;

extern char *path_config;
extern t_config_cpu config_cpu;

extern int fd_dispatch_server;
extern int fd_interrupt_server;
extern int fd_conexion_memoria;
extern int fd_dispatch;
extern int fd_interrupt;

extern int hay_interrupcion;

extern t_registros_cpu registros;

extern int TAM_PAGINA; 

extern int MAX_ENTRADAS;
extern char* ALGORITMO_TLB;
extern t_list* tabla_tlb;  //guardará un array de t_entrada_tlb

//extern op_code_instruccion instruccion;

extern char** instruccion_ejecutando; // array de strings. Ej: {"SUM", "AX", "BX"}
extern t_dictionary* opCodes_diccionario;
extern t_dictionary* registros_diccionario;

extern int se_devolvio_contexto;

extern t_list *direcciones;

extern t_list* lista_interrupciones;

extern int PID;

#endif