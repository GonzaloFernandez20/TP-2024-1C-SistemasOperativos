#include <inicializar/inicializar.h>


void init(void){
    kernel_log = iniciar_logger("kernel_debug.log", "KERNEL", 1, LOG_LEVEL_DEBUG);                
    kernel_log_debugg = iniciar_logger("kernel.log", "KERNEL", 0, LOG_LEVEL_DEBUG);
    _leer_configuracion(path_config);
    inicializar_semaforos();
    iniciar_colas_planificacion();
    inicializar_recursos();
    planificacion_pausada = 0;
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
    config_kernel.ALGORITMO_PLANIFICACION = strdup(config_get_string_value(archivo_config, "ALGORITMO_PLANIFICACION"));
    config_kernel.QUANTUM = config_get_int_value(archivo_config, "QUANTUM");
    config_kernel.GRADO_MULTIPROGRAMACION = config_get_int_value(archivo_config, "GRADO_MULTIPROGRAMACION");
    config_kernel.RECURSOS = config_get_array_value(archivo_config, "RECURSOS");
    config_kernel.INSTANCIAS_RECURSOS = config_get_array_value(archivo_config, "INSTANCIAS_RECURSOS");
    
    config_destroy(archivo_config);
}

