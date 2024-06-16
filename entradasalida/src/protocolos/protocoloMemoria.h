#ifndef IO_PROTOCOLO_MEMORIA_H_
#define IO_PROTOCOLO_MEMORIA_H_

#include <include/estructuras.h>
#include <utils/serializacion/serializacion.h>
#include <utils/hilos/hilos.h>

void enviar_a_memoria(int PID, void* particion, int bytes, int direccion_fisica);
char* leer_direccion_fisica(uint32_t direccionFisica, int* resultado_de_operacion);

#endif