#ifndef MEMORIA_INICIALIZAR_H_INCLUDED
#define MEMORIA_INICIALIZAR_H_INCLUDED

#include <include/estructuras.h>
#include <utils/inicializaciones/inicializar.h>
// ---------- DEFINICIONES DE LAS ESTRUCTURAS


/*Seteamos las configuraciones iniciales de logs y configs*/ 
void init(void);
void _leer_configuracion(char *path_config);


#endif