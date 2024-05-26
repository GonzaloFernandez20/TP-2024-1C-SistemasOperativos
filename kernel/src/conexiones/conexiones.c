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

void establecer_conexiones(void){
    conectar_memoria();
    conectar_interrupt();
    conectar_dispatch();
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

void esperar_dispositivos_IO(){
    // Creamos un hilo para atender conexiones de dispositivos IO's
    pthread_t atender_IO;
    pthread_create(&atender_IO, NULL, (void *)atender_entradasalida, NULL);
    pthread_detach(atender_IO);
}


void *atender_entradasalida(){
    interfaces_conectadas =  dictionary_create();
    peticiones_interfaz = dictionary_create();
    cargar_diccionario_instrucciones();
    while(1)
    atender_interfaz((void *)procesar_conexion_es);
}

void cargar_diccionario_instrucciones(void){
     instrucciones_por_interfaz = dictionary_create();

     int instrucciones_GENERICA[2] = {IO_GEN_SLEEP, -1};
     dictionary_put(instrucciones_por_interfaz, "GENERICA", instrucciones_GENERICA);

     int instrucciones_STDIN[2] = {IO_STDIN_READ, -1};
     dictionary_put(instrucciones_por_interfaz, "STDIN", instrucciones_STDIN);

     int instrucciones_STDOUT[2] = {IO_STDOUT_WRITE, -1};
     dictionary_put(instrucciones_por_interfaz, "STDOUT", instrucciones_STDOUT);

     int instrucciones_DIALFS[6] = {IO_FS_CREATE,IO_FS_DELETE, IO_FS_TRUNCATE, IO_FS_READ, IO_FS_READ, -1};
     dictionary_put(instrucciones_por_interfaz, "DIALFS", instrucciones_DIALFS);
}

void atender_interfaz(void (*procesar_conexion)(void*)){

    int *fd_cliente = malloc(sizeof(int));
        *fd_cliente = esperar_cliente(fd_server_kernel);
    
    pthread_mutex_lock(&mutex_log_debug);
    log_info(kernel_log_debugg, "Se conecto un modulo E/S");
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
        log_error(kernel_log_debugg, "No se puedo establecer comuniciacion con el cliente");
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
	log_info(kernel_log_debugg, "Handshake exitoso: Establecida comunicacion con %s:%s\n", nombre_Interfaz, tipo_Interfaz);
    pthread_mutex_unlock(&mutex_log_debug);
	
    registrar_interfaz_conectada(nombre_Interfaz,tipo_Interfaz,fd_cliente);

    free(tipo_Interfaz);
	eliminar_buffer(buffer);

    return nombre_Interfaz;
}

void registrar_interfaz_conectada(char* nombre_interfaz, char* tipo_interfaz, int fd){
    t_estado* cola_bloqueados = malloc(sizeof(t_estado));
    cola_bloqueados->cola = list_create();
    pthread_mutex_init(&(cola_bloqueados->mutex_cola),NULL); 
    cola_bloqueados->nombre = strdup("BLOCKED");

    t_interfaz* interfaz = malloc(sizeof(t_interfaz));
    interfaz->tipo = strdup(tipo_interfaz);
    interfaz->fd = fd;
    interfaz->bloqueados = cola_bloqueados;
    pthread_mutex_init(&(interfaz->interfaz_en_uso), NULL);
    sem_init(&interfaz->hay_peticiones, 0,0);
    pthread_create(&interfaz->peticiones, NULL, (void *)gestionar_peticiones, (void*)interfaz);
    pthread_detach(interfaz->peticiones);

    pthread_mutex_lock(&diccionario_interfaces);
        dictionary_put(interfaces_conectadas, nombre_interfaz, interfaz);
    pthread_mutex_unlock(&diccionario_interfaces);
}

void* gestionar_peticiones(void* interfaz){
    t_interfaz* interfaz_actual = (t_interfaz*)interfaz;

    sem_wait(&interfaz_actual->hay_peticiones);
    pthread_mutex_lock(&interfaz_actual->interfaz_en_uso);

    t_pcb* pcb = list_get(interfaz_actual->bloqueados->cola, 0);
    char* PID = strdup(string_itoa(pcb->pid));

    pthread_mutex_lock(&diccionario_peticiones);
        t_peticion* peticion = dictionary_get(peticiones_interfaz, PID);
    pthread_mutex_unlock(&diccionario_peticiones);

    void (*funcion)(t_peticion*, int, int) = peticion->funcion;
    funcion(peticion, pcb->pid, interfaz_actual->fd);
    
    pthread_mutex_lock(&diccionario_peticiones);
        peticion = dictionary_remove(peticiones_interfaz, PID);
    pthread_mutex_unlock(&diccionario_peticiones);

    free(peticion);
    free(PID);
    
    return NULL;
}

