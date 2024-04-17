#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED


// ---------------- Librerias Standard
#include <stdlib.h>
#include <stdio.h>

// ---------------- Librerias Propias
#include <utils/hello.h>
//#include <utils/conexiones/clientes/clientes.h>

// ---------------- Definiciones de variables
t_log* cpu_log;


// ---------------- Definiciones de funciones
void leer_configuracion(char *path_config);

//
typedef struct {
    char* IP_MEMORIA;
    char* PUERTO_MEMORIA;
    char* PUERTO_ESCUCHA_DISPATCH;
    char* PUERTO_ESCUCHA_INTERRUPT;
    int   CANTIDAD_ENTRADAS_TLB;
    char* ALGORITMO_TLB;
}t_config_cpu;

t_config_cpu config_cpu;

#endif