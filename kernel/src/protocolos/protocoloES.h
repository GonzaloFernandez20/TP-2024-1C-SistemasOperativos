#ifndef PROTOCOLOS_H_INCLUDED
#define PROTOCOLOS_H_INCLUDED


// ---------------- Librerias Standard

// ---------------- Librerias Propias
#include <include/estructuras.h>

#include <utils/hilos/hilos.h>
#include <utils/serializacion/serializacion.h>
// ---------------- Definiciones de variables

// ---------------- Definiciones de funciones
void *procesar_conexion_es(void *fd_cpu_casteado);
#endif