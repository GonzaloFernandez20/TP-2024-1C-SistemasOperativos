#ifndef CLIENTES_H_
#define CLIENTES_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>

#include "utils.h"


int conectar_con_CPU(void);
int conectar_con_MEMORIA(void);
int conectar_con_KERNEL(void);

int conectar_con_modulo(char* nombreArchivoConfigConexion);

void escribir_log(t_log* log, t_log_level level, char* mensaje);

t_log* iniciar_logger(char* nombreDelArchivo, char* nombreDelProceso, bool estaLaConsolaActiva, t_log_level nivel);

t_config* iniciar_config(char*);

void leer_consola(t_log*);
void paquete(int);
void liberar_recursos(int conexion, t_log* logger, t_config* config);

#endif /* CLIENTES_H_ */






