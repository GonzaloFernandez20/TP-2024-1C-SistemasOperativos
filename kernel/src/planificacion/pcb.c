#include <planificacion/pcb.h>

t_pcb *crear_pcb(int PID, char* path_pseudocodigo){

    t_pcb *pcb_nuevo = malloc(sizeof(t_pcb));

    pcb_nuevo->pid = PID;
    pcb_nuevo->PC = 0;
    pcb_nuevo->quantum = config_kernel.QUANTUM;
    pcb_nuevo->path_pseudocodigo = strdup(path_pseudocodigo);
    _inicializar_registros_cpu(&pcb_nuevo->registros_cpu);

    return pcb_nuevo; 
}

void _inicializar_registros_cpu(t_registros_cpu *registro){
    memset(registro, 0, sizeof(t_registros_cpu));
}

int buscar_y_trasladar_pid(t_estado *estado, int pid_buscado){
    int size = list_size(estado->cola);
    int encontro = (-1);

    for (int i = 0; i < size; i++) {
        pthread_mutex_lock(&(estado->mutex_cola));
            t_pcb *pcb = list_get(estado->cola, i);
        pthread_mutex_unlock(&(estado->mutex_cola));

        if (pcb->pid == pid_buscado)
        {   
            pthread_mutex_lock(&(estado->mutex_cola));
                list_remove((estado->cola), i);
            pthread_mutex_unlock(&(estado->mutex_cola));

            pthread_mutex_lock(&(estado_exit->mutex_cola));
                list_add((estado_exit->cola), pcb);
            pthread_mutex_unlock(&(estado_exit->mutex_cola));

            encontro = 1;
            free(pcb);
            break;
        }
    }
    return encontro;
}