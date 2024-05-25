#ifndef ESTRUCTURAS_H_INCLUDED
#define ESTRUCTURAS_H_INCLUDED

// ---------- INCLUSIONES
#include <stdio.h>
#include <string.h>
#include <commons/log.h>
#include <stdint.h>
#include <utils/serializacion/serializacion.h>


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
//REGISTROS GENERALES
typedef struct {
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
} t_registros;
//REGISTROS GENERALES + PCB
typedef struct{
    uint32_t PC;// Program Counter, indica la próxima instrucción a ejecutar
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
}t_estado_cpu;
//PCB
typedef struct {
    int PID;
    int Quantum;
    uint32_t PC;// Program Counter, indica la próxima instrucción a ejecutar
    t_registros registros; 
} t_pcb;
//INSTRUCCION RECIBIDA DE MEMORIA
typedef struct{
    char* operacion_str;
    char* arg1;
    char* arg2;
    char* arg3;
    char* arg4;
    char* arg5;    
}t_instruccion;
//OPERACION
typedef enum{
    SET,
    MOV_IN,
    MOV_OUT, 
    SUM,
    SUB,
    JNZ,
    RESIZE,
    COPY_STRING,
    WAIT,
    SIGNAL,
    IO_GEN_SLEEP,
    IO_STDIN_READ,
    IO_STDOUT_WRITE,
    IO_FS_CREATE,
    IO_FS_DELETE,
    IO_FS_TRUNCATE, 
    IO_FS_WRITE,
    IO_FS_READ,
    EXIT_OS //EXIT pelado en C es una palabra reservada
} op_code_instruccion; //19
//SERIALIZACION
//buffer ya declarado en otro lado, pero sin el campo offset entonces no me sirve, revisar con todos dps
typedef struct {
    uint32_t size; // Tamaño del payload
    uint32_t offset; // Desplazamiento dentro del payload
    void* stream; // Payload
} t_buffer_PCB;
//paquete_PCB
typedef struct {
    int codigo_operacion;
	int motivo_de_desalojo;
    t_buffer_PCB* buffer_PCB;
} t_paquete_PCB;


//VARIABLES GLOBALES
extern pthread_mutex_t mutex_dispatch;//locked al inicializarlo
extern pthread_mutex_t mutex_ciclo;//locked al inicializarlo 
extern t_pcb PCB;
extern t_estado_cpu registrosCPU;   
extern t_instruccion instruccion;

extern t_log* cpu_log;
extern t_log* cpu_log_debug;

extern char *path_config;
extern t_config_cpu config_cpu;

extern int fd_dispatch_server;
extern int fd_interrupt_server;
extern int fd_conexion_memoria;

extern int dispatch_conectado;
extern int interrupt_conectado;

extern int sistema_encendido;

#endif