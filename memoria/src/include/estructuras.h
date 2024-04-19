#ifndef ESTRUCTURAS_H_INCLUDED
#define ESTRUCTURAS_H_INCLUDED
// ---------- INCLUSIONES

#include <commons/log.h>


// ---------- ESTRUCTURAS PARA EL ARCHIVO CONFIG
typedef struct config_memoria
{
    char* IP_MEMORIA;
    char* PUERTO_ESCUCHA;
    int TAM_MEMORIA;
    int TAM_PAGINA;
    char* PATH_INSTRUCCIONES;
    int RETARDO_RESPUESTA;
    
} t_config_memoria;



// ---------- DEFINICIONES DE LAS ESTRUCTURAS
t_config_memoria configuraciones_memoria;


// ---------- DEFINICIONES DE LAS VARIABLES
t_log* log_memoria;


#endif