#include <conexiones.h>

void atender_dispatch(void){
    atender_cliente(fd_dispatch_server, procesar_operacion_dispatch, cpu_log_debug);
}

void atender_interrupt(void){
    atender_cliente(fd_interrupt_server, procesar_operacion_interrupt, cpu_log_debug);
}

void gestionar_conexiones_kernel(void){
    atender_dispatch();
    atender_interrupt();
}