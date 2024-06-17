#ifndef INSTRUCCIONES_H_INCLUDED
#define INSTRUCCIONES_H_INCLUDED

#include <include/estructuras.h>
#include <pthread.h>
#include <stdint.h>

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

void* leer_string(int cantidad_de_bytes);

void escribir_string(void* string_leido);

void io_stdin_read(void);

void io_stdout_write(void);

void io_gen_sleep(void);

void wait_kernel(void);

void signal_kernel(void);

void exit_os(void);

void* direccion_del_registro(char* nombre_registro);

int tamanio_de_registro(char* registro);

void* obtener_direcciones_fisicas(void);

char *generar_cadena(void* particion, int bytes);

#endif