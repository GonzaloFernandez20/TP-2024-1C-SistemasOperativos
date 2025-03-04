#include <conexiones/conexiones.h>

void iniciar_servidor_memoria(void){
    char* IP = strdup(config_memoria.IP_MEMORIA);
    char* PUERTO = strdup(config_memoria.PUERTO_ESCUCHA);

    fd_server_memoria = iniciar_servidor(IP , PUERTO);

    if(fd_server_memoria == -1){
        log_error(memoria_log_debugg,"No se pudo iniciar servidor MEMORIA");
    }
    else{
        log_info(memoria_log_debugg, "Servidor iniciado! MEMORIA escuchando en %s:%s\n", IP, PUERTO);
    }

    free(IP);
    free(PUERTO);
}

void gestionar_conexiones_clientes(void){
   atender_cpu();
   atender_kernel();
   atender_entradaSalida();
}

void atender_cpu(void){
    int *fd_cliente = malloc(sizeof(int));
        *fd_cliente = esperar_cliente(fd_server_memoria);
    
    
    log_info(memoria_log_debugg, "Se conecto %s", "CPU");

    recibir_handshake_cpu(*fd_cliente, memoria_log_debugg);

    asignar_hilo(fd_cliente, (void *)procesar_operacion_cpu);


   
}

void recibir_handshake_cpu(int fd_cliente, t_log* logger){
    
    int handshake_ok = config_memoria.TAM_PAGINA;
    int handshake_error = -1;
    
    int handshake = recibir_operacion(fd_cliente);

    if(handshake == 1){
        send(fd_cliente, &handshake_ok, sizeof(int), 0);
        recibir_presentacion(fd_cliente, logger);
    }
    else{
        send(fd_cliente, &handshake_error, sizeof(int), 0);
        log_error(logger, "No se puedo establecer comuniciacion con el cliente");
    }
}


void atender_kernel(void){
    atender_cliente(fd_server_memoria, (void *)procesar_operacion_kernel, memoria_log_debugg, "KERNEL");
}

void atender_entradaSalida(void){
    interfaces_conectadas = dictionary_create();
    while(1)
    atender_interfaz((void *)procesar_operacion_entradaSalida);
}

void atender_interfaz(void (*procesar_conexion)(void*)){

    int *fd_cliente = malloc(sizeof(int));
        *fd_cliente = esperar_cliente(fd_server_memoria);
    
    pthread_mutex_lock(&mutex_log_debug);
    log_info(memoria_log_debugg, "Se conecto un modulo E/S");
    pthread_mutex_unlock(&mutex_log_debug);

    char* nombre_interfaz = strdup(recibir_handshake_IO(*fd_cliente));

    asignar_hilo_interfaz(nombre_interfaz, procesar_conexion);
}

void asignar_hilo_interfaz(char *nombre_interfaz,  void (*procesar_conexion)(void*)){
    pthread_t thread_cliente;

	pthread_create(&thread_cliente, NULL, (void *)procesar_conexion, (void *)nombre_interfaz);

	pthread_detach(thread_cliente);
}

char* recibir_handshake_IO(int fd_cliente){
   
    int handshake_ok = 0;
    int handshake_error = -1;
     char* nombre_interfaz;
    
    int handshake = recibir_operacion(fd_cliente);

    if(handshake == 1){
        send(fd_cliente, &handshake_ok, sizeof(int), 0);
        nombre_interfaz = strdup(recibir_presentacion_IO(fd_cliente));
    }
    else{
        send(fd_cliente, &handshake_error, sizeof(int), 0);

        pthread_mutex_lock(&mutex_log_debug);
        log_error(memoria_log_debugg, "No se puedo establecer comuniciacion con el cliente");
        pthread_mutex_unlock(&mutex_log_debug);
    }

    return nombre_interfaz;
}


char* recibir_presentacion_IO(int fd_cliente){

	t_buffer *buffer = recibir_buffer(fd_cliente);
	void* stream = buffer->stream;
	
	int length_nombre = buffer_read_int(&stream);
	char* nombre_Interfaz = malloc(length_nombre);
	strcpy(nombre_Interfaz, buffer_read_string(&stream, length_nombre));

    int length_tipo = buffer_read_int(&stream);
	char* tipo_Interfaz = malloc(length_tipo);
	strcpy(tipo_Interfaz, buffer_read_string(&stream, length_tipo));

    pthread_mutex_lock(&mutex_log_debug);
	log_info(memoria_log_debugg, "Handshake exitoso: Establecida comunicacion con %s:%s\n", nombre_Interfaz, tipo_Interfaz);
    pthread_mutex_unlock(&mutex_log_debug);
	
    registrar_interfaz_conectada(nombre_Interfaz,fd_cliente);

    free(tipo_Interfaz);
	eliminar_buffer(buffer);

    return nombre_Interfaz;
}

void registrar_interfaz_conectada(char* nombre_interfaz, int fd){
    // Inicializacion de la interfaz
    t_interfaz* interfaz = malloc(sizeof(t_interfaz));
    interfaz->nombre = strdup(nombre_interfaz);
    interfaz->fd = fd;

    pthread_mutex_lock(&diccionario_interfaces);
        dictionary_put(interfaces_conectadas, nombre_interfaz, interfaz);
    pthread_mutex_unlock(&diccionario_interfaces);
}
