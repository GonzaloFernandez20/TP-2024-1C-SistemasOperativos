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
void mov_in(void);
void mov_out(void);
void resize(void);
void copy_string(void);
void io_stdin_read(void);
void io_stdout_write(void);
void io_gen_sleep(void);
void exit_os(void);

void* direccion_del_registro(char* nombre_registro);

int tamanio_de_registro(char* registro);

void copiar_string_desde_memoria(uint32_t dl_origen, uint32_t dl_destino, uint32_t cantidad_de_bytes);

#endif