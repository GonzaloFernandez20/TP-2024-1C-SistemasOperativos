#include <planificacion/desalojo.h>


// --------------- PROCESAMOS EL PEDIDO DE IO POR PARTE DE CPU


void interpretar_motivo_desalojo(t_pcb* pcb, t_motivo_desalojo motivo, void* stream){
    
    switch (motivo)
    {
        case FIN_DE_QUANTUM:
            trasladar(pcb->pid, exec, ready);
        
            break;
        case INTERRUPCION:
            trasladar(pcb->pid, exec, exit); // ENTIENDO QUE ES CUANDO SE INGRESA FINALIZAR_PROCESO POR CONSOLA.
            break;
        case LLAMADA_IO:

            int op_code_io = buffer_read_int(&stream);
            ejecutar_llamada_io(pcb, op_code_io, stream);

            break;
        case EXIT:
            trasladar(pcb->pid, exec, exit);
            break;

        default:
            break;
    }
}


void ejecutar_llamada_io(t_pcb* pcb, int op_code, void* stream){

    switch (op_code)
    {
    case IO_GEN_SLEEP: // (Interfaz, Unidades de trabajo)
        // Esta instrucción solicita al Kernel que se envíe a una interfaz de I/O a que realice un sleep por una cantidad de unidades de trabajo.
        int size_interfaz = buffer_read_int(&stream);
        char* nombre_interfaz = strdup(buffer_read_string(&stream, size_interfaz));
        int unidades_trabajo = buffer_read_int(&stream);

        if(!validar_peticion(nombre_interfaz, IO_GEN_SLEEP)){
            trasladar(pcb->pid, exec, exit);
            free(nombre_interfaz);

            break;
        }
        // --------- EJECUTAR LLAMADO A IO

        t_interfaz *interfaz = dictionary_get(interfaces_conectadas, nombre_interfaz);

        trasladar(pcb->pid, exec, interfaz->bloqueados);

        sem_wait(&(interfaz->interfaz_en_uso));

        t_datos_operacion* datos = malloc(sizeof(t_datos_operacion));
        datos->PID = pcb->pid;
        datos->FD = interfaz->fd;
        datos->unidades_trabajo = unidades_trabajo;
        datos->interfaz = interfaz;

        // -------------------------------------------------
        // Creamos un hilo para reservar la cola durante una peticion sin afectar al PCP 
        pthread_t manejo_peticion;
        pthread_create(&manejo_peticion, NULL, (void *)solicitar_operacion_IO_GEN_SLEEP, (void *)datos);
        pthread_detach(manejo_peticion);
        // -------------------------------------------------

        //solicitar_operacion_IO_GEN_SLEEP(pcb->pid, interfaz->fd, (void *)datos);
        free(nombre_interfaz);

        break;
    
    default:
        break;
    }
}



// --------------- FUNCIONES AUXILIARES

int validar_peticion(char* interfaz, t_llamada_io llamada){

    t_interfaz *interfaz_solicitante = dictionary_get(interfaces_conectadas, interfaz)
 
    if (interfaz_solicitante == NULL){
        return 0;
    } 

    int *operaciones_permitidas;
    operaciones_permitidas = dictionary_get(instrucciones_por_interfaz, (interfaz_solicitante->tipo));

    if (_puede_realizar_operacion(llamada, operaciones_permitidas))
    {
        return 1;
    }else{
        return 0;
    }
}

int _puede_realizar_operacion(int numero, int array[]) {
    size_t longitud = sizeof(array) / sizeof(array[0]);

    for (int i = 0; i < longitud; i++) {
        if (array[i] == numero) {
            return 1; // Devuelve 1 si el número está en el array
        }
    }
    return 0; // Devuelve 0 si el número no está en el array
}
