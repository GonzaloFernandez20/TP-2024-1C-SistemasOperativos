#ifndef PROTOCOLOS_H_INCLUDED
#define PROTOCOLOS_H_INCLUDED


// ---------------- Librerias Standard

// ---------------- Librerias Propias
#include <utils/hilos/hilos.h>
#include <include/main.h>
// ---------------- Definiciones de variables

// ---------------- Definiciones de funciones
void *procesar_conexion_es(void *fd_cpu_casteado);
#endif