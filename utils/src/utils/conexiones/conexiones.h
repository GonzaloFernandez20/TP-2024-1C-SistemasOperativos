#ifndef UTILS_CONEXIONES_H_
#define UTILS_CONEXIONES_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netdb.h>
#include<unistd.h>
#include<errno.h>
#include<sys/socket.h>


/**
 * @brief Inicia un servidor para escuchar conexiones entrantes.
 *
 * Esta función inicializa y configura un socket de servidor para escuchar conexiones entrantes
 * en la dirección IP y el puerto especificados.
 *
 * @param IP Un puntero a una cadena que representa la dirección IP en la que el servidor escuchará.
 * @param PUERTO Un puntero a una cadena que representa el número de puerto en el que el servidor escuchará.
 *
 * @return En caso de éxito, la función devuelve un descriptor de archivo de socket para el servidor.
 * En caso de fallo, devuelve -1.
 */
int iniciar_servidor(char* IP, char* PUERTO);

/**
 * @brief Crea una conexión a un servidor.
 *
 * Esta función crea un socket y lo conecta a la dirección IP y el puerto especificados.
 *
 * @param IP Un puntero a una cadena que representa la dirección IP del servidor.
 * @param PUERTO Un puntero a una cadena que representa el número de puerto del servidor.
 *
 * @return En caso de éxito, la función devuelve un descriptor de archivo de socket para la conexión.
 * En caso de fallo, devuelve -1.
 */
int crear_conexion(char* IP, char* PUERTO);

/**
 * @brief Espera a que un cliente se conecte al servidor.
 *
 * Esta función acepta una nueva conexión de cliente en el socket de servidor proporcionado.
 *
 * @param fd_servidor El descriptor de archivo de socket del servidor.
 *
 * @return En caso de éxito, la función devuelve un descriptor de archivo de socket para la conexión de cliente.
 * En caso de fallo, devuelve EXIT_FAILURE.
 */
int esperar_cliente(int fd_servidor);

/**
 * @brief Libera los recursos asociados con una conexión de cliente.
 * 
 * Simplemente hace un close(fd_cliente);
 *
 * @param fd_cliente El descriptor de archivo de socket de la conexión de cliente que se va a liberar.
 */
void liberar_conexion(int fd_cliente);


#endif