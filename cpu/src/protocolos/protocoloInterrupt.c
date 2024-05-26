#include <protocolos/protocoloInterrupt.h>

void *procesar_operacion_interrupt(void *fd_interrupt_casteado){

    fd_interrupt = _deshacer_casting(fd_interrupt_casteado);
	free(fd_interrupt_casteado);
	while (1)
	{
		tipo_interrupcion = recibir_operacion(fd_interrupt);
    	hay_interrupcion = 1; 
	}
	return (void *)EXIT_FAILURE;
}
