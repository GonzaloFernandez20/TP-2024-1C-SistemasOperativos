#include <protocolos/protocoloKernel.h>

void *procesar_operacion_kernel(void *fd_kernel_casteado){
    
    fd_kernel = _deshacer_casting(fd_kernel_casteado);
    free(fd_kernel_casteado);

    int cliente_conectado = 1;

    while(cliente_conectado){
        int cod_op = recibir_operacion(fd_kernel);

        usleep(1000*config_memoria.RETARDO_RESPUESTA); //retardo en peticiones

		switch (cod_op) {
		case CREAR_PROCESO:
            crear_proceso();
			break;
		
        case ELIMINAR_PROCESO:
            eliminar_proceso();
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


////////////////////////////CREAR PROCESOS///////////////////////////////////////////////////////////////////////////////
void crear_proceso(void){
    t_buffer *buffer = recibir_buffer(fd_kernel);
	void* stream = buffer->stream;

    int PID = buffer_read_int(&stream);
    int length_path = buffer_read_int(&stream);
	char* path_archivo = malloc(length_path);
	strcpy(path_archivo, buffer_read_string(&stream, length_path));

	eliminar_buffer(buffer);

	crear_estructuras_administrativas(PID, path_archivo);

	free(path_archivo);
}

void crear_estructuras_administrativas(int PID, char* path_archivo){
    crear_tabla_de_paginas(PID);
    cargar_instrucciones(PID, path_archivo);
}

void crear_tabla_de_paginas(int PID){
    t_tabla_paginas* tabla = malloc(sizeof(t_tabla_paginas));
    tabla->PID = PID;
    tabla->tabla_paginas = list_create();

    pthread_mutex_lock(&mutex_tablas_de_paginas);
        list_add(tablas_de_paginas, tabla);
    pthread_mutex_unlock(&mutex_tablas_de_paginas);

    pthread_mutex_lock(&mutex_log);
        log_info(memoria_log, "PID: < %d > - Tamaño: < 0 >", PID);
    pthread_mutex_unlock(&mutex_log);
}

void cargar_instrucciones(int PID, char* path_archivo){
    t_proceso* nuevo_proceso = malloc(sizeof(t_proceso));
    nuevo_proceso-> PID = PID;

    FILE* archivo_pseudocodigo = abrir_archivo(path_archivo);
    int pseudocodigo_ok = 1;
    int pseudocodigo_error = 0;
    
    if (archivo_pseudocodigo == NULL) {

        pthread_mutex_lock(&mutex_log_debug);
            log_error(memoria_log_debugg, "No se encontro el archivo de pseudocodigo del proceso <%d>\n", PID);
        pthread_mutex_unlock(&mutex_log_debug);

        send(fd_kernel, &pseudocodigo_error, sizeof(int), 0);
    }
    else{

        int cantidad_instrucciones = contar_lineas(archivo_pseudocodigo);
        nuevo_proceso->cant_instrucciones=cantidad_instrucciones;
        nuevo_proceso->CODE_segmento = malloc(cantidad_instrucciones * sizeof(char*));
        leer_almacenar_instrucciones(nuevo_proceso->CODE_segmento, path_archivo);

        pthread_mutex_lock(&mutex_memoria_instrucciones);
            list_add(memoria_de_instrucciones, nuevo_proceso);
        pthread_mutex_unlock(&mutex_memoria_instrucciones);

        pthread_mutex_lock(&mutex_log_debug);
            log_info(memoria_log_debugg,"PID: <%d> instrucciones cargadas en memoria", PID);
        pthread_mutex_unlock(&mutex_log_debug);

        mostrar_procesos_cargados();

        send(fd_kernel, &pseudocodigo_ok, sizeof(int), 0);
    }
}

FILE* abrir_archivo(char* archivo){
    char* path = concatenar_rutas(config_memoria.PATH_INSTRUCCIONES, archivo);
    FILE* archivo_pseudocodigo = fopen(path, "r");
    
    free(path);
    return archivo_pseudocodigo;
}

char* concatenar_rutas(char* directorio, char* archivo){
    int length = strlen(directorio) + strlen(archivo) + 1; // +1 para el terminador nulo

    char* path = malloc(length);

    strcpy(path, directorio);
    strcat(path, archivo);

    return path;
}

int contar_lineas(FILE* archivo_pseudocodigo){
    int lineas = 0;
    int caracter;
    int previo_caracter = 0;

    while ((caracter = fgetc(archivo_pseudocodigo)) != EOF) {
        if (caracter == '\n') {lineas++;}
        previo_caracter = caracter;
    }

    // Verificar si el archivo no termina con un salto de línea
    if (previo_caracter != '\n' && previo_caracter != EOF) {
        lineas++;
    }

    fclose(archivo_pseudocodigo);
    return lineas;
}

void leer_almacenar_instrucciones(char** CODE_segmento, char* path_archivo){
    FILE* archivo_pseudocodigo = abrir_archivo(path_archivo);
    char* buffer = NULL;
    size_t longitud = 0;
    int caracteres_leidos;
    int posicion_instruccion = 0;

    while ((caracteres_leidos = getline(&buffer, &longitud, archivo_pseudocodigo)) != -1) {
        // Sacar el salto de línea al final de la cadena
        if (buffer[caracteres_leidos - 1] == '\n') { buffer[caracteres_leidos - 1] = '\0'; }

        // Asignar memoria para la línea y copiar el contenido
        CODE_segmento[posicion_instruccion] = malloc((caracteres_leidos + 1) * sizeof(char));
        strcpy(CODE_segmento[posicion_instruccion], buffer);

        posicion_instruccion++;
    }
    free(buffer);
    fclose(archivo_pseudocodigo);
}

//////////////////// ELIMINAR PROCESOS //////////////////////////////////////////////////////////
void eliminar_proceso(void){
    t_buffer *buffer = recibir_buffer(fd_kernel);
	void* stream = buffer->stream;

    int PID = buffer_read_int(&stream);

    eliminar_buffer(buffer);
    
    liberar_estructuras_asociadas(PID);
    marcar_libre_espacio(PID);
    
    pthread_mutex_lock(&mutex_log_debug);
        log_info(memoria_log_debugg, "Liberado el espacio en memoria del proceso <%d>", PID);
    pthread_mutex_unlock(&mutex_log_debug);

    mostrar_procesos_cargados();
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

void marcar_libre_espacio(int PID){
    int indice_tabla = buscar_PID_tablas_paginas(PID);

    pthread_mutex_lock(&mutex_tablas_de_paginas);
        t_tabla_paginas* tabla_eliminar = list_remove(tablas_de_paginas, indice_tabla);
    pthread_mutex_unlock(&mutex_tablas_de_paginas);

    int cantidad_paginas = list_size(tabla_eliminar->tabla_paginas);

    pthread_mutex_lock(&mutex_log);
        log_info(memoria_log, "PID: < %d > - Tamaño: < %d >", PID, cantidad_paginas);
    pthread_mutex_unlock(&mutex_log);

    liberar_marcos(cantidad_paginas, tabla_eliminar->tabla_paginas);

    free(tabla_eliminar->tabla_paginas);
    free(tabla_eliminar);

}



