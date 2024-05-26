#ifndef INSTRUCCIONES_H_INCLUDED
#define INSTRUCCIONES_H_INCLUDED

#include <include/estructuras.h>
#include <pthread.h>

#include <commons/collections/dictionary.h>
#include <commons/string.h>
#include <protocolos/protocoloInterrupt.h>
#include <protocolos/protocoloDispatch.h>
#include <utils/serializacion/serializacion.h>

#define SET "SET"
#define MOV_IN "MOV_IN"
#define MOV_OUT "MOV_OUT"
#define SUM "SUM"
#define SUB "SUB"
#define JNZ "JNZ"
#define RESIZE "RESIZE"
#define COPY_STRING "COPY_STRING"
#define WAIT "WAIT"
#define SIGNAL "SIGNAL"
#define IO_GEN_SLEEP "IO_GEN_SLEEP"
#define IO_STDIN_READ "IO_STDIN_READ"
#define IO_STDOUT_WRITE "IO_STDOUT_WRITE"
#define IO_FS_CREATE "IO_FS_CREATE"
#define IO_FS_DELETE "IO_FS_DELETE"
#define IO_FS_TRUNCATE "IO_FS_TRUNCATE"
#define IO_FS_WRITE "IO_FS_WRITE"
#define IO_FS_READ "IO_FS_READ"
#define EXIT_OS "EXIT_OS"

void set(void);

void sum(void);

void sub(void);

void jnz(void);

void io_gen_sleep(void);

void* direccion_del_registro(char* nombre_registro);

int tamanio_de_registro(char* registro);

#endif