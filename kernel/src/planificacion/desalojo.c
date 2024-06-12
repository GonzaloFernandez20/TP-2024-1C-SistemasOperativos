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

        case LLAMADA_IO:
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
    
    default:
        break;
    }
}

void case_IO_GEN_SLEEP(t_pcb* pcb, void* stream){
    // Esta instrucción solicita al Kernel que se envíe a una interfaz de I/O a que realice un sleep por una cantidad de unidades de trabajo.
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