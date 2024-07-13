#include <protocolos/protocoloIO.h>


void* procesar_operacion_entradaSalida(void *nombre_interfaz){

    char* nombre = strdup((char*)nombre_interfaz);

	pthread_mutex_lock(&diccionario_interfaces);
		t_interfaz* interfaz = dictionary_get(interfaces_conectadas, nombre);
	pthread_mutex_unlock(&diccionario_interfaces);

	int cliente_conectado = 1;

    while(cliente_conectado){
        int cod_op = recibir_operacion(interfaz->fd);

		switch (cod_op) {
		case LECTURA:
			realizar_lectura(interfaz->fd);
			break;
			
		case ESCRITURA:
			realizar_escritura(interfaz->fd);
			break;
		
		case -1:
			pthread_mutex_lock(&mutex_log_debug);
			log_error(memoria_log_debugg, "%s se desconecto\n", nombre);
			pthread_mutex_unlock(&mutex_log_debug);
			sacar_interfaz_de_diccionario(nombre);
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

void sacar_interfaz_de_diccionario(char* nombre_interfaz){

	pthread_mutex_lock(&diccionario_interfaces);
		t_interfaz* interfaz = dictionary_remove(interfaces_conectadas, nombre_interfaz);
	pthread_mutex_unlock(&diccionario_interfaces);

	free(interfaz->nombre);
	free(interfaz);
}