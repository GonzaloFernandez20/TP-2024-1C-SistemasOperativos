#include <protocolos/protocoloDispatch.h>

void procesar_operacion_dispatch(void){
    int cliente_conectado = 1;

    while(cliente_conectado){
        int cod_op = recibir_operacion(fd_dispatch);

		switch (cod_op) {
			case CONTEXTO_EJECUCION:		
				recibir_contexto_ejecucion();
				ciclo();

				break;
			case -1: 
				log_error(cpu_log_debug, "KERNEL-DISPATCH se desconecto");
				cliente_conectado = 0;
				break;
			default:
				log_warning(cpu_log_debug,"Operacion desconocida de KERNEL-DISPATCH");
				break;
			}
	} 
}

// --------- ENVIO Y RECEPCION DE CONTEXTO DE EJECUCION CON KERNEL

void recibir_contexto_ejecucion() // PRIMER ENVIO DE CONTEXTO
{	
	t_buffer *buffer = recibir_buffer(fd_dispatch);
	void* stream = buffer->stream;

	PID            = buffer_read_int(&stream);
	registros.PC   = buffer_read_uint32(&stream);  
    registros.AX   = buffer_read_uint8(&stream);   
    registros.BX   = buffer_read_uint8(&stream);
    registros.CX   = buffer_read_uint8(&stream);
    registros.DX   = buffer_read_uint8(&stream);
    registros.EAX  = buffer_read_uint32(&stream);
    registros.EBX  = buffer_read_uint32(&stream);
    registros.ECX  = buffer_read_uint32(&stream); 
    registros.EDX  = buffer_read_uint32(&stream); 
    registros.SI   = buffer_read_uint32(&stream);
    registros.DI   = buffer_read_uint32(&stream);

	eliminar_buffer(buffer);

    log_info(cpu_log_debug, "Se recibio contexto de ejecucion PID: < %d > desde KERNEL.\n", PID);

}

void devolver_contexto_ejecucion(int motivo){
	t_paquete* paquete = crear_paquete(CONTEXTO_EJECUCION);

    int buffer_size = 2 * sizeof(int) + 4*sizeof(uint8_t) + 7*sizeof(uint32_t); 
	crear_buffer(paquete, buffer_size);

	_cargar_pcb(paquete, motivo); // 7 UINT32, 4 UINT8 Y 2 INT

	enviar_paquete(paquete, fd_dispatch);

    log_info(cpu_log_debug, "Enviado contexto de ejecucion PID: < %d > a KERNEL.", PID);

	eliminar_paquete(paquete);
}

void devolver_contexto_ejecucion_IO_GEN_SLEEP(char* nombre_interfaz, int unidades_trabajo){
    t_paquete* paquete = crear_paquete(CONTEXTO_EJECUCION);

    int buffer_size = 5 * sizeof(int) + 4*sizeof(uint8_t) + 7*(sizeof(uint32_t)) + strlen(nombre_interfaz) + 1; 
	crear_buffer(paquete, buffer_size);

	_cargar_pcb(paquete, LLAMADA_IO); // 7 UINT32, 4 UINT8 Y 2 INT
	buffer_add_int(paquete->buffer, IO_GEN_SLEEP);
	buffer_add_int(paquete->buffer, strlen(nombre_interfaz) + 1);
	buffer_add_string(paquete->buffer, nombre_interfaz);
	buffer_add_int(paquete->buffer, unidades_trabajo);

	enviar_paquete(paquete, fd_dispatch);

    log_info(cpu_log_debug, "Enviando contexto de ejecucion PID: < %d > a KERNEL.", PID);

    free(nombre_interfaz);
	eliminar_paquete(paquete);
}

void devolver_contexto_ejecucion_RECURSO(char *recurso, int codigo_recurso){
    t_paquete* paquete = crear_paquete(CONTEXTO_EJECUCION);

    int buffer_size = 4 * sizeof(int) + 4*sizeof(uint8_t) + 7*(sizeof(uint32_t)) + strlen(recurso) + 1; 
	crear_buffer(paquete, buffer_size);

	_cargar_pcb(paquete, LLAMADA_IO); // 7 UINT32, 4 UINT8 Y 2 INT
	buffer_add_int(paquete->buffer, codigo_recurso);
	buffer_add_int(paquete->buffer, strlen(recurso) + 1);
	buffer_add_string(paquete->buffer, recurso);
	
	enviar_paquete(paquete, fd_dispatch);

    log_info(cpu_log_debug, "Enviando operacion con recurso PID: < %d > a KERNEL.", PID);

    free(recurso);
	eliminar_paquete(paquete);
}

void devolver_contexto_ejecucion_IO_STDIN_READ(char* nombre_interfaz, int tamanio){
    t_paquete* paquete = crear_paquete(CONTEXTO_EJECUCION);
    int cant_direcciones = list_size(direcciones);

    int buffer_size = (6 + 2*cant_direcciones) * sizeof(int) + 4*sizeof(uint8_t) + 7*(sizeof(uint32_t)) + strlen(nombre_interfaz) + 1; 
	crear_buffer(paquete, buffer_size);

	_cargar_pcb(paquete, LLAMADA_IO); // 7 UINT32, 4 UINT8 Y 2 INT
	buffer_add_int(paquete->buffer, IO_STDIN_READ);
    buffer_add_int(paquete->buffer, strlen(nombre_interfaz) + 1);
	buffer_add_string(paquete->buffer, nombre_interfaz);
    buffer_add_int(paquete->buffer, tamanio); 
    buffer_add_int(paquete->buffer, cant_direcciones);

    for (int i = 0; i < cant_direcciones; i++)
    {
        t_datos_acceso *dato = list_remove(direcciones, 0);
        buffer_add_int(paquete->buffer, dato->bytes);
        buffer_add_int(paquete->buffer, dato->direccion_fisica);
        free(dato);
    }
    
    enviar_paquete(paquete, fd_dispatch);

    log_info(cpu_log_debug, "Enviando contexto de ejecucion PID: < %d > a KERNEL.", PID);

    free(nombre_interfaz);
	eliminar_paquete(paquete);
}

void devolver_contexto_ejecucion_IO_STDOUT_WRITE(char* nombre_interfaz, int tamanio){
    t_paquete* paquete = crear_paquete(CONTEXTO_EJECUCION);
    int cant_direcciones = list_size(direcciones);

    int buffer_size = (6 + 2*cant_direcciones) * sizeof(int) + 4*sizeof(uint8_t) + 7*(sizeof(uint32_t)) + strlen(nombre_interfaz) + 1; 
	crear_buffer(paquete, buffer_size);

	_cargar_pcb(paquete, LLAMADA_IO); // 7 UINT32, 4 UINT8 Y 2 INT
	buffer_add_int(paquete->buffer, IO_STDOUT_WRITE);
    buffer_add_int(paquete->buffer, strlen(nombre_interfaz) + 1);
	buffer_add_string(paquete->buffer, nombre_interfaz);
    buffer_add_int(paquete->buffer, tamanio); 
    buffer_add_int(paquete->buffer, cant_direcciones);

    for (int i = 0; i < cant_direcciones; i++)
    {
        t_datos_acceso *dato = list_remove(direcciones, 0);
        buffer_add_int(paquete->buffer, dato->bytes);
        buffer_add_int(paquete->buffer, dato->direccion_fisica);
        free(dato);
    }
    
    enviar_paquete(paquete, fd_dispatch);

    log_info(cpu_log_debug, "Enviando contexto de ejecucion PID: < %d > a KERNEL.", PID);

    free(nombre_interfaz);
	eliminar_paquete(paquete);
}

void devolver_contexto_ejecucion_IO_FS(char* nombre_interfaz, char* nombre_archivo, int operacion){
    t_paquete* paquete = crear_paquete(CONTEXTO_EJECUCION);

    int buffer_size = 5 * sizeof(int) + 4*sizeof(uint8_t) + 7*(sizeof(uint32_t)) + strlen(nombre_interfaz) + strlen(nombre_archivo) + 2; 
	crear_buffer(paquete, buffer_size);

	_cargar_pcb(paquete, LLAMADA_IO); // 7 UINT32, 4 UINT8 Y 2 INT
	buffer_add_int(paquete->buffer, operacion);
    buffer_add_int(paquete->buffer, strlen(nombre_interfaz) + 1);
	buffer_add_string(paquete->buffer, nombre_interfaz);
    buffer_add_int(paquete->buffer, strlen(nombre_archivo) + 1);
	buffer_add_string(paquete->buffer, nombre_archivo);
    
    enviar_paquete(paquete, fd_dispatch);

    log_info(cpu_log_debug, "Enviando contexto de ejecucion PID: < %d > a KERNEL.", PID);

    free(nombre_interfaz);
	eliminar_paquete(paquete);
}

void devolver_contexto_ejecucion_IO_FS_TRUNCATE(char* nombre_interfaz, char* nombre_archivo, int registro_tamanio){
    t_paquete* paquete = crear_paquete(CONTEXTO_EJECUCION);

    int buffer_size = 6 * sizeof(int) + 4*sizeof(uint8_t) + 7*(sizeof(uint32_t)) + strlen(nombre_interfaz) + strlen(nombre_archivo) + 2; 
	crear_buffer(paquete, buffer_size);

	_cargar_pcb(paquete, LLAMADA_IO); // 7 UINT32, 4 UINT8 Y 2 INT
	buffer_add_int(paquete->buffer, IO_FS_TRUNCATE);
    buffer_add_int(paquete->buffer, strlen(nombre_interfaz) + 1);
	buffer_add_string(paquete->buffer, nombre_interfaz);
    buffer_add_int(paquete->buffer, strlen(nombre_archivo) + 1);
	buffer_add_string(paquete->buffer, nombre_archivo);
    buffer_add_int(paquete->buffer, registro_tamanio);
    
    enviar_paquete(paquete, fd_dispatch);

    log_info(cpu_log_debug, "Enviando contexto de ejecucion PID: < %d > a KERNEL.", PID);

    free(nombre_interfaz);
	eliminar_paquete(paquete);
}
// (Interfaz, Nombre Archivo, Registro Dirección, Registro Tamaño, Registro Puntero Archivo)
void devolver_contexto_ejecucion_IO_FS_WRITE_READ(char* nombre_interfaz, char* nombre_archivo, int registro_direccion, int registro_tamanio, int reg_puntero, int operacion){
    t_paquete* paquete = crear_paquete(CONTEXTO_EJECUCION);

    int buffer_size = 8 * sizeof(int) + 4*sizeof(uint8_t) + 7*(sizeof(uint32_t)) + strlen(nombre_interfaz) + strlen(nombre_archivo) + 2; 
	crear_buffer(paquete, buffer_size);

	_cargar_pcb(paquete, LLAMADA_IO); // 7 UINT32, 4 UINT8 Y 2 INT
	buffer_add_int(paquete->buffer, operacion);
    buffer_add_int(paquete->buffer, strlen(nombre_interfaz) + 1);
	buffer_add_string(paquete->buffer, nombre_interfaz);
    buffer_add_int(paquete->buffer, strlen(nombre_archivo) + 1);
	buffer_add_string(paquete->buffer, nombre_archivo);
    buffer_add_int(paquete->buffer, registro_direccion);
    buffer_add_int(paquete->buffer, registro_tamanio);
    buffer_add_int(paquete->buffer, reg_puntero);
    
    enviar_paquete(paquete, fd_dispatch);

    log_info(cpu_log_debug, "Enviando contexto de ejecucion PID: < %d > a KERNEL.", PID);

    free(nombre_interfaz);
	eliminar_paquete(paquete);
}


// -------------------------------------------------------------

void _cargar_pcb(t_paquete* paquete, int motivo){ // Como solo se llama dentro de este archivo, la defino estatica para limitar su visibilidad solo a este archivo.
    buffer_add_int(paquete->buffer, PID );
    buffer_add_uint32(paquete->buffer, registros.PC );
    buffer_add_uint8(paquete->buffer, registros.AX );
    buffer_add_uint8(paquete->buffer, registros.BX );
    buffer_add_uint8(paquete->buffer, registros.CX );
    buffer_add_uint8(paquete->buffer, registros.DX );
    buffer_add_uint32(paquete->buffer, registros.EAX );
    buffer_add_uint32(paquete->buffer, registros.EBX );
    buffer_add_uint32(paquete->buffer, registros.ECX );
    buffer_add_uint32(paquete->buffer, registros.EDX );
    buffer_add_uint32(paquete->buffer, registros.SI );
    buffer_add_uint32(paquete->buffer, registros.DI );
    buffer_add_int(paquete->buffer, motivo);
}

/* 
int cantidad_direcciones = list_size(direcciones);
    int offset = 0;
    
    for(int i = 0; i < cantidad_direcciones; i++){
        t_datos_acceso* datos =list_remove(direcciones, 0);
        int bytes = datos->bytes;
        int DF = datos->direccion_fisica;
        free(datos);
    
        void* particion = leer_de_memoria(DF,bytes);
        memcpy(ptr_registro_datos + offset, particion, bytes);
        offset += bytes;

        pthread_mutex_lock(&mutex_log);
            log_info(cpu_log,"PID: < %d > - Acción: < LEER > - Dirección Física: < %d > - Valor: < %d >", PID, DF, *(int*)particion);
        pthread_mutex_unlock(&mutex_log);

    } */