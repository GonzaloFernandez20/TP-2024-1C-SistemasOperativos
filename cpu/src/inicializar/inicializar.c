#include <inicializar/inicializar.h>

void init(){
    cpu_log = iniciar_logger("cpu.log", "CPU", 1, LOG_LEVEL_INFO);
    cpu_log_debug = iniciar_logger("debugg.log", "CPU", 1, LOG_LEVEL_DEBUG);
    _leer_configuracion(path_config);
}

void _leer_configuracion(char *path_config){
	t_config* nuevo_config = iniciar_config(path_config);

    config_cpu.IP_CPU = strdup(config_get_string_value(nuevo_config,"IP_CPU"));
	config_cpu.IP_MEMORIA = strdup(config_get_string_value(nuevo_config,"IP_MEMORIA"));
    config_cpu.PUERTO_MEMORIA = strdup(config_get_string_value(nuevo_config,"PUERTO_MEMORIA"));
    config_cpu.PUERTO_ESCUCHA_DISPATCH = strdup(config_get_string_value(nuevo_config,"PUERTO_ESCUCHA_DISPATCH"));
    config_cpu.PUERTO_ESCUCHA_INTERRUPT = strdup(config_get_string_value(nuevo_config,"PUERTO_ESCUCHA_INTERRUPT"));
    config_cpu.CANTIDAD_ENTRADAS_TLB = config_get_int_value(nuevo_config,"CANTIDAD_ENTRADAS_TLB");
    config_cpu.ALGORITMO_TLB = strdup(config_get_string_value(nuevo_config,"ALGORITMO_TLB"));

	config_destroy(nuevo_config);
}