#ifndef CONEXIONES_H_INCLUDED
#define CONEXIONES_H_INCLUDED


// ---------------- Librerias Standard
#include <stdlib.h>
#include <stdio.h>

#include <commons/config.h>
#include <commons/log.h>
#include <string.h>

// ---------------- Librerias Propias
#include <utils/hilos/hilos.h>

// ---------------- Definiciones de variables


// ---------------- Definiciones de funciones
void atender_dispatch(void);
void atender_interrupt(void);
void gestionar_conexiones_kernel(void);


#endif