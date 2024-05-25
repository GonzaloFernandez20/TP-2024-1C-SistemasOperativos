#include <conexiones/conexiones.h>

void establecer_conexiones(void){
    if(!string_equals_ignore_case(config_IO.TIPO_INTERFAZ, "GENERICA")){
        conectar_memoria();
    }
    conectar_kernel();
}

void conectar_memoria(void){ 
    char* IP = strdup(config_IO.IP_MEMORIA);
    char* PUERTO = strdup(config_IO.PUERTO_MEMORIA);

    fd_conexion_memoria = crear_conexion(IP, PUERTO);

    if(fd_conexion_memoria == -1){
        
        pthread_mutex_lock(&mutex_log_debug);
        log_error(IO_log_debug,"%s:%s no se conecto a MEMORIA\n", nombre_Interfaz, config_IO.TIPO_INTERFAZ);
        pthread_mutex_unlock(&mutex_log_debug);
    }
    else{
        
        pthread_mutex_lock(&mutex_log_debug);
        log_info(IO_log_debug, "%s:%s conectado a MEMORIA en %s:%s\n", nombre_Interfaz, config_IO.TIPO_INTERFAZ, IP, PUERTO);
        pthread_mutex_unlock(&mutex_log_debug);

        enviar_handshake_IO(fd_conexion_memoria, nombre_Interfaz, config_IO.TIPO_INTERFAZ);
    }

    free(IP);
    free(PUERTO);
}

void conectar_kernel(void){ 
    char* IP = strdup(config_IO.IP_KERNEL);
    char* PUERTO = strdup(config_IO.PUERTO_KERNEL);

    fd_conexion_kernel = crear_conexion(IP, PUERTO);

    if(fd_conexion_kernel == -1){

        pthread_mutex_lock(&mutex_log_debug);
        log_error(IO_log_debug,"%s:%s no se conecto a KERNEL\n", nombre_Interfaz, config_IO.TIPO_INTERFAZ);
        pthread_mutex_unlock(&mutex_log_debug);
    }
    else{
        pthread_mutex_lock(&mutex_log_debug);
        log_info(IO_log_debug, "%s:%s conectado a KERNEL en %s:%s\n", nombre_Interfaz, config_IO.TIPO_INTERFAZ, IP, PUERTO);
        pthread_mutex_unlock(&mutex_log_debug);

        enviar_handshake_IO(fd_conexion_kernel, nombre_Interfaz, config_IO.TIPO_INTERFAZ);
    }

    free(IP);
    free(PUERTO);
}

void enviar_handshake_IO(int fd_conexion, char* nombre_Interfaz, char* tipo_Interfaz){
    int respuesta_servidor;

    enviar_presentacion_IO(nombre_Interfaz, tipo_Interfaz, fd_conexion);
    recv(fd_conexion, &respuesta_servidor, sizeof(int), MSG_WAITALL);

    if (respuesta_servidor == 0) {
        pthread_mutex_lock(&mutex_log_debug);
        log_info(IO_log_debug, "Handshake exitoso: comunicacion con servidor establecida\n");
        pthread_mutex_unlock(&mutex_log_debug);
    } 
    else {
        pthread_mutex_lock(&mutex_log_debug);
        log_error(IO_log_debug, "Error handshake: no se establecio comunicacion con servidor");
        pthread_mutex_unlock(&mutex_log_debug);
    }

}

void enviar_presentacion_IO(char* nombre_Interfaz, char* tipo_Interfaz, int socket_cliente){
	t_paquete* paquete = crear_paquete(HANDSHAKE);

	int buffer_size = strlen(tipo_Interfaz) + 1 + strlen(nombre_Interfaz) + 1 + 2*sizeof(int); 
	crear_buffer(paquete, buffer_size);


	buffer_add_int(paquete->buffer, strlen(nombre_Interfaz) + 1 );
	buffer_add_string(paquete->buffer, nombre_Interfaz);
	buffer_add_int(paquete->buffer, strlen(tipo_Interfaz) + 1 );
	buffer_add_string(paquete->buffer, tipo_Interfaz);

	enviar_paquete(paquete, socket_cliente);
	eliminar_paquete(paquete);
}