// instrucciones
#include <fileSystem/operaciones.h>

// Esta instrucción solicita al Kernel que mediante la interfaz seleccionada, se lea desde el archivo a partir del valor del Registro Puntero Archivo la cantidad de bytes indicada por Registro Tamaño y se escriban en la Memoria a partir de la dirección lógica indicada en el Registro Dirección.

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
    pthread_mutex_unlock(&mutex_log);

    char *cadena_leida = leer_de_archivo(nombre_archivo, tamanio_a_leer, puntero_archivo); // -> Codear...

    enviar_cadena_memoria(PID, cadena_leida, stream);
    eliminar_buffer(buffer);
}

 //Esta instrucción solicita al Kernel que mediante la interfaz seleccionada, se lea desde Memoria la cantidad de bytes indicadas por el Registro Tamaño a partir de la dirección lógica que se encuentra en el Registro Dirección y se escriban en el archivo a partir del valor del Registro Puntero Archivo.
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

    /* FILE *archivo = fopen(nombre_archivo, "r"); // se lea desde el archivo...
    char *cadena_leida = malloc(tamanio_a_leer); // Buffer para almacenar los caracteres leídos
    
    // Capaz es innecesario...
    if (!archivo) { 
        pthread_mutex_lock(&mutex_log);
	        log_info(IO_log,"No se pudo abrir el archivo %s", nombre_archivo);
        pthread_mutex_unlock(&mutex_log);    
    } 

    fseek(archivo, puntero_archivo, SEEK_SET); // a partir del valor del Registro Puntero Archivo... -> Posiciono el puntero en dicho valor
    fread(cadena_leida, 1, tamanio_a_leer, archivo); // lea desde el archivo [...] la cantidad de bytes indicada por Registro Tamaño

    fclose(archivo);*/

    return cadena_leida;
}

void escribir_en_archivo(char *nombre_archivo, int puntero_archivo, char *cadena){
    // LOGICA DE ENCONTRAR EL BLOQUE RESPECTO AL BYTE Y ESCRIBIR DESDE AHI EN BLOQUES.DAT
}