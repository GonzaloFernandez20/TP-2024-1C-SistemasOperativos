#include <planificacion/PLPyPCP.h>

int pid_nuevo_proceso = 0; 

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
       
        pcbExecute = pop_estado(ready);
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

void *planificador_largo_plazo(void){

    while (1)
    {
    sem_wait(&proceso_cargado);
    sem_wait(&grado_multiprogramacion);

    // TODO: modificar grado de multiprogramacion

    t_pcb* pcb_a_cargar = list_get(new->cola, 0);

    // ------------------ COMUNICACION CON MEMORIA
    
    enviar_path_seudocodigo(pcb_a_cargar->path_pseudocodigo, pcb_a_cargar->pid);
    int rta_memoria = recibir_confirmacion(pcb_a_cargar->pid);

     if (!rta_memoria){
        pthread_mutex_lock(&mutex_log_debug); 
            log_error(kernel_log_debugg, "No se pudo crear el proceso :( ");
        pthread_mutex_lock(&mutex_log_debug); 
        return NULL; 
    } 

    // ------------------ AGREGA A COLA READY
    trasladar(pcb_a_cargar->pid, new, ready);
    // ------------------ PROCESO CREADO CON EXITO
    
    }

} 

// ----------- FUNCIONES PRINCIPALES







// ----------- FUNCIONES SECUNDARIAS


void iniciar_planificacion(void){
    // Creamos un hilo para el planificador de corto plazo y este va a mantener conexion con CPU 
    pthread_t corto_plazo;
    pthread_create(&corto_plazo, NULL, (void *)planificador_corto_plazo, NULL);
    pthread_detach(corto_plazo);
    // -------------------------------------------------
    pthread_mutex_lock(&mutex_log_debug);
    log_info(kernel_log_debugg, "Iniciado planificador de corto plazo");
    pthread_mutex_unlock(&mutex_log_debug);
    // -------------------------------------------------
    // Creamos un hilo para el planificador de largo plazo, encargado del grado de multiprogramacion 
    pthread_t largo_plazo;
    pthread_create(&largo_plazo, NULL, (void *)planificador_largo_plazo, NULL);
    pthread_detach(largo_plazo);
    // -------------------------------------------------
    pthread_mutex_lock(&mutex_log_debug);
    log_info(kernel_log_debugg, "Iniciado planificador de largo plazo");
    pthread_mutex_unlock(&mutex_log_debug);
    // -------------------------------------------------
}



void inicializar_semaforos(void){

    // ------- SEMAFOROS DE LOGS
    pthread_mutex_init(&mutex_log_debug, NULL);
    pthread_mutex_init(&mutex_log, NULL);

    // ------- SEMAFOROS DEL PLANIFICADOR DE CORTO PLAZO
    sem_init(&proceso_listo, 0, 0);
    sem_init(&execute_libre, 0, 1);
    sem_init(&proceso_cargado, 0, 0);
    sem_init(&grado_multiprogramacion, 0, config_kernel.GRADO_MULTIPROGRAMACION);
}

// ----------- FUNCIONES AUXILIARES


