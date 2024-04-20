#ifndef ESTRUCTURAS_H_INCLUDED
#define ESTRUCTURAS_H_INCLUDED
// ---------- INCLUSIONES



// ---------- ESTRUCTURAS PARA EL ARCHIVO CONFIG
typedef struct config_cpu
{   
    char* IP_CPU;
    char* IP_MEMORIA;
    char* PUERTO_MEMORIA;
    char* PUERTO_ESCUCHA_DISPATCH;
    char* PUERTO_ESCUCHA_INTERRUPT;
    int   CANTIDAD_ENTRADAS_TLB;
    char* ALGORITMO_TLB;
}t_config_cpu;



// ---------- DEFINICIONES DE LAS ESTRUCTURAS
t_config_cpu config_cpu;


// ---------- DEFINICIONES DE LAS VARIABLES

#endif