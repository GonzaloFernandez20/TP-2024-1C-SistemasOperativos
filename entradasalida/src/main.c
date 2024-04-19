#include <include/main.h>

int main(int argc, char* argv[]) {
    
    if(argc < 1) {
        printf("Faltan parametros de entrada\n");
        return EXIT_FAILURE;
    }
    path_config = strdup(argv[1]);

    init();
    iniciar_conexiones();


    return 0;
}

// ----------------------------- Funciones principales

void init(void){
    entradaSalida_log = iniciar_logger("entradaSalida_log.log", "ENTRADASALIDA", 1, LOG_LEVEL_DEBUG);
    _leer_configuracion(path_config);
}

void iniciar_conexiones(){
    int conexionIOConKERNEL = conectar_con_KERNEL();
    int conexionIOConMEMORIA = conectar_con_MEMORIA();
}

// ----------------------------- Funciones auxiliares
void _leer_configuracion(char *path_config){
    t_config* archivo_config = iniciar_config(path_config);

    configuraciones_entradaSalida.PUERTO_MEMORIA = strdup(config_get_string_value(archivo_config, "PUERTO_MEMORIA"));
    configuraciones_entradaSalida.IP_MEMORIA = strdup(config_get_string_value(archivo_config, "IP_MEMORIA"));
    configuraciones_entradaSalida.PUERTO_KERNEL = strdup(config_get_string_value(archivo_config, "PUERTO_KERNEL"));
    configuraciones_entradaSalida.IP_KERNEL = strdup(config_get_string_value(archivo_config, "IP_KERNEL"));
        //TODO: strdup hace un malloc, chequear como liberar la memoria pedida...
    config_destroy(archivo_config);
}