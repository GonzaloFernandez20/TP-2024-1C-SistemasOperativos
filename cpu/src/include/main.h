#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

// ---------------- Librerias Standard
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <commons/config.h>

// ---------------- Librerias Propias
//#include <utils/conexiones/clientes/clientes.h>
#include <include/estructuras.h>
#include <conexiones/conexiones.h> //Libreria propia del modulo

#include <utils/hilos/hilos.h>

// ---------------- Definiciones de variables globales
t_log* cpu_log;
t_log* cpu_log_debug;

char *path_config;
t_config_cpu config_cpu;

int fd_dispatch_server;
int fd_interrupt_server;
int fd_conexion_memoria; 

// ---------------- Definiciones de funciones
void liberar_memoria(void);

#endif