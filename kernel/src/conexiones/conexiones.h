#ifndef CONEXIONES_H_INCLUDED
#define CONEXIONES_H_INCLUDED
// ---------------- Librerias Standard

// ---------------- Librerias Propias
// ------------ INTERNAS
#include <include/estructuras.h>
#include <protocolos/protocoloES.h>
// ------------ EXTERNAS
#include <utils/conexiones/conexiones.h>
#include <utils/hilos/hilos.h>
#include <utils/serializacion/serializacion.h>

// ---------------- Definiciones de variables
/* int fd_server_kernel;
int fd_conexion_memoria;

int fd_conexion_dispatch; 
int fd_conexion_interrupt; */
// ---------------- Definiciones de funciones
void iniciar_servidor_kernel(void);
void atender_entradasalida();

void gestionar_conexiones(void);
void conectar_memoria(void);
void conectar_dispatch(void);
void conectar_interrupt(void);

#endif