#include <conexiones/conexiones.h>

void iniciar_servidores(void){
    iniciar_servidor_dispatch();
    iniciar_servidor_interrupt();
}

/* 
En teoría no es necesario usar strdup() dentro de estas funciones
xq IP y PUERTO se usan nomás para pasarse como parámetros, 
por ende podemos asignarles el mismo puntero que config_cpu.IP_CPU y config_cpu.PUERTO_ESCUCHA_DISPATCH
y entonces no sería necesario usar free(), a no ser que sepamos que ya no queremos usar más los valores de config.

Dejo el código anterior por si se rompe.
Como estoy leyendo todo el codebase para entender bien las bases, 
hago de paso estos cambios para mantener la legibilidad de nuestro código.

void iniciar_servidor_dispatch(void){
    char* IP = strdup(config_cpu.IP_CPU);
    char* PUERTO = strdup(config_cpu.PUERTO_ESCUCHA_DISPATCH);

    iniciar_servidor(IP, PUERTO, "CPU-DISPATCH");

    free(IP);
    free(PUERTO);
}

void iniciar_servidor_interrupt(void){
    char* IP = strdup(config_cpu.IP_CPU);
    char* PUERTO = strdup(config_cpu.PUERTO_ESCUCHA_INTERRUPT);

    iniciar_servidor(IP, PUERTO, "CPU-INTERRUPT");

    free(IP);
    free(PUERTO);
}
*/

void iniciar_servidor_dispatch(void){
    char* IP = config_cpu.IP_CPU;
    char* PUERTO = config_cpu.PUERTO_ESCUCHA_DISPATCH;

    _iniciar_servidor(IP, PUERTO, "CPU-DISPATCH");
}

void iniciar_servidor_interrupt(void){
    char* IP = config_cpu.IP_CPU;
    char* PUERTO = config_cpu.PUERTO_ESCUCHA_INTERRUPT;

    _iniciar_servidor(IP, PUERTO, "CPU-INTERRUPT");

}

void _iniciar_servidor(char* IP, char* PUERTO, char* nombre_server)  {
    int fd_server = iniciar_servidor(IP , PUERTO);

    if(fd_server == -1){
        log_error(cpu_log_debug,"No se pudo iniciar servidor %s", nombre_server);
    }
    else{
        log_info(cpu_log_debug, "Servidor iniciado! %s escuchando en %s:%s\n", nombre_server, IP, PUERTO);
    }
}


void conectar_memoria(void){ 
    char* IP = config_cpu.IP_MEMORIA;
    char* PUERTO = config_cpu.PUERTO_MEMORIA;

    fd_conexion_memoria = crear_conexion(IP, PUERTO);

    if(fd_conexion_memoria == -1){
        log_error(cpu_log_debug,"CPU no se conecto a MEMORIA");
    }
    else{
        log_info(cpu_log_debug, "CPU conectado a MEMORIA en %s:%s", IP, PUERTO);
        enviar_handshake(fd_conexion_memoria, "CPU", cpu_log_debug);
    }
}

void gestionar_conexiones_kernel(void){
    atender_dispatch();
    atender_interrupt();

    while(dispatch_conectado || interrupt_conectado){
        // CONTINGENCIA: se borra en cuanto veamos señales y semaforos
        // es temporal para que el hilo main no finalice el programa
    }
}

void atender_dispatch(void){
    atender_cliente(fd_dispatch_server, (void *)procesar_operacion_dispatch, cpu_log_debug, "KERNEL-DISPATCH");
    dispatch_conectado = 1;
}

void atender_interrupt(void){
    atender_cliente(fd_interrupt_server, (void *)procesar_operacion_interrupt, cpu_log_debug,"KERNEL-INTERRUPT");
    interrupt_conectado = 1;
}
