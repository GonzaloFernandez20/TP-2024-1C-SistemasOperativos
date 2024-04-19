#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED


// ---------------- Librerias Standard
#include <stdlib.h>
#include <stdio.h>

// ---------------- Librerias Propias
#include <utils/conexiones/clientes/clientes.h>
#include <include/estructuras.h>

// ---------------- Definiciones de variables
t_log* kernel_log;


// ---------------- Definiciones de funciones
void leer_configuracion(char *path_config);



#endif