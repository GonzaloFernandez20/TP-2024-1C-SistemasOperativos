#include <include/main.h>

//CREACIÓN DEL CLIENTE (dispatcher)

int main(int argc, char *argv[]){

    if(argc < 1) {
        printf("Faltan parametros de entrada\n");
        return EXIT_FAILURE;
    }
    char *path_config = strdup(argv[1]); // Esto es para pasar el path del config por linea de comandos

    kernel_log = iniciar_logger();
    leer_configuracion(path_config);

    //----------------------------------------------------------------

    int conexionDispatcherConCPU = conectar_con_CPU();
    int conexionInterruptConCPU  = conectar_con_CPU();

    int conexionMemoria = conectar_con_MEMORIA();
	// Armamos y enviamos el paquete
	// ! paquete(conexionDispatcherConCPU); 
    
    // Liberamos recursos
	//iberar_recursos(conexionDispatcher, logger, configDispatcher);
    return 0;
}


// prueba
    

void leer_configuracion(char *path_config){
    t_config* archivo_config = iniciar_config(path_config);

    configuraciones_kernel.PUERTO_ESCUCHA = strdup(config_get_string_value(archivo_config,"PUERTO_MEMORIA"));
    configuraciones_kernel.IP_MEMORIA = strdup(config_get_string_value(un_config,"IP_MEMORIA"));

    config_destroy(archivo_config);
}