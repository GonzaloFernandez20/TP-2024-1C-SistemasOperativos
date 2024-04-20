#include <protocolos/protocoloES.h>

void *procesar_conexion_es(void *fd_es_casteado){

    int fd_es = _deshacer_casting(fd_es_casteado);

    int cliente_conectado = 1;

    while(cliente_conectado){
        int cod_op = recibir_operacion(fd_es);

		switch (cod_op) {
		case MENSAJE:
			//
			break;
		
		case -1:
			log_error(kernel_log_debugg, "ENTRADA/SALIDA se desconecto");
            cliente_conectado = 0;
			break;

		default:
			log_warning(kernel_log_debugg,"Operacion desconocida de ENTRADA/SALIDA");
			break;
		}
	}
	return (void *)EXIT_FAILURE;
}