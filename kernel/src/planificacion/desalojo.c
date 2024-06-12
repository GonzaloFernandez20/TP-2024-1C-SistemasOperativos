#include <planificacion/desalojo.h>

#define INSTRUCCIONES_GENERICA IO_GEN_SLEEP, X
#define INSTRUCCIONES_STDIN IO_STDIN_READ, X
#define INSTRUCCIONES_STDOUT IO_STDOUT_WRITE, X
#define INSTRUCCIONES_DIALFS IO_FS_CREATE,IO_FS_DELETE, IO_FS_TRUNCATE, IO_FS_READ, IO_FS_READ, X

// --------------- PROCESAMOS EL PEDIDO DE IO POR PARTE DE CPU

void interpretar_motivo_desalojo(t_pcb* pcb, void* stream){
    int op_code_motivo_desalojo = buffer_read_int(&stream);
    switch (op_code_motivo_desalojo)
    {
        case FIN_DE_QUANTUM:
            trasladar(pcb->pid, exec, ready);
            break;

        case INTERRUPCION:
            trasladar(pcb->pid, exec, estado_exit);
            pthread_mutex_lock(&mutex_log);
                log_info(kernel_log, "Finaliza el proceso < %d > - Motivo: < INTERRUPTED_BY_USER >", pcb->pid);
            pthread_mutex_unlock(&mutex_log);
            break;

        case LLAMADA_IO: // DEBERIA LLAMARSE "OPERACION" POR EL TEMA DE WAIT Y SIGNAL? 
            ejecutar_llamada_io(pcb, stream);
            break;

        case EXIT:
            trasladar(pcb->pid, exec, estado_exit);

            pthread_mutex_lock(&mutex_log);
                log_info(kernel_log, "Finaliza el proceso < %d > - Motivo: < SUCCESS >", pcb->pid);
            pthread_mutex_unlock(&mutex_log);
            break;

        default:
            break;
    }
}

void ejecutar_llamada_io(t_pcb* pcb, void* stream){
    int op_code_io = buffer_read_int(&stream);

    switch (op_code_io)
    {
    case IO_GEN_SLEEP: // (Interfaz, Unidades de trabajo)
        case_IO_GEN_SLEEP(pcb, stream);
        break;
    case WAIT: // (Recurso)
        case_WAIT(pcb, stream, op_code_io);
        break;
    case SIGNAL: // (Recurso)
        case_SIGNAL(pcb, stream, op_code_io);
        break;
    
    default:
        break;
    }
}

void case_IO_GEN_SLEEP(t_pcb* pcb, void* stream){
    
        int size_interfaz = buffer_read_int(&stream);
        char* nombre_interfaz = strdup(buffer_read_string(&stream, size_interfaz));
        int unidades_trabajo = buffer_read_int(&stream);

        if(!validar_peticion(nombre_interfaz, IO_GEN_SLEEP)){
            pthread_mutex_lock(&mutex_log);
                log_info(kernel_log, "Finaliza el proceso < %d > - Motivo: < INVALID_INTERFACE >", pcb->pid);
            pthread_mutex_unlock(&mutex_log);
            trasladar(pcb->pid, exec, estado_exit);
            free(nombre_interfaz);
        }
        else{

            t_peticion* nueva_peticion = malloc(sizeof(t_peticion));
            nueva_peticion->unidades_trabajo = unidades_trabajo;
            nueva_peticion->funcion = solicitar_operacion_IO_GEN_SLEEP;

            char *pid_string = string_itoa(pcb->pid);

            pthread_mutex_lock(&diccionario_peticiones);
                dictionary_put(peticiones_interfaz, pid_string, nueva_peticion);
            pthread_mutex_unlock(&diccionario_peticiones);

            pthread_mutex_lock(&diccionario_interfaces);
                t_interfaz *interfaz = dictionary_get(interfaces_conectadas, nombre_interfaz);
            pthread_mutex_unlock(&diccionario_interfaces);

            trasladar(pcb->pid, exec, interfaz->bloqueados);

            sem_post(&interfaz->hay_peticiones);

            free(nombre_interfaz);
            free(pid_string);
        }
}

// TODO: Agregar las declaraciones al .h

void case_recurso(t_pcb* pcb, void* stream, int op_code){ 
    int size_nombre_recurso = buffer_read_int(&stream);
    char* nombre_recurso = strdup(buffer_read_string(&stream, size_nombre_recurso));

    if (existe_recurso(nombre_recurso))
    {   
        t_recurso *recurso = dictionary_get(recursos_disponibles, nombre_recurso);

        if (op_code == WAIT){ 
            case_WAIT(pcb, recurso);
        }else{ 
            case_SIGNAL(pcb, recurso);
        }
        /*
        *Tomo de un issue: al enviar wait o signal el proceso debe volver al kernel, y como bien lo indica el enunciado, en caso de signal vuelve el mismo proceso a ejecutar, en caso de wait si hay recursos disponibles vuelve a ejecutar sino, se bloquea
        !Propuesta:
        ? Podriamos hacer un mini switch que reanude la planificacion desde la parte de enviar el pcb a cpu total la funcion enviar_pcb y recibir_pcb es la misma, el switch viene por el lado de contemplar el quantum en el algoritmo. 
        !retomar_ejecucion(pcb);
        */
    }else
    {
        trasladar(pcb->pid, exec, estado_exit);
    }


}

/*
? A la hora de recibir de la CPU un Contexto de Ejecución desalojado por WAIT, el Kernel deberá verificar primero que exista el recurso solicitado y en caso de que exista restarle 1 a la cantidad de instancias del mismo. En caso de que el número sea estrictamente menor a 0, el proceso que realizó WAIT se bloqueará en la cola de bloqueados correspondiente al recurso.
*/

void case_WAIT(t_pcb* pcb, t_recurso *recurso){
        
        if(recurso->instancias_recursos > 0){
            recurso->instancias_recursos--;
        }else{
            trasladar(pcb->pid, exec, recurso->cola_recurso);
        }  
}
/*
? A la hora de recibir de la CPU un Contexto de Ejecución desalojado por SIGNAL, el Kernel deberá verificar primero que exista el recurso solicitado, luego sumarle 1 a la cantidad de instancias del mismo. En caso de que corresponda, desbloquea al primer proceso de la cola de bloqueados de ese recurso. Una vez hecho esto, se devuelve la ejecución al proceso que peticiona el SIGNAL.
*/
void case_SIGNAL(t_pcb* pcb, t_recurso *recurso){ //! No usa el pcb, deberia sacarlo
    /*
        Tomo de un issue: Buenas! "En caso de que corresponda" Seria el caso en el que hay al menos un elemento bloqueado en la cola, porque en caso de no haber ningún proceso tomando el recurso, solo se debe aumentar la cantidad de instancias del mismo en 1.
    */
    if (!list_is_empty(recurso->cola_recurso))
    {
        t_pcb* pcb = pop_estado(recurso->cola_recurso);
        // trasladar(pcb->pid, recurso->cola_recurso, ready); // Como no vuelve de IO no vuelve a Ready+
        // Tecnicamente al poder usar el recurso, el proceso queda liberado y deberia volver a ready
    }else{
        recurso->instancias_recursos++;
    }
}

// --------------- FUNCIONES AUXILIARES

int validar_peticion(char* interfaz, op_code_instruccion llamada){
    pthread_mutex_lock(&diccionario_interfaces);
        t_interfaz *interfaz_solicitante = dictionary_get(interfaces_conectadas, interfaz);
    pthread_mutex_unlock(&diccionario_interfaces);
 
    if (interfaz_solicitante != NULL){
        if (admite_operacion_solicitada(llamada, interfaz_solicitante->tipo))
        {
            return 1;
        }
    }
    return 0;
}

int admite_operacion_solicitada(op_code_instruccion instruccion, char* tipo){

    if(strcmp(tipo, "GENERICA") == 0){
        op_code_instruccion array[] = {INSTRUCCIONES_GENERICA};
        return _puede_realizar_operacion(instruccion, array);
    }
    else if(strcmp(tipo, "STDIN") == 0){
        op_code_instruccion array[] = {INSTRUCCIONES_STDIN};
        return _puede_realizar_operacion(instruccion, array);
    }
    else  if(strcmp(tipo, "STDOUT") == 0){
        op_code_instruccion array[] = {INSTRUCCIONES_STDOUT};
        return _puede_realizar_operacion(instruccion, array);
    }
    else  if(strcmp(tipo, "DIALFS") == 0){
        op_code_instruccion array[] = {INSTRUCCIONES_DIALFS};
        return _puede_realizar_operacion(instruccion, array);
    }
    else   
        return 0;
}

int _puede_realizar_operacion(op_code_instruccion numero, op_code_instruccion array[]) {
    int i = 0;
    while (array[i] != X) {
        if (array[i] == numero) {
            return 1; // El elemento está presente en el array
        }
        i++;
    }
    return 0;
}