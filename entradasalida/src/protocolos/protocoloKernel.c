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

    pthread_mutex_lock(&mutex_log);
	log_info(IO_log,"PID: < %d > - Operacion: IO_GEN_SLEEP -> COMPLETADA", PID);
    pthread_mutex_unlock(&mutex_log);

    mandar_aviso_kernel(PID);
    
}

void realizar_un_stdin_read(){
    t_buffer *buffer = recibir_buffer(fd_conexion_kernel);
	void* stream = buffer->stream;
	int PID = buffer_read_int(&stream);

    pthread_mutex_lock(&mutex_log);
	    log_info(IO_log,"PID: < %d > - Operacion: IO_STDIN_READ", PID);
    pthread_mutex_unlock(&mutex_log);

    char* cadena_ingresada = readline("Ingrese la información: ");

    int tamanio_a_leer = buffer_read_int(&stream);
    int cant_direcciones = buffer_read_int(&stream);

    char* cadena_truncada = truncar_cadena(cadena_ingresada, tamanio_a_leer);

    int desplazamiento = 0;
    int bytes;
    int direccion_fisica;

    for (int i = 0; i < cant_direcciones; i++)
    {
        bytes = buffer_read_int(&stream);
        direccion_fisica = buffer_read_int(&stream);

        void* particion = malloc(bytes);

        void* valor = cadena_truncada;

        memcpy(particion, valor + desplazamiento, bytes);

        desplazamiento += bytes;

        enviar_a_memoria(PID, particion, bytes, direccion_fisica);

        particion = realloc(particion, bytes + 1);
        ((char*)particion)[bytes] = '\0';

        pthread_mutex_lock(&mutex_log_debug);
            log_info(IO_log_debug,"PID: < %d > - Accion: < ESCRIBIR > - Direccion Fisica: < %d > - Valor: < %s >", PID, direccion_fisica, (char*)particion);
        pthread_mutex_unlock(&mutex_log_debug);

        free(particion);
    }
    // ACA YA RECIBI TODO EL BUFFER ENVIADO DESDE KERNEL
    eliminar_buffer(buffer);
    free(cadena_truncada);
}


void realizar_un_stdout_write(){

    t_buffer *buffer = recibir_buffer(fd_conexion_kernel);
	void* stream = buffer->stream;
	int PID = buffer_read_int(&stream);

    pthread_mutex_lock(&mutex_log);
	    log_info(IO_log,"PID: < %d > - Operacion: IO_STDOUT_WRITE", PID);
    pthread_mutex_unlock(&mutex_log);

    int tamanio_a_leer = buffer_read_int(&stream);
    int cant_direcciones = buffer_read_int(&stream);

    int offset = 0;
    int bytes;
    int direccion_fisica;
    
    void* cadena_rearmada = malloc(tamanio_a_leer + 1);

    for(int i = 0; i < cant_direcciones; i++){
        bytes = buffer_read_int(&stream);
        direccion_fisica = buffer_read_int(&stream);
    
        void* particion = leer_de_memoria(PID, direccion_fisica, bytes);
        memcpy(cadena_rearmada + offset, particion, bytes);
        offset += bytes;

        particion = realloc(particion, bytes + 1);
        ((char*)particion)[bytes] = '\0';
    
        pthread_mutex_lock(&mutex_log_debug);
            log_info(IO_log_debug,"PID: < %d > - Acción: < LEER > - Dirección Física: < %d > - Valor: < %s >", PID, direccion_fisica, (char*)particion);
        pthread_mutex_unlock(&mutex_log_debug);
        free(particion);
    }
    // TECNICAMENTE LA CADENA YA ESTA ARMADA COMO VOID*
    eliminar_buffer(buffer);

    ((char*)cadena_rearmada)[tamanio_a_leer] = '\0';

    pthread_mutex_lock(&mutex_log_debug);
        log_info(IO_log_debug,"PID: < %d > - Cadena: < %s >", PID, (char*)cadena_rearmada);
    pthread_mutex_unlock(&mutex_log_debug);
    
    free(cadena_rearmada);
}
    
/* 
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
    pthread_mutex_lock(&mutex_log);
	log_info(IO_log,"PID: < %d > - Operacion: IO_STDOUT_WRITE", PID);
    pthread_mutex_unlock(&mutex_log);

    eliminar_buffer(buffer); 
}
*/

void mandar_aviso_kernel(int PID){
    t_paquete* paquete = crear_paquete(OPERACION_COMPLETADA);

	int buffer_size = sizeof(int); 
	crear_buffer(paquete, buffer_size);

	buffer_add_int(paquete->buffer, PID);

	enviar_paquete(paquete, fd_conexion_kernel);
	eliminar_paquete(paquete);
}

char* truncar_cadena(char *cadena, int tamanio){
    if (strlen(cadena) <= tamanio) {return cadena;}
    char *cadena_truncada = malloc(tamanio + 1);  // Asigna espacio para el carácter nulo aquí
    memcpy(cadena_truncada, cadena, tamanio);
    cadena_truncada[tamanio] = '\0';  // Establece el carácter nulo

    return cadena_truncada;
}

void* leer_de_memoria(int PID, int direccion_fisica, int bytes) { // ESTA FUNCION DESPUES TIENE QUE IR EN UTILS PORQUE LA USAN CPU E IO JAJAJAJA PERO AHORA ME LA PUNGUEO ACA Y CORTA
	peticion_lectura_memoria(PID, direccion_fisica, bytes);

	int codigo_operacion = recibir_operacion(fd_conexion_memoria); // por convención nos comunicamos usando paquetes, por eso debemos recibir la operación primero, a pesar de que no vayamos a usarlo.
    if(codigo_operacion != VALOR_LEIDO){ perror("Rompiste todo.");}

	t_buffer *buffer = recibir_buffer(fd_conexion_memoria);
	void* stream = buffer->stream;

	int bytes_leidos = buffer_read_int(&stream);
	void* valor = malloc(bytes_leidos);
	valor = buffer_read_valor(&stream, bytes);

	eliminar_buffer(buffer);

	return valor;                
}

void peticion_lectura_memoria(int PID, int direccion_fisica, int bytes) {
    t_paquete* paquete = crear_paquete(LECTURA);

    int buffer_size = 3* sizeof(int);
    crear_buffer(paquete, buffer_size);

    buffer_add_int(paquete->buffer, PID);
	buffer_add_int(paquete->buffer, direccion_fisica);
	buffer_add_int(paquete->buffer, bytes);
    
    enviar_paquete(paquete, fd_conexion_memoria);
    
    eliminar_paquete(paquete);

}