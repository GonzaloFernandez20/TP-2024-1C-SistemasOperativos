#include <conexiones/conexiones.h>

void iniciar_servidor_kernel(void){
    char* IP = strdup(config_kernel.IP_KERNEL);
    char* PUERTO = strdup(config_kernel.PUERTO_ESCUCHA);

    fd_server_kernel = iniciar_servidor(IP , PUERTO);

    if(fd_server_kernel == -1){
        log_error(kernel_log_debugg,"No se pudo iniciar servidor KERNEL");
    }
    else{
        log_info(kernel_log_debugg, "Servidor iniciado! KERNEL escuchando en %s:%s\n", IP, PUERTO);
    }

    free(IP);
    free(PUERTO);
}

void atender_entradasalida(){
    while (1)
    {
        atender_cliente(fd_server_kernel, (void *)procesar_conexion_es, kernel_log_debugg, "ENTRADA/SALIDA");
        entrada_salida_conectada = 1;

        //while(entrada_salida_conectada){
            // CONTINGENCIA: se borra en cuanto veamos señales y semaforos
            // es temporal para que el hilo main no finalice el programa
        //}
        
    }
    
    
}


void establecer_conexiones(void){
    conectar_memoria();
    conectar_dispatch();
    conectar_interrupt();
}

void conectar_memoria(void){ 
    char* IP = strdup(config_kernel.IP_MEMORIA);
    char* PUERTO = strdup(config_kernel.PUERTO_MEMORIA);

    fd_conexion_memoria = crear_conexion(config_kernel.IP_MEMORIA, config_kernel.PUERTO_MEMORIA);

    if(fd_conexion_memoria == -1){
        log_error(kernel_log_debugg,"KERNEL no se conecto a MEMORIA\n");
    }
    else{
        log_info(kernel_log_debugg, "KERNEL conectado a MEMORIA en %s:%s", IP, PUERTO);
        enviar_handshake(fd_conexion_memoria, "KERNEL", kernel_log_debugg);
    }
    
    free(IP);
    free(PUERTO);
}

void conectar_dispatch(void){
    char* IP = strdup(config_kernel.IP_CPU);
    char* PUERTO = strdup(config_kernel.PUERTO_CPU_DISPATCH);

    fd_conexion_dispatch = crear_conexion(IP, PUERTO);

    if(fd_conexion_dispatch == -1){
        log_error(kernel_log_debugg,"KERNEL no se conecto a CPU DISPATCH\n");
    }
    else{
        log_info(kernel_log_debugg, "KERNEL conectado a CPU DISPATCH en %s:%s", IP, PUERTO);
        enviar_handshake(fd_conexion_dispatch, "KERNEL", kernel_log_debugg);
    }
    
    free(IP);
    free(PUERTO);
}

void conectar_interrupt(void){
    char* IP = strdup(config_kernel.IP_CPU);
    char* PUERTO = strdup(config_kernel.PUERTO_CPU_INTERRUPT);

    fd_conexion_interrupt = crear_conexion(IP, PUERTO);

    if(fd_conexion_interrupt == -1){
        log_error(kernel_log_debugg,"KERNEL no se conecto a CPU INTERRUPT\n");
    }
    else{
        log_info(kernel_log_debugg, "KERNEL conectado a CPU INTERRUPT  en %s:%s", IP, PUERTO);
        enviar_handshake(fd_conexion_interrupt, "KERNEL", kernel_log_debugg);
    }

    free(IP);
    free(PUERTO);
}

void atender_entradasalida(){
    while(1)
    atender_interfaz((void *)procesar_conexion_es);
}

void atender_interfaz(void (*procesar_conexion)(void*)){

    int *fd_cliente = malloc(sizeof(int));
        *fd_cliente = esperar_cliente(fd_server_kernel);
    
    pthread_mutex_lock(&mutex_log_debug);
    log_info(kernel_log_debugg, "Se conecto un modulo E/S");
    pthread_mutex_unlock(&mutex_log_debug);

    recibir_handshake_IO(*fd_cliente);

    asignar_hilo(fd_cliente, procesar_conexion);
}

void recibir_handshake_IO(int fd_cliente){
   
    int handshake_ok = 0;
    int handshake_error = -1;
    
    int handshake = recibir_operacion(fd_cliente);

    if(handshake == 1){
        send(fd_cliente, &handshake_ok, sizeof(int), 0);
        recibir_presentacion_IO(fd_cliente);
    }
    else{
        send(fd_cliente, &handshake_error, sizeof(int), 0);

        pthread_mutex_lock(&mutex_log_debug);
        log_error(kernel_log_debugg, "No se puedo establecer comuniciacion con el cliente");
        pthread_mutex_unlock(&mutex_log_debug);
    }
}

void recibir_presentacion_IO(int fd_cliente){

	t_buffer *buffer = recibir_buffer(fd_cliente);
	void* stream = buffer->stream;
	
	int length_nombre = buffer_read_int(&stream);
	char* nombre_Interfaz = malloc(length_nombre);
	strcpy(nombre_Interfaz, buffer_read_string(&stream, length_nombre));

    int length_tipo = buffer_read_int(&stream);
	char* tipo_Interfaz = malloc(length_tipo);
	strcpy(tipo_Interfaz, buffer_read_string(&stream, length_tipo));

    pthread_mutex_lock(&mutex_log_debug);
	log_info(kernel_log_debugg, "Handshake exitoso: Establecida comunicacion con %s:%s\n", nombre_Interfaz, tipo_Interfaz);
    pthread_mutex_unlock(&mutex_log_debug);
	
    //registrar_interfaz_conectada(nombre_Interfaz,tipo_Interfaz,fd_cliente);

	free(nombre_Interfaz);
    free(tipo_Interfaz);
	eliminar_buffer(buffer);
}

