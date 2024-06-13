#ifndef RECURSOS_H_INCLUDED
#define RECURSOS_H_INCLUDED

// ------------- LIBRERIAS
#include <include/estructuras.h>
#include <planificacion/pcb.h>

#include <commons/collections/list.h>
#include <commons/collections/dictionary.h>
#include <commons/string.h>

#include <stdlib.h>
#include <stdint.h>


// ------------- ESTRUCTURAS


// ---------- DECLARACIONES DE FUNCIONES
void inicializar_recursos(void);
int existe_recurso(char* nombre_recurso);
void agregar_instancia_recurso(int PID, t_recurso *recurso);
void eliminar_instancia_recurso(int PID, t_recurso *recurso);
void liberar_recursos(int PID);



#endif