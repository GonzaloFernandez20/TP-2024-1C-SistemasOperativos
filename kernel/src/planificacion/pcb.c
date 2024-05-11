#include <planificacion/pcb.h>

t_pcb *crear_pcb(int pid){

    t_pcb *pcb_nuevo = malloc(sizeof(t_pcb));

    pcb_nuevo->pid = pid;
    pcb_nuevo->PC = 0;
    pcb_nuevo->quantum = 0;
    _inicializar_registros_cpu(&pcb_nuevo->registros_cpu);
    // TODO: Inicializar registros de la cpu en 0

    return pcb_nuevo; 
}

void _inicializar_registros_cpu(t_registros_cpu *registro){
    *registro = (t_registros_cpu){0};
}