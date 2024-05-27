#include <protocolos/protocoloKernel.h>

void atender_peticiones_kernel(void){
    while(1){
        int cod_op = recibir_operacion(fd_conexion_kernel);

        switch (cod_op){
		case IO_GEN_SLEEP:
            realizar_un_sleep();
			break;


        default:
            pthread_mutex_lock(&mutex_log_debug);
            log_warning(IO_log_debug,"Instruccion desconocida\n");
            pthread_mutex_unlock(&mutex_log_debug);
            break;
        }

    }
}

void realizar_un_sleep(void){
    t_buffer *buffer = recibir_buffer(fd_conexion_kernel);
	void* stream = buffer->stream;
    
	int PID = buffer_read_int(&stream);
	int unidades_de_trabajo = buffer_read_int(&stream);
	eliminar_buffer(buffer);
    
    pthread_mutex_lock(&mutex_log);
	log_info(IO_log,"PID: < %d > - Operacion: IO_GEN_SLEEP", PID);
    pthread_mutex_unlock(&mutex_log);

    usleep(1000*unidades_de_trabajo*config_IO.TIEMPO_UNIDAD_TRABAJO);
    mandar_aviso_kernel(PID);
    
}

void mandar_aviso_kernel(int PID){
    t_paquete* paquete = crear_paquete(OPERACION_COMPLETADA);

	int buffer_size = sizeof(int); 
	crear_buffer(paquete, buffer_size);

	buffer_add_int(paquete->buffer, PID);

	enviar_paquete(paquete, fd_conexion_kernel);
	eliminar_paquete(paquete);
}