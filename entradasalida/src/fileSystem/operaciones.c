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
        log_info(IO_log,"PID: < %d > - Crear Archivo: < %s >", PID, nombre_archivo);
    pthread_mutex_unlock(&mutex_log);

    crear_archivo(nombre_archivo);
    free(nombre_archivo);

    usleep(1000*config_IO.TIEMPO_UNIDAD_TRABAJO);
    mandar_aviso_kernel(PID);
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
        log_info(IO_log,"PID: < %d > - Eliminar Archivo: < %s >", PID, nombre_archivo);
    pthread_mutex_unlock(&mutex_log);

    eliminar_archivo(nombre_archivo);
    free(nombre_archivo);

    usleep(1000*config_IO.TIEMPO_UNIDAD_TRABAJO);
    mandar_aviso_kernel(PID);
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


///// IO FS TRUNCATE //////////////////////////////////////////////////////////////////////////////////// 
void realizar_un_fs_truncate(void){
    t_buffer *buffer = recibir_buffer(fd_conexion_kernel);
	void* stream = buffer->stream;
    
	int PID = buffer_read_int(&stream);
    int length_nombre = buffer_read_int(&stream);
    char* nombre_archivo = malloc(length_nombre);
	strcpy(nombre_archivo, buffer_read_string(&stream, length_nombre));
    int nuevo_tamanio = buffer_read_int(&stream);

	eliminar_buffer(buffer);
    
    pthread_mutex_lock(&mutex_log);
	    log_info(IO_log,"PID: < %d > - Operacion: IO_FS_TRUNCATE", PID);
        log_info(IO_log,"PID: < %d > - Truncar Archivo: < %s > - Tamaño: < %d >", PID, nombre_archivo, nuevo_tamanio);
    pthread_mutex_unlock(&mutex_log);

    truncar_archivo(nombre_archivo, nuevo_tamanio, PID);
    free(nombre_archivo);

    usleep(1000*config_IO.TIEMPO_UNIDAD_TRABAJO);
    mandar_aviso_kernel(PID);
}

void truncar_archivo(char* nombre_archivo, int nuevo_tamanio, int PID){
    int indice = buscar_FCB_en_archivos_metadata(nombre_archivo);
    t_FCB* FCB = list_get(archivos_metadata, indice);

    if(nuevo_tamanio < FCB->tamanio_archivo){ achicar_archivo(FCB, nuevo_tamanio);}
    if(nuevo_tamanio > FCB->tamanio_archivo){ agrandar_archivo(FCB, nuevo_tamanio, PID);}
}

void achicar_archivo(t_FCB* FCB, int nuevo_tamanio){
    int cantidad_bloques_actuales = ceil(((double)FCB->tamanio_archivo) / config_IO.BLOCK_SIZE);
    int nueva_cantidad_bloques = ceil(((double)nuevo_tamanio) / config_IO.BLOCK_SIZE);
    int primer_bloque_eliminar = FCB->bloque_inicial + nueva_cantidad_bloques;
    int ultimo_bloque_eliminar = FCB->bloque_inicial + cantidad_bloques_actuales;

    liberar_bloques(primer_bloque_eliminar, ultimo_bloque_eliminar);
    actualizar_tamanio_archivo(FCB, nuevo_tamanio);
}

void agrandar_archivo(t_FCB* FCB, int nuevo_tamanio, int PID){
    int cantidad_bloques_actuales = ceil(((double)FCB->tamanio_archivo) / config_IO.BLOCK_SIZE);
    int nueva_cantidad_bloques = ceil(((double)nuevo_tamanio) / config_IO.BLOCK_SIZE);
    
    int primer_nuevo_bloque = FCB->bloque_inicial + cantidad_bloques_actuales;
    int ultimo_nuevo_bloque = FCB->bloque_inicial + nueva_cantidad_bloques;

    if(hay_suficientes_bloques_contiguos(primer_nuevo_bloque, ultimo_nuevo_bloque)){
        ocupar_bloques(primer_nuevo_bloque, ultimo_nuevo_bloque);
        actualizar_tamanio_archivo(FCB, nuevo_tamanio);
    }
    else{
        pthread_mutex_lock(&mutex_log);
	        log_info(IO_log,"PID: < %d > - Inicio Compactación.", PID);
        pthread_mutex_unlock(&mutex_log);

        compactar(FCB);
        usleep(1000*config_IO.RETRASO_COMPACTACION);

         pthread_mutex_lock(&mutex_log);
	        log_info(IO_log,"PID: < %d > - Fin Compactación.", PID);
        pthread_mutex_unlock(&mutex_log);

        // el bloque inicial ahora es otro porque el archivo a amplicar se movio al final del ultimo archivo
        primer_nuevo_bloque = FCB->bloque_inicial + cantidad_bloques_actuales;
        ultimo_nuevo_bloque = FCB->bloque_inicial + nueva_cantidad_bloques;
        ocupar_bloques(primer_nuevo_bloque, ultimo_nuevo_bloque);
        actualizar_tamanio_archivo(FCB, nuevo_tamanio);
        //lo agregamos al final de la lista por ser el de mayor bloque inicial
        list_add(archivos_metadata,FCB);
    }
}

void actualizar_tamanio_archivo(t_FCB* FCB, int nuevo_tamanio){
    FCB->tamanio_archivo = nuevo_tamanio;
    t_config* config = iniciar_config(FCB->path);
    config_set_value(config, "TAMANIO_ARCHIVO", string_itoa(nuevo_tamanio));
    config_destroy(config);
}

void compactar(t_FCB* FCB){
    // hacemos de cuenta que el archivo no esta, marcando sus bloques como libres y sacandolo de la lista
    char* contenido_metadata = extraer_contenido_archivo(FCB); // como sus bloques pueden ser sobrescritos por otros archivos, guardamos el contenido del archivo
    int indice = buscar_FCB_en_archivos_metadata(FCB->archivo); 
    list_remove(archivos_metadata,indice);

    int cantidad_archivos = list_size(archivos_metadata);

    // iteramos la lista de archivos cargados para compactarlos
    for (int i = 0; i < cantidad_archivos; i++) {
        t_FCB* archivo = list_get(archivos_metadata, i);
        int primer_bloque_libre = obtener_primer_bloque_libre();

        if(primer_bloque_libre < FCB->bloque_inicial){// significa que hay espacio libre antes del archivo
            desplazar_metadata(primer_bloque_libre, archivo); // lo escribimos a partir de ese espacio libre
        }
    }

    // volvemos a escribir el contenido del archivo a ampliar al final del ultimo archivo cargado
    int nuevo_bloque_inicial = obtener_primer_bloque_libre();
    realocar_contenido_metadata(nuevo_bloque_inicial, FCB, contenido_metadata);
    free(contenido_metadata);
}

char* extraer_contenido_archivo(t_FCB* FCB){
    char* buffer_lectura = malloc(FCB->tamanio_archivo);

    archivo_bloques = fopen(path_archivo_bloques, "r");
        fseek(archivo_bloques, FCB->bloque_inicial*config_IO.BLOCK_SIZE, SEEK_SET);
        fread(buffer_lectura, sizeof(char),FCB->tamanio_archivo,archivo_bloques);
    fclose(archivo_bloques);

    int cantidad_bloques = ceil(((double)FCB->tamanio_archivo) / config_IO.BLOCK_SIZE);
    liberar_bloques(FCB->bloque_inicial, FCB->bloque_inicial+cantidad_bloques);

    return buffer_lectura;
}

void desplazar_metadata(int nuevo_bloque_inicial, t_FCB* FCB){
    char* contenido_metadata = extraer_contenido_archivo(FCB);
    realocar_contenido_metadata(nuevo_bloque_inicial, FCB, contenido_metadata);
}

void realocar_contenido_metadata(int nuevo_bloque_inicial, t_FCB* FCB, char* contenido_metadata){

    archivo_bloques = fopen(path_archivo_bloques, "r+b");
        fseek(archivo_bloques, nuevo_bloque_inicial*config_IO.BLOCK_SIZE, SEEK_SET);
        fwrite(contenido_metadata, sizeof(char),FCB->tamanio_archivo,archivo_bloques);
    fclose(archivo_bloques);

    actualizar_bloque_inicial(FCB, nuevo_bloque_inicial);
    int cantidad_bloques = ceil(((double)FCB->tamanio_archivo) / config_IO.BLOCK_SIZE);
    ocupar_bloques(FCB->bloque_inicial, FCB->bloque_inicial+cantidad_bloques);
 }

void actualizar_bloque_inicial(t_FCB* FCB, int nuevo_bloque_inicial){
    FCB->bloque_inicial = nuevo_bloque_inicial;
    t_config* config = iniciar_config(FCB->path);
    config_set_value(config, "BLOQUE_INICIAL", string_itoa(nuevo_bloque_inicial));
    config_destroy(config);
}



/////
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

    char *cadena_leida = leer_de_archivo(nombre_archivo, tamanio_a_leer, puntero_archivo);

    enviar_cadena_memoria(PID, cadena_leida, stream);
    usleep(1000*config_IO.TIEMPO_UNIDAD_TRABAJO);
    mandar_aviso_kernel(PID);
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

    usleep(1000*config_IO.TIEMPO_UNIDAD_TRABAJO);
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
    fwrite(cadena, 1, (strlen(cadena) - 1), archivo_bloques);

    fclose(archivo_bloques);
}