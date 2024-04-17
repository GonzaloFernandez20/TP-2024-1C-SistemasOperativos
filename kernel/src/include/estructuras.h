#ifndef ESTRUCTURAS_H_INCLUDED
#define ESTRUCTURAS_H_INCLUDED
// ---------- INCLUSIONES



// ---------- ESTRUCTURAS PARA EL ARCHIVO CONFIG
typedef struct config_kernel
{
    char* PUERTO_ESCUCHA;
    char* IP_MEMORIA;
    //char* PUERTO_MEMORIA;
    //char* IP_CPU;
    //char* PUERTO_CPU_DISPATCH;
    //char* PUERTO_CPU_INTERRUPT;
    //char* ALGORITMO_PLANIFICACION;
    //int QUANTUM;
    //char* RECURSOS;
    //char* INSTANCIAS_RECURSOS;
    //int GRADO_MULTIPROGRAMACION;
} t_config_kernel;



// ---------- DEFINICIONES DE LAS ESTRUCTURAS
t_config_kernel configuraciones_kernel;

#endif