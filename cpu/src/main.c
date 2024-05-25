#include <include/main.h>

int main(int argc,char* argv[]) {

    path_config = chequear_parametros(argc, argv);

	init();
    iniciar_servidores();
    conectar_memoria();
    gestionar_conexiones_kernel();
    arranque();
    

    liberar_cpu();
    
	return 0;
}


/*Se libera toda la memoria asignada durante la ejecucion de Cpu.*/
void liberar_cpu(void){
    free(config_cpu.IP_CPU);
    free(config_cpu.IP_MEMORIA);
    free(config_cpu.PUERTO_MEMORIA);
    free(config_cpu.PUERTO_ESCUCHA_DISPATCH);
    free(config_cpu.PUERTO_ESCUCHA_INTERRUPT);
    free(config_cpu.ALGORITMO_TLB);

    free(path_config);
    log_destroy(cpu_log);
    log_destroy(cpu_log_debug);
    liberar_conexion(fd_dispatch_server);
    liberar_conexion(fd_interrupt_server);
    liberar_conexion(fd_conexion_memoria);
}