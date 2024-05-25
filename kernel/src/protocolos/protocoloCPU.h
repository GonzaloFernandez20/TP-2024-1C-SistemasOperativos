#ifndef PROT_CPU_H_INCLUDED
#define PROT_CPU_H_INCLUDED

// ---------------- LIBRERIAS 
#include <utils/serializacion/serializacion.h>
#include <include/estructuras.h>
#include <planificacion/pcb.h>
#include <planificacion/desalojo.h>


// ---------- DECLARACIONES DE FUNCIONES
void enviar_pcb(t_pcb *pcb_envio);
void recibir_pcb(t_pcb* pcb);

// ---------- DECLARACIONES DE ESTRUCTURAS

// ---------- DECLARACIONES DE VARIABLES

#endif