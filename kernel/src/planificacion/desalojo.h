#ifndef DESALOJO_H_INCLUDED
#define DESALOJO_H_INCLUDED

// ----------- LIBRERIAS 
#include <utils/serializacion/serializacion.h>
#include <include/estructuras.h>
#include <planificacion/pcb.h>
#include <planificacion/colasEstados.h>
#include <protocolos/protocoloES.h>
#include <planificacion/recursos.h>

// ---------- DECLARACIONES DE ESTRUCTURAS

// ---------- DECLARACIONES DE FUNCIONES
void interpretar_motivo_desalojo(t_pcb* pcb, void* stream);
void ejecutar_llamada_io(t_pcb* pcb, void* stream);
void case_recurso(t_pcb* pcb, void* stream, int op_code);

void case_IO_GEN_SLEEP(t_pcb* pcb, void* stream);
void case_IO_STDIN_READ(t_pcb* pcb, void* stream);
void case_WAIT(int PID, t_recurso *recurso);
void case_SIGNAL(int PID, t_recurso *recurso);
void case_IO_STDOUT_WRITE(t_pcb *pcb, void *stream);


int validar_peticion(char* interfaz, op_code_instruccion llamada);
int admite_operacion_solicitada(op_code_instruccion instruccion, char* tipo);
int _puede_realizar_operacion(op_code_instruccion numero, op_code_instruccion array[]); 
#endif