#include <protocolos/protocoloIO.h>


void *procesar_operacion_entradaSalida(void *fd_ent_sal_casteado){

    int fd_ent_sal = _deshacer_casting(fd_ent_sal_casteado);

    int cliente_conectado = 1;

    while(cliente_conectado){
        int cod_op = recibir_operacion(fd_ent_sal);

		switch (cod_op) {
		case MENSAJE:
			//
			break;
		
		case -1:
			log_error(memoria_log_debugg, "E/S se desconecto");
            cliente_conectado = 0;
			break;
			
		default:
			log_warning(memoria_log_debugg,"Operacion desconocida de E/S");
			break;
		}
	}
	return (void *)EXIT_FAILURE;
}

