#ifndef UTILS_HILOS_H_
#define UTILS_HILOS_H_


#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include <utils/serializacion/serializacion.h>
#include <utils/conexiones/conexiones.h>



void atender_cliente(int fd_servidor, void (*procesar_conexion)(void*), t_log* logger, char* modulo);

void asignar_hilo(int *fd_cliente,  void (*procesar_conexion)(void*));

int _deshacer_casting(void *fd_cpu_casteado);


#endif