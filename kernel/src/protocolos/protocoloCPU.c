#include <protocolos/protocoloCPU.h>

void enviar_contexto_ejecucion(t_pcb* pcb){
    t_paquete* paquete = crear_paquete(CONTEXTO_EJECUCION);

    int buffer_size = 12 * sizeof(int); 
	crear_buffer(paquete, buffer_size);

    buffer_add_int(paquete->buffer, pcb->pid );
    buffer_add_int(paquete->buffer, pcb->PC );
    buffer_add_uint8(paquete->buffer, pcb->registros_cpu.AX );
    buffer_add_uint8(paquete->buffer, pcb->registros_cpu.BX );
    buffer_add_uint8(paquete->buffer, pcb->registros_cpu.CX );
    buffer_add_uint32(paquete->buffer, pcb->registros_cpu.DX );
    buffer_add_uint32(paquete->buffer, pcb->registros_cpu.EAX );
    buffer_add_uint32(paquete->buffer, pcb->registros_cpu.EBX );
    buffer_add_uint32(paquete->buffer, pcb->registros_cpu.ECX );
    buffer_add_uint32(paquete->buffer, pcb->registros_cpu.EDX );
    buffer_add_uint32(paquete->buffer, pcb->registros_cpu.SI );
    buffer_add_uint32(paquete->buffer, pcb->registros_cpu.DI );

    enviar_paquete(paquete, fd_conexion_dispatch);

    pthread_mutex_lock(&mutex_log_debug);
        log_info(kernel_log_debugg, "Enviando contexto de ejecucion PID: < %d > a CPU.", pcb->pid);
    pthread_mutex_lock(&mutex_log_debug);

	eliminar_paquete(paquete);
}

void recibir_contexto_ejecucion(t_pcb* pcb){

    int opcode = recibir_operacion(fd_conexion_dispatch);

    int es_RR = string_equals_ignore_case(config_kernel.ALGORITMO_PLANIFICACION, "RR");
    if (es_RR && !termino_quantum){ 
        pthread_cancel(manejo_quantum);     
    }

    if(opcode != CONTEXTO_EJECUCION){perror("Rompiste todo");}

    pthread_mutex_lock(&mutex_log_debug);
	    log_info(kernel_log_debugg, "CPU devolvio el contexto de ejecucion de PID < %d >", pcb->pid);
	pthread_mutex_unlock(&mutex_log_debug);

	t_buffer *buffer = recibir_buffer(fd_conexion_dispatch);
	void* stream = buffer->stream;
    
    //Actualizamos el PCB guardando su contexto de ejecucion
	pcb->pid = buffer_read_int(&stream);
    pcb->PC = buffer_read_int(&stream);
    pcb->registros_cpu.AX = buffer_read_uint8(&stream);
    pcb->registros_cpu.BX = buffer_read_uint8(&stream);
    pcb->registros_cpu.CX = buffer_read_uint8(&stream);
    pcb->registros_cpu.DX = buffer_read_uint8(&stream);
    pcb->registros_cpu.EAX = buffer_read_uint32(&stream);
    pcb->registros_cpu.EBX = buffer_read_uint32(&stream);
    pcb->registros_cpu.ECX = buffer_read_uint32(&stream);
    pcb->registros_cpu.EDX = buffer_read_uint32(&stream);
    pcb->registros_cpu.SI = buffer_read_uint32(&stream);
    pcb->registros_cpu.DI = buffer_read_uint32(&stream);

    interpretar_motivo_desalojo(pcb, &stream);

	eliminar_buffer(buffer);
}
