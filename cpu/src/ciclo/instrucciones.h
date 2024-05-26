#ifndef INSTRUCCIONES_H_INCLUDED
#define INSTRUCCIONES_H_INCLUDED

#include <include/estructuras.h>
#include <pthread.h>

#include <commons/collections/dictionary.h>
#include <commons/string.h>
#include <protocolos/protocoloInterrupt.h>
#include <protocolos/protocoloDispatch.h>
#include <utils/serializacion/serializacion.h>

void set(void);

void sum(void);

void sub(void);

void jnz(void);

void io_gen_sleep(void);

void exit_os(void);

void* direccion_del_registro(char* nombre_registro);

int tamanio_de_registro(char* registro);

#endif