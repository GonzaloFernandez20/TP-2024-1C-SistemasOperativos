#ifndef CPU_PROTOCOLO_MEMORIA_H_
#define CPU_PROTOCOLO_MEMORIA_H_

#include <include/estructuras.h>

#include <utils/serializacion/serializacion.h>


void solicitar_instruccion_a_memoria(int PC);

char* recibir_instruccion(void);

#endif 