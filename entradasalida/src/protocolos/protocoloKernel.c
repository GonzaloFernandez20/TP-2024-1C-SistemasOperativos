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
        case IO_FS_CREATE:
                realizar_un_fs_create();
			break;
        case IO_FS_DELETE:
                realizar_un_fs_delete();
			break;
        case IO_FS_TRUNCATE:
                realizar_un_fs_truncate();
			break;
        case IO_FS_READ:
                realizar_un_fs_read();
			break;
        case IO_FS_WRITE:
                realizar_un_fs_write();
			break;
        default:
            pthread_mutex_lock(&mutex_log_debug);
            log_warning(IO_log_debug,"Instruccion desconocida\n");
            sleep(10);
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

    pthread_mutex_lock(&mutex_log_debug);
	    log_info(IO_log_debug,"PID: < %d > - Operacion: IO_GEN_SLEEP -> COMPLETADA", PID);
    pthread_mutex_unlock(&mutex_log_debug);

    mandar_aviso_kernel(PID);
}

void realizar_un_stdin_read(){
    t_buffer *buffer = recibir_buffer(fd_conexion_kernel);
	void* stream = buffer->stream;
	int PID = buffer_read_int(&stream);

    int tamanio_a_leer = buffer_read_int(&stream);

    pthread_mutex_lock(&mutex_log);
	    log_info(IO_log,"PID: < %d > - Operacion: IO_STDIN_READ", PID);
    pthread_mutex_unlock(&mutex_log);

    char* cadena_ingresada = readline("Ingrese la información: ");

    char* cadena_truncada = truncar_cadena(cadena_ingresada, tamanio_a_leer);

    enviar_cadena_memoria(PID, cadena_truncada, stream);
    mandar_aviso_kernel(PID);
    eliminar_buffer(buffer);

}

void realizar_un_stdout_write(){

    t_buffer *buffer = recibir_buffer(fd_conexion_kernel);
	void* stream = buffer->stream;
	int PID = buffer_read_int(&stream);

    pthread_mutex_lock(&mutex_log);
	    log_info(IO_log,"PID: < %d > - Operacion: IO_STDOUT_WRITE", PID);
    pthread_mutex_unlock(&mutex_log);

    int tamanio_a_leer = buffer_read_int(&stream);

    char *cadena = traer_cadena_memoria(PID, tamanio_a_leer, stream);
    printf(" ");
    free(cadena);
    eliminar_buffer(buffer);
    mandar_aviso_kernel(PID);
}
    


//------------------------ FUNCIONES AUXILIARES ------------------------//

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

void* leer_de_memoria(int PID, int direccion_fisica, int bytes) { 
	peticion_lectura_memoria(PID, direccion_fisica, bytes);

	int codigo_operacion = recibir_operacion(fd_conexion_memoria); 
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

char *generar_cadena(void* particion, int bytes){
    char* nueva_cadena = malloc(bytes + 1);
    int i;

    for (i = 0; i < bytes; i++){
        nueva_cadena[i] = *(char *)(particion + i);
    }

    nueva_cadena[i]='\0';

    return nueva_cadena;
}

void enviar_cadena_memoria(int PID, char* cadena_leida, void *stream){

    int cant_direcciones = buffer_read_int(&stream);

    int desplazamiento = 0;
    int bytes;
    int direccion_fisica;

    for (int i = 0; i < cant_direcciones; i++)
    {
        bytes = buffer_read_int(&stream);
        direccion_fisica = buffer_read_int(&stream);

        void* particion = malloc(bytes);

        void* valor = cadena_leida;

        memcpy(particion, valor + desplazamiento, bytes);

        desplazamiento += bytes;

        enviar_a_memoria(PID, particion, bytes, direccion_fisica);

        char* cadena = generar_cadena(particion, bytes);

        free(particion);

        pthread_mutex_lock(&mutex_log_debug);
            log_info(IO_log_debug,"PID: < %d > - Accion: < ESCRIBIR > - Direccion Fisica: < %d > - Valor: < %s >", PID, direccion_fisica, cadena);
        pthread_mutex_unlock(&mutex_log_debug);

        free(cadena);
    }

    free(cadena_leida);
}

char *traer_cadena_memoria(int PID, int tamanio_a_leer, void *stream){
    
    int cant_direcciones = buffer_read_int(&stream);
    
    int offset = 0;
    int bytes;
    int direccion_fisica;
    
    void* cadena_rearmada = malloc(tamanio_a_leer);

    for(int i = 0; i < cant_direcciones; i++){
        bytes = buffer_read_int(&stream);
        direccion_fisica = buffer_read_int(&stream);
    
        void* particion = leer_de_memoria(PID, direccion_fisica, bytes);
        memcpy(cadena_rearmada + offset, particion, bytes);
        offset += bytes;

        char* cadena = generar_cadena(particion, bytes);
    
        pthread_mutex_lock(&mutex_log_debug);
            log_info(IO_log_debug,"PID: < %d > - Acción: < LEER > - Dirección Física: < %d > - Valor: < %s >", PID, direccion_fisica, cadena);
        pthread_mutex_unlock(&mutex_log_debug);
        
        free(particion);
        free(cadena);
    }

    char* cadena = generar_cadena(cadena_rearmada, tamanio_a_leer);
    
  
    printf("PID: < %d > - < %s >\n", PID, cadena);
    
    free(cadena_rearmada);

    return cadena;
}