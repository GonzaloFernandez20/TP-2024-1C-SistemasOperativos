#include <protocolos/protocoloKernel.h>


void atender_peticiones_kernel(void){
    while(1){
        int cod_op = recibir_operacion(fd_conexion_kernel);

        switch (cod_op){
		case IO_GEN_SLEEP:
            realizar_un_sleep();
			break;
        case IO_STDIN_READ:
            realizar_un_stdin_read();
        break;
        case IO_STDOUT_WRITE:
            realizar_un_stdout_write();
        break;
        default:
            pthread_mutex_lock(&mutex_log_debug);
            log_warning(IO_log_debug,"Instruccion desconocida\n");
            pthread_mutex_unlock(&mutex_log_debug);
            break;
        }

    }
}

void realizar_un_stdin_read(){
    t_buffer *buffer = recibir_buffer(fd_conexion_kernel);
	void* stream = buffer->stream;
	int PID = buffer_read_int(&stream);
//Hasta acá igual en todas las interfaces
    int* resultado_de_operacion = malloc(sizeof(int));

    uint32_t direccionFisica = buffer_read_uint32(&stream);
    char* inputUsuario = readline("Ingrese la información: ");
    printf("\n");
    enviar_a_direccion_fisica(inputUsuario, direccionFisica, resultado_de_operacion);    
    free(inputUsuario);

    t_buffer* bufferRTA = recibir_buffer(fd_conexion_memoria);//no hay que enviar cod op desde memoria
    void* streamRTA = bufferRTA->stream;
    int direccion = buffer_read_uint32(&streamRTA);
    int stringLength = buffer_read_int(&streamRTA);//revisar si se envia el \0 desde memoria
    char* respuesta = buffer_read_string(&streamRTA, stringLength);

    printf("%s %d\n", respuesta, direccion);

    eliminar_buffer(bufferRTA);

    if(*resultado_de_operacion == FAILED_WRITE){//LOGGEAR ERROR TMB

    }else{//LOGGEAR TRIUNFO?
    }



    free(resultado_de_operacion);

//A partir de acá se repite para todas las interfaces
    pthread_mutex_lock(&mutex_log);
	log_info(IO_log,"PID: < %d > - Operacion: IO_STDIN_READ", PID);
    pthread_mutex_unlock(&mutex_log);

    eliminar_buffer(buffer);
}


void realizar_un_stdout_write(){
    t_buffer *buffer = recibir_buffer(fd_conexion_kernel);
	void* stream = buffer->stream;
	int PID = buffer_read_int(&stream);
//Hasta acá igual en todas las interfaces
    int* resultado_de_operacion = malloc(sizeof(int));

    uint32_t direccionFisica = buffer_read_uint32(&stream);
    char* datoEnDireccionFisica = leer_direccion_fisica(direccionFisica, resultado_de_operacion);

    free(datoEnDireccionFisica);

    if(*resultado_de_operacion == FAILED_READ){//LOGGEAR ERROR TMB
        printf("%s %d\n", datoEnDireccionFisica, direccionFisica);
    }else{//LOGGEAR TRIUNFO?
        printf("En la dirección %d estaba el dato : %s", direccionFisica, datoEnDireccionFisica);    
    }

    free(resultado_de_operacion);
//A partir de acá se repite para todas las interfaces
    pthread_mutex_lock(&mutex_log);
	log_info(IO_log,"PID: < %d > - Operacion: IO_STDOUT_WRITE", PID);
    pthread_mutex_unlock(&mutex_log);

    eliminar_buffer(buffer);
}

void realizar_un_sleep(void){
    t_buffer *buffer = recibir_buffer(fd_conexion_kernel);
	void* stream = buffer->stream;
    
	int PID = buffer_read_int(&stream);
//Hasta acá igual en todas las interfaces

	int unidades_de_trabajo = buffer_read_int(&stream);

	eliminar_buffer(buffer);
    
    pthread_mutex_lock(&mutex_log);
	log_info(IO_log,"PID: < %d > - Operacion: IO_GEN_SLEEP", PID);
    pthread_mutex_unlock(&mutex_log);

    usleep(1000*unidades_de_trabajo*config_IO.TIEMPO_UNIDAD_TRABAJO);

    pthread_mutex_lock(&mutex_log);
	log_info(IO_log,"PID: < %d > - Operacion: IO_GEN_SLEEP -> COMPLETADA", PID);
    pthread_mutex_unlock(&mutex_log);

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