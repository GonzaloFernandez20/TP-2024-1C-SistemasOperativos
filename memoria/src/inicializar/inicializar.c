#include <inicializar/inicializar.h>


void init(void){
    memoria_log = iniciar_logger("memoria.log", "MEMORIA", 1, LOG_LEVEL_INFO);
    memoria_log_debugg = iniciar_logger("debugg.log", "MEMORIA", 1, LOG_LEVEL_DEBUG);
    _leer_configuracion(path_config);
}

void _leer_configuracion(char *path_config){
    t_config* config = iniciar_config(path_config);

    config_memoria.IP_MEMORIA = strdup(config_get_string_value(config, "IP_MEMORIA"));
    config_memoria.PUERTO_ESCUCHA = strdup(config_get_string_value(config, "PUERTO_ESCUCHA"));
    config_memoria.TAM_MEMORIA = config_get_int_value(config, "TAM_MEMORIA");
    config_memoria.TAM_PAGINA = config_get_int_value(config, "TAM_PAGINA");
    config_memoria.PATH_INSTRUCCIONES = strdup(config_get_string_value(config, "PATH_INSTRUCCIONES"));
    config_memoria.RETARDO_RESPUESTA = config_get_int_value(config, "RETARDO_RESPUESTA");

    config_destroy(config);
}
