#include <include/main.h>

int main(int argc, char *argv[]){

    path_config = chequear_parametros(argc, argv);

    init(); 
    //iniciar_servidor_kernel();
    //establecer_conexiones();
    //atender_entradasalida();
    
    iniciar_colas_planificacion();
    inicializar_semaforos();

    iniciar_Consola();

    liberar_kernel();
    return 0;
}

/*Se libera toda la memoria asignada durante la ejecucion de kernel.*/
void liberar_kernel(void){ 
    free(config_kernel.IP_KERNEL);
    free(config_kernel.PUERTO_ESCUCHA);
    free(config_kernel.IP_MEMORIA);
    free(config_kernel.PUERTO_MEMORIA);
    free(config_kernel.IP_CPU);
    free(config_kernel.PUERTO_CPU_DISPATCH);
    free(config_kernel.PUERTO_CPU_INTERRUPT);

    free(path_config);

    log_destroy(kernel_log);
    log_destroy(kernel_log_debugg);

    liberar_conexion(fd_conexion_memoria);
    liberar_conexion(fd_conexion_dispatch);
    liberar_conexion(fd_conexion_interrupt);
    liberar_conexion(fd_server_kernel);
    
    
}

