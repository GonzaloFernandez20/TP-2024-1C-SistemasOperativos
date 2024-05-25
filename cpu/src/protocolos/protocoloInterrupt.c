#include <protocolos/protocoloInterrupt.h>

void *procesar_operacion_interrupt(void *fd_interrupt_casteado){

    int fd_interrupt = _deshacer_casting(fd_interrupt_casteado);

    int cliente_conectado = 1;

    while(cliente_conectado){
        int cod_op = recibir_operacion(fd_interrupt);

		switch (cod_op) {
		case MENSAJE:
			int PID_a_interrumpir = recibir_PID(fd_interrupt);			
			log_info(cpu_log_debug, "Kernel pide interrumpir proceso PID=%d", PID_a_interrumpir);
			if(PID_a_interrumpir == PCB.PID) {
				hayInterrupcion = 1; // debe ser variable global bool.
			}
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

int recibir_PID(int fd_interrupt) {
	t_buffer *buffer = recibir_buffer(fd_interrupt);

	int PID = buffer_read_int(&(buffer->stream));
	
	eliminar_buffer(buffer);

	return PID;
}