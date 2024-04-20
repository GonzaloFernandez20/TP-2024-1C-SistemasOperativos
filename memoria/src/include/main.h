#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED


// ---------------- Librerias Standard
#include <stdlib.h>
#include <stdio.h>
#include <commons/config.h>

#include <string.h>

// ---------------- Librerias Propias
//#include <utils/conexiones/clientes/clientes.h>
#include <include/estructuras.h>
#include <utils/inicializaciones/inicializar.h>
#include <utils/conexiones/conexiones.h>
#include <utils/serializacion/serializacion.h>



// ---------------- Definiciones de variables
t_log* memoria_log;
t_log* memoria_log_debugg;
char *path_config;
int fd_server_memoria;
// ---------------- Definiciones de funciones
void init(void);
void _leer_configuracion(char *path_config);
void iniciar_servidor_memoria(void);


#endif