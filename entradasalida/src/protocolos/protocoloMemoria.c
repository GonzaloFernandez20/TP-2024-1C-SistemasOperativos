#include <protocolos/protocoloMemoria.h>

char* leer_direccion_fisica(uint32_t direccionFisica, int* resultado_de_operacion){
//Pedido de lectura de una dirección física
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

    return datoEnDirFisica;    
}

void enviar_a_direccion_fisica(char* info, uint32_t direccionFisica, int* resultado_de_operacion){
    t_paquete* paquete = crear_paquete(WRITE_DIRECCION_FISICA);

    int bufferSize = sizeof(direccionFisica)+sizeof(int)+strlen(info)+1;

    crear_buffer(paquete, bufferSize);

    buffer_add_uint32(paquete->buffer,direccionFisica);
    buffer_add_int(paquete->buffer,strlen(info)+1);
    buffer_add_string(paquete->buffer,info);

    serializar_paquete(paquete, bufferSize + sizeof(int));//buffer + el código de operacion
    
    enviar_paquete(paquete, fd_conexion_memoria);
    
    eliminar_paquete(paquete);

    //TODO BIEN?  
    *resultado_de_operacion = recibir_operacion(fd_conexion_memoria);   
}