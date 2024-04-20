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
#include <conexiones/conexiones.h> //Libreria propia del modulo

#include <utils/inicializaciones/inicializar.h>
#include <utils/conexiones/conexiones.h>

// ---------------- Definiciones de variables
t_log* cpu_log;
t_log cpu_log_debug;
char *path_config;

int fd_dispatch_server;
int fd_interrupt_server;

int fd_cpu;

pthread_t thread_atender;
pthread_t thread_dispatch;
pthread_t thread_interrupt;

// ---------------- Definiciones de funciones
void init(void);
void _leer_configuracion(char *path_config);
void conectar_dispatch(void);

void _chequear_parametros(int argc, char* argv[]);
void iniciar_servidores();
void conectar_memoria(void)

// ---------------- Liberar memoria utilizada durante la ejecucion
void liberar_memoria(void);

#endif