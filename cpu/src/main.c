#include <include/main.h>

int main(int argc,char* argv[]) {

    if(argc < 1) {
            printf("Faltan parametros de entrada\n");
            return EXIT_FAILURE;
        }
    char *path_config = strdup(argv[1]); // Esto es para pasar el path del config por linea de comandos

	init();

    //conectar_dispatch(); //TODO
    //conectar_interruptor(); //TODO

	return 0;
}

// ----------------------------- Funciones principales

void init(){
    cpu_log = iniciar_logger();
    _leer_configuracion(path_config);

}



// ----------------------------- Funciones auxiliares

void _leer_configuracion(char *path_config){
	t_config* nuevo_config = iniciar_config(path_config);

	config_cpu.IP_MEMORIA               = config_get_string_value(nuevo_config,"IP_MEMORIA");
    config_cpu.PUERTO_MEMORIA           = config_get_string_value(nuevo_config,"PUERTO_MEMORIA");
    config_cpu.PUERTO_ESCUCHA_DISPATCH  = config_get_string_value(nuevo_config,"PUERTO_ESCUCHA_DISPATCH");
    config_cpu.PUERTO_ESCUCHA_INTERRUPT = config_get_string_value(nuevo_config,"PUERTO_ESCUCHA_INTERRUPT");
    config_cpu.CANTIDAD_ENTRADAS_TLB    = config_get_string_value(nuevo_config,"CANTIDAD_ENTRADAS_TLB");
    config_cpu.ALGORITMO_TLB            = config_get_string_value(nuevo_config,"ALGORITMO_TLB");

	config_destroy(nuevo_config);
}

