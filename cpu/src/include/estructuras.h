#ifndef ESTRUCTURAS_H_INCLUDED
#define ESTRUCTURAS_H_INCLUDED
// ---------- INCLUSIONES



// ---------- ESTRUCTURAS PARA EL ARCHIVO CONFIG
typedef struct cpu_kernel
{
    char* IP_MEMORIA;
    char* PUERTO_MEMORIA;
    char* PUERTO_ESCUCHA_DISPATCH;
    char* PUERTO_ESCUCHA_INTERRUPT;
    int   CANTIDAD_ENTRADAS_TLB;
    char* ALGORITMO_TLB;
}t_config_cpu;



// ---------- DEFINICIONES DE LAS ESTRUCTURAS
t_config_kernel configuraciones_kernel;


// ---------- DEFINICIONES DE LAS VARIABLES

#endif