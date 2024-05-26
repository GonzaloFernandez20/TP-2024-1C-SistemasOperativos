#include <planificacion/desalojo.h>


// --------------- PROCESAMOS EL PEDIDO DE IO POR PARTE DE CPU


void interpretar_motivo_desalojo(t_pcb* pcb, void* stream){
    int op_code_motivo_desalojo = buffer_read_int(&stream);
    switch (op_code_motivo_desalojo)
    {
        case FIN_DE_QUANTUM:
            trasladar(pcb->pid, exec, ready);
            break;

        case INTERRUPCION:
            trasladar(pcb->pid, exec, estado_exit); // ENTIENDO QUE ES CUANDO SE INGRESA FINALIZAR_PROCESO POR CONSOLA.
            break;

        case LLAMADA_IO:
            ejecutar_llamada_io(pcb, stream);
            break;

        case EXIT:
            trasladar(pcb->pid, exec, estado_exit);
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
            trasladar(pcb->pid, exec, estado_exit);
            free(nombre_interfaz);
        }
        else{

            t_peticion* nueva_peticion = malloc(sizeof(t_peticion));
            nueva_peticion->unidades_trabajo = unidades_trabajo;
            nueva_peticion->funcion = solicitar_operacion_IO_GEN_SLEEP;

            pthread_mutex_lock(&diccionario_peticiones);
                dictionary_put(peticiones_interfaz, string_itoa(pcb->pid), nueva_peticion);
            pthread_mutex_unlock(&diccionario_peticiones);

            pthread_mutex_lock(&diccionario_interfaces);
                t_interfaz *interfaz = dictionary_get(interfaces_conectadas, nombre_interfaz);
            pthread_mutex_unlock(&diccionario_interfaces);

            trasladar(pcb->pid, exec, interfaz->bloqueados);

            sem_post(&interfaz->hay_peticiones);

            free(nombre_interfaz);
        }
}

// --------------- FUNCIONES AUXILIARES

int validar_peticion(char* interfaz, op_code_instruccion llamada){
    pthread_mutex_lock(&diccionario_interfaces);
        t_interfaz *interfaz_solicitante = dictionary_get(interfaces_conectadas, interfaz);
    pthread_mutex_unlock(&diccionario_interfaces);
 
    if (interfaz_solicitante == NULL){
        return 0;
    } 

    //int operaciones_permitidas[] = dictionary_get(instrucciones_por_interfaz, (interfaz_solicitante->tipo));
    //operaciones_permitidas = dictionary_get(instrucciones_por_interfaz, (interfaz_solicitante->tipo));

    if (_puede_realizar_operacion(llamada, dictionary_get(instrucciones_por_interfaz, (interfaz_solicitante->tipo))))
    {return 1; }
    else{return 0;}
}

int _puede_realizar_operacion(int numero, int array[]) {
    int i = 0;
    while (array[i] != -1) {
        if (array[i] == numero) {
            return 1; // El elemento está presente en el array
        }
        i++;
    }
    return 0;
}