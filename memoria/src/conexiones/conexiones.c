#include <conexiones/conexiones.h>

void iniciar_servidor_memoria(void){
    char* IP = strdup(config_memoria.IP_MEMORIA);
    char* PUERTO = strdup(config_memoria.PUERTO_ESCUCHA);

    fd_server_memoria = iniciar_servidor(IP , PUERTO);

    if(fd_server_memoria == -1){
        log_error(memoria_log_debugg,"No se pudo iniciar servidor MEMORIA");
    }
    else{
        log_info(memoria_log_debugg, "Servidor iniciado! MEMORIA escuchando en %s:%s\n", IP, PUERTO);
    }

    free(IP);
    free(PUERTO);
}

void gestionar_conexiones_clientes(void){
   atender_cpu();
   atender_kernel();
   atender_entradaSalida();

   while(cpu_conectada || kernel_conectado || IO_conectado){
    // CONTINGENCIA: se borra en cuanto veamos señales y semaforos
    // es temporal para que el hilo main no finalice el programa
   }
}

void atender_cpu(void){
    atender_cliente(fd_server_memoria, (void *)procesar_operacion_cpu, memoria_log_debugg, "CPU");
    cpu_conectada = 1;
}
void atender_kernel(void){
    atender_cliente(fd_server_memoria, (void *)procesar_operacion_kernel, memoria_log_debugg, "KERNEL");
    kernel_conectado = 1;
}
void atender_entradaSalida(void){
    atender_cliente(fd_server_memoria, (void *)procesar_operacion_entradaSalida, memoria_log_debugg, "E/S");
    IO_conectado = 1;
}
