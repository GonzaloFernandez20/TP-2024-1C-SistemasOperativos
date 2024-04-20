#include <include/main.h>

int main(int argc, char *argv[]){

    path_config = chequear_parametros(argc, argv);

    init(); // Seteamos las configuraciones iniciales de logs y configs
    iniciar_servidor_kernel();
    //atender_entradasalida();
    gestionar_conexiones();

    liberar_memoria();
    return 0;
}



























// ----------------------------- Funciones auxiliares


void liberar_memoria(void){ //? Aca deberia liberarse toda la memoria asignada durante la ejecucion.
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
}

