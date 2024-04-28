#include <include/main.h>

int main(int argc, char* argv[]) {
    
    path_config = chequear_parametros(argc, argv);

    init();
    establecer_conexiones();
    manos_a_la_obra_interfaces();
    liberar_entradaSalida();

    return 0;
}

void manos_a_la_obra_interfaces(void){
     while(1){//no me gustan estos pero por ahora queda para probar
    	interfaz_code que_hago = recibir_operacion(fd_conexion_kernel);
        t_list* datos_recibidos;//no lo liberé
		switch (que_hago) {
		case IO_GEN_SLEEP://NECESARIA PARA LA ENTREGA 2
            datos_recibidos = recibir_paquete(fd_conexion_kernel);
            log_info(IO_log, "El kernel dice:\n");
			list_iterate(datos_recibidos, (void*) iterator);
            enviar_code_op_y_datos(IO_GEN_SLEEP,"DORMIMOS 5 segundos",fd_conexion_kernel);
			break;
		case IO_STDIN_READ:

			break;
    	case IO_STDOUT_WRITE:

			break;

		default:
			log_warning(IO_log_debug,"Operacion desconocida de ENTRADA/SALIDA");
			break;
		}
	}
}

void iterator(char* value) {
	log_info(IO_log,"%s", value);
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








