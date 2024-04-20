#include <inicializar/inicializar.h>

void init(void){
    IO_log = iniciar_logger("entradaSalida.log", "ENTRADASALIDA", 1, LOG_LEVEL_INFO);
    IO_log_debug = iniciar_logger("debug.log", "ENTRADASALIDA", 1, LOG_LEVEL_DEBUG);
    _leer_configuracion(path_config);
}

void _leer_configuracion(char *path_config){
    t_config* config = iniciar_config(path_config);

    config_IO.PUERTO_MEMORIA = strdup(config_get_string_value(config, "PUERTO_MEMORIA"));
    config_IO.IP_MEMORIA = strdup(config_get_string_value(config, "IP_MEMORIA"));
    config_IO.PUERTO_KERNEL = strdup(config_get_string_value(config, "PUERTO_KERNEL"));
    config_IO.IP_KERNEL = strdup(config_get_string_value(config, "IP_KERNEL"));
    //agregar demas configuraciones

    config_destroy(config);
}