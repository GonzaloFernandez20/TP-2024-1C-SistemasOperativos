#include <protocolos/protocoloDispatch.h>

#define TAMANIO_DEL_PCB 4*sizeof(uint8_t)+7*sizeof(uint32_t)+2*sizeof(int)
#define TAMANIO_CAMPO_SIZE sizeof(uint32_t)
#define TAMANIO_COD_OP sizeof(int)
#define TAMANIO_MOTIVO_DESALOJO sizeof(int)

#define UN_PCB 0//va en unn enum pero por ahora lo dejo acá

//NO TIENEN QUE ESTAR DEFINIDAS ACA, SACAR DPS
int cliente_conectado = 1;
int causa_de_desalojo;//interrupt() lo tiene que recibir de kernel



void *procesar_operacion_dispatch(void *fd_dispatch_casteado){

    int fd_dispatch = _deshacer_casting(fd_dispatch_casteado);//no se hace nada con esto
//estas dos lineas solamente se ejecutan por única cuando se levanta la conexion de dispatch y sirven para establecer las condiciones iniciales del mutex
 	pthread_mutex_init(&mutex_dispatch, NULL);
 	ocupar_mutex(&mutex_dispatch);//si no se bloquea al principio sale una segmentation fault

//este primer ciclo se diferencia del while(cliente_conectado) en que NO SE ENVIA UN PCB porque el 
//PCB que se recibe por primera vez no está desalojando a ningún  otro
// 	ocupar_mutex(&mutex_dispatch);//la primer interrupcion lo va a habilitar
	t_pcb* PCB_inicial = recibirPCB();//aca ta

	//Cargamos el primer PCB
	actualizarPCB(PCB_inicial);
   	actualizarRegistrosDeCPU(PCB_inicial);

	desocupar_mutex(&mutex_ciclo);

	// Liberamos memoria
	free(PCB_inicial);

    while(cliente_conectado){
		ocupar_mutex(&mutex_dispatch); //el signal/post de esto lo hace la rutina de atender interrupciones
		
		t_pcb* PCB_recibido = recibirPCB();
		// Se devuelve el Contexto de Ejecución (guardado en el PCB) actualizado al Kernel con motivo de la interrupción.
		enviar_PCB();

		//Actualizamos el PCB
		actualizarPCB(PCB_recibido);
    	actualizarRegistrosDeCPU(PCB_recibido);

		desocupar_mutex(&mutex_ciclo);

		// Liberamos memoria
		free(PCB_recibido);
	}
	return (void *)EXIT_FAILURE;
}
t_pcb* recibirPCB(){
	t_pcb* PCB_recibido;
	t_paquete_PCB* paquete = recibir_paquete_PCB(fd_dispatch_server);
	switch (paquete->codigo_operacion) {
		case UN_PCB:		
			// Se recibe el PCB desde Kernel, lo cual interpretamos como una orden para ejecutar el proceso identificado por el PCB.
			PCB_recibido = deserializar_PCB(paquete->buffer_PCB);
			liberar_paquete(paquete);
			return PCB_recibido;
			break;
		case -1: 
			// cierra la conexión 
			log_error(cpu_log_debug, "KERNEL-DISPATCH se desconecto");
		          cliente_conectado = 0;
			dispatch_conectado = 0;
			return NULL;
			break;
		default:
			log_warning(cpu_log_debug,"Operacion desconocida de KERNEL-DISPATCH");
			return NULL;
			break;
		}

}
void enviar_PCB(){
	t_paquete_PCB* paquete = crear_paquete_PCB();
    
	paquete->codigo_operacion = UN_PCB;
	paquete->motivo_de_desalojo = causa_de_desalojo;
	// Armamos el stream a enviar
	void* a_enviar = malloc(TAMANIO_DEL_PCB + TAMANIO_CAMPO_SIZE + TAMANIO_COD_OP + TAMANIO_MOTIVO_DESALOJO);
	int offset = 0;
	//Serializacion
	serializar_PCB(paquete->buffer_PCB, &PCB);
	//Envio
	send(fd_dispatch_server, a_enviar, TAMANIO_DEL_PCB + TAMANIO_CAMPO_SIZE + TAMANIO_COD_OP + TAMANIO_MOTIVO_DESALOJO, 0);
	//Liberamos memoria
	liberar_paquete(paquete);
	free(a_enviar);
}
t_pcb* deserializar_PCB(t_buffer_PCB* buffer_PCB_llegada)
{
	t_pcb* PCB_recibido = malloc(TAMANIO_DEL_PCB);
	int size = read_buffer_PCB_uint32_t(buffer_PCB_llegada);//se ignora en los tests, sólo necesario en la implementación
    if(size == TAMANIO_DEL_PCB){
	//EL CODIGO DE OPERACION TUVO QUE HABER SIDO LEIDO DEL STREAM RECIBIDO DE ANTEMANO
	PCB_recibido->PID            = read_buffer_PCB_int(buffer_PCB_llegada);
	PCB_recibido->Quantum        = read_buffer_PCB_int(buffer_PCB_llegada);
	PCB_recibido->PC             = read_buffer_PCB_uint32_t(buffer_PCB_llegada);  
    PCB_recibido->registros.AX   = read_buffer_PCB_uint8_t (buffer_PCB_llegada);   
    PCB_recibido->registros.BX   = read_buffer_PCB_uint8_t (buffer_PCB_llegada);
    PCB_recibido->registros.CX   = read_buffer_PCB_uint8_t (buffer_PCB_llegada);
    PCB_recibido->registros.DX   = read_buffer_PCB_uint8_t (buffer_PCB_llegada);
    PCB_recibido->registros.EAX  = read_buffer_PCB_uint32_t(buffer_PCB_llegada);
    PCB_recibido->registros.EBX  = read_buffer_PCB_uint32_t(buffer_PCB_llegada);
    PCB_recibido->registros.ECX  = read_buffer_PCB_uint32_t(buffer_PCB_llegada); 
    PCB_recibido->registros.EDX  = read_buffer_PCB_uint32_t(buffer_PCB_llegada); 
    PCB_recibido->registros.SI   = read_buffer_PCB_uint32_t(buffer_PCB_llegada);
    PCB_recibido->registros.DI   = read_buffer_PCB_uint32_t(buffer_PCB_llegada);
	return PCB_recibido;
    }
    else{
        //Devolver un error de que faltan datos y dejar el PCB como está
        return &PCB;
    }
}
t_paquete_PCB*  recibir_paquete_PCB(int fd){
	t_paquete_PCB* paquete = crear_paquete_PCB();
	//Inicializo el ofset del buffer
    paquete->buffer_PCB->offset = 0;
    // Primero recibimos el codigo de operacion
	paquete->codigo_operacion = recibir_operacion(fd);
	// Recibimos el motivo de desalojo, en CPU no se usa pero en kernel sí
	paquete->motivo_de_desalojo = recibir_motivo_de_dealojo(fd);
	// Después ya podemos recibir el buffer. Primero su tamaño seguido del contenido
	recibir_buffer_PCB(fd, paquete);
	return paquete;
}
int recibir_motivo_de_dealojo(int socket_cliente){
	int motivoRecibido;
	if(recv(socket_cliente, &motivoRecibido, sizeof(int), MSG_WAITALL) > 0)
		return motivoRecibido;
	else
	{
		close(socket_cliente);
		//ERROR
		return 0;
	}
}
void actualizarPCB(t_pcb* PCB_recibido){

    PCB.PID           = PCB_recibido->PID;
    PCB.Quantum       = PCB_recibido->Quantum;
    PCB.PC            = PCB_recibido->PC;
    PCB.registros.AX  = PCB_recibido->registros.AX;
    PCB.registros.BX  = PCB_recibido->registros.BX;
    PCB.registros.CX  = PCB_recibido->registros.CX;
    PCB.registros.DX  = PCB_recibido->registros.DX;
    PCB.registros.EAX = PCB_recibido->registros.EAX;
    PCB.registros.EBX = PCB_recibido->registros.EBX;
    PCB.registros.ECX = PCB_recibido->registros.ECX;
    PCB.registros.EDX = PCB_recibido->registros.EDX; 
    PCB.registros.SI  = PCB_recibido->registros.SI;
    PCB.registros.DI  = PCB_recibido->registros.DI;
}
void actualizarRegistrosDeCPU(t_pcb* un_pcb){
    registrosCPU.PC  = un_pcb->PC;
    registrosCPU.AX  = un_pcb->registros.AX;
    registrosCPU.BX  = un_pcb->registros.BX;
    registrosCPU.CX  = un_pcb->registros.CX;
    registrosCPU.DX  = un_pcb->registros.DX;
    registrosCPU.EAX = un_pcb->registros.EAX;
    registrosCPU.EBX = un_pcb->registros.EBX;
    registrosCPU.ECX = un_pcb->registros.ECX;
    registrosCPU.EDX = un_pcb->registros.EDX; 
    registrosCPU.SI  = un_pcb->registros.SI;
    registrosCPU.DI  = un_pcb->registros.DI;
}
uint32_t read_buffer_PCB_uint32_t(t_buffer_PCB* buffer_PCB_llegada){
	uint32_t datoEnbuffer_PCB;
    memcpy(&datoEnbuffer_PCB, buffer_PCB_llegada->stream+buffer_PCB_llegada->offset, sizeof(uint32_t));
    buffer_PCB_llegada->offset += sizeof(uint32_t);
	return datoEnbuffer_PCB;
}
uint8_t read_buffer_PCB_uint8_t(t_buffer_PCB* buffer_PCB_llegada){
	uint8_t datoEnbuffer_PCB;
    memcpy(&datoEnbuffer_PCB, buffer_PCB_llegada->stream+buffer_PCB_llegada->offset, sizeof(uint8_t));
    buffer_PCB_llegada->offset += sizeof(uint8_t);
	return datoEnbuffer_PCB;
}
int read_buffer_PCB_int(t_buffer_PCB* buffer_PCB_llegada){
	int datoEnbuffer_PCB;
    memcpy(&datoEnbuffer_PCB, buffer_PCB_llegada->stream+buffer_PCB_llegada->offset, sizeof(int));
    buffer_PCB_llegada->offset += sizeof(int);
	return datoEnbuffer_PCB;
}
void  serializar_PCB(t_buffer_PCB* buffer_PCB,t_pcb* PCB)
{
	 write_buffer_PCB_int(buffer_PCB, &(PCB->PID));
	 write_buffer_PCB_int(buffer_PCB, &(PCB->Quantum));
	 write_buffer_PCB_uint32_t(buffer_PCB, &(PCB->PC ));  
     write_buffer_PCB_uint8_t (buffer_PCB, &(PCB->registros.AX ));   
     write_buffer_PCB_uint8_t (buffer_PCB, &(PCB->registros.BX ));
     write_buffer_PCB_uint8_t (buffer_PCB, &(PCB->registros.CX ));
     write_buffer_PCB_uint8_t (buffer_PCB, &(PCB->registros.DX ));
     write_buffer_PCB_uint32_t(buffer_PCB, &(PCB->registros.EAX));
     write_buffer_PCB_uint32_t(buffer_PCB, &(PCB->registros.EBX));
     write_buffer_PCB_uint32_t(buffer_PCB, &(PCB->registros.ECX)); 
     write_buffer_PCB_uint32_t(buffer_PCB, &(PCB->registros.EDX)); 
     write_buffer_PCB_uint32_t(buffer_PCB, &(PCB->registros.SI ));
     write_buffer_PCB_uint32_t(buffer_PCB, &(PCB->registros.DI ));
}
void crear_buffer_PCB(t_paquete_PCB* paquete_PCB)
{   
	paquete_PCB->buffer_PCB = malloc(sizeof(t_buffer_PCB));
	paquete_PCB->buffer_PCB->size = TAMANIO_DEL_PCB;
    paquete_PCB->buffer_PCB->offset = 0;
	paquete_PCB->buffer_PCB->stream = malloc(TAMANIO_DEL_PCB);
}
t_paquete_PCB* crear_paquete_PCB(void)
{
	t_paquete_PCB* paquete_PCB = malloc(sizeof(t_paquete_PCB));
	crear_buffer_PCB(paquete_PCB);
	return paquete_PCB;
}
void write_buffer_PCB_uint32_t(t_buffer_PCB* buffer_PCB,uint32_t* dato){
    memcpy(buffer_PCB->stream + buffer_PCB->offset, dato, sizeof(uint32_t));
    buffer_PCB->offset += sizeof(uint32_t);
}
void write_buffer_PCB_uint8_t(t_buffer_PCB* buffer_PCB,uint8_t* dato){
    memcpy(buffer_PCB->stream + buffer_PCB->offset, dato, sizeof(uint8_t));
    buffer_PCB->offset += sizeof(uint8_t);
}
void write_buffer_PCB_int(t_buffer_PCB* buffer_PCB,int* dato){
    memcpy(buffer_PCB->stream + buffer_PCB->offset, dato, sizeof(int));
    buffer_PCB->offset += sizeof(int);
}
void liberar_paquete(t_paquete_PCB* paquete){
	free(paquete->buffer_PCB->stream);
	free(paquete->buffer_PCB);
	free(paquete);
}
void recibir_buffer_PCB(int fd, t_paquete_PCB* paquete){
		recv(fd, &(paquete->buffer_PCB->size), TAMANIO_CAMPO_SIZE, 0);
//		paquete->buffer->stream = malloc(paquete->buffer->size);no hace falta el malloc porque ya de reservó el espacio fuera de la funcion???
		recv(fd, paquete->buffer_PCB->stream, paquete->buffer_PCB->size, 0);
}





