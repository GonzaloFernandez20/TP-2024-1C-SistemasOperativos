#include <planificacion/desalojo.h>

void interpretar_motivo_desalojo(t_pcb* pcb, t_motivo_desalojo motivo, void* stream){
    
    switch (motivo)
    {
        case FIN_DE_QUANTUM:
            /* code */
            break;
        case INTERRUPCION:
            /* code */
            break;
        case LLAMADA_IO:

            int op_code_io = buffer_read_int(&stream);
            ejecutar_llamada_io(pcb, op_code_io, stream);

            break;
        case EXIT:
            /* code */
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
        char* interfaz = strdup(buffer_read_string(&stream, size_interfaz));
        int unidades_trabajo = buffer_read_int(&stream);

        
        //Ejecutar llamado a la IO

        free(interfaz);
        break;
    
    default:
        break;
    }
}