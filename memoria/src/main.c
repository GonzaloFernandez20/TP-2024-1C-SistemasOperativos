#include <include/main.h>

int main(int argc, char* argv[]) {
    
    if(argc < 1) {
        printf("Faltan parametros de entrada\n");
        return EXIT_FAILURE;
    }
    path_config = strdup(argv[1]); // Esto es para pasar el path del config por linea de comandos

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
    int fd_cpu = esperar_cliente(fd_server_memoria);
    recibir_handshake(fd_cpu);

}

// void procesar_operacion_kernel()
// void procesar_operacion_cpu()
// void procesar_operacion_entradaSalida()




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