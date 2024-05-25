#include <inicializar/inicializar.h>



void chequear_parametros_inicio_interfaz(int argc, char* argv[]){ 
    if(argc < 2) {
        printf("Faltan parametros de entrada\n");
    }
	path_config = strdup(argv[1]); 
	nombre_Interfaz = strdup(argv[2]); 
}

void init(void){
    IO_log = iniciar_logger("entradaSalida.log", "ENTRADASALIDA", 1, LOG_LEVEL_INFO);
    IO_log_debug = iniciar_logger("debug.log", "ENTRADASALIDA", 1, LOG_LEVEL_DEBUG);
    leer_configuracion();
    inicializar_semaforos();
}

void leer_configuracion(){
    t_config* config = iniciar_config(path_config);

    config_IO.TIPO_INTERFAZ = strdup(config_get_string_value(config, "TIPO_INTERFAZ"));
    config_IO.PUERTO_KERNEL = strdup(config_get_string_value(config, "PUERTO_KERNEL"));
    config_IO.IP_KERNEL = strdup(config_get_string_value(config, "IP_KERNEL"));

    if(!string_equals_ignore_case(config_IO.TIPO_INTERFAZ, "Generica")){
        config_IO.PUERTO_MEMORIA = strdup(config_get_string_value(config, "PUERTO_MEMORIA"));
        config_IO.IP_MEMORIA = strdup(config_get_string_value(config, "IP_MEMORIA"));
    }

    if(!string_equals_ignore_case(config_IO.TIPO_INTERFAZ, "STDIN")){
        config_IO.TIEMPO_UNIDAD_TRABAJO = config_get_int_value(config, "TIEMPO_UNIDAD_TRABAJO");
    } 

    if(string_equals_ignore_case(config_IO.TIPO_INTERFAZ, "DialFS")){
        config_IO.PATH_BASE_DIALFS= strdup(config_get_string_value(config, "PATH_BASE_DIALFS"));
        config_IO.BLOCK_SIZE = config_get_int_value(config, "BLOCK_SIZE");
        config_IO.BLOCK_COUNT = config_get_int_value(config, "BLOCK_COUNT");
        config_IO.RETRASO_COMPACTACION = config_get_int_value(config, "RETRASO_COMPACTACION");
    }

    config_destroy(config);
}

void inicializar_semaforos(void){
    pthread_mutex_init(&mutex_log_debug, NULL);
    pthread_mutex_init(&mutex_log, NULL);
}