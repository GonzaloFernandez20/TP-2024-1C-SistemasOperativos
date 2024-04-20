#ifndef ESTRUCTURAS_H_INCLUDED
#define ESTRUCTURAS_H_INCLUDED

// ---------- INCLUSIONES

#include <stdio.h>
#include <string.h>
#include <commons/log.h>


// ---------- ESTRUCTURAS 
typedef struct configuraciones_memoria
{
    char* IP_MEMORIA;
    char* PUERTO_ESCUCHA;
    int TAM_MEMORIA;
    int TAM_PAGINA;
    char* PATH_INSTRUCCIONES;
    int RETARDO_RESPUESTA;
    
} t_config_memoria;



// ---------- VARIABLES GLOBALES
extern t_log* memoria_log;
extern t_log* memoria_log_debugg;

extern char *path_config;
extern t_config_memoria config_memoria;

extern int fd_server_memoria;


#endif