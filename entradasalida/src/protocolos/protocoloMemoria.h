#ifndef IO_PROTOCOLO_MEMORIA_H_
#define IO_PROTOCOLO_MEMORIA_H_

#include <include/estructuras.h>
#include <utils/serializacion/serializacion.h>
#include <utils/hilos/hilos.h>

void enviar_a_direccion_fisica(char* info, uint32_t direccionFisica, int* resultado_de_operacion);
char* leer_direccion_fisica(uint32_t direccionFisica, int* resultado_de_operacion);

#endif