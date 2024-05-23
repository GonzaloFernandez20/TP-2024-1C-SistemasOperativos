#include <planificacion/planificador.h>

int pid_nuevo_proceso = 0; 
t_list *new;
t_list *ready;
t_list *exec;
t_list *estado_exit;
t_list *blocked;

// ----------- SEMAFOROS
pthread_mutex_t mutex_cola;
pthread_mutex_t cola_ready;
sem_t hay_proceso_ready;


// ----------- PLANIFICADOR CORTO PLAZO

void *planificador_corto_plazo(){
    //desalojadoAntes = true;
    //pcbInicial = true;
    t_pcb *pcbExecute;

    while (1)
    {
        sem_wait(&hay_proceso_ready);
        //sem_wait(&execute_libre);
        //sem_wait(&planificacionPausada);
        //sem_post(&planificacionPausada);
       
        pcbExecute = pop_estado(ready);
        log_info(kernel_log_debugg, "ENVIANDO PCB %d A CPU", pcbExecute->pid);
        enviar_pcb(pcbExecute);

        //recibirPCB modificado y motivo de desalojo
        //asignar tarea, algo tiene que hacer ahora, capaz conviene q siga otro hilo por la suya
        
        //sem_post(&execute_libre); //avisa q la cpu ya no esta ejecutando
    }
}

// ----------- FUNCIONES PRINCIPALES

void* crear_proceso(void *path_proceso_void){
    pthread_mutex_init(&mutex_cola, NULL);

    //char *path_proceso = (char *)path_proceso_void;
    // LOG MINIMO: Creación de Proceso: "Se crea el proceso <PID> en NEW"

    int pid_asignado = _asignar_PID();
    t_pcb *nuevo_pcb = crear_pcb(pid_asignado);


    // ------------------ AGREGA A COLA NEW 
    pthread_mutex_lock(&mutex_cola);
        list_add(new, (void *)nuevo_pcb);
    pthread_mutex_unlock(&mutex_cola);

    //pthread_mutex_lock(log);
        //log_info(kernelLogger, "Se crea el proceso <%d> en NEW", nuevo_pcb->pid);
    //pthread_mutex_unlock(log);

    // ------------------ COMUNICACION CON MEMORIA
    // chequeo_multiprogramacion();

    //enviar_path(path_proceso, pid_asignado);

    //recibir_confirmacion(pid_asignado);

    // ------------------ AGREGA A COLA READY
    pthread_mutex_lock(&mutex_cola);
        trasladar(pid_asignado, new, ready);
    pthread_mutex_unlock(&mutex_cola);

    // ------------------ PROCESO CREADO CON EXITO
    //pthread_mutex_destroy(&mutex_cola);
    return NULL;
} 

void extraer_proceso(int pid){

    t_list *array_colas[] = {new, ready, exec, blocked};
    
    int cant_elementos = (int)(sizeof(array_colas) / sizeof(array_colas[0]));

    for (int i = 0; i < cant_elementos; i++)
    {
        if(buscar_y_eliminar_pid(array_colas[i], pid) != (-1)){
            break;
        }
    }
}

void iniciar_colas_planificacion(void){
    new = list_create();
    ready = list_create();
    exec = list_create();
    estado_exit = list_create();
    blocked = list_create();
    // list_create hace malloc, guarda con eso

    pthread_mutex_init(&mutex_cola, NULL);
    pthread_mutex_init(&cola_ready, NULL);

    sem_init(&hay_proceso_ready,0,0);
}

// ----------- FUNCIONES SECUNDARIAS

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


// ----------- FUNCIONES AUXILIARES

int _asignar_PID(void){
    return pid_nuevo_proceso++;
}//Lo uso y lo incremento para que otro PCB tenga un pid distinto.

void _mostrar_pcbs(void *pcbDeLista) {
    t_pcb *pcb = (t_pcb *)pcbDeLista;
    printf("\t\tPID: %d\n", pcb->pid);
}

void _enviar_ready(t_pcb *pcb){
    pthread_mutex_lock(&cola_ready);
        list_add(ready, pcb);
    pthread_mutex_unlock(&cola_ready);
    sem_post(&hay_proceso_ready);
}