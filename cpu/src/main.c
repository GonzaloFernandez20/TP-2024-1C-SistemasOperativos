#include <include/main.h>

int main(int argc,char* argv[]) {

    path_config = chequear_parametros(argc, argv);

	init();
    iniciar_servidores();
    conectar_memoria();
    gestionar_conexiones_kernel();


    // arranque();


    
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

/*
En este momento, se deberá chequear si el Kernel nos envió una interrupción al PID que se está
ejecutando, en caso afirmativo, se devuelve el Contexto de Ejecución actualizado al Kernel con
motivo de la interrupción. Caso contrario, se descarta la interrupción.

Cabe aclarar que en todos los casos el Contexto de Ejecución debe ser devuelto a través de la
conexión de dispatch, quedando la conexión de interrupt dedicada solamente a recibir mensajes de
interrupción.
*/

int hayInterrupcion(){

}

exportarPCB(){}


recibirPCB(){}
