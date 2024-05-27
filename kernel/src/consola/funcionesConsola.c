#include <consola/funcionesConsola.h>


void crear_proceso(char *path_proceso){
    
    int pid_asignado = _asignar_PID();
    t_pcb *nuevo_pcb = crear_pcb(pid_asignado, path_proceso);

    // ------------------ AGREGA A COLA NEW 
    pthread_mutex_lock(&new->mutex_cola);
        list_add(new->cola, nuevo_pcb);
    pthread_mutex_unlock(&new->mutex_cola);

    pthread_mutex_lock(&mutex_log);
        log_info(kernel_log, "Se crea el proceso < %d > en NEW", nuevo_pcb->pid);
    pthread_mutex_unlock(&mutex_log);

    sem_post(&proceso_cargado);
}

void extraer_proceso(int pid){

    //t_estado *array_estados[] = {new, ready, exec, estado_exit}; -> DE EXIT NO LO PUEDO EXTRAER Y DE EJECUTANDO LO HAGO APARTE
    //pthread_mutex_t array_semaforos[] = {new->mutex_cola, ready->mutex_cola, exec->mutex_cola, estado_exit->mutex_cola};
    t_estado *array_estados[] = {new, ready};
    pthread_mutex_t array_semaforos[] = {new->mutex_cola, ready->mutex_cola};
    
    int cant_elementos = (int)(sizeof(array_estados) / sizeof(array_estados[0]));
    int resultado;

    if ((_esta_ejecutando(pid)))
    {
        //puts("PROCESO EJECUTANDO");
        enviar_interrupcion(INTERRUPCION); // LE MANDO A CPU LA INTERRUPCION DE FIN DE PROCESO
    }else
    {
        for (int i = 0; i < cant_elementos; i++)
        {   
            pthread_mutex_lock(&array_semaforos[i]);
                resultado = buscar_y_eliminar_pid(array_estados[i], pid);
            pthread_mutex_unlock(&array_semaforos[i]);
            
            if(resultado != (-1)){  break;  }
        }
    }
}

int _asignar_PID(void){
    return pid_nuevo_proceso++;
}//Lo uso y lo incremento para que otro PCB tenga un pid distinto.


void _imprimir_estados_procesos(void){

    puts("Cola NEW: ");
    imprimir_cola(new, _mostrar_pcbs);

    puts("Cola READY: ");
    imprimir_cola(ready, _mostrar_pcbs);

    puts("Cola EXECUTE: ");
    imprimir_cola(exec, _mostrar_pcbs);
    
    //puts("Cola BLOCKED: ");
    //imprimir_cola(blocked, _mostrar_pcbs);

    puts("Cola EXIT: ");
    imprimir_cola(estado_exit, _mostrar_pcbs);
}


void _mostrar_pcbs(void *pcbDeLista) {
    t_pcb *pcb = (t_pcb *)pcbDeLista;
    printf("\t\tPID: %d\n", pcb->pid);
}


int _esta_ejecutando(int pid_buscado){
    int size = list_size(exec->cola);
    int encontro = 0;

    for (int i = 0; i < size; i++) {
            pthread_mutex_lock(&(exec->mutex_cola));
                t_pcb *pcb = list_get(exec->cola, i);
            pthread_mutex_unlock(&(exec->mutex_cola));

            if (pcb->pid == pid_buscado)
            {   
                free(pcb);
                encontro = 1;
                break;
            }
        }
    return encontro;
}
