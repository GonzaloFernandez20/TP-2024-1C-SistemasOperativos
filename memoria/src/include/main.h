#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED


// ---------------- Librerias Standard
#include <stdlib.h>
#include <stdio.h>
#include <commons/config.h>

#include <string.h>
#include <pthread.h>

// ---------------- Librerias Propias
//#include <utils/conexiones/clientes/clientes.h>
#include <include/estructuras.h>
#include <utils/inicializaciones/inicializar.h>
#include <utils/conexiones/conexiones.h>

// ---------------- Definiciones de variables
t_log* memoria_log;
t_log* memoria_log_debugg;
char *path_config;
int fd_server_memoria;

// ---------------- Definiciones de funciones
void init(void);
void _leer_configuracion(char *path_config);
void iniciar_servidor_memoria(void);

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