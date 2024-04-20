#include <include/main.h>

int main(int argc,char* argv[]) {

    _chequear_parametros(argc, argv);

	init();

    iniciar_servidores();
    conectar_memoria();

    gestionar_conexiones_kernel();

    liberar_memoria();
	return 0;
}

// ----------------------------- Funciones principales

void init(){
    cpu_log = iniciar_logger("cpu_log.log", "CPU", 1, LOG_LEVEL_DEBUG);
    _leer_configuracion(path_config);
}
void iniciar_servidores(void){
    fd_dispatch_server = iniciar_servidor(config_cpu.IP_CPU, config_cpu.PUERTO_ESCUCHA_DISPATCH);    
    fd_interrupt_server = iniciar_servidor(config_cpu.IP_CPU, config_cpu.PUERTO_ESCUCHA_INTERRUPT);
}
void conectar_memoria(void){ //? Conexion con memoria
    fd_cpu = crear_conexion(config_cpu.IP_MEMORIA, config_cpu.PUERTO_MEMORIA);
    enviar_handshake(fd_cpu, "CPU", t_log* cpu_log_debug);
}

// ----------------------------- Funciones auxiliares

void _leer_configuracion(char *path_config){
	t_config* nuevo_config = iniciar_config(path_config);

    config_cpu.IP_CPU                   = strdup(config_get_string_value(nuevo_config,"IP_CPU"));
	config_cpu.IP_MEMORIA               = strdup(config_get_string_value(nuevo_config,"IP_MEMORIA"));
    config_cpu.PUERTO_MEMORIA           = strdup(config_get_string_value(nuevo_config,"PUERTO_MEMORIA"));
    config_cpu.PUERTO_ESCUCHA_DISPATCH  = strdup(config_get_string_value(nuevo_config,"PUERTO_ESCUCHA_DISPATCH"));
    config_cpu.PUERTO_ESCUCHA_INTERRUPT = strdup(config_get_string_value(nuevo_config,"PUERTO_ESCUCHA_INTERRUPT"));
    config_cpu.CANTIDAD_ENTRADAS_TLB    = config_get_int_value(nuevo_config,"CANTIDAD_ENTRADAS_TLB");
    config_cpu.ALGORITMO_TLB            = strdup(config_get_string_value(nuevo_config,"ALGORITMO_TLB"));

	config_destroy(nuevo_config);
}

void liberar_memoria(void){
    free(config_cpu.IP_CPU);
    free(config_cpu.IP_MEMORIA);
    free(config_cpu.PUERTO_MEMORIA);
    free(config_cpu.PUERTO_ESCUCHA_DISPATCH);
    free(config_cpu.PUERTO_ESCUCHA_INTERRUPT);
    free(config_cpu.CANTIDAD_ENTRADAS_TLB);
    free(config_cpu.ALGORITMO_TLB);
}