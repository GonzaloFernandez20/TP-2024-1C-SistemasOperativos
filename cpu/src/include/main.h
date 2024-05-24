#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

// ---------------- Librerias Standard
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <commons/config.h>

// ---------------- Librerias Propias
// ---- INTERNAS
#include <include/estructuras.h>
#include <conexiones/conexiones.h> 
// #include <include/ciclo.h>

// ---- EXTERNAS
#include <utils/hilos/hilos.h>

// ---------------- Definiciones de variables globales
t_log* cpu_log;         // Ojito que parece que en todo el módulo CPU no parece que se haya usado el cpu_log. 
t_log* cpu_log_debug;   // Considerar juntar cpu_log con cpu_log_debug.

char *path_config;
t_config_cpu config_cpu;

int fd_dispatch_server;
int fd_interrupt_server;
int fd_conexion_memoria; 

int dispatch_conectado;
int interrupt_conectado;

// ---------------- Definiciones de funciones
void liberar_cpu(void);

#endif