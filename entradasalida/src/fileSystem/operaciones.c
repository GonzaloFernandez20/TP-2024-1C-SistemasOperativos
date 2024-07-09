// instrucciones
#include <stdio.h>


// Esta instrucción solicita al Kernel que mediante la interfaz seleccionada, se lea desde el archivo a partir del valor del Registro Puntero Archivo la cantidad de bytes indicada por Registro Tamaño y se escriban en la Memoria a partir de la dirección lógica indicada en el Registro Dirección.

void realizar_un_fs_read(){

    t_buffer *buffer = recibir_buffer(fd_conexion_kernel);
	void* stream = buffer->stream;
	int PID = buffer_read_int(&stream);

    int size_nombre_archivo = buffer_read_int(&stream);
    int nombre_archivo = buffer_read_string(&stream, size_nombre_archivo);

    int tamanio_a_leer = buffer_read_int(&stream);
    int puntero_archivo = buffer_read_int(&stream);

    int cant_direcciones = buffer_read_int(&stream);

    // RECIBI TODOS LOS DATOS EXCEPTO LAS DIRECCIONES COMO TAL

    pthread_mutex_lock(&mutex_log);
	    log_info(IO_log,"PID: < %d > - Operacion: IO_FS_READ", PID);
    pthread_mutex_unlock(&mutex_log);

    FILE *archivo = fopen(nombre_archivo, "r"); // se lea desde el archivo...
    char cadena_leida[tamanio_a_leer]; // Buffer para almacenar los caracteres leídos
    
    if (!archivo) { // Capaz es innecesario...
        printf("No se pudo abrir el archivo %s", nombre_archivo);
        return EXIT_FAILURE;
    }

    fseek(archivo, puntero_archivo, SEEK_SET); // a partir del valor del Registro Puntero Archivo... -> Posiciono el puntero en dicho valor
    size_t leidos = fread(cadena_leida, 1, tamanio_a_leer, archivo); // lea desde el archivo [...] la cantidad de bytes indicada por Registro Tamaño

    fclose(archivo);
    // Escritura de los bytes leidos en memoria: 

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
    // ACA YA RECIBI TODO EL BUFFER ENVIADO DESDE KERNEL
    eliminar_buffer(buffer);
    free(cadena_leida);

    mandar_aviso_kernel(PID);
}

 //Esta instrucción solicita al Kernel que mediante la interfaz seleccionada, se lea desde Memoria la cantidad de bytes indicadas por el Registro Tamaño a partir de la dirección lógica que se encuentra en el Registro Dirección y se escriban en el archivo a partir del valor del Registro Puntero Archivo.
void realizar_un_fs_write(){

    t_buffer *buffer = recibir_buffer(fd_conexion_kernel);
	void* stream = buffer->stream;
	int PID = buffer_read_int(&stream);

    int size_nombre_archivo = buffer_read_int(&stream);
    int nombre_archivo = buffer_read_string(&stream, size_nombre_archivo);

    int tamanio_a_leer = buffer_read_int(&stream);
    int puntero_archivo = buffer_read_int(&stream);

    int cant_direcciones = buffer_read_int(&stream);

    // RECIBI TODOS LOS DATOS EXCEPTO LAS DIRECCIONES COMO TAL

    pthread_mutex_lock(&mutex_log);
	    log_info(IO_log,"PID: < %d > - Operacion: IO_STDOUT_WRITE", PID);
    pthread_mutex_unlock(&mutex_log);
    
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
    
    eliminar_buffer(buffer);

    char* cadena = generar_cadena(cadena_rearmada, tamanio_a_leer);

    FILE *archivo = fopen(nombre_archivo, "r+"); // se lea desde el archivo... -> Modo lectura y escritura pero sin borrar su contenido
    fseek(archivo, puntero_archivo, SEEK_SET); // a partir del valor del Registro Puntero Archivo... -> Posiciono el puntero en dicho valor

    fwrite(cadena, sizeof(char), sizeof(cadena) - 1, archivo);

    fclose(archivo);
    
    pthread_mutex_lock(&mutex_log_debug);
        log_info(IO_log_debug,"PID: < %d > - Cadena: < %s >", PID, cadena);
    pthread_mutex_unlock(&mutex_log_debug);
    
    free(cadena_rearmada);
    free(cadena);

    mandar_aviso_kernel(PID);
}