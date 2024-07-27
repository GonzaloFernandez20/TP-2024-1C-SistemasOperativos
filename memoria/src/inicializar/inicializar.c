#include <inicializar/inicializar.h>


void init(void){
    iniciar_loggers();
    leer_configuracion(path_config);
    inicializar_estructuras_memoria();
    inicializar_semaforos();

}

void iniciar_loggers(void){
    memoria_log = iniciar_logger("memoria.log", "MEMORIA", 1, LOG_LEVEL_INFO);
    memoria_log_debugg = iniciar_logger("debugg.log", "MEMORIA", 0, LOG_LEVEL_DEBUG);
}

void leer_configuracion(char *path_config){
    t_config* config = iniciar_config(path_config);

    config_memoria.IP_MEMORIA = strdup(config_get_string_value(config, "IP_MEMORIA"));
    config_memoria.PUERTO_ESCUCHA = strdup(config_get_string_value(config, "PUERTO_ESCUCHA"));
    config_memoria.TAM_MEMORIA = config_get_int_value(config, "TAM_MEMORIA");
    config_memoria.TAM_PAGINA = config_get_int_value(config, "TAM_PAGINA");
    config_memoria.PATH_INSTRUCCIONES = strdup(config_get_string_value(config, "PATH_INSTRUCCIONES"));
    config_memoria.RETARDO_RESPUESTA = config_get_int_value(config, "RETARDO_RESPUESTA");

    config_destroy(config);
}

void inicializar_estructuras_memoria(void){
    memoria_de_instrucciones = list_create();
    tablas_de_paginas = list_create();

    espacio_usuario = malloc(config_memoria.TAM_MEMORIA);

    cantidad_marcos = config_memoria.TAM_MEMORIA/config_memoria.TAM_PAGINA;
    bitarray = init_bitarray();

}

t_bitarray* init_bitarray(void){
    int cantidad_bits = (cantidad_marcos + 7) / 8; // es para que se redondee para arriba y no haya menos bits que marcos
    void* espacio_bitarray = malloc(cantidad_bits);
    memset(espacio_bitarray, 0, cantidad_bits); 
    t_bitarray* un_bitarray = bitarray_create_with_mode (espacio_bitarray, cantidad_bits, LSB_FIRST);
    return un_bitarray;
}


void inicializar_semaforos(void){
    pthread_mutex_init(&mutex_log_debug, NULL);
    pthread_mutex_init(&mutex_log, NULL);
    pthread_mutex_init(&mutex_memoria_instrucciones, NULL);
    pthread_mutex_init(&mutex_espacio_usuario, NULL);
    pthread_mutex_init(&mutex_bitarray, NULL);
    pthread_mutex_init(&mutex_tablas_de_paginas, NULL);
    pthread_mutex_init(&diccionario_interfaces, NULL);
}