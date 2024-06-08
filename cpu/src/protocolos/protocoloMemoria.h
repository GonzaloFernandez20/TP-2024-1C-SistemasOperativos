#ifndef CPU_PROTOCOLO_MEMORIA_H_
#define CPU_PROTOCOLO_MEMORIA_H_

#include <include/estructuras.h>

#include <utils/serializacion/serializacion.h>


char* solicitar_instruccion_a_memoria(void);
char* _recibir_instruccion(void);

bool solicitar_ajustar_tamanio_de_proceso_a_memoria(int);
bool recibir_respuesta_por_ajuste_de_tamanio(void);

#endif 