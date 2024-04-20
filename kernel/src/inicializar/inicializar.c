#include <inicializar/inicializar.h>


void init(void){
    kernel_log = iniciar_logger("kernel_log", "KERNEL", 1, LOG_LEVEL_DEBUG);
    _leer_configuracion(path_config);
}

void _leer_configuracion(char *path_config){
    t_config* archivo_config = iniciar_config(path_config);

    config_kernel.IP_KERNEL = strdup(config_get_string_value(archivo_config, "IP_KERNEL"));
    config_kernel.PUERTO_ESCUCHA = strdup(config_get_string_value(archivo_config, "PUERTO_ESCUCHA"));
    config_kernel.IP_MEMORIA = strdup(config_get_string_value(archivo_config, "IP_MEMORIA"));
    config_kernel.PUERTO_MEMORIA = strdup(config_get_string_value(archivo_config, "PUERTO_MEMORIA"));
    config_kernel.IP_CPU = strdup(config_get_string_value(archivo_config, "IP_CPU"));
    config_kernel.PUERTO_CPU_DISPATCH = strdup(config_get_string_value(archivo_config, "PUERTO_CPU_DISPATCH"));
    config_kernel.PUERTO_CPU_INTERRUPT = strdup(config_get_string_value(archivo_config, "PUERTO_CPU_INTERRUPT"));

    config_destroy(archivo_config);
}