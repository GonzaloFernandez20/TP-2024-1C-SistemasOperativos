#include <protocolos/protocoloMemoria.h>

void enviar_path_seudocodigo(char* archivo, int PID){
    t_paquete* paquete = crear_paquete(CREAR_PROCESO);

    int buffer_size = strlen(archivo) + 1 + 2 * sizeof(int); 
	crear_buffer(paquete, buffer_size);

    buffer_add_int(paquete->buffer, PID );
    buffer_add_int(paquete->buffer, strlen(archivo) + 1);
	buffer_add_string(paquete->buffer, archivo);

    enviar_paquete(paquete, fd_conexion_memoria);
    log_info(kernel_log_debugg, "Solicitud de creacion de proceso enviada a memoria");

	eliminar_paquete(paquete);
}

void finalizar_proceso(int PID){
    t_paquete* paquete = crear_paquete(ELIMINAR_PROCESO);

    int buffer_size = sizeof(int); 
    crear_buffer(paquete, buffer_size);


    buffer_add_int(paquete->buffer, PID);

    enviar_paquete(paquete, fd_conexion_memoria);
    eliminar_paquete(paquete);
}


int recibir_confirmacion(int pid){
    int rta_memoria; 
    recv(fd_conexion_memoria, &rta_memoria, sizeof(int), MSG_WAITALL);
    if(rta_memoria == 0){
        buscar_y_trasladar_pid(new, pid);
        return 0;
    }else
    {
        return 1;
    }
} 