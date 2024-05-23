#include <planificacion/planificador.h>

int pid_nuevo_proceso = 0; 
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
// ----------- PLANIFICADOR CORTO PLAZO

void *planificador_corto_plazo(){
    //desalojadoAntes = true;
    //pcbInicial = true;
    t_pcb *pcbExecute;

    while (1)
    {
        sem_wait(&proceso_listo);
        //sem_wait(&execute_libre);
        //sem_wait(&planificacionPausada);
        //sem_post(&planificacionPausada);
       
        pcbExecute = pop_estado(ready);
        printf("ENVIANDO PCB %d A CPU", pcbExecute->pid);
        //log_info(kernel_log_debugg, "ENVIANDO PCB %d A CPU", pcbExecute->pid);
        //enviar_pcb(pcbExecute);

        //recibirPCB modificado y motivo de desalojo
        //asignar tarea, algo tiene que hacer ahora, capaz conviene q siga otro hilo por la suya
        
        //sem_post(&execute_libre); //avisa q la cpu ya no esta ejecutando
    }
}

// ----------- FUNCIONES PRINCIPALES

void* crear_proceso(void *path_proceso_void){

    //char *path_proceso = (char *)path_proceso_void;
    // LOG MINIMO: Creación de Proceso: "Se crea el proceso <PID> en NEW"

    int pid_asignado = _asignar_PID();
    t_pcb *nuevo_pcb = crear_pcb(pid_asignado);


    // ------------------ AGREGA A COLA NEW 
    pthread_mutex_lock(&mutexNew);
        list_add(new, nuevo_pcb);
    pthread_mutex_unlock(&mutexNew);

    //pthread_mutex_lock(log);
        //log_info(kernelLogger, "Se crea el proceso <%d> en NEW", nuevo_pcb->pid);
    //pthread_mutex_unlock(log);

    // ------------------ COMUNICACION CON MEMORIA
    // chequeo_multiprogramacion();

    //enviar_path_seudocodigo(path_proceso, pid_asignado);
    //recibir_confirmacion(pid_asignado);

    // ------------------ AGREGA A COLA READY

    trasladar(pid_asignado, new, ready);
 

    // ------------------ PROCESO CREADO CON EXITO
    
    return NULL;
} 

void extraer_proceso(int pid){

    t_list *array_colas[] = {new, ready, exec, blocked};
    pthread_mutex_t array_semaforos[] = {mutexNew, mutexReady, mutexExec, mutexBlocked};
    
    int cant_elementos = (int)(sizeof(array_colas) / sizeof(array_colas[0]));
    int resultado;

    for (int i = 0; i < cant_elementos; i++)
    {   
        pthread_mutex_lock(&array_semaforos[i]);
            resultado = buscar_y_eliminar_pid(array_colas[i], pid);
        pthread_mutex_unlock(&array_semaforos[i]);
        
        if(resultado != (-1)){  break;  }
    }
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


void iniciar_planificacion(void){

    // -------------------------------------------------
    // Creamos un hilo para el planificador de corto plazo y este va a vivir junto con el main
    pthread_t corto_plazo;
    pthread_create(&corto_plazo, NULL, (void *)planificador_corto_plazo, NULL);
    pthread_detach(corto_plazo);
    // -------------------------------------------------
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

void inicializar_semaforos(void){

    // ------- SEMAFOROS DE LAS COLAS DE PROCESOS

    pthread_mutex_init(&mutexBlocked, NULL);
    pthread_mutex_init(&mutexNew, NULL);
    pthread_mutex_init(&mutexReady, NULL);
    pthread_mutex_init(&mutexExec, NULL);
    pthread_mutex_init(&mutexExit, NULL);


    // ------- SEMAFOROS DE LOGS


    // ------- SEMAFOROS DEL PLANIFICADOR DE CORTO PLAZO
    sem_init(&proceso_listo,0,0);
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
    pthread_mutex_lock(&mutexReady);
        list_add(ready, pcb);
    pthread_mutex_unlock(&mutexReady);
    sem_post(&proceso_listo);
}