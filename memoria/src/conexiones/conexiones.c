#include <conexiones/conexiones.h>

void iniciar_servidor_memoria(void){
    char* IP = strdup(config_memoria.IP_MEMORIA);
    char* PUERTO = strdup(config_memoria.PUERTO_ESCUCHA);

    fd_server_memoria = iniciar_servidor(IP , PUERTO);

    log_info(memoria_log_debugg, "Servidor iniciado! MEMORIA escuchando en %s:%s\n", IP, PUERTO);

    free(IP);
    free(PUERTO);
}

void gestionar_conexiones_clientes(void){
   atender_cpu();
   atender_kernel();
   atender_entradaSalida();
}

void atender_cpu(void){
   
    int *fd_cpu = malloc(sizeof(int));
        *fd_cpu = esperar_cliente(fd_server_memoria);

    //recibir_handshake(fd_cpu);

    pthread_t thread_cpu;
	pthread_create(&thread_cpu, NULL, (void*) procesar_operacion_cpu, (void *)fd_cpu);
	log_info(memoria_log_debugg, "hilo [%lu] recibio a CPU\n", thread_cpu);

	pthread_detach(thread_cpu);
}
void atender_kernel(void){
    pthread_t thread_kernel;

    int *fd_kernel = malloc(sizeof(int));
        *fd_kernel = esperar_cliente(fd_server_memoria);

    //recibir_handshake(fd_kernel);

	pthread_create(&thread_kernel, NULL, (void*) procesar_operacion_kernel, (void *)fd_kernel);
	//log_info(memoria_log_debugg, "hilo [%lu] listo para atender al cliente", thread_kernel);

	pthread_detach(thread_kernel);
}
void atender_entradaSalida(void){
    pthread_t thread_ent_sal;

    int *fd_ent_sal = malloc(sizeof(int));
        *fd_ent_sal = esperar_cliente(fd_server_memoria);

    //recibir_handshake(fd_ent_sal);

	pthread_create(&thread_ent_sal, NULL, (void*) procesar_operacion_kernel, (void *)fd_ent_sal);
	//log_info(memoria_log_debugg, "hilo [%lu] listo para atender al cliente", thread_ent_sal);

	pthread_detach(thread_ent_sal);
}
