#include <protocolos/protocoloIO.h>

void *procesar_operacion_entradaSalida(void *fd_IO_casteado)
{

	fd_IO = _deshacer_casting(fd_IO_casteado);
	free(fd_IO_casteado);

	int cliente_conectado = 1;

	while (cliente_conectado)
	{
		int cod_op = recibir_operacion(fd_IO);

		switch (cod_op) {
		case LECTURA:
			realizar_lectura(fd_IO);
			break;
			
		case ESCRITURA:
			realizar_escritura(fd_cpu);
			break;
		case -1:
			pthread_mutex_lock(&mutex_log_debug);
			log_error(memoria_log_debugg, "un modulo de E/S se desconecto\n");
			pthread_mutex_unlock(&mutex_log_debug);

			cliente_conectado = 0;
			break;

		default:
			pthread_mutex_lock(&mutex_log_debug);
			log_warning(memoria_log_debugg, "Operacion desconocida de E/S");
			pthread_mutex_unlock(&mutex_log_debug);
			break;
		}
	}
	return (void *)EXIT_FAILURE;
}

void atender_peticion_de_lectura()
{
	t_buffer *buffer = recibir_buffer(fd_IO);
	void *stream = buffer->stream;
	uint32_t direccionFisica = buffer_read_uint32(&stream);

	char *datoEnDirFisica = leer_direccion_fisica(direccionFisica);

	enviar_a_interfaz(datoEnDirFisica, SUCCESFULL_READ);
}
void atender_peticion_de_escritura()
{
	// Recepción de lo que se manda a la dirección física

	t_buffer *buffer = recibir_buffer(fd_IO); // no hay que enviar cod op desde memoria
	void *stream = buffer->stream;
	uint32_t direccionFisica = buffer_read_uint32(&stream);
	int stringLength = buffer_read_int(&stream); // revisar si se envia el \0 desde memoria
	char *datosParaDirFisica = buffer_read_string(&stream, stringLength);

	eliminar_buffer(buffer);

	// IMPLEMENTAR CON MELI guardar_en_direccion_fisica(uint32_t direccionFisica,char* datosParaDirFisica)

	if (guardar_en_direccion_fisica(direccionFisica, datosParaDirFisica))
	{
		// Responderle SUCCESFULL_WRITE a la interfaz si salio todo bien
		enviar_a_interfaz("Info guardada exitosamente en ", SUCCESFULL_WRITE);
	}
	else
	{
		enviar_a_interfaz("Error al guerdar en ", FAILED_WRITE);
	}
}
void enviar_a_interfaz(char *info, int resultado_de_operacion)
{
	t_paquete *paquete = crear_paquete(resultado_de_operacion);

	int bufferSize = sizeof(int) + strlen(info) + 1; // el +1 está por el \0???

	crear_buffer(paquete, bufferSize);

	buffer_add_int(paquete->buffer, strlen(info) + 1);
	buffer_add_string(paquete->buffer, info);

	serializar_paquete(paquete, bufferSize + sizeof(int));//buffer + cod op

	enviar_paquete(paquete, fd_IO); // hay que conocer la ip de la interfaz que se comunicó o con el socket es suficiente???

	eliminar_paquete(paquete);
}
int guardar_en_direccion_fisica(uint32_t direccionFisica,char* datosParaDirFisica){return 0;}//implementar con MELI
char* leer_direccion_fisica(uint32_t direccionFisica){return "ESTO ES UNA PRUEBA";}
