#include <protocolos/protocoloKernel.h>

void *procesar_operacion_kernel(void *fd_kernel_casteado){
    
    int fd_kernel = _deshacer_casting(fd_kernel_casteado);

    int cliente_conectado = 1;

    while(cliente_conectado){
        int cod_op = recibir_operacion(fd_kernel);

		switch (cod_op) {
		case MENSAJE:
			//
			break;
		
		case -1:
			log_error(memoria_log_debugg, "KERNEL se desconecto");
            cliente_conectado = 0;
			return EXIT_FAILURE;
		default:
			log_warning(memoria_log_debugg,"Operacion desconocida de KERNEL");
			break;
		}
	}
    
}
