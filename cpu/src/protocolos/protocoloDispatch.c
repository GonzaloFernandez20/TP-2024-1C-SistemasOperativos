#include <protocolos/protocoloDispatch.h>

void *procesar_operacion_dispatch(void *fd_dispatch_casteado){

    int fd_dispatch = _deshacer_casting(fd_dispatch_casteado);

    int cliente_conectado = 1;

    while(cliente_conectado){
        int cod_op = recibir_operacion(fd_dispatch);

		switch (cod_op) {
		case MENSAJE:
			//
			break;
		
		case -1:
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
