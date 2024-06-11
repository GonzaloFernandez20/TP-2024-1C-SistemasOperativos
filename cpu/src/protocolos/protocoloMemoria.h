#ifndef CPU_PROTOCOLO_MEMORIA_H_
#define CPU_PROTOCOLO_MEMORIA_H_

#include <include/estructuras.h>

#include <utils/serializacion/serializacion.h>

#include <MMU/MMU.h>


char* solicitar_instruccion_a_memoria(void);
char* _recibir_instruccion(void);

bool solicitar_ajustar_tamanio_de_proceso_a_memoria(int);
bool _recibir_respuesta_por_ajuste_de_tamanio(void);

char* leer_de_memoria(uint32_t direccion_logica);
void _solicitar_lectura_de_memoria(uint32_t direccion_fisica);
char* _recibir_string_por_lectura(void);

char* escribir_en_memoria(uint32_t direccion_logica, char* string_a_escribir);
void _solicitar_escritura_en_memoria(uint32_t direccion_fisica, char* string);
char* _recibir_respuesta_por_escritura(void);

#endif 