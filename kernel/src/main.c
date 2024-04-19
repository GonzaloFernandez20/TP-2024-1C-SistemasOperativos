#include <include/main.h>

int main(int argc, char *argv[]){

    if(argc < 1) {
        printf("Faltan parametros de entrada\n");
        return EXIT_FAILURE;
    }
    path_config = strdup(argv[1]); // Esto es para pasar el path del config por linea de comandos

    init(); // Seteamos las configuraciones iniciales de logs y configs
    iniciar_conexiones();


    printf("IP Memoria: %s\n", configuraciones_kernel.IP_MEMORIA);
    printf("Puerto de escucha: %s\n", configuraciones_kernel.PUERTO_ESCUCHA);

    //----------------------------------------------------------------

    return 0;
}

// ----------------------------- Funciones principales
void init(void){
    kernel_log = iniciar_logger("kernel_log", "KERNEL", 1, LOG_LEVEL_DEBUG);
    _leer_configuracion(path_config);
}

void iniciar_conexiones(){
    //int conexionDispatcherConCPU = conectar_con_CPU();
    //int conexionInterruptConCPU  = conectar_con_CPU();

    //int conexionMemoria = conectar_con_MEMORIA();
	// Armamos y enviamos el paquete
	// ! paquete(conexionDispatcherConCPU); 
    
    // Liberamos recursos
	//iberar_recursos(conexionDispatcher, logger, configDispatcher);
}




// ----------------------------- Funciones auxiliares
void _leer_configuracion(char *path_config){
    t_config* archivo_config = iniciar_config(path_config);

<<<<<<< HEAD
    configuraciones_kernel.PUERTO_ESCUCHA = strdup(config_get_string_value(archivo_config,"PUERTO_MEMORIA"));
    configuraciones_kernel.IP_MEMORIA = strdup(config_get_string_value(archivo_config,"IP_MEMORIA"));

=======
    configuraciones_kernel.PUERTO_ESCUCHA = strdup(config_get_string_value(archivo_config, "PUERTO_MEMORIA"));
    configuraciones_kernel.IP_MEMORIA = strdup(config_get_string_value(archivo_config, "IP_MEMORIA"));
        //TODO: strdup hace un malloc, chequear como liberar la memoria pedida...
>>>>>>> origin/RamaGonza
    config_destroy(archivo_config);
}