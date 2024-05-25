#ifndef DESALOJO_H_INCLUDED
#define DESALOJO_H_INCLUDED

// ----------- LIBRERIAS 
#include <utils/serializacion/serializacion.h>
#include <include/estructuras.h>
#include <planificacion/pcb.h>
#include <planificacion/colasEstados.h>

// ---------- DECLARACIONES DE ESTRUCTURAS

// ---------- DECLARACIONES DE FUNCIONES
void interpretar_motivo_desalojo(t_pcb* pcb, t_motivo_desalojo motivo, void* stream);
void ejecutar_llamada_io(t_pcb* pcb, int op_code, void* stream);
void validar_peticion(char* interfaz, t_llamada_io llamada);

int _puede_realizar_operacion(int numero, int array[]);
#endif