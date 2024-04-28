#include <protocolos/protocoloES.h>

void *procesar_conexion_es(void *fd_es_casteado){

    int fd_es = _deshacer_casting(fd_es_casteado);

    int interfaz_conectada = 1;

    while(interfaz_conectada){
    	que_hace_kernel tipo_de_interfaz = recibir_operacion(fd_es);//global en algún .h
		t_list* datos_recibidos;//falta liberar
		switch (tipo_de_interfaz) {
		case IO_GEN_SLEEP:
		//NECESARIA PARA LA ENTREGA 2
			datos_recibidos = recibir_paquete(fd_es);
            log_info(kernel_log, "La interfaz IO respondio:\n");
			list_iterate(datos_recibidos, (void*) iterator);
			break;
		case IO_STDIN_READ:

			break;
    	case IO_STDOUT_WRITE:

			break;
		case -1:
			log_error(kernel_log_debugg, "ENTRADA/SALIDA se desconecto\n");
            cliente_conectado = 0;
			entrada_salida_conectada = 0;
			break;

		default:
			log_warning(kernel_log_debugg,"Operacion desconocida de ENTRADA/SALIDA");
			break;
		}
	}
	return (void *)EXIT_FAILURE;
}

