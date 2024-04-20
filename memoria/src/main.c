#include <include/main.h>

int main(int argc, char* argv[]) {
    
    _chequear_parametros(argc, argv);
    init(); 
    iniciar_servidor_memoria();
    gestionar_conexiones_clientes();

    return 0;
}

// ----------------------------- Funciones principales

// Seteamos las configuraciones iniciales de logs y configs
void init(void){
    memoria_log = iniciar_logger("memoria.log", "MEMORIA", 1, LOG_LEVEL_INFO);
    memoria_log_debugg = iniciar_logger("debugg.log", "MEMORIA", 1, LOG_LEVEL_DEBUG);
    _leer_configuracion(path_config);
}

void iniciar_servidor_memoria(void){
    char* IP = strdup(configuraciones_memoria.IP_MEMORIA);
    char* PUERTO = strdup(configuraciones_memoria.PUERTO_ESCUCHA);

    fd_server_memoria = iniciar_servidor(IP , PUERTO);

    log_info(memoria_log_debugg, "Servidor iniciado! MEMORIA escuchando en %s:%s\n", IP, PUERTO);
}

void gestionar_conexiones_clientes(void){
    atender_cpu();
    atender_kernel();
    atender_entradaSalida();
}

void atender_cpu(void){
    pthread_t thread_cpu;

    int *fd_cpu = malloc(sizeof(int));
        *fd_cpu = esperar_cliente(fd_server_memoria);

    //recibir_handshake(fd_cpu);

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

void *procesar_operacion_cpu(void *fd_cpu_casteado){

    int fd_cpu = _deshacer_casting(fd_cpu_casteado);

    // TODO: Implementacion de las operaciones que puede hacer el cpu

    
}
void *procesar_operacion_kernel(void *fd_kernel_casteado){
    int fd_kernel = _deshacer_casting(fd_kernel_casteado);

    // TODO: Implementacion de las operaciones que puede hacer el kernel

    
}
void *procesar_operacion_entradaSalida(void *fd_ent_sal_casteado){
    int fd_ent_sal = _deshacer_casting(fd_ent_sal_casteado);

    // TODO: Implementacion de las operaciones que puede hacer entrada/salida

    
}

// ----------------------------- Funciones auxiliares

void _leer_configuracion(char *path_config){
    t_config* archivo_config = iniciar_config(path_config);

    configuraciones_memoria.IP_MEMORIA = strdup(config_get_string_value(archivo_config, "IP_MEMORIA"));
    configuraciones_memoria.PUERTO_ESCUCHA = strdup(config_get_string_value(archivo_config, "PUERTO_ESCUCHA"));
    configuraciones_memoria.TAM_MEMORIA = config_get_int_value(archivo_config, "TAM_MEMORIA");
    configuraciones_memoria.TAM_PAGINA = config_get_int_value(archivo_config, "TAM_PAGINA");
    configuraciones_memoria.PATH_INSTRUCCIONES = strdup(config_get_string_value(archivo_config, "PATH_INSTRUCCIONES"));
    configuraciones_memoria.RETARDO_RESPUESTA = config_get_int_value(archivo_config, "RETARDO_RESPUESTA");

    config_destroy(archivo_config);
}

int _deshacer_casting(void *fd_cpu_casteado){ //! Esta funcion deberia ir en utils, porque es generica, si es que se va a usar en otro lado

    int *dato_casteado = (int *)fd_cpu_casteado; //? Convierto el puntero a void en un puntero a int con un nuevo casting
    int dato_sin_casting = *dato_casteado; //? Desreferencio para obtener el valor original

    return dato_sin_casting; //? Devuelvo dicho valor original 
}

void _chequear_parametros(int argc, char* argv[]){ //! Esta funcion deberia ir en utils, porque es generica, si es que se va a usar en otro lado
    if(argc < 1) {
        printf("Faltan parametros de entrada\n");
    }
    path_config = strdup(argv[1]); // Esto es para pasar el path del config por linea de comandos
}