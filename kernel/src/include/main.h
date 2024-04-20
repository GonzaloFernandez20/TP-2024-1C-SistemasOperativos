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

// ---- EXTERNAS
#include <utils/inicializaciones/inicializar.h>
#include <utils/conexiones/conexiones.h>
// ---------------- Definiciones de variables
t_log* kernel_log;
t_log* kernel_log_debugg;

char *path_config;

// ---------------- Definiciones de funciones
void liberar_memoria(void);

#endif