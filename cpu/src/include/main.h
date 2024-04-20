#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED


// ---------------- Librerias Standard
#include <stdlib.h>
#include <stdio.h>

#include <commons/config.h>
#include <commons/log.h>
#include <string.h>
// ---------------- Librerias Propias
//#include <utils/conexiones/clientes/clientes.h>
#include <include/estructuras.h>
#include <utils/inicializaciones/inicializar.h>
#include <utils/conexiones/conexiones.h>


// ---------------- Definiciones de variables
t_log* cpu_log;
char *path_config;
int fd_cpu_dispatch;
// ---------------- Definiciones de funciones
void init(void);
void _leer_configuracion(char *path_config);
void conectar_dispatch(void);

#endif