#include <planificacion/pcb.h>

t_pcb *crear_pcb(int PID, char* path_pseudocodigo){

    t_pcb *pcb_nuevo = malloc(sizeof(t_pcb));

    pcb_nuevo->pid = PID;
    pcb_nuevo->PC = 0;
    pcb_nuevo->quantum = 0;
    pcb_nuevo->path_pseudocodigo = strdup(path_pseudocodigo);
    _inicializar_registros_cpu(&pcb_nuevo->registros_cpu);

    return pcb_nuevo; 
}

void _inicializar_registros_cpu(t_registros_cpu *registro){
    memset(registro, 0, sizeof(t_registros_cpu));
    /* En caso de que no funcione memset... 
    registro->AX = 0;
    registro->BX = 0;
    registro->CX = 0;
    registro->DX = 0;
    registro->EAX = 0;
    registro->EBX = 0;
    registro->ECX = 0;
    registro->EDX = 0;
    registro->SI = 0;
    registro->DI = 0;
    */  
}

int buscar_y_eliminar_pid(t_estado *estado, int pid_buscado){
    int size = list_size(estado->cola);
    int encontro = (-1);

    for (int i = 0; i < size; i++) {
        pthread_mutex_lock(&(estado->mutex_cola));
            t_pcb *pcb = list_get(estado->cola, i);
        pthread_mutex_unlock(&(estado->mutex_cola));

        if (pcb->pid == pid_buscado)
        {   
            free(pcb);
            pthread_mutex_lock(&(estado->mutex_cola));
                list_remove(estado->cola, i);
            pthread_mutex_unlock(&(estado->mutex_cola));
            encontro = 1;
            break;
        }
    }
    return encontro;
}