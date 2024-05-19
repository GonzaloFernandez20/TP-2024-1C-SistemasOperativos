#include <protocolos/protocoloDispatch.h>
#include 

/**
 * EN REALIDAD CPU NO VA A RECIBIR SOLICITUDES DE DISPATCH, 
 * SINO QUE EL DISPATCH VA A ENVIAR PCBs a KERNEL.
 * 
 * ENUNCIADO:
 * 
 * Durante el transcurso de la ejecución de un proceso, 
 * se irá actualizando su Contexto de Ejecución, que luego 
 * será devuelto al Kernel bajo los siguientes escenarios: 
 * finalización del mismo (instrucción EXIT o ante un error), 
 * solicitar una llamada al Kernel, o deber ser desalojado (interrupción).
 * 
*/


void *procesar_operacion_dispatch(void *fd_casteado){

    int fd = _deshacer_casting(fd_casteado);

    int cliente_conectado = 1;

    while(cliente_conectado){
        int cod_op = recibir_operacion(fd);

		switch (cod_op) {
		case MENSAJE:		
			// Se devuelve el Contexto de Ejecución (guardado en el PCB) actualizado al Kernel con motivo de la interrupción.

			break;
		
		case -1: 
			// cierra la conexión 
			log_error(cpu_log_debug, "KERNEL-DISPATCH se desconecto");
            cliente_conectado = 0;
			dispatch_conectado = 0;
			break;

		default:
			log_warning(cpu_log_debug,"Operacion desconocida de KERNEL-DISPATCH");
			break;
		}
	}
	return (void *)EXIT_FAILURE;
}


