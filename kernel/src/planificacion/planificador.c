#include <planificacion/planificador.h>

int pid_nuevo_proceso = 0; 
t_list *new;
t_list *ready;
t_list *exec;
t_list *estado_exit;
t_list *blocked;



// ----------- FUNCIONES PRINCIPALES

void* crear_proceso(char *path_proceso){
    // LOG MINIMO: Creación de Proceso: "Se crea el proceso <PID> en NEW"

    //Iniciar proceso: Se encargará de iniciar un nuevo proceso cuyas instrucciones a ejecutar se encontrarán en el archivo <path> dentro del file system de la máquina donde corra la Memoria. Este mensaje se encargará de la creación del proceso (PCB) en estado NEW.
    int pid_asignado = _asignar_PID();
    t_pcb *nuevo_pcb = crear_pcb(pid_asignado);

    //Envio del path a memoria para que haga lo suyo


/*  pthread_mutex_lock(log);
    log_info(kernelLogger, "Se crea el proceso <%d> en NEW", nuevo_pcb->pid);
    pthread_mutex_unlock(log); */

    //mutex
    //lo agrego a la lista de procesos en NEW
    list_add(new, (void *)nuevo_pcb);
    //mutex


    return NULL;
} 

void extraer_proceso(int pid){

    /*
    TIENE QUE: 
                ENCONTRAR AL PROCESO EN LA COLA QUE ESTE
                SACARLO DE LA COLA
                BORRARLO DE LA MEMORIA
    */

    t_list *array_colas[] = {new, ready, exec, blocked};
    
    int cant_elementos = (int)(sizeof(array_colas) / sizeof(array_colas[0]));

    for (int i = 0; i < cant_elementos; i++)
    {
        if(_buscar_y_eliminar_pid(array_colas[i], pid) != (-1)){
            break;
        }
    }
}

int _buscar_y_eliminar_pid(t_list *cola, int pid_buscado){
    int size = list_size(cola);
    int encontro = (-1);

    for (int i = 0; i < size; i++) {
        t_pcb *pcb = list_get(cola, i);
        if (pcb->pid == pid_buscado)
        {
            list_remove(cola, i);
            encontro = 1;
            break;
        }
    }
    return encontro;
}

void iniciar_colas_planificacion(void){
    new = list_create();
    ready = list_create();
    exec = list_create();
    estado_exit = list_create();
    blocked = list_create();
    // list_create hace malloc, guarda con eso
}

// ----------- FUNCIONES SECUNDARIAS

void imprimir_cola(t_list *cola, void(*_mostrar_pcbs)(void*)) {
    /* int size = queue_size(cola);

    for (int i = 0; i < size; i++) {
        void *pcb = list_get(cola->elements, i);
        _mostrar_pcbs(pcb);
    } */
    list_iterate(cola, _mostrar_pcbs);
}

// ----------- FUNCIONES AUXILIARES

int _asignar_PID(void){
    return pid_nuevo_proceso++;
}//Lo uso y lo incremento para que otro PCB tenga un pid distinto.

void _mostrar_pcbs(void *pcbDeLista) {
    t_pcb *pcb = (t_pcb *)pcbDeLista;
    printf("\t\tPID: %d\n", pcb->pid);
}
