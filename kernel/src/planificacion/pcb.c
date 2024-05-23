#include <planificacion/pcb.h>

t_pcb *crear_pcb(int pid){

    t_pcb *pcb_nuevo = malloc(sizeof(t_pcb));

    pcb_nuevo->pid = pid;
    pcb_nuevo->PC = 0;
    pcb_nuevo->quantum = 0;
    _inicializar_registros_cpu(&pcb_nuevo->registros_cpu);

    return pcb_nuevo; 
}

void _inicializar_registros_cpu(t_registros_cpu *registro){
    *registro = (t_registros_cpu){0};  // designated initializers
}


int buscar_y_eliminar_pid(t_list *cola, int pid_buscado){
    int size = list_size(cola);
    int encontro = (-1);

    for (int i = 0; i < size; i++) {
        t_pcb *pcb = list_get(cola, i);
        if (pcb->pid == pid_buscado)
        {
            list_remove(cola, i);
            free(pcb);
            encontro = 1;
            break;
        }
    }
    return encontro;
}