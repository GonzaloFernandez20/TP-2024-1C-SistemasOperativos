#ifndef UTILS_CONEXIONES_H_
#define UTILS_CONEXIONES_H_

#define MAX_INTENTOS_CONEXION 5

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netdb.h>
#include<unistd.h>
#include<errno.h>
#include<sys/socket.h>


int iniciar_servidor(char* IP, char* PUERTO);

int crear_conexion(char* IP, char* PUERTO);

int esperar_cliente(int fd_servidor);

void liberar_conexion(int fd_cliente);

#endif