#include <protocolos/protocoloCPU.h>

void enviar_pcb(t_pcb* pcb){
    t_paquete* paquete = crear_paquete(CONTEXTO_EJECUCION);

    int buffer_size = 12 * sizeof(int); 
	crear_buffer(paquete, buffer_size);

    buffer_add_int(paquete->buffer, pcb->pid );
    buffer_add_int(paquete->buffer, pcb->PC );
    buffer_add_int(paquete->buffer, pcb->registros_cpu.AX );
    buffer_add_int(paquete->buffer, pcb->registros_cpu.BX );
    buffer_add_int(paquete->buffer, pcb->registros_cpu.CX );
    buffer_add_int(paquete->buffer, pcb->registros_cpu.DX );
    buffer_add_int(paquete->buffer, pcb->registros_cpu.EAX );
    buffer_add_int(paquete->buffer, pcb->registros_cpu.EBX );
    buffer_add_int(paquete->buffer, pcb->registros_cpu.ECX );
    buffer_add_int(paquete->buffer, pcb->registros_cpu.EDX );
    buffer_add_int(paquete->buffer, pcb->registros_cpu.SI );
    buffer_add_int(paquete->buffer, pcb->registros_cpu.DI );

    enviar_paquete(paquete, fd_conexion_dispatch);

    pthread_mutex_lock(&mutex_log_debug);
        log_info(kernel_log_debugg, "Enviando PCB de PID: < %d > a CPU.", pcb->pid);
    pthread_mutex_lock(&mutex_log_debug);

	eliminar_paquete(paquete);
}

int recibir_pcb(t_pcb* pcb){

    int opcode = recibir_operacion(fd_conexion_dispatch);

    if (opcode != CONTEXTO_EJECUCION)
    {
        perror("NO SE RECIBIO LO QUE SE ESPERABA");
    }

	t_buffer *buffer = recibir_buffer(fd_conexion_dispatch);
	void* stream = buffer->stream;
    
	int op_code_motivo_desalojo = buffer_read_int(&stream);

	pcb->pid = buffer_read_int(&stream);
    pcb->PC = buffer_read_int(&stream);
    pcb->registros_cpu.AX = buffer_read_int(&stream);
    pcb->registros_cpu.BX = buffer_read_int(&stream);
    pcb->registros_cpu.CX = buffer_read_int(&stream);
    pcb->registros_cpu.DX = buffer_read_int(&stream);
    pcb->registros_cpu.EAX = buffer_read_int(&stream);
    pcb->registros_cpu.EBX = buffer_read_int(&stream);
    pcb->registros_cpu.ECX = buffer_read_int(&stream);
    pcb->registros_cpu.ECX = buffer_read_int(&stream);
    pcb->registros_cpu.SI = buffer_read_int(&stream);
    pcb->registros_cpu.DI = buffer_read_int(&stream);

    pthread_mutex_lock(&mutex_log_debug);
	log_info(logger, "CPU envio el contexto de ejecucion del proceso < %d >", pcb->pid);
	pthread_mutex_unlock(&mutex_log_debug);

	eliminar_buffer(buffer);

    return op_code_motivo_desalojo;
}