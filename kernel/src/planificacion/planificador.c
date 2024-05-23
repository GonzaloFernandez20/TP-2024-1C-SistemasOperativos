#include <planificacion/planificador.h>

int pid_nuevo_proceso = 0; 

pthread_mutex_t mutex_log_debug;
pthread_mutex_t mutex_log;
sem_t execute_libre;

// ----------- PLANIFICADOR CORTO PLAZO

void *planificador_corto_plazo(){
    //desalojadoAntes = true;
    //pcbInicial = true;
    t_pcb *pcbExecute;

    while (1)
    {
        sem_wait(&proceso_listo);
        sem_wait(&execute_libre);
        //sem_wait(&planificacionPausada);
        //sem_post(&planificacionPausada);
       
        pcbExecute = pop_estado(ready->cola);
        printf("ENVIANDO PCB %d A CPU", pcbExecute->pid);

        pthread_mutex_lock(&mutex_log_debug);
        log_info(kernel_log_debugg, "ENVIANDO PCB %d A CPU", pcbExecute->pid);
        pthread_mutex_unlock(&mutex_log_debug);

        //enviar_pcb(pcbExecute);
        usleep(400000000);
        //recibirPCB modificado y motivo de desalojo
        //asignar tarea, algo tiene que hacer ahora, capaz conviene q siga otro hilo por la suya
        
        sem_post(&execute_libre); //avisa q la cpu ya no esta ejecutando
    }
}

// ----------- PLANIFICADOR LARGO PLAZO

/* void *planificador_largo_plazo(void){

    // ------------------ COMUNICACION CON MEMORIA
    // chequeo_multiprogramacion();

    //enviar_path_seudocodigo(path_proceso, pid_asignado);
    //int rta_memoria = recibir_confirmacion(pid_asignado);

     if (rta_memoria){
        pthread_mutex_lock(&mutex_log_debug); 
            log_error(kernel_log_debugg, "No se pudo crear el proceso :( ");
        pthread_mutex_lock(&mutex_log_debug); 
        return NULL; 
    } 

    // ------------------ AGREGA A COLA READY

    trasladar(pid_asignado, new, ready);
 
    // ------------------ PROCESO CREADO CON EXITO
}  */




// ----------- FUNCIONES PRINCIPALES

void* crear_proceso(void *path_proceso_void){

    //char *path_proceso = (char *)path_proceso_void;
    
    int pid_asignado = _asignar_PID();
    t_pcb *nuevo_pcb = crear_pcb(pid_asignado);

    // ------------------ AGREGA A COLA NEW 
    pthread_mutex_lock(&new->mutex_cola);
        list_add(new->cola, nuevo_pcb);
    pthread_mutex_unlock(&new->mutex_cola);

    pthread_mutex_lock(&mutex_log);
        log_info(kernel_log, "Se crea el proceso <%d> en NEW", nuevo_pcb->pid);
    pthread_mutex_unlock(&mutex_log);

    //sem_post(&proceso_cargado);
    trasladar(pid_asignado, new, ready);
    return NULL;
}


void extraer_proceso(int pid){

    t_list *array_colas[] = {cola_new, cola_ready, cola_exec, cola_estado_exit};
    pthread_mutex_t array_semaforos[] = {new->mutex_cola, ready->mutex_cola, exec->mutex_cola, estado_exit->mutex_cola};
    
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



// ----------- FUNCIONES SECUNDARIAS


void iniciar_planificacion(void){
    pthread_mutex_lock(&mutex_log_debug);
    log_info(kernel_log_debugg, "iniciando planificador de corto plazo");
    pthread_mutex_unlock(&mutex_log_debug);
    // -------------------------------------------------
    // Creamos un hilo para el planificador de corto plazo y este va a mantener conexion con CPU 
    pthread_t corto_plazo;
    pthread_create(&corto_plazo, NULL, (void *)planificador_corto_plazo, NULL);
    pthread_detach(corto_plazo);
    // -------------------------------------------------
}



void inicializar_semaforos(void){

    // ------- SEMAFOROS DE LOGS
    pthread_mutex_init(&mutex_log_debug, NULL);
    pthread_mutex_init(&mutex_log, NULL);

    // ------- SEMAFOROS DEL PLANIFICADOR DE CORTO PLAZO
    sem_init(&proceso_listo, 0, 0);
    sem_init(&execute_libre, 0, 1);
}

// ----------- FUNCIONES AUXILIARES

int _asignar_PID(void){
    return pid_nuevo_proceso++;
}//Lo uso y lo incremento para que otro PCB tenga un pid distinto.

