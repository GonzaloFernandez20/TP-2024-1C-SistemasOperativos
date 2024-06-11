#include <protocolos/protocoloDispatch.h>

void procesar_operacion_dispatch(void){
    int cliente_conectado = 1;

    while(cliente_conectado){
        int cod_op = recibir_operacion(fd_dispatch);

		switch (cod_op) {
			case CONTEXTO_EJECUCION:		
				recibir_contexto_ejecucion();
				ciclo();

				break;
			case -1: 
				log_error(cpu_log_debug, "KERNEL-DISPATCH se desconecto");
				cliente_conectado = 0;
				break;
			default:
				log_warning(cpu_log_debug,"Operacion desconocida de KERNEL-DISPATCH");
				break;
			}
	} 
}

// --------- ENVIO Y RECEPCION DE CONTEXTO DE EJECUCION CON KERNEL

void recibir_contexto_ejecucion() // PRIMER ENVIO DE CONTEXTO
{	
	t_buffer *buffer = recibir_buffer(fd_dispatch);
	void* stream = buffer->stream;

	PID            = buffer_read_int(&stream);
	registros.PC   = buffer_read_uint32(&stream);  
    registros.AX   = buffer_read_uint8(&stream);   
    registros.BX   = buffer_read_uint8(&stream);
    registros.CX   = buffer_read_uint8(&stream);
    registros.DX   = buffer_read_uint8(&stream);
    registros.EAX  = buffer_read_uint32(&stream);
    registros.EBX  = buffer_read_uint32(&stream);
    registros.ECX  = buffer_read_uint32(&stream); 
    registros.EDX  = buffer_read_uint32(&stream); 
    registros.SI   = buffer_read_uint32(&stream);
    registros.DI   = buffer_read_uint32(&stream);
    PTBR           = buffer_read_uint32(&stream);

	eliminar_buffer(buffer);

    log_info(cpu_log_debug, "Se recibio contexto de ejecucion PID: < %d > desde KERNEL.\n", PID);

}

void devolver_contexto_ejecucion(int motivo){
	t_paquete* paquete = crear_paquete(CONTEXTO_EJECUCION);

    int buffer_size = 3 * sizeof(int) + 4*sizeof(uint8_t) + 7*sizeof(uint32_t); 
	crear_buffer(paquete, buffer_size);

	buffer_add_int(paquete->buffer, PID );
    buffer_add_uint32(paquete->buffer, registros.PC );
    buffer_add_uint8(paquete->buffer, registros.AX );
    buffer_add_uint8(paquete->buffer, registros.BX );
    buffer_add_uint8(paquete->buffer, registros.CX );
    buffer_add_uint8(paquete->buffer, registros.DX );
    buffer_add_uint32(paquete->buffer, registros.EAX );
    buffer_add_uint32(paquete->buffer, registros.EBX );
    buffer_add_uint32(paquete->buffer, registros.ECX );
    buffer_add_uint32(paquete->buffer, registros.EDX );
    buffer_add_uint32(paquete->buffer, registros.SI );
    buffer_add_uint32(paquete->buffer, registros.DI );
    buffer_add_uint32(paquete->buffer, PTBR);
	buffer_add_int(paquete->buffer, motivo);

	enviar_paquete(paquete, fd_dispatch);

    log_info(cpu_log_debug, "Enviado contexto de ejecucion PID: < %d > a KERNEL.", PID);

	eliminar_paquete(paquete);
}

void devolver_contexto_ejecucion_IO_GEN_SLEEP(char* nombre_interfaz, int unidades_trabajo){
	int motivo = LLAMADA_IO;
    t_paquete* paquete = crear_paquete(CONTEXTO_EJECUCION);

    int buffer_size = 6 * sizeof(int) + 4*sizeof(uint8_t) + 7*(sizeof(uint32_t)) + strlen(nombre_interfaz) + 1; 
	crear_buffer(paquete, buffer_size);

	buffer_add_int(paquete->buffer, PID );
    buffer_add_uint32(paquete->buffer, registros.PC );
    buffer_add_uint8(paquete->buffer, registros.AX );
    buffer_add_uint8(paquete->buffer, registros.BX );
    buffer_add_uint8(paquete->buffer, registros.CX );
    buffer_add_uint8(paquete->buffer, registros.DX );
    buffer_add_uint32(paquete->buffer, registros.EAX );
    buffer_add_uint32(paquete->buffer, registros.EBX );
    buffer_add_uint32(paquete->buffer, registros.ECX );
    buffer_add_uint32(paquete->buffer, registros.EDX );
    buffer_add_uint32(paquete->buffer, registros.SI );
    buffer_add_uint32(paquete->buffer, registros.DI );
    buffer_add_uint32(paquete->buffer, PTBR);
	buffer_add_int(paquete->buffer, motivo);
	buffer_add_int(paquete->buffer, IO_GEN_SLEEP);
	buffer_add_int(paquete->buffer, strlen(nombre_interfaz) + 1);
	buffer_add_string(paquete->buffer, nombre_interfaz);
	buffer_add_int(paquete->buffer, unidades_trabajo);

	enviar_paquete(paquete, fd_dispatch);

    log_info(cpu_log_debug, "Enviando contexto de ejecucion PID: < %d > a KERNEL.", PID);

	eliminar_paquete(paquete);
}

