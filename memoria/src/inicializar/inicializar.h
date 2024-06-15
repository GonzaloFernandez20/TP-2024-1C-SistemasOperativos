#ifndef MEMORIA_INICIALIZAR_H_
#define MEMORIA_INICIALIZAR_H_

#include <commons/log.h>
#include <commons/config.h>
#include <include/estructuras.h>
#include <utils/inicializaciones/inicializar.h>



void init(void);

void iniciar_loggers(void);

void leer_configuracion(char *path_config);

void inicializar_estructuras_memoria(void);

t_bitarray* init_bitarray(void);

void inicializar_semaforos(void);


#endif