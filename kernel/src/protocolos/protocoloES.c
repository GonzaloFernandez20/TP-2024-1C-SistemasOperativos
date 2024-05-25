#include <protocolos/protocoloES.h>

void *procesar_conexion_es(void *nombre_interfaz){

	char* nombre = strdup((char*)nombre_interfaz);
	
	t_interfaz* interfaz = dictionary_get(interfaces_conectadas, nombre);

    int cliente_conectado = 1;

    while(cliente_conectado){
        int cod_op = recibir_operacion(interfaz->fd);

		switch (cod_op) {
		case OPERACION_COMPLETADA:
			recibir_aviso(nombre, interfaz->fd);
			break;
		
		case -1:

			pthread_mutex_lock(&mutex_log_debug);
			log_error(kernel_log_debugg, "ENTRADA/SALIDA se desconecto\n");
			pthread_mutex_unlock(&mutex_log_debug);
			sacar_interfaz_de_diccionario(nombre);
            cliente_conectado = 0;
			break;

		default:
			pthread_mutex_lock(&mutex_log_debug);
			log_warning(kernel_log_debugg,"Operacion desconocida de ENTRADA/SALIDA");
			pthread_mutex_unlock(&mutex_log_debug);
			break;
		}
	} 
	return (void *)EXIT_FAILURE;
}

void sacar_interfaz_de_diccionario(char* nombre_interfaz){

	t_interfaz* interfaz = dictionary_remove(interfaces_conectadas, nombre_interfaz);

	free(interfaz->bloqueados->nombre);
	//pasa los procesos bloqueados en la cola a exit
	list_destroy(interfaz->bloqueados->cola);
	pthread_mutex_destroy(&interfaz->bloqueados->mutex_cola);
	free(interfaz->tipo);
	free(interfaz->bloqueados);
	free(interfaz);
}

void solicitar_operacion_IO_GEN_SLEEP(int PID, int fd, int parametro){
    t_paquete* paquete = crear_paquete(IO_GEN_SLEEP);

	int buffer_size = 2 * sizeof(int); 
	crear_buffer(paquete, buffer_size);


	buffer_add_int(paquete->buffer, PID);
	buffer_add_int(paquete->buffer, parametro);

	enviar_paquete(paquete, fd);
	eliminar_paquete(paquete);
}

void recibir_aviso(char* nombre, int fd){
	t_buffer *buffer = recibir_buffer(fd);
	void* stream = buffer->stream;
	
	int PID = buffer_read_int(&stream);
	eliminar_buffer(buffer);

	pthread_mutex_lock(&mutex_log_debug);
	log_info(kernel_log_debugg, "Operacion %s completada", nombre);
	pthread_mutex_unlock(&mutex_log_debug);
	
		//sabiendo el pid buscas en el dicc con la nombre y accedes a su t_estado 
		// buscar en su cola el pid y pasarlo de blocked a ready
}

