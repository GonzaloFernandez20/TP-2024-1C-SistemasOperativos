#include <protocolos/protocoloMemoria.h>

char* leer_direccion_fisica(uint32_t direccionFisica, int* resultado_de_operacion){
/* //Pedido de lectura de una dirección física
    t_paquete* paquete = crear_paquete(READ_DIRECCION_FISICA);

    int bufferSize = sizeof(direccionFisica);

    crear_buffer(paquete, bufferSize);

    buffer_add_uint32(paquete->buffer,direccionFisica);

    serializar_paquete(paquete, bufferSize + sizeof(int));//buffer + cod op
    
    enviar_paquete(paquete, fd_conexion_memoria);
    
    eliminar_paquete(paquete);
//Recepción de lo que había en la dirección física
    *resultado_de_operacion = recibir_operacion(fd_conexion_memoria);

    t_buffer* buffer = recibir_buffer(fd_conexion_memoria);
    void* stream = buffer->stream;
    int stringLength = buffer_read_int(&stream);//revisar si se envia el \0 desde memoria
    char* datoEnDirFisica = buffer_read_string(&stream, stringLength);

    eliminar_buffer(buffer);

    return datoEnDirFisica;     */
    return "";
}

void enviar_a_memoria(void* particion, int bytes, int direccion_fisica){

    t_paquete* paquete = crear_paquete(ESCRITURA);

    int buffer_size = sizeof(bytes) + 2 * sizeof(int);

    crear_buffer(paquete, buffer_size);

    buffer_add_int(paquete->buffer, bytes);
    buffer_add_int(paquete->buffer, direccion_fisica);
    buffer_add_valor(paquete->buffer, particion, bytes);

    enviar_paquete(paquete, fd_conexion_memoria);
    eliminar_paquete(paquete);
 
    int respuesta; // ME QUEDO ESPERANDO LA RESPUESTA DE PARTE DE MEMORIA ...
    recv(fd_conexion_memoria, &respuesta, sizeof(int), MSG_WAITALL);
}