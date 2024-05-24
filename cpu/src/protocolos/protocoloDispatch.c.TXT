#include <protocolos/protocoloDispatch.h>
#include 

/**
 * 
 * ENUNCIADO:
 * 
 * Durante el transcurso de la ejecución de un proceso, 
 * se irá actualizando su Contexto de Ejecución, que luego 
 * será devuelto al Kernel bajo los siguientes escenarios: 
 * finalización del mismo (instrucción EXIT o ante un error), 
 * solicitar una llamada al Kernel, o deber ser desalojado (interrupción).
 * 
*/

typedef struct {
    int codigo_operacion;
    t_buffer* buffer;
} t_paquete;

void *procesar_operacion_dispatch(void *fd_casteado){

    int fd = _deshacer_casting(fd_casteado);

    int cliente_conectado = 1;

    while(cliente_conectado){
		t_paquete* paquete = malloc(sizeof(t_paquete));
		paquete->buffer = malloc(sizeof(t_buffer));

        // Primero recibimos el codigo de operacion
		recibir_operacion(fd,paquete->codigo_operacion);
		// Después ya podemos recibir el buffer. Primero su tamaño seguido del contenido
		recibir_buffer(paquete);

		switch (paquete->codigo_operacion) {
		case un_PCB:		
			// Se recibe el PCB desde Kernel, lo cual interpretamos como una orden para ejecutar el proceso identificado por el PCB.
			struct_PCB* PCB_recibido = deserializar_PCB(paquete->buffer);
			// Se devuelve el Contexto de Ejecución (guardado en el PCB) actualizado al Kernel con motivo de la interrupción.
			enviar_PCB(PCB);

			free(PCB_recibido);
			break;
		
		case -1: 
			// cierra la conexión 
			log_error(cpu_log_debug, "KERNEL-DISPATCH se desconecto");
            cliente_conectado = 0;
			dispatch_conectado = 0;
			break;

		default:
			log_warning(cpu_log_debug,"Operacion desconocida de KERNEL-DISPATCH");
			break;
		}

		// Liberamos memoria
		free(paquete->buffer->stream);
		free(paquete->buffer);
		free(paquete);
	}
	return (void *)EXIT_FAILURE;
}

void recibir_buffer(t_paquete* t_paquete){
		recv(fd, &(paquete->buffer->size), sizeof(uint32_t), 0);
		paquete->buffer->stream = malloc(paquete->buffer->size);
		recv(fd, paquete->buffer->stream, paquete->buffer->size, 0);
}

void recibir_operacion(int socket_cliente, int* cod_op_pauete)
{
	int cod_op;
	if(recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) > 0)
		*cod_op_pauete = cod_op;
	else
	{
		close(socket_cliente);
		*cod_op_pauete = -1;
	}
}

struct_PCB* deserializar_PCB(t_buffer* buffer)
{
	struct_PCB* PCB_recibido = malloc(sizeof(struct_PCB));
	int* size = read_buffer_unint32_t(buffer);
	//EL CODIGO DE OPERACION TUVO QUE HABER SIDO LEIDO DEL BUFFER DE ANTEMANO
	PCB_recibido->PID = read_buffer_unint32_t(buffer);
	PCB_recibido->CPU = read_buffer_unint32_t(buffer);
	PCB_recibido->registros->PC  = read_buffer_uint32_t(buffer);  
    PCB_recibido->registros->AX  = read_buffer_uint8_t (buffer);   
    PCB_recibido->registros->BX  = read_buffer_uint8_t (buffer);
    PCB_recibido->registros->CX  = read_buffer_uint8_t (buffer);
    PCB_recibido->registros->DX  = read_buffer_uint8_t (buffer);
    PCB_recibido->registros->EAX = read_buffer_uint32_t(buffer);
    PCB_recibido->registros->EBX = read_buffer_uint32_t(buffer);
    PCB_recibido->registros->ECX = read_buffer_uint32_t(buffer); 
    PCB_recibido->registros->EDX = read_buffer_uint32_t(buffer); 
    PCB_recibido->registros->SI  = read_buffer_uint32_t(buffer);
    PCB_recibido->registros->DI  = read_buffer_uint32_t(buffer);
	return PCB_recibido;
}
unint32_t read_buffer_unint_t(t_buffer* buffer){
	unint32_t datoEnBuffer;
	memcpy(&datoEnBuffer, stream, sizeof(uint32_t));
    buffer->stream += sizeof(uint32_t);
	return datoEnBuffer;
}
unint8_t read_buffer_unint_t(t_buffer* buffer){
	unint8_t datoEnBuffer;
	memcpy(&datoEnBuffer, stream, sizeof(uint8_t));
    buffer->stream += sizeof(uint8_t);
	return datoEnBuffer;
}