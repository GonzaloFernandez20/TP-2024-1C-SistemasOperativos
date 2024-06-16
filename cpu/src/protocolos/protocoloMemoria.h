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

uint32_t _recibir_entero_por_lectura(void);

char *escribir_string_en_memoria(uint32_t direccion_logica, char *string_a_escribir);

char *escribir_uint32_t_en_memoria(uint32_t direccion_logica, uint32_t entero_a_escribir);

char *escribir_uint8_t_en_memoria(uint32_t direccion_logica, uint8_t entero_a_escribir);

void _solicitar_escritura_uint32_t_en_memoria(uint32_t direccion_fisica, uint32_t entero);

void _solicitar_escritura_string_en_memoria(uint32_t direccion_fisica, char *string);

char* _recibir_respuesta_por_escritura(void);

uint32_t consultar_marco_en_TP(uint32_t nro_pagina);
void _solicitar_busqueda_de_marco_en_TP(uint32_t nro_pagina);
uint32_t _recibir_marco(void);

#endif 