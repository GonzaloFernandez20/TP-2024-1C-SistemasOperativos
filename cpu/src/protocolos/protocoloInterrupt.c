#include <protocolos/protocoloInterrupt.h>

void *	procesar_operacion_interrupt(void *fd_interrupt_casteado){

	int tipo_interrupcion;
    fd_interrupt = _deshacer_casting(fd_interrupt_casteado);
	free(fd_interrupt_casteado);
	
	while (1)
	{
		tipo_interrupcion = recibir_operacion(fd_interrupt);
		int *ptr_interrupcion = malloc(sizeof(int));
		*ptr_interrupcion = tipo_interrupcion;
		pthread_mutex_lock(&mutex_lista_interrupciones);
			list_add(lista_interrupciones, ptr_interrupcion);
		pthread_mutex_unlock(&mutex_lista_interrupciones);

		pthread_mutex_lock(&mutex_hay_interrupcion);
    		hay_interrupcion = 1; 
		pthread_mutex_unlock(&mutex_hay_interrupcion);
	}
	return (void *)EXIT_FAILURE;
}
