#ifndef CPU_PROTOCOLO_MEMORIA_H_
#define CPU_PROTOCOLO_MEMORIA_H_

#include <include/estructuras.h>

#include <utils/serializacion/serializacion.h>

#include <MMU/MMU.h>


char* solicitar_instruccion_a_memoria(void);
char* _recibir_instruccion(void);

int consultar_marco_en_TP(int nro_pagina);
int recibir_marco(void);

int solicitar_ajustar_tamanio(int tamanio_nuevo);

void* leer_de_memoria(int direccion_fisica, int bytes);
void peticion_lectura_memoria(int direccion_fisica, int bytes);

void escribir_en_memoria(void* particion, int bytes, int direccion_fisica);

#endif 