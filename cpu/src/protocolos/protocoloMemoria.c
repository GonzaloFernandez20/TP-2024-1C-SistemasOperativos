#include <protocolos/protocoloMemoria.h>


///// Peticion de instruccion//////////////////////////////////////////////////////////////////////////////////////////////
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
		perror("El mensaje recibido no es una instruccion.");	
	}

	t_buffer *buffer = recibir_buffer(fd_conexion_memoria);
	void* stream = buffer->stream;

	int length = buffer_read_int(&stream);

    char* instruccion = malloc(length);
	strcpy(instruccion, buffer_read_string(&stream, length));

	eliminar_buffer(buffer);

	return instruccion;                
}

/// Peticion marco ////////////////////////////////////////////////////////////////////////////////////////
int consultar_marco_en_TP(int nro_pagina) {
	t_paquete* paquete = crear_paquete(OBTENER_NRO_MARCO);

	int buffer_size = 2* sizeof(int);
    crear_buffer(paquete, buffer_size);

	buffer_add_int(paquete->buffer, PID);			// le paso el pid del proceso actual.
    buffer_add_int(paquete->buffer, nro_pagina);	// le paso el número de página del cual quiero saber su marco.

    enviar_paquete(paquete, fd_conexion_memoria);
    
    eliminar_paquete(paquete);
	
	return recibir_marco();
}

int recibir_marco(void) {
	op_code operacion = recibir_operacion(fd_conexion_memoria);
	if(operacion != MARCO) {
		perror("El mensaje recibido no es un NRO_MARCO.");
	}

    t_buffer* buffer = recibir_buffer(fd_conexion_memoria);
    void* stream = buffer->stream;

    int nro_marco = buffer_read_int(&stream);

    eliminar_buffer(buffer);

    return nro_marco;
} 

///// Peticion de cambio de tamaño//////////////////////////////////////////////////////////////////////////////////////////////

int solicitar_ajustar_tamanio(int tamanio_nuevo){
	t_paquete* paquete = crear_paquete(AJUSTAR_TAMANIO);

	int buffer_size = 2*sizeof(int);
	crear_buffer(paquete, buffer_size);

	buffer_add_int(paquete->buffer, PID);   			// PID del proceso en ejecución
	buffer_add_int(paquete->buffer, tamanio_nuevo);    	// Tamaño nuevo al cual queremos cambiarle al proceso

	enviar_paquete(paquete, fd_conexion_memoria);
	eliminar_paquete(paquete);

	int respuesta;
	recv(fd_conexion_memoria, &respuesta, sizeof(int), MSG_WAITALL);

	return respuesta;
}

/////// Peticion de lectura //////////////////////////////////////////////////////////////////
void* leer_de_memoria(int direccion_fisica, int bytes) {
	peticion_lectura_memoria(direccion_fisica, bytes);

	int codigo_operacion = recibir_operacion(fd_conexion_memoria); // por convención nos comunicamos usando paquetes, por eso debemos recibir la operación primero, a pesar de que no vayamos a usarlo.
    if(codigo_operacion != VALOR_LEIDO){ perror("Rompiste todo.");}

	t_buffer *buffer = recibir_buffer(fd_conexion_memoria);
	void* stream = buffer->stream;

	int bytes_leidos = buffer_read_int(&stream);
	void* valor = malloc(bytes_leidos);
	valor = buffer_read_valor(&stream, bytes);

	eliminar_buffer(buffer);

	return valor;                
}

void peticion_lectura_memoria(int direccion_fisica, int bytes) {
    t_paquete* paquete = crear_paquete(LECTURA);

    int buffer_size = 3* sizeof(int);
    crear_buffer(paquete, buffer_size);

    buffer_add_int(paquete->buffer, PID);
	buffer_add_int(paquete->buffer, direccion_fisica);
	buffer_add_int(paquete->buffer, bytes);
    
    enviar_paquete(paquete, fd_conexion_memoria);
    
    eliminar_paquete(paquete);

}


//// Peticion escritura ////////////////////////////////////////////////////////////////////////////////////////////////
void escribir_en_memoria(void* particion, int bytes, int direccion_fisica){

    t_paquete* paquete = crear_paquete(ESCRITURA);

    int buffer_size = bytes + 3 * sizeof(int);

    crear_buffer(paquete, buffer_size);

    buffer_add_int(paquete->buffer, PID);
    buffer_add_int(paquete->buffer, direccion_fisica);
	buffer_add_int(paquete->buffer, bytes);
    buffer_add_valor(paquete->buffer, particion, bytes);

    enviar_paquete(paquete, fd_conexion_memoria);
    eliminar_paquete(paquete);
 
    int respuesta; // ME QUEDO ESPERANDO LA RESPUESTA DE PARTE DE MEMORIA ...
    recv(fd_conexion_memoria, &respuesta, sizeof(int), MSG_WAITALL); 
}