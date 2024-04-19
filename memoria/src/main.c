#include <include/main.h>

int main(int argc, char* argv[]) {
    
    if(argc < 1) {
        printf("Faltan parametros de entrada\n");
        return EXIT_FAILURE;
    }
    path_config = strdup(argv[1]); // Esto es para pasar el path del config por linea de comandos

    init(); // Seteamos las configuraciones iniciales de logs y configs

    printf("IP MEMORIA: %s\n", configuraciones_memoria.PUERTO_ESCUCHA);
    printf("TAM MEMORIA: %d\n", configuraciones_memoria.TAM_MEMORIA);
    printf("PATH DE INSTRUCCIONES: %s\n", configuraciones_memoria.PATH_INSTRUCCIONES);

    while (0)
    {
        /* implementacion de hilos para atender clientes */
    }
       
    return 0;
}

// ----------------------------- Funciones principales

void init(void){
    memoria_log = iniciar_logger("memoria_log", "MEMORIA", 1, LOG_LEVEL_DEBUG);
    _leer_configuracion(path_config);
}


// ----------------------------- Funciones auxiliares

void _leer_configuracion(char *path_config){
    t_config* archivo_config = iniciar_config(path_config);

    configuraciones_memoria.PUERTO_ESCUCHA = strdup(config_get_string_value(archivo_config, "PUERTO_ESCUCHA"));
    configuraciones_memoria.TAM_MEMORIA = config_get_int_value(archivo_config, "TAM_MEMORIA");
    configuraciones_memoria.TAM_PAGINA = config_get_int_value(archivo_config, "TAM_PAGINA");
    configuraciones_memoria.PATH_INSTRUCCIONES = strdup(config_get_string_value(archivo_config, "PATH_INSTRUCCIONES"));
    configuraciones_memoria.RETARDO_RESPUESTA = config_get_int_value(archivo_config, "RETARDO_RESPUESTA");

    config_destroy(archivo_config);
}