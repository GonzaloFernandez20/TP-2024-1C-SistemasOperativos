#ifndef PROT_MEMORIA_H_INCLUDED
#define PROT_MEMORIA_H_INCLUDED

// ---------------- LIBRERIAS 
#include <utils/serializacion/serializacion.h>
#include <include/estructuras.h>
#include <planificacion/pcb.h>

// ---------- DECLARACIONES DE FUNCIONES

void enviar_path_seudocodigo(char* archivo, int PID);
int recibir_confirmacion(int pid);
void finalizar_proceso(int PID);
// ---------- DECLARACIONES DE ESTRUCTURAS

typedef struct path
{
	op_code codigo_operacion;
    int pid_proceso;
	t_buffer* buffer;
} t_path_proceso;


// ---------- DECLARACIONES DE VARIABLES

#endif