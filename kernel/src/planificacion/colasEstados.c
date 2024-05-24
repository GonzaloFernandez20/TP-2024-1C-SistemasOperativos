#include <planificacion/colasEstados.h>

// ---------- DEFINICIONES DE VARIABLES

t_list *cola_new;
t_list *cola_ready;
t_list *cola_exec;
t_list *cola_estado_exit;
t_list *cola_blocked;


// ---------- DEFINICIONES DE FUNCIONES

void iniciar_colas_planificacion(void){
    cola_new = list_create();
    cola_ready = list_create();
    cola_exec = list_create();
    cola_estado_exit = list_create();
    cola_blocked = list_create();

    // Crear e inicializar la estructura t_estado para la cola new
    new = malloc(sizeof(t_estado));
    pthread_mutex_init(&(new->mutex_cola), NULL); 
    new->cola = cola_new;
    new->nombre = string_duplicate("NEW");

    // Crear e inicializar la estructura t_estado para la cola ready
    ready = malloc(sizeof(t_estado));
    pthread_mutex_init(&(ready->mutex_cola), NULL); 
    ready->cola = cola_ready;
    ready->nombre = string_duplicate("READY");

    // Crear e inicializar la estructura t_estado para la cola exec
    exec = malloc(sizeof(t_estado));
    pthread_mutex_init(&(exec->mutex_cola), NULL); 
    exec->cola = cola_exec;
    exec->nombre = string_duplicate("EXEC");

    // Crear e inicializar la estructura t_estado para la cola estado_exit
    estado_exit = malloc(sizeof(t_estado));
    pthread_mutex_init(&(estado_exit->mutex_cola), NULL); 
    estado_exit->cola = cola_estado_exit;
    estado_exit->nombre = string_duplicate("BLOCKED");

    // Crear e inicializar la estructura t_estado para la cola blocked
    blocked = malloc(sizeof(t_estado));
    pthread_mutex_init(&(blocked->mutex_cola), NULL); 
    blocked->cola = cola_blocked;
    blocked->nombre = string_duplicate("EXIT");

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
                //sem_post(&proceso_listo);
            }
}

t_pcb *pop_estado(t_estado* estado){
    pthread_mutex_lock(&(estado->mutex_cola));
        t_pcb* pcb = list_remove(estado->cola, 0);
    pthread_mutex_unlock(&(estado->mutex_cola));

    return pcb;
}

// TODO: t_pcb *push_estado(t_estado* estado){}


// ---------- FUNCIONES AUXILIARES

void _mostrar_pcbs(void *pcbDeLista) {
    t_pcb *pcb = (t_pcb *)pcbDeLista;
    printf("\t\tPID: %d\n", pcb->pid);
}

void _loggear_ingreso_ready(){
    pthread_mutex_lock(&(ready->mutex_cola));
    char* lista_de_pids = string_duplicate(__armar_lista_pids());
    pthread_mutex_unlock(&(ready->mutex_cola));

    pthread_mutex_lock(&mutex_log);
        log_info(kernel_log, "Cola Ready: [%s]", lista_de_pids);
    pthread_mutex_unlock(&mutex_log);

    free(lista_de_pids);
}

char* __armar_lista_pids() {
    int size = list_size(cola_ready);
    char* lista = string_duplicate(""); // El máximo tamaño de un número de 2 dígitos + coma + carácter nulo 
    
    if (size < 1)
    {
        char *vacia = string_duplicate("");
        return vacia;
    }

    for (int i = 0; i < size; i++) {
        char* cadena = malloc(sizeof(char) * 3); // Tamaño suficiente para un número de hasta 2 dígitos + carácter nulo

        t_pcb* pcb = list_get(cola_ready, i);
        sprintf(cadena, " %d ", pcb->pid);
        string_append(&lista, cadena);

        free(cadena);
    }
    return lista;
}
