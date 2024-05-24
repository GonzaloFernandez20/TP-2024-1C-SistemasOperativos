#include <protocolos/protocoloInterrupt.h>

/**
 * @brief Maneja las operaciones entrantes desde un descriptor de archivo de servicio de interrupción en un hilo separado.
 *
 *
 * La función `procesar_operacion_interrupt` procesa las operaciones recibidas desde el descriptor de archivo
 * asociado con el servicio de interrupción. Funciona en un bucle, manejando continuamente las operaciones hasta
 * que la conexión se termina. Esta función está diseñada para ser utilizada como una función de hilo.
 *
 * @param fd_interrupt_casteado Un puntero void que se espera sea un descriptor de archivo casteado.
 * Este descriptor de archivo se utiliza para comunicarse con el servicio de interrupción.
 *
 * @return void* Esta función retorna (void *)EXIT_FAILURE para indicar que el hilo ha completado
 * su ejecución debido a la desconexión o un error.
 */
void *procesar_operacion_interrupt(void *fd_interrupt_casteado){

    int fd_interrupt = _deshacer_casting(fd_interrupt_casteado);

    int cliente_conectado = 1;

    while(cliente_conectado){
        int cod_op = recibir_operacion(fd_interrupt);

		switch (cod_op) {
		case MENSAJE:
			/** 
			 * Se recibe por la conexión Interrupt de CPU un mensaje 
			 * desde Kernel especificando el PID del proceso que solicita interrumpir
			 * 
			 * Evaluar durante el ciclo checkInterrupt():
			 * Si la PID del proceso en ejecución coincide con el PID solicitado a interrumpir, 
			 * entonces se realiza lo solicitado. Caso contrario, lo desestima.
			 * 
			 */
		
			PID_a_interrumpir = atoi(recibir_mensaje(fd_interrupt));
			log_info(cpu_log_debug, "Kernel pide interrumpir proceso PID=%d", PID_a_interrumpir);

			break;
		
		case -1:
			log_error(cpu_log_debug, "KERNEL-INTERRUPT se desconecto");
            cliente_conectado = 0;
			interrupt_conectado = 0;
			break;

		default:
			log_warning(cpu_log_debug,"Operacion desconocida de KERNEL-INTERRUPT");
			break;
		}
	}
	return (void *)EXIT_FAILURE;
}