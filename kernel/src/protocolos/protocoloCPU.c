#include <protocolos/protocoloCPU.h>

void enviar_contexto_ejecucion(t_pcb* pcb){
    t_paquete* paquete = crear_paquete(CONTEXTO_EJECUCION);

    int buffer_size = 2*sizeof(int) + 4*sizeof(uint8_t) + 7*sizeof(uint32_t); 
	crear_buffer(paquete, buffer_size);

    buffer_add_int(paquete->buffer, pcb->pid );
    buffer_add_uint32(paquete->buffer, pcb->PC );
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
    pthread_mutex_unlock(&mutex_log_debug);

	eliminar_paquete(paquete);
}

void recibir_contexto_ejecucion(t_pcb* pcb){

    int opcode = recibir_operacion(fd_conexion_dispatch);

    if (_es_algoritmo_con_quantum() && !termino_quantum){ 
        pthread_cancel(manejo_quantum);    
    }

    if(opcode != CONTEXTO_EJECUCION){perror("Rompiste todo");}

	t_buffer *buffer = recibir_buffer(fd_conexion_dispatch);
	void* stream = buffer->stream;
    
    //Actualizamos el PCB guardando su contexto de ejecucion
	pcb->pid = buffer_read_int(&stream);
    pcb->PC = buffer_read_uint32(&stream);
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

    pthread_mutex_lock(&mutex_log_debug);
	    log_info(kernel_log_debugg, "CPU devolvio el contexto de ejecucion de PID < %d >\n", pcb->pid);
	pthread_mutex_unlock(&mutex_log_debug);

    verificar_estado_planificacion(); // ANTES DE VER QUE HACER CON EL PCB DEVUELTO, SE CLAVA SI ES NECESARIO

    interpretar_motivo_desalojo(pcb, stream);

	eliminar_buffer(buffer);
}

int _es_algoritmo_con_quantum(void){
    return strcmp(config_kernel.ALGORITMO_PLANIFICACION, "RR") || strcmp(config_kernel.ALGORITMO_PLANIFICACION, "VRR");
}