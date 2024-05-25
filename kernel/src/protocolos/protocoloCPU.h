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

typedef enum llamada_io{
    IO_GEN_SLEEP,
    IO_STDIN_READ,
    IO_STDOUT_WRITE,
    IO_FS_CREATE,
    IO_FS_DELETE,
    IO_FS_TRUNCATE,
    IO_FS_WRITE,
    IO_FS_READ
}t_llamada_io;

// ---------- DECLARACIONES DE VARIABLES

#endif