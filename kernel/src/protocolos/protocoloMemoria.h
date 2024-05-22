#ifndef SERIALIZACION_H_INCLUDED
#define SERIALIZACION_H_INCLUDED

// ---------------- LIBRERIAS 
#include <utils/serializacion/serializacion.h>
#include <include/estructuras.h>
#include <planificacion/pcb.h>

// ---------- DECLARACIONES DE FUNCIONES

void enviar_path(char* path, int pid_asociado);
void recibir_confirmacion(int pid);

// ---------- DECLARACIONES DE ESTRUCTURAS

typedef struct path
{
	op_code codigo_operacion;
    int pid_proceso;
	t_buffer* buffer;
} t_path_proceso;


// ---------- DECLARACIONES DE VARIABLES

#endif