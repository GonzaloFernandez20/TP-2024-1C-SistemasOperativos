#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED


// ---------------- Librerias Standard
#include <stdlib.h>
#include <stdio.h>
#include <commons/config.h>

#include <string.h>
#include <pthread.h>

// ---------------- Librerias Propias

#include <include/estructuras.h>
#include <conexiones/conexiones.h>

// ---------- ESTRUCTURAS PARA EL ARCHIVO CONFIG


// ---------------- Definiciones de variables globales
t_log* memoria_log;
t_log* memoria_log_debugg;
char *path_config;
int fd_server_memoria;
t_config_memoria config_memoria;

// ---------------- Definiciones de funciones



void gestionar_conexiones_clientes(void);

void atender_cpu(void);
void atender_kernel(void);
void atender_entradaSalida(void);

void *procesar_operacion_cpu(void *fd_cpu_casteado);
void *procesar_operacion_kernel(void *fd_kernel_casteado);
void *procesar_operacion_entradaSalida(void *fd_ent_sal_casteado);

int _deshacer_casting(void *fd_cpu_casteado);
void _chequear_parametros(int argc, char* argv[]);


#endif