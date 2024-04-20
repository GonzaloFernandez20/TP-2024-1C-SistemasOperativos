#include <protocolos/protocoloInterrupt.h>

void *procesar_operacion_interrupt(void *fd_interrupt_casteado){

    int fd_interrupt = _deshacer_casting(fd_interrupt_casteado);

    int cliente_conectado = 1;

    while(cliente_conectado){
        int cod_op = recibir_operacion(fd_interrupt);

		switch (cod_op) {
		case MENSAJE:
			//
			break;
		
		case -1:
			log_error(cpu_log_debug, "KERNEL-INTERRUPT se desconecto");
            cliente_conectado = 0;
			interrupt_conectado = 0;
			break;

		default:
			log_warning(cpu_log_debug,"Operacion desconocida de KERNEL-INTERRUPT");
			break;
		}
	}
	return (void *)EXIT_FAILURE;
}
