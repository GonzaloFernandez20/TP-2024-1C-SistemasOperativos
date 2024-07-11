#include <fileSystem/operaciones.h>


/// IO FS CREATE /////////////////////////////////////////////////////////////////////////////////////
void realizar_un_fs_create(void){
     t_buffer *buffer = recibir_buffer(fd_conexion_kernel);
	void* stream = buffer->stream;
    
	int PID = buffer_read_int(&stream);
    int length_nombre = buffer_read_int(&stream);
    char* nombre_archivo = malloc(length_nombre);
	strcpy(nombre_archivo, buffer_read_string(&stream, length_nombre));

	eliminar_buffer(buffer);
    
    pthread_mutex_lock(&mutex_log);
	    log_info(IO_log,"PID: < %d > - Operacion: IO_FS_CREATE", PID);
        log_info(IO_log,"PID: < %d > - Crear Archivo: %s", PID, nombre_archivo);
    pthread_mutex_unlock(&mutex_log);

    crear_archivo(nombre_archivo);
    free(nombre_archivo);
}

void crear_archivo(char* nombre_archivo){
    
    int bloque_inicial = asignar_primer_bloque_libre();
    char* path = concatenar_rutas(config_IO.PATH_BASE_DIALFS, nombre_archivo);
    
    t_config *config = malloc(sizeof(t_config));
	config->path = strdup(path);
	config->properties = dictionary_create(); 
    config_set_value(config, "BLOQUE_INICIAL", string_itoa(bloque_inicial));
    config_set_value(config, "TAMANIO_ARCHIVO", "0");
    config_save(config); // se crea en el directorio el archivo de metadata con los valores
    config_destroy(config);

    t_FCB* metadata = malloc(sizeof(t_FCB));
    metadata->archivo = strdup(nombre_archivo);
    metadata->path = strdup(path);
    metadata->bloque_inicial = bloque_inicial;
    metadata->tamanio_archivo = 0;
    list_add_sorted(archivos_metadata, metadata, comparar_por_bloque_inicial);

    free(path);
}



/// IO FS DELETE ///////////////////////////////////////////////////////////////////////////
void realizar_un_fs_delete(void){
     t_buffer *buffer = recibir_buffer(fd_conexion_kernel);
	void* stream = buffer->stream;
    
	int PID = buffer_read_int(&stream);
    int length_nombre = buffer_read_int(&stream);
    char* nombre_archivo = malloc(length_nombre);
	strcpy(nombre_archivo, buffer_read_string(&stream, length_nombre));

	eliminar_buffer(buffer);
    
    pthread_mutex_lock(&mutex_log);
	    log_info(IO_log,"PID: < %d > - Operacion: IO_FS_DELETE", PID);
        log_info(IO_log,"PID: < %d > - Eliminar Archivo: %s", PID, nombre_archivo);
    pthread_mutex_unlock(&mutex_log);

    eliminar_archivo(nombre_archivo);
    free(nombre_archivo);
}

void eliminar_archivo(char* nombre_archivo){
    int indice = buscar_FCB_en_archivos_metadata(nombre_archivo);
    t_FCB* FCB_eliminar = list_remove(archivos_metadata, indice);

    remove(FCB_eliminar->path);
    free(FCB_eliminar->path);
    free(FCB_eliminar->archivo);

    int cantidad_bloques = ceil(((double)FCB_eliminar->tamanio_archivo) / config_IO.BLOCK_SIZE);
    liberar_bloques(FCB_eliminar->bloque_inicial, FCB_eliminar->bloque_inicial+cantidad_bloques);
    
    free(FCB_eliminar);
}


// Esta instrucción solicita al Kernel que mediante la interfaz seleccionada, se lea desde el archivo a partir del valor del Registro Puntero Archivo la cantidad de bytes indicada por Registro Tamaño y se escriban en la Memoria a partir de la dirección lógica indicada en el Registro Dirección.
/// IO FS READ ///////////////////////////////////////////////////////////////////////////

void realizar_un_fs_read(void){

    t_buffer *buffer = recibir_buffer(fd_conexion_kernel);
	void* stream = buffer->stream;
	int PID = buffer_read_int(&stream);

    int size_nombre_archivo = buffer_read_int(&stream);
    char *nombre_archivo = buffer_read_string(&stream, size_nombre_archivo);

    int tamanio_a_leer = buffer_read_int(&stream);
    int puntero_archivo = buffer_read_int(&stream);

    // RECIBI TODOS LOS DATOS EXCEPTO LAS DIRECCIONES COMO TAL

    pthread_mutex_lock(&mutex_log);
	    log_info(IO_log,"PID: < %d > - Operacion: IO_FS_READ", PID);
	    log_info(IO_log,"PID: < %d > - Leer Archivo: < %s > - Tamaño a Leer: < %d > - Puntero Archivo: < %d >", PID, nombre_archivo, tamanio_a_leer, puntero_archivo);
    pthread_mutex_unlock(&mutex_log);

    char *cadena_leida = leer_de_archivo(nombre_archivo, tamanio_a_leer, puntero_archivo); // -> Codear...

    enviar_cadena_memoria(PID, cadena_leida, stream);
    eliminar_buffer(buffer);
}

 //Esta instrucción solicita al Kernel que mediante la interfaz seleccionada, se lea desde Memoria la cantidad de bytes indicadas por el Registro Tamaño a partir de la dirección lógica que se encuentra en el Registro Dirección y se escriban en el archivo a partir del valor del Registro Puntero Archivo.
/// IO FS WRITE ///////////////////////////////////////////////////////////////////////////

void realizar_un_fs_write(void){

    t_buffer *buffer = recibir_buffer(fd_conexion_kernel);
	void* stream = buffer->stream;
	int PID = buffer_read_int(&stream);

    int size_nombre_archivo = buffer_read_int(&stream);
    char *nombre_archivo = buffer_read_string(&stream, size_nombre_archivo);

    int tamanio_a_leer = buffer_read_int(&stream);
    int puntero_archivo = buffer_read_int(&stream);

    // RECIBI TODOS LOS DATOS EXCEPTO LAS DIRECCIONES COMO TAL

    pthread_mutex_lock(&mutex_log);
	    log_info(IO_log,"PID: < %d > - Operacion: IO_STDOUT_WRITE", PID);
	    log_info(IO_log,"PID: < %d > - Escribir Archivo: < %s > - Tamaño a Escribir: < %d > - Puntero Archivo: < %d >", PID, nombre_archivo, tamanio_a_leer, puntero_archivo);
    pthread_mutex_unlock(&mutex_log);
    
    char *cadena = traer_cadena_memoria(PID, tamanio_a_leer, stream);

    escribir_en_archivo(nombre_archivo, puntero_archivo, cadena);

    free(cadena);
    eliminar_buffer(buffer);
    mandar_aviso_kernel(PID);
}


// <--------------------------- FUNCIONES AUXILIARES ---------------------------> // 
char *leer_de_archivo(char *nombre_archivo, int tamanio_a_leer, int puntero_archivo){
    char *cadena_leida = malloc(tamanio_a_leer);

    // LOGICA DE ENCONTRAR EL BLOQUE RESPECTO AL BYTE Y LEER DESDE AHI EN BLOQUES.DAT

    int indice_FCB = buscar_FCB_en_archivos_metadata(nombre_archivo);
    t_FCB *FCB_archivo = list_get(archivos_metadata, indice_FCB);
    archivo_bloques = fopen(path_archivo_bloques, "rb");

    int byte_del_archivo_bloques = FCB_archivo->bloque_inicial * config_IO.BLOCK_SIZE + puntero_archivo;

    fseek(archivo_bloques, byte_del_archivo_bloques, SEEK_SET);
    fread(cadena_leida, 1, tamanio_a_leer, archivo_bloques);

    fclose(archivo_bloques);

    return cadena_leida;
}

void escribir_en_archivo(char *nombre_archivo, int puntero_archivo, char *cadena){

    // LOGICA DE ENCONTRAR EL BLOQUE RESPECTO AL BYTE Y ESCRIBIR DESDE AHI EN BLOQUES.DAT

    int indice_FCB = buscar_FCB_en_archivos_metadata(nombre_archivo);
    t_FCB *FCB_archivo = list_get(archivos_metadata, indice_FCB);
    archivo_bloques = fopen(path_archivo_bloques, "r+");

    int byte_del_archivo_bloques = FCB_archivo->bloque_inicial * config_IO.BLOCK_SIZE + puntero_archivo;

    fseek(archivo_bloques, byte_del_archivo_bloques, SEEK_SET);
    fwrite(cadena, 1, strlen(cadena), archivo_bloques);

    fclose(archivo_bloques);
}