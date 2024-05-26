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
	return (void *)EXIT_FAILURE;
}

// ---------------------------------------------------

void recibir_contexto_ejecucion()
{	
	t_buffer *buffer = recibir_buffer(fd_conexion_dispatch);
	void* stream = buffer->stream;

	PCB.PID            = buffer_read_int(&stream);
	PCB.PC             = buffer_read_int(&stream);  
    PCB.registros.AX   = buffer_read_uint8(&stream);   
    PCB.registros.BX   = buffer_read_uint8(&stream);
    PCB.registros.CX   = buffer_read_uint8(&stream);
    PCB.registros.DX   = buffer_read_uint8(&stream);
    PCB.registros.EAX  = buffer_read_uint32(&stream);
    PCB.registros.EBX  = buffer_read_uint32(&stream);
    PCB.registros.ECX  = buffer_read_uint32(&stream); 
    PCB.registros.EDX  = buffer_read_uint32(&stream); 
    PCB.registros.SI   = buffer_read_uint32(&stream);
    PCB.registros.DI   = buffer_read_uint32(&stream);

	eliminar_buffer(buffer);
}

void actualizarPCB(t_pcb* PCB_recibido){

    PCB.PID           = PCB_recibido->PID;
    PCB.Quantum       = PCB_recibido->Quantum;
    PCB.PC            = PCB_recibido->PC;
    PCB.registros.AX  = PCB_recibido->registros.AX;
    PCB.registros.BX  = PCB_recibido->registros.BX;
    PCB.registros.CX  = PCB_recibido->registros.CX;
    PCB.registros.DX  = PCB_recibido->registros.DX;
    PCB.registros.EAX = PCB_recibido->registros.EAX;
    PCB.registros.EBX = PCB_recibido->registros.EBX;
    PCB.registros.ECX = PCB_recibido->registros.ECX;
    PCB.registros.EDX = PCB_recibido->registros.EDX; 
    PCB.registros.SI  = PCB_recibido->registros.SI;
    PCB.registros.DI  = PCB_recibido->registros.DI;
}

void sync_PCB_registrosCPU(void){
    PCB.PC            = registrosCPU.PC;
    PCB.registros.AX  = registrosCPU.AX;
    PCB.registros.BX  = registrosCPU.BX;
    PCB.registros.CX  = registrosCPU.CX;
    PCB.registros.DX  = registrosCPU.DX;
    PCB.registros.EAX = registrosCPU.EAX;
    PCB.registros.EBX = registrosCPU.EBX;
    PCB.registros.ECX = registrosCPU.ECX;
    PCB.registros.EDX = registrosCPU.EDX; 
    PCB.registros.SI  = registrosCPU.SI;
    PCB.registros.DI  = registrosCPU.DI;
}

void devolver_contexto_ejecucion(){
    sync_PCB_registrosCPU();
	t_paquete* paquete = crear_paquete(CONTEXTO_EJECUCION);

    int buffer_size = 14 * sizeof(int); 
	crear_buffer(paquete, buffer_size);

	buffer_add_int(paquete->buffer, PCB.pid );
    buffer_add_int(paquete->buffer, PCB.PC );
    buffer_add_uint8(paquete->buffer, PCB.registros_cpu.AX );
    buffer_add_uint8(paquete->buffer, PCB.registros_cpu.BX );
    buffer_add_uint8(paquete->buffer, PCB.registros_cpu.CX );
    buffer_add_uint32(paquete->buffer, PCB.registros_cpu.DX );
    buffer_add_uint32(paquete->buffer, PCB.registros_cpu.EAX );
    buffer_add_uint32(paquete->buffer, PCB.registros_cpu.EBX );
    buffer_add_uint32(paquete->buffer, PCB.registros_cpu.ECX );
    buffer_add_uint32(paquete->buffer, PCB.registros_cpu.EDX );
    buffer_add_uint32(paquete->buffer, PCB.registros_cpu.SI );
    buffer_add_uint32(paquete->buffer, PCB.registros_cpu.DI );
	buffer_add_int(paquete->buffer, tipo_interrupcion);

	enviar_paquete(paquete, fd_dispatch);

    pthread_mutex_lock(&mutex_log_debug);
        log_info(cpu_log_debug, "Enviando contexto de ejecucion PID: < %d > a CPU.", PCB.pid);
    pthread_mutex_lock(&mutex_log_debug);

	eliminar_paquete(paquete);
}

void devolver_contexto_ejecucion_IO_GEN_SLEEP(char* nombre_interfaz, int unidades_trabajo){
    sync_PCB_registrosCPU();
	t_paquete* paquete = crear_paquete(CONTEXTO_EJECUCION);

    int buffer_size = 17 * sizeof(int) + strlen(nombre_interfaz) + 1; // RE-VER
	crear_buffer(paquete, buffer_size);

	buffer_add_int(paquete->buffer, PCB.pid );
    buffer_add_int(paquete->buffer, PCB.PC );
    buffer_add_uint8(paquete->buffer, PCB.registros_cpu.AX );
    buffer_add_uint8(paquete->buffer, PCB.registros_cpu.BX );
    buffer_add_uint8(paquete->buffer, PCB.registros_cpu.CX );
    buffer_add_uint32(paquete->buffer, PCB.registros_cpu.DX );
    buffer_add_uint32(paquete->buffer, PCB.registros_cpu.EAX );
    buffer_add_uint32(paquete->buffer, PCB.registros_cpu.EBX );
    buffer_add_uint32(paquete->buffer, PCB.registros_cpu.ECX );
    buffer_add_uint32(paquete->buffer, PCB.registros_cpu.EDX );
    buffer_add_uint32(paquete->buffer, PCB.registros_cpu.SI );
    buffer_add_uint32(paquete->buffer, PCB.registros_cpu.DI );
	buffer_add_int(paquete->buffer, LLAMADA_IO);
	buffer_add_int(paquete->buffer, IO_GEN_SLEEP);
	buffer_add_int(paquete->buffer, strlen(nombre_interfaz) + 1);
	buffer_add_string(paquete->buffer, nombre_interfaz);
	buffer_add_int(paquete->buffer, unidades_trabajo);

	enviar_paquete(paquete, fd_dispatch);

    pthread_mutex_lock(&mutex_log_debug);
        log_info(cpu_log_debug, "Enviando contexto de ejecucion PID: < %d > a CPU.", PCB.pid);
    pthread_mutex_lock(&mutex_log_debug);

	eliminar_paquete(paquete);
}