#ifndef ESTRUCTURAS_H_INCLUDED
#define ESTRUCTURAS_H_INCLUDED
// ---------- INCLUSIONES



// ---------- ESTRUCTURAS PARA EL ARCHIVO CONFIG
typedef struct config_entradaSalida
{
    char* TIPO_INTERFAZ,
    char* TIEMPO_UNIDAD_TRABAJO,
    char* IP_KERNEL,
    char* PUERTO_KERNEL,
    char* IP_MEMORIA,
    char* PUERTO_MEMORIA,
    char* PATH_BASE_DIALFS,
    char* BLOCK_SIZE,
    char* BLOCK_COUNT
} t_config_entradaSalida;



// ---------- DEFINICIONES DE LAS ESTRUCTURAS
t_config_entradaSalida configuraciones_entradaSalida;


// ---------- DEFINICIONES DE LAS VARIABLES


#endif