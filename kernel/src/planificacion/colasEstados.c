#include <planificacion/colasEstados.h>


void iniciar_colas_planificacion(void){

    // NEW
    new = malloc(sizeof(t_estado));
    pthread_mutex_init(&(new->mutex_cola), NULL); 
    new->cola = list_create();
    new->nombre = string_duplicate("NEW");

    // READY
    ready = malloc(sizeof(t_estado));
    pthread_mutex_init(&(ready->mutex_cola), NULL); 
    ready->cola = list_create();
    ready->nombre = string_duplicate("READY");

    // EXEC
    exec = malloc(sizeof(t_estado));
    pthread_mutex_init(&(exec->mutex_cola), NULL); 
    exec->cola = list_create();
    exec->nombre = string_duplicate("EXEC");

    // EXIT
    estado_exit = malloc(sizeof(t_estado));
    pthread_mutex_init(&(estado_exit->mutex_cola), NULL); 
    estado_exit->cola = list_create();
    estado_exit->nombre = string_duplicate("EXIT");

    if (algoritmo_es_VRR())
    {
        // READY +
        ready_plus = malloc(sizeof(t_estado));
        pthread_mutex_init(&(ready_plus->mutex_cola), NULL); 
        ready_plus->cola = list_create();
        ready_plus->nombre = string_duplicate("READY +");
    }

}

void imprimir_cola(t_estado *estado, void(*_mostrar_pcbs)(void*)) {
    list_iterate(estado->cola, _mostrar_pcbs);
}

void trasladar(int pid_buscado,  t_estado *origen, t_estado *destino){
    t_list *cola_origen = origen->cola;
    t_list *cola_destino = destino->cola;

    int size_origen = list_size(cola_origen);

    for (int i = 0; i < size_origen; i++) {
        pthread_mutex_lock(&(origen->mutex_cola));
            t_pcb *pcb = list_get(cola_origen, i);
        pthread_mutex_unlock(&(origen->mutex_cola));

        if (pcb->pid == pid_buscado)
        {   
            pthread_mutex_lock(&(origen->mutex_cola));
            list_remove(cola_origen, i);
            pthread_mutex_unlock(&(origen->mutex_cola));
            
            pthread_mutex_lock(&(destino->mutex_cola));
            list_add(cola_destino, pcb);
            pthread_mutex_unlock(&(destino->mutex_cola));
            
            break;
        }
    }
    pthread_mutex_lock(&mutex_log);
        log_info(kernel_log, "PID: <%d> - Estado Anterior: < %s > - Estado Actual: < %s >", pid_buscado, origen->nombre, destino->nombre);
    pthread_mutex_unlock(&mutex_log);

    if (string_equals_ignore_case(destino->nombre, "READY"))
            {
                _loggear_ingreso_ready();
                sem_post(&proceso_listo);
            }

    /* if (string_equals_ignore_case(destino->nombre, "EXIT"))
    {
        sem_post(&hay_proceso_exec);
    } */  // SI LA COLA EXEC LA MANEJA UN HILO PROPIO QUE SE ENCARGA DE LIBERAR LOS RECURSOS, LE DAMOS AVISO ACA
    
}

t_pcb *pop_estado(t_estado* estado){
    pthread_mutex_lock(&(estado->mutex_cola));
        t_pcb* pcb = list_remove(estado->cola, 0);
    pthread_mutex_unlock(&(estado->mutex_cola));

    return pcb;
}

void push_estado(t_estado* estado, t_pcb* pcb){
    pthread_mutex_lock(&(estado->mutex_cola));
        list_add(estado->cola, pcb);
    pthread_mutex_unlock(&(estado->mutex_cola));

}

int algoritmo_es_VRR(){
    return (string_equals_ignore_case(config_kernel.ALGORITMO_PLANIFICACION, "VRR")) ? 1 : 0;
}

// ---------- FUNCIONES AUXILIARES


void _loggear_ingreso_ready(){
    pthread_mutex_lock(&(ready->mutex_cola));
    char* lista_de_pids = __armar_lista_pids();
    pthread_mutex_unlock(&(ready->mutex_cola));

    pthread_mutex_lock(&mutex_log);
        log_info(kernel_log, "Cola Ready: [%s]", lista_de_pids);
    pthread_mutex_unlock(&mutex_log);

    free(lista_de_pids);
}

char* __armar_lista_pids() {
    int size = list_size(ready->cola);
    char* lista = string_duplicate(""); // El máximo tamaño de un número de 2 dígitos + coma + carácter nulo 
    
    if (size < 1)
    {
        char *vacia = string_duplicate("");
        return vacia;
    }

    for (int i = 0; i < size; i++) {
        t_pcb* pcb = list_get(ready->cola, i);
        int pid = pcb->pid;
        
        char* num_to_string = string_itoa(pid); // Convierte el PID a cadena

        // Agrega un espacio antes y después del número
        string_append(&lista, " ");
        string_append(&lista, num_to_string);
        string_append(&lista, " ");

        free(num_to_string);
    }
    return lista;
}

