#include <protocolos/protocoloMemoria.h>


char* solicitar_instruccion_a_memoria(){
	t_paquete* paquete = crear_paquete(OBTENER_INSTRUCCION);

	int buffer_size = sizeof(int) + sizeof(uint32_t); 
	crear_buffer(paquete, buffer_size);

	buffer_add_int(paquete->buffer, PID);   // PID del proceso en ejecución
	buffer_add_uint32(paquete->buffer, registros.PC);    // PC de la instrucción que queremos del proceso

	enviar_paquete(paquete, fd_conexion_memoria);
	eliminar_paquete(paquete);

	return _recibir_instruccion();
}

char* _recibir_instruccion(void){
	int codigo_operacion = recibir_operacion(fd_conexion_memoria); // por convención nos comunicamos usando paquetes, por eso debemos recibir la operación primero, a pesar de que no vayamos a usarlo.
    if(codigo_operacion != INSTRUCCION){ 
		perror("No se recibio una instruccion");
		return "ERROR";	// 	VER DSP CÓMO NOS CONVIENE MANEJAR EL ERROR 
	}

	t_buffer *buffer = recibir_buffer(fd_conexion_memoria);
	void* stream = buffer->stream;

	int length = buffer_read_int(&stream);

    char* instruccion = malloc(length);
	strcpy(instruccion, buffer_read_string(&stream, length));

	eliminar_buffer(buffer);

	return instruccion;                
}

// Solicitud a Memoria para cambiar tamaño del proceso (identicado por PID) al tamaño nuevo especificado.
bool solicitar_ajustar_tamanio_de_proceso_a_memoria(int tamanio_nuevo) {
	t_paquete* paquete = crear_paquete(CAMBIO_TAMANIO);

	int buffer_size = 2*sizeof(int);
	crear_buffer(paquete, buffer_size);

	buffer_add_int(paquete->buffer, PID);   			// PID del proceso en ejecución
	buffer_add_int(paquete->buffer, tamanio_nuevo);    	// Tamaño nuevo al cual queremos cambiarle al proceso

	enviar_paquete(paquete, fd_conexion_memoria);
	eliminar_paquete(paquete);

	return recibir_respuesta_por_ajuste_de_tamanio();
}

bool recibir_respuesta_por_ajuste_de_tamanio(void) {
	int codigo_operacion = recibir_operacion(fd_conexion_memoria); // por convención nos comunicamos usando paquetes, por eso debemos recibir la operación primero, a pesar de que no vayamos a usarlo.
    if(codigo_operacion != ESTADO){ 
		perror("No se recibio un estado");
		return "ERROR";	// 	VER DSP CÓMO NOS CONVIENE MANEJAR EL ERROR 
	}

	t_buffer *buffer = recibir_buffer(fd_conexion_memoria);
	void* stream = buffer->stream;

	int estado_respuesta = buffer_read_int(&stream);

	eliminar_buffer(buffer);

	return estado_respuesta; 
}
