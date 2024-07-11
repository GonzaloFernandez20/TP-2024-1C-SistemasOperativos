#ifndef OPERACIONES_H_INCLUDED
#define OPERACIONES_H_INCLUDED

// ---------- INCLUSIONES
#include <include/estructuras.h>
#include <utils/serializacion/serializacion.h>
#include <protocolos/protocoloMemoria.h>
#include <protocolos/protocoloKernel.h>
#include <fileSystem/estructurasFS.h>
#include <commons/string.h>


#include <stdio.h>
#include <stdlib.h>

// ---------- DECLARACIONES DE FUNCIONES

void realizar_un_fs_create(void);

void crear_archivo(char* nombre_archivo);

/*******/

void realizar_un_fs_delete(void);

void eliminar_archivo(char* nombre_archivo);

/*******/

void realizar_un_fs_read(void);

char *leer_de_archivo(char *nombre_archivo, int tamanio_a_leer, int puntero_archivo);

/*******/

void realizar_un_fs_write(void);

void escribir_en_archivo(char *nombre_archivo, int puntero_archivo, char *cadena);

/*******/

#endif