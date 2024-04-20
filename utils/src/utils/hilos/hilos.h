#ifndef HILOS_H_INCLUDED
#define HILOS_H_INCLUDED


// ---------------- Librerias Standard
#include <stdlib.h>
#include <stdio.h>

// ---------------- Librerias Propias



// ---------------- Definiciones de variables



// ---------------- Definiciones de funciones
void atender_cliente(int *socket_servidor, void (*procesar_conexion)(void*), t_log* logger);
void asignar_hilo(int *fd_cliente,  void (*procesar_conexion)(void*));

#endif