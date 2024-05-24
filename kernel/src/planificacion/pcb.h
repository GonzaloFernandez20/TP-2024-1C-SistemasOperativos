#ifndef PCB_H_INCLUDED
#define PCB_H_INCLUDED

// ------------- LIBRERIAS
#include <include/estructuras.h>

#include <commons/collections/list.h>


#include <stdlib.h>
#include <stdint.h>
// ------------- ESTRUCTURAS
// TODO: TENDRIA QUE TENER TODA LA INFO DEL CONTEXTO NECESARIO DE UN PROCESO
typedef struct registros_cpu{
    uint8_t AX;     // Registro Numérico de propósito general
    uint8_t BX;     // idem
    uint8_t CX;     // idem 
    uint8_t DX;     // idem
    uint32_t EAX;   // idem
    uint32_t EBX;   // idem
    uint32_t ECX;   // idem
    uint32_t EDX;   // idem 
    uint32_t SI;    // Contiene la dirección lógica de memoria de origen desde donde se va a copiar un string.
    uint32_t DI;    // Contiene la dirección lógica de memoria de destino a donde se va a copiar un string.
} t_registros_cpu;
typedef struct pcb
{
    int pid;
    int PC;
    int quantum;
    char* path_pseudocodigo;
    t_registros_cpu registros_cpu;
}t_pcb;


// ---------- DECLARACIONES DE FUNCIONES

t_pcb *crear_pcb(int pid, char* path_pseudocodigo);
void _inicializar_registros_cpu(t_registros_cpu *registro);
int buscar_y_eliminar_pid(t_estado *estado, int pid_buscado);
extern int pid_nuevo_proceso; // No se si esta bien declarada esta global
#endif