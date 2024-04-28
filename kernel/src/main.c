#include <include/main.h>

int main(int argc, char *argv[]){

    path_config = chequear_parametros(argc, argv);

    init(); 
    iniciar_servidor_kernel_interface();
    establecer_conexiones();
    manos_a_la_obra_kernel();
    //atender_entradasalida();//no estoy seguro que esto vaya acá porque no se ejecutaría hasta el fin de la linea anterior
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
    liberar_conexion(fd_server_kernel_interface);
    
    
}

//esto no va en el main pero por ahora lo dejo acá para no saltar de un archivo al otro
que_hace_kernel que_hago;
void manos_a_la_obra_kernel(void){
    while(1){//el que_hago se debería llenar por consola, eso entiendo del enunciado
        que_hago = IO_GEN_SLEEP; //queda clavado acá para las primeras pruebas
                                //pero la idea sería que al ser global los distintos módulos del
                               //kernel lo puedan modificar de manera conveniente, se presta  para semáforos creo
        switch (que_hago)
        {
        case IO_GEN_SLEEP:
            IO_GEN_SLEEP_func();
            break;
        case INICIAR_PROCESO:
            break;
        case INICIAR_PLANIFICACION:
        default:
            break;
        }
    }
}

void IO_GEN_SLEEP_func(void){
    char* tiempo = readline("Tiempo de SLEEP: ");//escribí algo porque crashea
    enviar_code_op_y_datos(IO_GEN_SLEEP,tiempo,fd_server_kernel_interface);
    atender_entradasalida();//después va a cambiar, pero por ahora quiero una respuesta de la I/O para probar
    return void;
}



