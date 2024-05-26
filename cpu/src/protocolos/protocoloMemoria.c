#include <protocolos/protocoloMemoria.h>


void solicitar_instruccion_a_memoria(int PC){
	t_paquete* paquete = crear_paquete(OBTENER_INSTRUCCION);

	int buffer_size = 2*sizeof(int); 
	crear_buffer(paquete, buffer_size);

	buffer_add_int(paquete->buffer, PCB.PID);   // PID del proceso en ejecución
	buffer_add_int(paquete->buffer, PC);        // PC de la instrucción que queremos del proceso

	enviar_paquete(paquete, fd_conexion_memoria);
	eliminar_paquete(paquete);
}

char* recibir_instruccion(void){
	int codigo_operacion = recibir_operacion(fd_conexion_memoria); // por convención nos comunicamos usando paquetes, por eso debemos recibir la operación primero.
    if(codigo_operacion!==INSTRUCCION){perror("No se recibio una instruccion");}

	t_buffer *buffer = recibir_buffer(fd_conexion_memoria);
	void* stream = buffer->stream;

	int length = buffer_read_int(&stream);

    char* instruccion = malloc(length);
	strcpy(instruccion, buffer_read_string(&stream, length));

	eliminar_buffer(buffer);

	return instruccion;                
}
