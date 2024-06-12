#include <planificacion/PLPyPCP.h>

int pid_nuevo_proceso = 0; 


// ----------- PLANIFICADOR CORTO PLAZO

void *planificador_corto_plazo(){
    //desalojadoAntes = true;
    //pcbInicial = true;
    t_pcb *pcb_execute;
    t_algoritmo algoritmo_planificacion = _chequear_algoritmo();
    int *pidsito = malloc(sizeof(int));

    pthread_mutex_lock(&mutex_log_debug); // -> Capaz ni es necesario
        log_info(kernel_log_debugg, "Algoritmo de planificacion: < %s >", config_kernel.ALGORITMO_PLANIFICACION);
    pthread_mutex_unlock(&mutex_log_debug);

    while (1)
    {       
        sem_wait(&proceso_listo);
        //sem_wait(&planificacionPausada);
        //sem_post(&planificacionPausada);

       switch (algoritmo_planificacion)
       {
            case FIFO:
                    pcb_execute = pop_estado(ready);
                    push_estado(exec, pcb_execute);

                    pthread_mutex_lock(&mutex_log);
                        log_info(kernel_log, "PID: <%d> - Estado Anterior: < READY > - Estado Actual: < EXEC >", pcb_execute->pid);
                    pthread_mutex_unlock(&mutex_log);

                    enviar_contexto_ejecucion(pcb_execute);
                    recibir_contexto_ejecucion(pcb_execute);
                break;
            case RR:
                    termino_quantum = 0;
                    
                    pcb_execute = pop_estado(ready);
                    push_estado(exec, pcb_execute);

                    pthread_mutex_lock(&mutex_log);
                        log_info(kernel_log, "PID: <%d> - Estado Anterior: < READY > - Estado Actual: < EXEC >", pcb_execute->pid);
                    pthread_mutex_unlock(&mutex_log);

                    enviar_contexto_ejecucion(pcb_execute);

                    *pidsito = pcb_execute->pid;

                    // THREAD QUE CONTROLA EL QUANTUM PARA UN PROCESO EN EJECUCION
                    pthread_create(&manejo_quantum, NULL, (void *)iniciar_quantum, (void *)pidsito);
                    pthread_detach(manejo_quantum);

                    recibir_contexto_ejecucion(pcb_execute);
            
                break;
            case VRR:
                    termino_quantum = 0;

                    //Chequeo si hay proceso en la cola ready+ 
                    if(list_is_empty(ready_plus->cola)){
                        pcb_execute = pop_estado(ready);
                        push_estado(exec, pcb_execute);
                    }else{
                        pcb_execute = pop_estado(ready_plus);
                        push_estado(exec, pcb_execute);
                    }

                    enviar_contexto_ejecucion(pcb_execute);

                    *pidsito = pcb_execute->pid;

                    //THREAD QUE CONTROLA EL QUANTUM PARA UN PROCESO EN EJECUCION
                    pthread_create(&manejo_quantum, NULL, (void *)iniciar_quantum, (void *)pidsito);
                    pthread_detach(manejo_quantum);

                    recibir_contexto_ejecucion(pcb_execute);
                break;
            
            default:
                break;
       } 
    }
}

// ----------- PLANIFICADOR LARGO PLAZO

void *planificador_largo_plazo(void){ // DIVIDIDO EN 2 PARTES: UNA PARA LLEVAR PROCESOS A READY Y OTRA PARA SACARLOS DE LA MEMORIA

    pthread_t garbage_collector;
    pthread_create(&garbage_collector, NULL, (void *)limpieza_cola_exit, NULL);
    pthread_detach(garbage_collector);

    while (1)
    {
        sem_wait(&proceso_cargado);
        sem_wait(&grado_multiprogramacion);

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
    }
} 

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
    pthread_mutex_init(&diccionario_interfaces, NULL);
    pthread_mutex_init(&diccionario_peticiones, NULL);

    // ------- SEMAFOROS DEL PLANIFICADOR DE CORTO PLAZO
    sem_init(&proceso_listo, 0, 0);
    sem_init(&execute_libre, 0, 1);
    sem_init(&proceso_cargado, 0, 0);
    sem_init(&grado_multiprogramacion, 0, config_kernel.GRADO_MULTIPROGRAMACION);

    sem_init(&hay_proceso_exit, 0, 0);
}

void *iniciar_quantum(void* PID_PROCESO){

    int PID = _deshacer_casting(PID_PROCESO);
    free(PID_PROCESO);

    pthread_mutex_lock(&mutex_log_debug);
        log_info(kernel_log_debugg, "Proceso < %d > comienza su Quantum", PID);
    pthread_mutex_unlock(&mutex_log_debug);

    usleep(config_kernel.QUANTUM * 2000);
    termino_quantum = 1;
    enviar_interrupcion(FIN_DE_QUANTUM);

    pthread_mutex_lock(&mutex_log);
        log_info(kernel_log, "PID: %d interrumpido por fin de Quantum", PID);
    pthread_mutex_unlock(&mutex_log);

    return NULL;
}  

// ----------- FUNCIONES AUXILIARES

t_algoritmo _chequear_algoritmo(void){
    if (string_equals_ignore_case(config_kernel.ALGORITMO_PLANIFICACION, "FIFO")) { return FIFO; }
    if (string_equals_ignore_case(config_kernel.ALGORITMO_PLANIFICACION, "RR"))   { return RR; }
    if (string_equals_ignore_case(config_kernel.ALGORITMO_PLANIFICACION, "VRR"))  { return VRR; } 

    return ERROR;  
}

void enviar_interrupcion(t_motivo_desalojo motivo){
    int motivo_interrupcion = motivo;
    send(fd_conexion_interrupt, &motivo_interrupcion, sizeof(int), 0);
}