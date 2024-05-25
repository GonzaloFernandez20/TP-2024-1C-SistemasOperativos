#include <planificacion/PLPyPCP.h>

int pid_nuevo_proceso = 0; 


// ----------- PLANIFICADOR CORTO PLAZO

void *planificador_corto_plazo(){
    //desalojadoAntes = true;
    //pcbInicial = true;
    t_pcb *pcbExecute;
    t_algoritmo algoritmo_planificacion = _chequear_algoritmo();

    while (1)
    {   
        sem_wait(&proceso_listo);
        //sem_wait(&planificacionPausada);
        //sem_post(&planificacionPausada);
       switch (algoritmo_planificacion)
       {
            case FIFO:
                    pcbExecute = pop_estado(ready);
                    push_estado(exec, pcbExecute);

                    //enviar_pcb(pcbExecute);

                    pthread_mutex_lock(&mutex_log_debug);
                        log_info(kernel_log_debugg, "ENVIANDO PCB %d A CPU", pcbExecute->pid);
                    pthread_mutex_unlock(&mutex_log_debug);

                    usleep(35000000);
                    recibir_pcb(pcbExecute);

                    //trasladar(pcbExecute->pid, exec, blocked);
                    //interpretar_desalojo(op_code_desalojo, pcbExecute, arrayDeParametros);
                break;
            case RR:
                    termino_quantum = 0;
                    pcbExecute = pop_estado(ready);
                    push_estado(exec, pcbExecute);

                    //enviar_pcb(pcbExecute);

                    pthread_mutex_lock(&mutex_log_debug);
                        log_info(kernel_log_debugg, "ENVIANDO PCB %d A CPU", pcbExecute->pid);
                    pthread_mutex_unlock(&mutex_log_debug);

                    int *pidsito = malloc(sizeof(int));
                        *pidsito = pcbExecute->pid;
                    // THREAD QUE CONTROLA EL QUANTUM PARA UN PROCESO EN EJECUCION
                    pthread_t manejo_quantum;
                    pthread_create(&manejo_quantum, NULL, (void *)iniciar_quantum, (void *)pidsito);
                    pthread_detach(manejo_quantum);

                    usleep(1000*5000);
                    //int op_code_desalojo = recibir_pcb(pcbExecute);
                    if (termino_quantum == 0){ 
                        pthread_cancel(manejo_quantum); 

                        pthread_mutex_lock(&mutex_log_debug);
                            log_info(kernel_log_debugg, "CPU devolvio el contexto de PID: %d ", pcbExecute->pid);
                        pthread_mutex_unlock(&mutex_log_debug);
                    }

                    
                    //interpretar_desalojo(op_code_desalojo, pcbExecute);
                    trasladar(pcbExecute->pid, exec, blocked);

                    free(pidsito);
                break;
            case VRR:
                /* code */
                break;
            
            default:
                break;
       }
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
    
    /* enviar_path_seudocodigo(pcb_a_cargar->path_pseudocodigo, pcb_a_cargar->pid);
    int rta_memoria = recibir_confirmacion(pcb_a_cargar->pid);

     if (!rta_memoria){
        pthread_mutex_lock(&mutex_log_debug); 
            log_error(kernel_log_debugg, "No se pudo crear el proceso :( ");
        pthread_mutex_lock(&mutex_log_debug); 
        return NULL; 
    }  */

    // ------------------ AGREGA A COLA READY
    trasladar(pcb_a_cargar->pid, new, ready);
    // ------------------ PROCESO CREADO CON EXITO
    
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

    // ------- SEMAFOROS DEL PLANIFICADOR DE CORTO PLAZO
    sem_init(&proceso_listo, 0, 0);
    sem_init(&execute_libre, 0, 1);
    sem_init(&proceso_cargado, 0, 0);
    sem_init(&grado_multiprogramacion, 0, config_kernel.GRADO_MULTIPROGRAMACION);
}

void *iniciar_quantum(void* PID_PROCESO){

    int PID = _deshacer_casting(PID_PROCESO);

    pthread_mutex_lock(&mutex_log_debug);
        log_info(kernel_log_debugg, "Proceso < %d > comienza su Quantum", PID);
    pthread_mutex_unlock(&mutex_log_debug);

    usleep(config_kernel.QUANTUM * 2000);
    termino_quantum = 1;
    //enviar_interrupcion(fd_conexion_interrupt);

    pthread_mutex_lock(&mutex_log_debug);
        log_info(kernel_log_debugg, "PID: %d Desalojado por fin de Quantum", PID);
    pthread_mutex_unlock(&mutex_log_debug);

    return NULL;
}  

// ----------- FUNCIONES AUXILIARES

t_algoritmo _chequear_algoritmo(void){
    if (string_equals_ignore_case(config_kernel.ALGORITMO_PLANIFICACION, "FIFO")) { return FIFO; }
    if (string_equals_ignore_case(config_kernel.ALGORITMO_PLANIFICACION, "RR"))   { return RR; }
    if (string_equals_ignore_case(config_kernel.ALGORITMO_PLANIFICACION, "VRR"))  { return VRR; } 
    
    pthread_mutex_lock(&mutex_log_debug);
        log_info(kernel_log_debugg, "Algoritmo de planificacion: < %s >", config_kernel.ALGORITMO_PLANIFICACION);
    pthread_mutex_unlock(&mutex_log_debug);

    return ERROR;  
}
