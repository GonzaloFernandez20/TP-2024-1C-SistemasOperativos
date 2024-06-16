#ifndef CPU_PROTOCOLO_MEMORIA_H_
#define CPU_PROTOCOLO_MEMORIA_H_

#include <include/estructuras.h>

#include <utils/serializacion/serializacion.h>

#include <MMU/MMU.h>


char* solicitar_instruccion_a_memoria(void);
char* _recibir_instruccion(void);

int solicitar_ajustar_tamanio(int tamanio_nuevo);

void* leer_de_memoria(int direccion_fisica, int bytes);
void peticion_lectura_memoria(int direccion_fisica, int bytes);



char* _recibir_string_por_lectura(void);

char* escribir_en_memoria(uint32_t direccion_logica, char* string_a_escribir);
void _solicitar_escritura_en_memoria(uint32_t direccion_fisica, char* string);
char* _recibir_respuesta_por_escritura(void);

uint32_t consultar_marco_en_TP(uint32_t nro_pagina);
void _solicitar_busqueda_de_marco_en_TP(uint32_t nro_pagina);
uint32_t _recibir_marco(void);

#endif 