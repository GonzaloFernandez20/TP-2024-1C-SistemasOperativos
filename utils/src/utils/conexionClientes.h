#include <iostream>
#include <stdlib.h>
#include <stdio.h>

int conectar_con_CPU(void);
 
int conectar_con_MEMORIA(void);
    
int conectar_con_KERNEL(void);

int conectar_con_IO(void);
    
int conectar_con_modulo(char* nombreArchivoConfigConexion);

void escribir_log(t_log* log, t_log_level level, char* mensaje);

t_log* iniciar_logger(char* nombreDelArchivo, char* nombreDelProceso, bool estaLaConsolaActiva, t_log_level nivel);

t_config* iniciar_config(char *direccion);

void leer_consola(t_log* logger);

void paquete(int conexion);

void liberar_recursos(int conexion, t_log* logger, t_config* config);

