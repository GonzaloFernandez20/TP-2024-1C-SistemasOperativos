#ifndef PROT_CPU_H_INCLUDED
#define PROT_CPU_H_INCLUDED

// ---------------- LIBRERIAS 
#include <utils/serializacion/serializacion.h>
#include <include/estructuras.h>
#include <planificacion/pcb.h>
#include <planificacion/desalojo.h>


// ---------- DECLARACIONES DE FUNCIONES
void enviar_contexto_ejecucion(t_pcb *pcb_envio);
void recibir_contexto_ejecucion(t_pcb* pcb);

int _es_algoritmo_con_quantum(void);

// ---------- DECLARACIONES DE ESTRUCTURAS

// ---------- DECLARACIONES DE VARIABLES

#endif