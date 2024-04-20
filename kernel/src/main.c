#include <include/main.h>

int main(int argc, char *argv[]){
    _chequear_parametros(argc, argv);

    init(); // Seteamos las configuraciones iniciales de logs y configs
    //iniciar_conexiones();
    conectar_kernel();
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

void conectar_kernel(void){
    fd_kernel = crear_conexion(config_kernel.IP_MEMORIA, config_kernel.PUERTO_MEMORIA);
    if(fd_kernel != (-1)){ puts("Me conecte"); }
}

// ----------------------------- Funciones auxiliares
void _leer_configuracion(char *path_config){
    t_config* archivo_config = iniciar_config(path_config);

    config_kernel.PUERTO_ESCUCHA = strdup(config_get_string_value(archivo_config, "PUERTO_ESCUCHA"));
    config_kernel.IP_MEMORIA = strdup(config_get_string_value(archivo_config, "IP_MEMORIA"));
    config_kernel.PUERTO_MEMORIA = strdup(config_get_string_value(archivo_config, "PUERTO_MEMORIA"));

    config_destroy(archivo_config);
}
