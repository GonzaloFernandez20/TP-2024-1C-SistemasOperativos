#ifndef CICLO_H_INCLUDED
#define CICLO_H_INCLUDED

#include <include/estructuras.h>
#include <pthread.h>

#include <commons/collections/dictionary.h>
#include <commons/string.h>
#include <protocolos/protocoloInterrupt.h>
#include <ciclo/instrucciones.h>
#include <utils/serializacion/serializacion.h>
#include <protocolos/protocoloMemoria.h>

void ciclo(void);

void fetch(void);

void decode_and_execute(void);

void checkInterrupt(void);

char* _armado_parametros(char** instrucciones);

char* _enum_interrupcion_string();

#endif