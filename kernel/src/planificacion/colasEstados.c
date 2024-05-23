#include <planificacion/colasEstados.h>


t_list *new;
t_list *ready;
t_list *exec;
t_list *estado_exit;
t_list *blocked;

pthread_mutex_t mutexBlocked;
pthread_mutex_t mutexNew;
pthread_mutex_t mutexReady;
pthread_mutex_t mutexExec;
pthread_mutex_t mutexExit;

sem_t proceso_listo;


void iniciar_colas_planificacion(void){
    new = list_create();
    ready = list_create();
    exec = list_create();
    estado_exit = list_create();
    blocked = list_create();
}

void imprimir_cola(t_list *cola, void(*_mostrar_pcbs)(void*)) {
    list_iterate(cola, _mostrar_pcbs);
}

void trasladar(int pid_buscado,  t_list *cola_origen, t_list *cola_destino){
    int size_origen = list_size(cola_origen);

    for (int i = 0; i < size_origen; i++) {
        t_pcb *pcb = list_get(cola_origen, i);
        if (pcb->pid == pid_buscado)
        {
            list_remove(cola_origen, i);
            _enviar_ready(pcb);
            break;
        }
    }
}

t_pcb *pop_estado(t_list* lista){
    t_pcb* pcb = list_get(lista, 0);
    list_remove(lista, 0);
    return pcb;
}

void _mostrar_pcbs(void *pcbDeLista) {
    t_pcb *pcb = (t_pcb *)pcbDeLista;
    printf("\t\tPID: %d\n", pcb->pid);
}

void _enviar_ready(t_pcb *pcb){
    pthread_mutex_lock(&mutexReady);
        list_add(ready, pcb);
    pthread_mutex_unlock(&mutexReady);
    sem_post(&proceso_listo);
}