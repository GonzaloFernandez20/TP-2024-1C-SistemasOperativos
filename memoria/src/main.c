#include <include/main.h>

int main(int argc, char* argv[]) {
    
    path_config = chequear_parametros(argc, argv);

    init(); 
    iniciar_servidor_memoria();
    gestionar_conexiones_clientes();
    liberar_memoria();

    return 0;
}




/*Se libera toda la memoria asignada durante la ejecucion de memoria.*/
void liberar_memoria(void){ 
    free(config_memoria.IP_MEMORIA);
    free(config_memoria.PUERTO_ESCUCHA);
    free(config_memoria.PATH_INSTRUCCIONES);

    free(path_config);

    log_destroy(memoria_log);
    log_destroy(memoria_log_debugg);
    liberar_conexion(fd_server_memoria);

}
