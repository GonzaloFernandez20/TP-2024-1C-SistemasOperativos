#ifndef UTILS_CONEXIONES_H_
#define UTILS_CONEXIONES_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netdb.h>
#include<unistd.h>
#include<errno.h>
#include<sys/socket.h>


/** Crea el socket servidor, lo bindea al puerto y lo pone a escuchar **/
int iniciar_servidor(char* IP, char* PUERTO);

/************ Crea el socket cliente y lo conecta al servidor **********/
int crear_conexion(char* IP, char* PUERTO);


#endif