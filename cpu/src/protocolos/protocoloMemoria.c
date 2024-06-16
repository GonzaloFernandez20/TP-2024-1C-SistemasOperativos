#include <protocolos/protocoloMemoria.h>
#include "protocoloMemoria.h"


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
		return "OPCODE ERROR";	// 	VER DSP CÓMO NOS CONVIENE MANEJAR EL ERROR 
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

	return _recibir_respuesta_por_ajuste_de_tamanio();
}

bool _recibir_respuesta_por_ajuste_de_tamanio(void) {
	int codigo_operacion = recibir_operacion(fd_conexion_memoria); // por convención nos comunicamos usando paquetes, por eso debemos recibir la operación primero, a pesar de que no vayamos a usarlo.
    if(codigo_operacion != ESTADO){ 
		perror("El mensaje recibido no es un estado.");
		return "OPCODE ERROR";	// 	VER DSP CÓMO NOS CONVIENE MANEJAR EL ERROR 
	}

	t_buffer *buffer = recibir_buffer(fd_conexion_memoria);
	void* stream = buffer->stream;

	int estado_respuesta = buffer_read_int(&stream);

	eliminar_buffer(buffer);

	return estado_respuesta;
}


uint32_t leer_de_memoria(uint32_t direccion_logica) {
	uint32_t direccion_fisica = dl_a_df(direccion_logica);	// convertimos la dirección lógica a física 
	_solicitar_lectura_de_memoria(direccion_fisica); 		// pedimos a memoria que nos devuelva el string alojado en la dirección física especificada.
	uint32_t dato_recibido = _recibir_entero_por_lectura();
	log_info(cpu_log, "PID: %d - Acción: LEER - Dirección Física: %d - Valor: %s", PID, direccion_fisica, dato_recibido);
	return dato_recibido;
}

//Pedido de lectura de una dirección física
void _solicitar_lectura_de_memoria(uint32_t direccion_fisica) {
	
    t_paquete* paquete = crear_paquete(READ);

    size_t buffer_size = sizeof(direccion_fisica);
    crear_buffer(paquete, buffer_size);

    buffer_add_uint32(paquete->buffer, direccion_fisica);
    
    enviar_paquete(paquete, fd_conexion_memoria);
    
    eliminar_paquete(paquete);
}

//Recepción de lo que había en la dirección física
char* _recibir_string_por_lectura(void) {
    int operacion = recibir_operacion(fd_conexion_memoria);

    t_buffer* buffer = recibir_buffer(fd_conexion_memoria);
    void* stream = buffer->stream;
    size_t string_length = buffer_read_int(&stream); //revisar si se envia el \0 desde memoria
    char* string_recibido = buffer_read_string(&stream, string_length);

    eliminar_buffer(buffer);

    return string_recibido; 
}
uint32_t _recibir_entero_por_lectura(void) {
    int operacion = recibir_operacion(fd_conexion_memoria);

    t_buffer* buffer = recibir_buffer(fd_conexion_memoria);
    void* stream = buffer->stream;
    uint32_t string_recibido = buffer_read_uint32(&stream);

    eliminar_buffer(buffer);

    return string_recibido; 
}

int escribir_string_en_memoria(uint32_t direccion_logica, char* string_a_escribir) {
	
	log_info(cpu_log, "PID: %d - Acción: ESCRIBIR - Dirección Física: %d - Valor: %s", PID, direccion_fisica, string_a_escribir);
	
	uint32_t direccion_fisica = dl_a_df(direccion_logica);
	_solicitar_escritura_string_en_memoria(direccion_fisica, string_a_escribir);

    char* respuesta_peticion = _recibir_respuesta_por_escritura();	// nos dice si todo "OK" o si hubo "ERROR".

	if (!strcmp(respuesta_peticion, "OK")) {
		log_info(cpu_log_debug, "ESCRITURA EXITOSA!");
	} else {
		log_info(cpu_log_debug, "ESCRITURA FALLIDA!");
	}	
}
int escribir_uint8_t_en_memoria(uint32_t direccion_logica, uint8_t entero_a_escribir) {

	log_info(cpu_log, "PID: %d - Acción: ESCRIBIR - Dirección Física: %d - Valor: %s", PID, direccion_fisica, entero_a_escribir);
	
	uint32_t direccion_fisica = dl_a_df(direccion_logica);
	_solicitar_escritura_uint8_t_en_memoria(direccion_fisica, entero_a_escribir);

    char* respuesta_peticion = _recibir_respuesta_por_escritura();	// nos dice si todo "OK" o si hubo "ERROR".


	
}
void escribir_uint32_t_en_memoria(uint32_t direccion_logica, uint32_t entero_a_escribir) {
	int contador_ok = 0;
	uint32_t* prt_entero_a_escribir = malloc(sizeof(uint32_t));
	*prt_entero_a_escribir = entero_a_escribir;
	for(int offset = 0 ; offset < sizeof(uint32_t);offset++){
		uint32_t direccion_fisica = dl_a_df(direccion_logica + offset);
		_solicitar_escritura_uint8_t_en_memoria(direccion_fisica, *(uint8_t)(prt_entero_a_escribir + offset));
	    int respuesta_peticion = _recibir_respuesta_por_escritura();	// nos dice si todo "OK" o si hubo "ERROR".
		if(respuesta_peticion){
			contador_ok++;
		}		
	}
	uint32_t direccion_fisica = dl_a_df(direccion_logica);
	if(contador_ok == 4){ // se realizaron con éxito las 4 escrituras en memoria
		log_info(cpu_log, "PID: %d - Acción: ESCRIBIR - Dirección Física: %d - Valor: %s", PID, direccion_fisica, entero_a_escribir);
	}
	else{
		log_error(cpu_log)
	}

	free(prt_entero_a_escribir);
}

// pasamos la direccion fisica en donde queremos guardar el string.
void _solicitar_escritura_uint8_t_en_memoria(uint32_t direccion_fisica, uint32_t entero) {
	t_paquete* paquete = crear_paquete(WRITE);

    int buffer_size = sizeof(direccion_fisica)+sizeof(entero);

    crear_buffer(paquete, buffer_size);

    buffer_add_uint8(paquete->buffer, direccion_fisica);
    buffer_add_string(paquete->buffer, entero);

    enviar_paquete(paquete, fd_conexion_memoria);
    
    eliminar_paquete(paquete);
}
void _solicitar_escritura_string_en_memoria(uint32_t direccion_fisica, char* string) {
	t_paquete* paquete = crear_paquete(WRITE);

    int buffer_size = sizeof(direccion_fisica)+sizeof(int)+strlen(string)+1;

    crear_buffer(paquete, buffer_size);

    buffer_add_uint32(paquete->buffer, direccion_fisica);
    buffer_add_int(paquete->buffer, strlen(string)+1);
    buffer_add_string(paquete->buffer, string);

    enviar_paquete(paquete, fd_conexion_memoria);
    
    eliminar_paquete(paquete);
}

int _recibir_respuesta_por_escritura(void) {
	int operacion = recibir_operacion(fd_conexion_memoria);
	if(operacion!=ESTADO) {
		perror("El mensaje recibido no es un ESTADO.");
		return 0;
	}
	
    t_buffer* buffer = recibir_buffer(fd_conexion_memoria);
    void* stream = buffer->stream;
    size_t string_length = buffer_read_int(&stream);
    char* respuesta_peticion = buffer_read_string(&stream, string_length);

    eliminar_buffer(buffer);

	if (!strcmp(respuesta_peticion, "OK")) {
		free(respuesta_peticion);
		return 1; 
	} 
	free(respuesta_peticion);
	return 0;
}

uint32_t consultar_marco_en_TP(uint32_t nro_pagina) {
	_solicitar_busqueda_de_marco_en_TP(nro_pagina);
	return _recibir_marco();
}

void _solicitar_busqueda_de_marco_en_TP(uint32_t nro_pagina) {
	t_paquete* paquete = crear_paquete(OBTENER_NRO_MARCO);

	size_t buffer_size = sizeof(nro_pagina);
    crear_buffer(paquete, buffer_size);

	buffer_add_int(paquete->buffer, PID);			// le paso el pid del proceso actual.
    buffer_add_uint32(paquete->buffer, nro_pagina);	// le paso el número de página del cual quiero saber su marco.

    enviar_paquete(paquete, fd_conexion_memoria);
    
    eliminar_paquete(paquete);
}

uint32_t _recibir_marco(void) {
	op_code operacion = recibir_operacion(fd_conexion_memoria);
	if(operacion != NRO_MARCO) {
		perror("El mensaje recibido no es un NRO_MARCO.");
		return -1;  // para q haga overflow y rompa todo.
	}

    t_buffer* buffer = recibir_buffer(fd_conexion_memoria);
    void* stream = buffer->stream;
    uint32_t nro_marco = buffer_read_uint32(&stream);

    eliminar_buffer(buffer);

    return nro_marco;
} 

// bool checkear_operacion(op_code codigo_de_operacion) {
// 	op_code operacion = recibir_operacion(fd_conexion_memoria);
// 	if(operacion != codigo_de_operacion) {
// 		perror("El mensaje recibido no es un NRO_MARCO.");
// 		return 0;	// medio raro, ver cómo cambiar esto después
// 	}
// }