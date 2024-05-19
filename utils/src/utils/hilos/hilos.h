#ifndef UTILS_HILOS_H_
#define UTILS_HILOS_H_


#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include <utils/serializacion/serializacion.h>
#include <utils/conexiones/conexiones.h>



/**
 * @brief Atiende a un cliente que se conecta al servidor.
 *
 * Esta función acepta una conexión de cliente en el socket del servidor, procesa la conexión utilizando
 * una función de procesamiento proporcionada y registra eventos utilizando un logger.
 *
 * @param fd_servidor El descriptor de archivo de socket del servidor.
 * @param procesar_conexion Un puntero a una función que procesa la conexión del cliente.
 * @param logger Un puntero al struct logger.
 * @param modulo Un string del nombre del módulo del cual recibe la conexión cliente.
 */
void atender_cliente(int fd_servidor, void (*procesar_conexion)(void*), t_log* logger, char* modulo);

/**
 * @brief Asigna un hilo para procesar la conexión de un cliente. 
 * 
 * Dejamos que el hilo main se concentre en recibir conexiones entrantes.
 * 
 * Crea un hilo para la función proporcionada, y le hace detach para que se limpie solo al terminar su ejecución.
 *
 * @param fd_cliente Un puntero al descriptor de archivo de socket del cliente.
 * @param procesar_conexion Un puntero a una función que procesa la conexión del cliente.
 */
void _asignar_hilo(int *fd_cliente, void (*procesar_conexion)(void*));

int _deshacer_casting(void *fd_cpu_casteado);

#endif