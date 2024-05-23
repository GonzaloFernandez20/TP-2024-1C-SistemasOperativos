#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED


// ---------------- Librerias Standard
#include <stdlib.h>
#include <stdio.h>

#include <commons/config.h>
#include <commons/log.h>
#include <string.h>
#include <pthread.h>
// ---------------- Librerias Propias
// ---- INTERNAS
#include <include/estructuras.h>
#include <inicializar/inicializar.h>
#include <conexiones/conexiones.h>
#include <protocolos/protocoloES.h>
#include <consola/consola.h>
#include <planificacion/colasEstados.h>

// ---- EXTERNAS
#include <utils/inicializaciones/inicializar.h>

// ---------------- Definiciones de variables globales

t_log* kernel_log;
t_log* kernel_log_debugg;
char *path_config;
t_config_kernel config_kernel;
int fd_server_kernel;
int fd_conexion_memoria;
int fd_conexion_dispatch; 
int fd_conexion_interrupt;

int entrada_salida_conectada;

// ---------------- Definiciones de funciones
void liberar_kernel(void);


t_estado *new;
t_estado *ready;
t_estado *exec;
t_estado *blocked;
t_estado *estado_exit;
#endif