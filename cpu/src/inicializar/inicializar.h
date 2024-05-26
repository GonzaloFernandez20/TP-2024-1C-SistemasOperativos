#ifndef CPU_INICIALIZAR_H_
#define CPU_INICIALIZAR_H_

#include <commons/log.h>
#include <commons/config.h>
#include <include/estructuras.h>
#include <ciclo/instrucciones.h>
#include <utils/inicializaciones/inicializar.h>
#include <pthread.h>
#include <semaphore.h>



/*Seteamos las configuraciones iniciales de logs y configs*/ 
void init(void);

void _leer_configuracion(char *path_config);
void inicializar_semaforos(void);

void init_opCodes_dictionary(void);

void init_registros_dictionary(void);


#endif