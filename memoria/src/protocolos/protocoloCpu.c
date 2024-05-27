#include <protocolos/protocoloCpu.h>

void *procesar_operacion_cpu(void *fd_cpu_casteado){

    fd_cpu = _deshacer_casting(fd_cpu_casteado);
	free(fd_cpu_casteado);

    int cliente_conectado = 1;

    while(cliente_conectado){
        int cod_op = recibir_operacion(fd_cpu);

		switch (cod_op) {
		case OBTENER_INSTRUCCION:
			devolver_instruccion();
			break;
		
		case -1:
			pthread_mutex_lock(&mutex_log_debug);
			log_error(memoria_log_debugg, "CPU se desconecto\n");
			pthread_mutex_unlock(&mutex_log_debug);

            cliente_conectado = 0;
			break;

		default:
			pthread_mutex_lock(&mutex_log_debug);
			log_warning(memoria_log_debugg,"Operacion desconocida de CPU");
			pthread_mutex_unlock(&mutex_log_debug);
			break;
		}
	}
	return (void *)EXIT_FAILURE;
}

void devolver_instruccion(void){
	t_buffer *buffer = recibir_buffer(fd_cpu);
	void* stream = buffer->stream;

	int PID = buffer_read_int(&stream);
	int PC = buffer_read_int(&stream);

	int indice_proceso = buscar_PID_memoria_instrucciones(PID);
	t_proceso* proceso_ejecutando = list_get(memoria_de_instrucciones, indice_proceso);

	char* instruccion = strdup(proceso_ejecutando->CODE_segmento[PC]);
	
	enviar_instruccion_serializada(instruccion);

	pthread_mutex_lock(&mutex_log_debug);
	log_info(memoria_log_debugg,"PID < %d > enviada la instruccion solicitada PC: %d -> %s\n", PID, PC, instruccion);
	pthread_mutex_unlock(&mutex_log_debug);
}

void enviar_instruccion_serializada(char* instruccion){
	t_paquete* paquete = crear_paquete(INSTRUCCION);

	int buffer_size = strlen(instruccion) + 1 + sizeof(int); 
	crear_buffer(paquete, buffer_size);


	buffer_add_int(paquete->buffer, strlen(instruccion) + 1 );
	buffer_add_string(paquete->buffer, instruccion);

	usleep(1000*config_memoria.RETARDO_RESPUESTA);
	enviar_paquete(paquete, fd_cpu);
	//free(instruccion);
	
	eliminar_paquete(paquete);
}