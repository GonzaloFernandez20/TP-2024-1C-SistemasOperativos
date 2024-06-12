#ifndef RECURSOS_H_INCLUDED
#define RECURSOS_H_INCLUDED

// ------------- LIBRERIAS
#include <include/estructuras.h>

#include <commons/collections/list.h>
#include <commons/collections/dictionary.h>
#include <commons/string.h>

#include <stdlib.h>
#include <stdint.h>


// ------------- ESTRUCTURAS


// ---------- DECLARACIONES DE FUNCIONES
void inicializar_recursos(void);
int existe_recurso(char* nombre_recurso);

#endif