#include <protocolos/protocoloCpu.h>

void *procesar_operacion_cpu(void *fd_cpu_casteado){

    int fd_cpu = _deshacer_casting(fd_cpu_casteado);

    int cliente_conectado = 1;

    while(cliente_conectado){
        int cod_op = recibir_operacion(fd_cpu);

		switch (cod_op) {
		case MENSAJE:
			//
			break;
		
		case -1:
			log_error(memoria_log_debugg, "CPU se desconecto");
            cliente_conectado = 0;
			return EXIT_FAILURE;
		default:
			log_warning(memoria_log_debugg,"Operacion desconocida de CPU");
			break;
		}
	}
	
}



    