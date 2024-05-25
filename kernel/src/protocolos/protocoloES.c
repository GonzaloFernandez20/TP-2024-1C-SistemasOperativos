#include <protocolos/protocoloES.h>

void *procesar_conexion_es(void *nombre_interfaz){

	char* nombre = strdup((char*)nombre_interfaz);
	
	t_interfaz* interfaz = dictionary_get(interfaces_conectadas, nombre);

    int cliente_conectado = 1;

    while(cliente_conectado){
        int cod_op = recibir_operacion(interfaz->fd);

		switch (cod_op) {
		case MENSAJE:
			//
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
	free(interfaz->bloqueados);
	free(interfaz);
}