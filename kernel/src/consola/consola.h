#ifndef CONSOLA_H_INCLUDED
#define CONSOLA_H_INCLUDED

// ------------- LIBRERIAS INCLUIDAS
#include <include/estructuras.h>
#include <planificacion/PLPyPCP.h>
#include <utils/hilos/hilos.h>

#include <commons/log.h>
#include <commons/string.h>
#include <readline/readline.h>

#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

// ------------- COMANDOS DE CONSOLA

typedef enum comandos_consola{
    EJECUTAR_SCRIPT,
    INICIAR_PROCESO,
    FINALIZAR_PROCESO,
    DETENER_PLANIFICACION,
    INICIAR_PLANIFICACION,
    MULTIPROGRAMACION,
    PROCESO_ESTADO,
    COMANDO_NO_RECONOCIDO
}t_comandos_consola;




// ------------- VARIABLES

extern int planificacion_pausada;


// ------------- FUNCIONES DE CONSOLA

void *iniciar_Consola(void);
t_comandos_consola _obtener_enum_comando(char *comando_ingresado);
char **_interpretar(char *comando);
void _imprimir_estados_procesos(void);


#endif