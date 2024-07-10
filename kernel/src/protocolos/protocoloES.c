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
	free(interfaz->nombre);
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

void solicitar_operacion_IO_STDIN_READ(t_peticion* peticion, int PID, int fd){
	t_paquete* paquete = crear_paquete(IO_STDIN_READ);
	int cant_direcciones = peticion->cant_direcciones;

	int buffer_size = 3 * sizeof(int) + 2 * cant_direcciones * sizeof(int); 
	crear_buffer(paquete, buffer_size);

	buffer_add_int(paquete->buffer, PID);
	buffer_add_int(paquete->buffer, peticion->tamanio_a_leer);
	buffer_add_int(paquete->buffer, cant_direcciones);

	for (int i = 0; i < cant_direcciones; i++)
    {
        t_datos_acceso *dato = list_remove(peticion->direcciones, 0);
        buffer_add_int(paquete->buffer, dato->bytes);
        buffer_add_int(paquete->buffer, dato->direccion_fisica);
        free(dato);
    }
	enviar_paquete(paquete, fd);
	eliminar_paquete(paquete);
}

void solicitar_operacion_IO_STDOUT_WRITE(t_peticion* peticion, int PID, int fd){
	t_paquete* paquete = crear_paquete(IO_STDOUT_WRITE);
	int cant_direcciones = peticion->cant_direcciones;

	int buffer_size = 3 * sizeof(int) + 2 * cant_direcciones * sizeof(int); 
	crear_buffer(paquete, buffer_size);

	buffer_add_int(paquete->buffer, PID);
	buffer_add_int(paquete->buffer, peticion->tamanio_a_leer);
	buffer_add_int(paquete->buffer, cant_direcciones);

	for (int i = 0; i < cant_direcciones; i++)
    {
        t_datos_acceso *dato = list_remove(peticion->direcciones, 0);
        buffer_add_int(paquete->buffer, dato->bytes);
        buffer_add_int(paquete->buffer, dato->direccion_fisica);
        free(dato);
    }

	enviar_paquete(paquete, fd);
	eliminar_paquete(paquete);
}

void solicitar_operacion_IO_FS_CREATE_DELETE(t_peticion* peticion, int PID, int fd){
	t_paquete* paquete = crear_paquete(peticion->tipo_operacion); // PUEDE SER CREATE O DELETE, EL RESTO ES IGUAL PARA AMBOS...
	int size = strlen(peticion->nombre_archivo) + 1;

	int buffer_size = 2 * sizeof(int) + size; 
	crear_buffer(paquete, buffer_size);

	buffer_add_int(paquete->buffer, PID);
	buffer_add_int(paquete->buffer, size);
	buffer_add_string(paquete->buffer, peticion->nombre_archivo);

	enviar_paquete(paquete, fd);
	eliminar_paquete(paquete);
}

void solicitar_operacion_IO_FS_TRUNCATE(t_peticion* peticion, int PID, int fd){
	t_paquete* paquete = crear_paquete(IO_FS_TRUNCATE);
	int size = strlen(peticion->nombre_archivo) + 1;

	int buffer_size = 3 * sizeof(int) + size; 
	crear_buffer(paquete, buffer_size);

	buffer_add_int(paquete->buffer, PID);
	buffer_add_int(paquete->buffer, size);
	buffer_add_string(paquete->buffer, peticion->nombre_archivo);
	buffer_add_int(paquete->buffer, peticion->registro_tamanio);

	enviar_paquete(paquete, fd);
	eliminar_paquete(paquete);
}
// (Interfaz, Nombre Archivo, Registro Dirección, Registro Tamaño, Registro Puntero Archivo)
void solicitar_operacion_IO_FS_WRITE_READ(t_peticion* peticion, int PID, int fd){
	
	t_paquete* paquete = crear_paquete(peticion->tipo_operacion);
	int size = strlen(peticion->nombre_archivo) + 1;
	int cant_direcciones = peticion->cant_direcciones;
	int buffer_size = (5 + 2 * cant_direcciones) * sizeof(int) + size; 
	crear_buffer(paquete, buffer_size);

	buffer_add_int(paquete->buffer, PID);
	buffer_add_int(paquete->buffer, size);
	buffer_add_string(paquete->buffer, peticion->nombre_archivo);
	buffer_add_int(paquete->buffer, peticion->registro_tamanio);
	buffer_add_int(paquete->buffer, peticion->reg_puntero_archivo);
	buffer_add_int(paquete->buffer, peticion->cant_direcciones);

	for (int i = 0; i < cant_direcciones; i++)
    {
        t_datos_acceso *dato = list_remove(peticion->direcciones, 0);
        buffer_add_int(paquete->buffer, dato->bytes);
        buffer_add_int(paquete->buffer, dato->direccion_fisica);
        free(dato);
    }

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
	
	verificar_estado_planificacion();
 	if (algoritmo_es_VRR()){  // --> PARA EL CASO DE VRR 
		trasladar(PID, interfaz->bloqueados, ready_plus);
	}else{ trasladar(PID, interfaz->bloqueados, ready); } 
	
}

