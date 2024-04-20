#include <include/main.h>

int main(int argc, char* argv[]) {
    
    path_config = chequear_parametros(argc, argv);

    init();
    establecer_conexiones();
    liberar_entradaSalida();

    return 0;
}





/*Se libera toda la memoria asignada durante la ejecucion de Entrada/Salida.*/

void liberar_entradaSalida(void){
    free(config_IO.IP_KERNEL);
    free(config_IO.IP_MEMORIA);
    free(config_IO.PUERTO_KERNEL);
    free(config_IO.PUERTO_MEMORIA);

    free(path_config);
    log_destroy(IO_log);
    log_destroy(IO_log_debug);
    liberar_conexion(fd_conexion_memoria);
    liberar_conexion(fd_conexion_kernel);
}








