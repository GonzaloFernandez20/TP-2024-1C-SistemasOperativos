#ifndef MEMORIA_INICIALIZAR_H_
#define MEMORIA_INICIALIZAR_H_

#include <commons/log.h>
#include <commons/config.h>
#include <include/estructuras.h>
#include <utils/inicializaciones/inicializar.h>



/*Seteamos las configuraciones iniciales de logs y configs y se incializan estructuras globales*/ 
void init(void);

void leer_configuracion(char *path_config);

void inicializar_semaforos(void);


#endif