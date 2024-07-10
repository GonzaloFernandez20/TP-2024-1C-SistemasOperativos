#ifndef IO_INICIALIZAR_H_
#define IO_INICIALIZAR_H_

#include <commons/log.h>
#include <commons/string.h>
#include <include/estructuras.h>
#include <fileSystem/estructurasFS.h>


void chequear_parametros_inicio_interfaz(int argc, char* argv[]);

void init(void);

void leer_configuracion();

void inicializar_semaforos(void);


#endif