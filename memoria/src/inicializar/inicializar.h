#ifndef MEMORIA_INICIALIZAR_H_
#define MEMORIA_INICIALIZAR_H_

#include <commons/log.h>
#include <commons/config.h>
#include <include/estructuras.h>
#include <utils/inicializaciones/inicializar.h>




// ---------- DEFINICIONES DE LAS ESTRUCTURAS


/*Seteamos las configuraciones iniciales de logs y configs*/ 
void init(void);

void _leer_configuracion(char *path_config);


#endif