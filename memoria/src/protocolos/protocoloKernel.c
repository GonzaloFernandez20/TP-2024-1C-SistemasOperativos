#include <protocolos/protocoloKernel.h>

void *procesar_operacion_kernel(void *fd_kernel_casteado){
    
    int fd_kernel = _deshacer_casting(fd_kernel_casteado);

    int cliente_conectado = 1;

    while(cliente_conectado){
        int cod_op = recibir_operacion(fd_kernel);

		switch (cod_op) {
		case CREAR_PROCESO:
			//
			break;
		
		case -1:
			log_error(memoria_log_debugg, "KERNEL se desconecto\n");
            cliente_conectado = 0;
			break;
			
		default:
			log_warning(memoria_log_debugg,"Operacion desconocida de KERNEL");
			break;
		}
	}
    return (void *)EXIT_FAILURE;
}


/////////////// KERNEL serializacion

void enviar_path_seudocodigo(char* archivo, int PID){
    t_paquete* paquete = crear_paquete(CREAR_PROCESO);

    int buffer_size = strlen(archivo) + 1 + 2*sizeof(int); 
	crear_buffer(paquete, buffer_size);

    buffer_add_int(paquete->buffer, PID );
    buffer_add_int(paquete->buffer, strlen(archivo) + 1);
	buffer_add_string(paquete->buffer, archivo);

    enviar_paquete(paquete, fd_server_memoria);
    log_info(kernel_log_debugg; "Solicitud de creacion de proceso enviada a memoria")

	eliminar_paquete(paquete);
}


/////////////// MEMORIA serializacion

void crear_proceso(void){
    t_buffer *buffer = recibir_buffer(fd_cliente);
	void* stream = buffer->stream;

    int PID = buffer_read_int(&stream);
    int length_path = buffer_read_int(&stream);
	char* path_archivo = malloc(length_path);
	strcpy(path_archivo, buffer_read_string(&stream, length_path));

    log_info(memoria_log_debugg, "Cargando instrucciones del proceso PID %d desde el archivo %s\n", PID, path_archivo);
	eliminar_buffer(buffer);

	crear_estructuras_administrativas(PID, path_archivo);

	free(path);
}

void crear_estructuras_administrativas(int PID, char* path_archivo){
    cargar_instrucciones(PID, path_archivo);
    // algo relacionado a las paginas y marcos del checkp3
}

void cargar_instrucciones(int PID, char* path_archivo){
    
}