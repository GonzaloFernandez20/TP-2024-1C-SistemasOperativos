#include <protocolos/protocoloMemoria.h>


/* void enviar_path(char* path, int pid_asociado){
	
	t_path_proceso* path_proceso = malloc(sizeof(t_path_proceso));

	path_proceso->codigo_operacion = CREAR_PROCESO;
    path_proceso->pid_proceso = pid_asociado;
	path_proceso->buffer = malloc(sizeof(t_buffer));
	path_proceso->buffer->size = strlen(path) + 1;
	path_proceso->buffer->stream = malloc(path_proceso->buffer->size);
	memcpy(path_proceso->buffer->stream, path, path_proceso->buffer->size);

	int bytes = path_proceso->buffer->size + 3*sizeof(int);

	void* a_enviar = serializar_paquete(path_proceso, bytes);

	send(fd_conexion_memoria, a_enviar, bytes, 0);

	free(a_enviar);
	eliminar_paquete(path_proceso);
}

void recibir_confirmacion(int pid){

    int rta_memoria = recv(fd_conexion_memoria, &rta_memoria, sizeof(int), MSG_WAITALL);

        if(rta_memoria < 0){
            // Logeo de error: no se pudo conectar con memoria...
        }

    if (rta_memoria == 0)
    {
        buscar_y_eliminar_pid(new, pid);
    }
} */