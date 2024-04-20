#ifndef UTILS_HILOS_H_
#define UTILS_HILOS_H_


// ---------------- Librerias Standard
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

// ---------------- Librerias Propias

#include <utils/serializacion/serializacion.h>



// ---------------- Definiciones de variables



// ---------------- Definiciones de funciones
void atender_cliente(int *fd_servidor, void (*procesar_conexion)(void*), t_log* logger, char* modulo);

void _asignar_hilo(int *fd_cliente,  void (*procesar_conexion)(void*));

int _deshacer_casting(void *fd_cpu_casteado);

#endif