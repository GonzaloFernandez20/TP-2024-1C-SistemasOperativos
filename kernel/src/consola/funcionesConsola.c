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
    // 1. Si el proceso esta en alguna cola de new o ready lo mando a exit instantaneamente, listo
    // 2. si esta en exec mando una interrumpcion a cpu y cuando vuelva lo mando a exit, listo
    // 3. si esta en la cola de bloqueados espero a que la IO me indique que termino de procesar la peticion de ese proceso para mandarlo a exit
    
    //t_estado *array_estados[] = {new, ready, exec, estado_exit}; -> DE EXIT NO LO PUEDO EXTRAER Y DE EJECUTANDO LO HAGO APARTE
    //pthread_mutex_t array_semaforos[] = {new->mutex_cola, ready->mutex_cola, exec->mutex_cola, estado_exit->mutex_cola};
    
    t_estado *array_estados[] = {new, ready};
    pthread_mutex_t array_semaforos[] = {new->mutex_cola, ready->mutex_cola};
    
    int cant_elementos = (int)(sizeof(array_estados) / sizeof(array_estados[0]));
    int resultado;

    if ((_esta_ejecutando(pid)))
    {
        enviar_interrupcion(INTERRUPCION); // LE MANDO A CPU LA INTERRUPCION DE FIN DE PROCESO
    }else
    {
        for (int i = 0; i < cant_elementos; i++)
        {   
            pthread_mutex_lock(&array_semaforos[i]);
                resultado = buscar_y_trasladar_pid(array_estados[i], pid);
            pthread_mutex_unlock(&array_semaforos[i]);
            
            if(resultado != (-1)){  break;  }
        }
    }

    // Agregar el caso de que lo busque en las colas de bloqueados y setee una variable global que tiene q chequear un proceso cuando vuelve de io para saber si puede seguir ejecutando. (punto 3 de mi lista)
}

int _asignar_PID(void){
    return pid_nuevo_proceso++;
}//Lo uso y lo incremento para que otro PCB tenga un pid distinto.

void _imprimir_estados_procesos(void){

    puts("Cola NEW: ");
    imprimir_cola(new);

    puts("Cola READY: ");
    imprimir_cola(ready);

    if (algoritmo_es_VRR()){
        puts("Cola READY +: ");
        imprimir_cola(ready_plus);
    }
    
    puts("Cola EXECUTE: ");
    imprimir_cola(exec);
    
    puts("Cola BLOCKED: ");
    imprimir_cola_bloqueados();

    puts("Cola EXIT: ");
    imprimir_cola(estado_exit);
}

// CAPAZ ESTAS 3 FUNCIONES DEBERIAN IR EN colasEstados.c
void imprimir_cola(t_estado *estado) {
    list_iterate(estado->cola, _mostrar_pcbs);
}

void _mostrar_pcbs(void *pcbDeLista) {
    t_pcb *pcb = (t_pcb *)pcbDeLista;
    printf("\t\tPID: %d\n", pcb->pid);
}

void imprimir_cola_bloqueados(){

    t_list* interfaces = dictionary_elements(interfaces_conectadas); // Lista de elementos de tipo t_interfaz*
    int cant_interfaces = list_size(interfaces);

    for (int i = 0; i < cant_interfaces; i++)
    {
        t_interfaz *interfaz = list_remove(interfaces, 0);
        printf("\t%s:\n", interfaz->nombre);
        imprimir_cola(interfaz->bloqueados);
    }

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
