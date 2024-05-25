#include <protocolos/protocoloES.h>

void *procesar_conexion_es(void *nombre_interfaz){

	char* nombre = strdup((char*)nombre_interfaz);

	pthread_mutex_lock(&diccionario_interfaces);
		t_interfaz* interfaz = dictionary_get(interfaces_conectadas, nombre);
	pthread_mutex_unlock(&diccionario_interfaces);

    int cliente_conectado = 1;

    while(cliente_conectado){
        int cod_op = recibir_operacion(interfaz->fd);

		switch (cod_op) {
		case OPERACION_COMPLETADA:
			recibir_aviso(nombre, interfaz->fd);
			pthread_mutex_unlock(&(interfaz->interfaz_en_uso));
			break;
		
		case -1:

			pthread_mutex_lock(&mutex_log_debug);
			log_error(kernel_log_debugg, "%s se desconecto\n", nombre);
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

	pthread_mutex_lock(&diccionario_interfaces);
		t_interfaz* interfaz = dictionary_remove(interfaces_conectadas, nombre_interfaz);
	pthread_mutex_unlock(&diccionario_interfaces);

	mandar_procesos_a_exit(interfaz->bloqueados);
	free(interfaz->bloqueados->nombre);
	list_destroy(interfaz->bloqueados->cola);
	pthread_mutex_destroy(&interfaz->bloqueados->mutex_cola);

	free(interfaz->tipo);
	free(interfaz->bloqueados);
	pthread_mutex_destroy(&interfaz->interfaz_en_uso);
	sem_destroy(&interfaz->hay_peticiones);
	pthread_cancel(interfaz->peticiones);
	free(interfaz);
}

void mandar_procesos_a_exit(t_estado* bloqueados){
	int cantidad_procesos_bloqueados = list_size(bloqueados->cola);
	
	for(int i=0; i<cantidad_procesos_bloqueados; i++){
		t_pcb* PCB = list_get(bloqueados->cola, 0);
		trasladar(PCB->pid, bloqueados, estado_exit);
	}
}

void solicitar_operacion_IO_GEN_SLEEP(t_peticion* peticion, int PID, int fd){

    t_paquete* paquete = crear_paquete(IO_GEN_SLEEP);

	int buffer_size = 2 * sizeof(int); 
	crear_buffer(paquete, buffer_size);

	buffer_add_int(paquete->buffer, PID);
	buffer_add_int(paquete->buffer, peticion->unidades_trabajo);

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

	pthread_mutex_lock(&diccionario_interfaces);
		t_interfaz* interfaz = dictionary_get(interfaces_conectadas, nombre);
	pthread_mutex_unlock(&diccionario_interfaces);

	trasladar(PID, interfaz->bloqueados, ready);
}

