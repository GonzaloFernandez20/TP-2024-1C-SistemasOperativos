#include <protocolos/protocoloKernel.h>

void *procesar_operacion_kernel(void *fd_kernel_casteado){
    
    fd_kernel = _deshacer_casting(fd_kernel_casteado);

    int cliente_conectado = 1;

    while(cliente_conectado){
        int cod_op = recibir_operacion(fd_kernel);

		switch (cod_op) {
		case CREAR_PROCESO:
            atender_solicitud((void *)crear_proceso);
			break;
		
        case FINALIZAR_PROCESO:
            atender_solicitud((void *)eliminar_proceso);
			break;

		case -1:
			pthread_mutex_lock(&mutex_log_debug);
            log_error(memoria_log_debugg, "KERNEL se desconecto\n");
            pthread_mutex_unlock(&mutex_log_debug);

            cliente_conectado = 0;
			break;
			
		default:
			pthread_mutex_lock(&mutex_log_debug);
            log_warning(memoria_log_debugg,"Operacion desconocida de KERNEL\n");
            pthread_mutex_unlock(&mutex_log_debug);
			break;
		}
	}
    return (void *)EXIT_FAILURE;
}



void* crear_proceso(void){
    t_buffer *buffer = recibir_buffer(fd_kernel);
	void* stream = buffer->stream;

    int PID = buffer_read_int(&stream);
    int length_path = buffer_read_int(&stream);
	char* path_archivo = malloc(length_path);
	strcpy(path_archivo, buffer_read_string(&stream, length_path));

    pthread_mutex_lock(&mutex_log_debug);
    log_info(memoria_log_debugg, "Cargando instrucciones del proceso <%d> desde el archivo %s\n", PID, path_archivo);
    pthread_mutex_unlock(&mutex_log_debug);

	eliminar_buffer(buffer);

	crear_estructuras_administrativas(PID, path_archivo);

	free(path_archivo);
    return NULL;
}

void crear_estructuras_administrativas(int PID, char* path_archivo){
    cargar_instrucciones(PID, path_archivo);
    // algo relacionado a las paginas y marcos del checkp3
}

void cargar_instrucciones(int PID, char* path_archivo){
    t_proceso* nuevo_proceso = malloc(sizeof(t_proceso));
    nuevo_proceso-> PID = PID;

    FILE* archivo_seudocodigo = abrir_archivo(path_archivo);
    int seudocodigo_ok = 1;
    int seudocodigo_error = 0;
    
    if (archivo_seudocodigo == NULL) {

        pthread_mutex_lock(&mutex_log_debug);
        log_error(memoria_log_debugg, "No se encontro el archivo de seudocodigo del proceso <%d>\n", PID);
        pthread_mutex_unlock(&mutex_log_debug);

        send(fd_kernel, &seudocodigo_error, sizeof(int), 0);
    }
    else{

        int cantidad_instrucciones = contar_lineas(archivo_seudocodigo);
        nuevo_proceso->cant_instrucciones=cantidad_instrucciones;
        nuevo_proceso->CODE_segmento = malloc(cantidad_instrucciones * sizeof(char*));
        leer_almacenar_instrucciones(nuevo_proceso->CODE_segmento, archivo_seudocodigo);

        pthread_mutex_lock(&mutex_memoria_instrucciones);
        list_add(memoria_de_instrucciones, nuevo_proceso);
        pthread_mutex_unlock(&mutex_memoria_instrucciones);

        pthread_mutex_lock(&mutex_log_debug);
        log_info(memoria_log_debugg,"PID: <%d> instrucciones cargadas en memoria\n", PID);
        pthread_mutex_unlock(&mutex_log_debug);

        send(fd_kernel, &seudocodigo_ok, sizeof(int), 0);
    }

    fclose(archivo_seudocodigo);
}

FILE* abrir_archivo(char* archivo){
    char* path = concatenar_rutas(config_memoria.PATH_INSTRUCCIONES, archivo);
    FILE* archivo_seudocodigo = fopen(path, "r");
    
    free(path);
    return archivo_seudocodigo;
    
}

char* concatenar_rutas(char* directorio, char* archivo){
   
    int length = strlen(directorio) + strlen(archivo) + 1; // +1 para el terminador nulo

    char* path = malloc(length);

    strcpy(path, directorio);
    strcat(path, archivo);

    return path;
}

int contar_lineas(FILE* archivo_seudocodigo){
    
    int lineas = 0;
    int caracter;
    int previo_caracter = 0;

    while ((caracter = fgetc(archivo_seudocodigo)) != EOF) {
        if (caracter == '\n') {lineas++;}
        previo_caracter = caracter;
    }

    // Verificar si el archivo no termina con un salto de línea
    if (previo_caracter != '\n' && previo_caracter != EOF) {
        lineas++;
    }

    return lineas;
}

void leer_almacenar_instrucciones(char** CODE_segmento, FILE* seudocodigo){
    
    char* buffer = NULL;
    size_t longitud = 0;
    int caracteres_leidos;
    int posicion_instruccion = 0;

    while ((caracteres_leidos = getline(&buffer, &longitud, seudocodigo)) != -1) {
        // Sacar el salto de línea al final de la cadena
        if (buffer[caracteres_leidos - 1] == '\n') { buffer[caracteres_leidos - 1] = '\0'; }

        // Asignar memoria para la línea y copiar el contenido
        CODE_segmento[posicion_instruccion] = malloc((caracteres_leidos + 1) * sizeof(char));
        strcpy(CODE_segmento[posicion_instruccion], buffer);
        posicion_instruccion++;
    }
    free(buffer);
}

void* eliminar_proceso(void){
    t_buffer *buffer = recibir_buffer(fd_kernel);
	void* stream = buffer->stream;

    int PID = buffer_read_int(&stream);
    liberar_estructuras_asociadas(PID);
    //marcar_libre_espacio();
        ////marcar los frames como libres pero sin sobreescribir su contenido

    pthread_mutex_lock(&mutex_log_debug);
    log_info(memoria_log_debugg, "Liberado el espacio en memoria del proceso <%d>\n", PID);
    pthread_mutex_unlock(&mutex_log_debug);

    return NULL;
}

void liberar_estructuras_asociadas(int PID){
    int indice_proceso = buscar_PID_memoria_instrucciones(PID);

    pthread_mutex_lock(&mutex_memoria_instrucciones);
    t_proceso* proceso_a_eliminar = list_remove(memoria_de_instrucciones, indice_proceso);
    pthread_mutex_unlock(&mutex_memoria_instrucciones);

    liberar_segmento_codigo(proceso_a_eliminar);
    free(proceso_a_eliminar);
}

void liberar_segmento_codigo(t_proceso* proceso){
    for (int i = 0; i < proceso->cant_instrucciones; i++) {
        free(proceso->CODE_segmento[i]);
    }
    free(proceso->CODE_segmento);
}



