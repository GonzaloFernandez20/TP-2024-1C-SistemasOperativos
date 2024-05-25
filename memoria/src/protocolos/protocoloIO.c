#include <protocolos/protocoloIO.h>


void* procesar_operacion_entradaSalida(void* fd_IO_casteado){

    fd_IO = _deshacer_casting(fd_IO_casteado);
	free(fd_IO_casteado);

	int cliente_conectado = 1;

    while(cliente_conectado){
        int cod_op = recibir_operacion(fd_IO);

		switch (cod_op) {
		case MENSAJE:
			//
			break;
		
		case -1:
			pthread_mutex_lock(&mutex_log_debug);
			log_error(memoria_log_debugg, "un modulo de E/S se desconecto\n");
			pthread_mutex_unlock(&mutex_log_debug);

            cliente_conectado = 0;
			break;
			
		default:
			pthread_mutex_lock(&mutex_log_debug);
			log_warning(memoria_log_debugg,"Operacion desconocida de E/S");
			pthread_mutex_unlock(&mutex_log_debug);
			break;
		}
	}
	return (void* )EXIT_FAILURE;
}

