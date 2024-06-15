#include <protocolos/protocoloCpu.h>

void *procesar_operacion_cpu(void *fd_cpu_casteado){

    fd_cpu = _deshacer_casting(fd_cpu_casteado);
	free(fd_cpu_casteado);

    int cliente_conectado = 1;

    while(cliente_conectado){
        int cod_op = recibir_operacion(fd_cpu);

		usleep(1000*config_memoria.RETARDO_RESPUESTA);

		switch (cod_op) {
		case OBTENER_INSTRUCCION:
			devolver_instruccion();
			break;

		case AJUSTAR_TAMANIO:
			ajustar_tamanio_proceso();
			break;

		case OBTENER_NRO_MARCO:
			acceso_a_tabla_paginas();
			break;
		
		case LECTURA:
			realizar_lectura(fd_cpu);
			break;
		
		case ESCRITURA:
			realizar_escritura(fd_cpu);
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

/////////////// OBTENER INSTRUCCION ////////////////////////////////////////////////////////////////////////////////
void devolver_instruccion(void){
	t_buffer *buffer = recibir_buffer(fd_cpu);
	void* stream = buffer->stream;

	int PID = buffer_read_int(&stream);
	int PC = buffer_read_int(&stream);

	int indice_proceso = buscar_PID_memoria_instrucciones(PID);

	pthread_mutex_lock(&mutex_memoria_instrucciones);
		t_proceso* proceso_ejecutando = list_get(memoria_de_instrucciones, indice_proceso);
	pthread_mutex_unlock(&mutex_memoria_instrucciones);

	char* instruccion = proceso_ejecutando->CODE_segmento[PC];
	
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
	
	enviar_paquete(paquete, fd_cpu);
	
	eliminar_paquete(paquete);
}

////////////// RESIZE //////////////////////////////////////////////////////////////////////////////////////////////////

void ajustar_tamanio_proceso(void){
	t_buffer *buffer = recibir_buffer(fd_cpu);
	void* stream = buffer->stream;

	int PID = buffer_read_int(&stream);
	int tamanio = buffer_read_int(&stream);

	int indice_tabla = buscar_PID_tablas_paginas(PID);

	pthread_mutex_lock(&mutex_tablas_de_paginas);
		t_tabla_paginas* tabla = list_get(tablas_de_paginas, indice_tabla);
	pthread_mutex_unlock(&mutex_tablas_de_paginas);

	int cantidad_paginas = list_size(tabla->tabla_paginas);
	int tamanio_actual = cantidad_paginas * config_memoria.TAM_PAGINA;

	if(tamanio_actual<tamanio){
		int tamanio_a_ampliar = tamanio-tamanio_actual;

		pthread_mutex_lock(&mutex_log);
			log_info(memoria_log, "PID: < %d > - Tamaño Actual: < %d > - Tamaño a Ampliar: < %d >", PID, tamanio_actual, tamanio_a_ampliar);
		pthread_mutex_unlock(&mutex_log);

		ampliar_proceso(tabla->tabla_paginas,tamanio_a_ampliar);
	}
	else{
		int tamanio_a_reducir = tamanio_actual-tamanio;

		pthread_mutex_lock(&mutex_log);
			log_info(memoria_log, "PID: < %d > - Tamaño Actual: < %d > - Tamaño a Reducir: < %d >", PID, tamanio_actual, tamanio_a_reducir);
		pthread_mutex_unlock(&mutex_log);
		
		reducir_proceso(tabla->tabla_paginas,tamanio_a_reducir);
	}
	
}

void ampliar_proceso(t_list* tabla, int tamanio_a_ampliar){
	int paginas_necesarias = (tamanio_a_ampliar + (config_memoria.TAM_PAGINA - 1)) / config_memoria.TAM_PAGINA ;
	
	if(paginas_necesarias <= marcos_libres()){
		
		for(int i=0; i < paginas_necesarias; i++){
			int marco = buscar_marco_libre();

			pthread_mutex_lock(&mutex_bitarray);
				bitarray_set_bit(bitarray, marco);
			pthread_mutex_unlock(&mutex_bitarray);

			t_pagina* nueva_pagina = malloc(sizeof(t_pagina));
			nueva_pagina->marco = marco;
			list_add(tabla, nueva_pagina);
		}

		int mensaje_ok = 1;
		send(fd_cpu, &mensaje_ok, sizeof(int), 0);
	}
	else{

		int mensaje_error = 0;
		send(fd_cpu, &mensaje_error, sizeof(int), 0);
	}
}

void reducir_proceso(t_list* tabla, int tamanio_a_reducir){
	int paginas_eliminar = tamanio_a_reducir/config_memoria.TAM_PAGINA;
	
	liberar_marcos(paginas_eliminar, tabla);

	int mensaje_ok = 1;
	send(fd_cpu, &mensaje_ok, sizeof(int), 0);
}

/////////////////// ACCESO A TABLA DE PAGINAS /////////////////////////////////////////////////////////////////

void acceso_a_tabla_paginas(void){
	t_buffer *buffer = recibir_buffer(fd_cpu);
	void* stream = buffer->stream;

	int PID = buffer_read_int(&stream);
	int pagina = buffer_read_int(&stream);

	int indice_tabla = buscar_PID_tablas_paginas(PID);

	pthread_mutex_lock(&mutex_tablas_de_paginas);
		t_tabla_paginas* tabla = list_get(tablas_de_paginas, indice_tabla);
	pthread_mutex_unlock(&mutex_tablas_de_paginas);

	t_pagina* pagina_del_marco = list_get(tabla->tabla_paginas, pagina);

	pthread_mutex_lock(&mutex_log);
		log_info(memoria_log, "PID: < %d > - Pagina: < %d > - Marco: < %d >", PID, pagina,pagina_del_marco->marco);
	pthread_mutex_unlock(&mutex_log);

	enviar_marco(pagina_del_marco->marco);
}

void enviar_marco(int marco){
	t_paquete* paquete = crear_paquete(MARCO);

	int buffer_size =  sizeof(int); 
	crear_buffer(paquete, buffer_size);

	buffer_add_int(paquete->buffer, marco);
	
	enviar_paquete(paquete, fd_cpu);
	
	eliminar_paquete(paquete);
}