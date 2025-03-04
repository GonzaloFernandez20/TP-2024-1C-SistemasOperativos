#ifndef OPERACIONES_H_INCLUDED
#define OPERACIONES_H_INCLUDED

// ---------- INCLUSIONES
#include <include/estructuras.h>
#include <utils/serializacion/serializacion.h>
#include <protocolos/protocoloMemoria.h>
#include <protocolos/protocoloKernel.h>
#include <fileSystem/estructurasFS.h>
#include <commons/string.h>
#include <math.h>


#include <stdio.h>
#include <stdlib.h>

// ---------- DECLARACIONES DE FUNCIONES

void realizar_un_fs_create(void);
void crear_archivo(char* nombre_archivo);

/*******/

void realizar_un_fs_delete(void);
void eliminar_archivo(char* nombre_archivo);

/*******/

void realizar_un_fs_truncate(void);
void truncar_archivo(char* nombre_archivo, int nuevo_tamanio, int PID);
void achicar_archivo(t_FCB* FCB, int nuevo_tamanio);
void agrandar_archivo(t_FCB* FCB, int nuevo_tamanio, int PID);
void actualizar_tamanio_archivo(t_FCB* FCB, int nuevo_tamanio);
void compactar(t_FCB* FCB);
char* extraer_contenido_archivo(t_FCB* FCB);
void desplazar_metadata(int nuevo_bloque_inicial, t_FCB* FCB);
void realocar_contenido_metadata(int nuevo_bloque_inicial, t_FCB* FCB, char* contenido_metadata);
void actualizar_bloque_inicial(t_FCB* FCB, int nuevo_bloque_inicial);

/*******/

void realizar_un_fs_read(void);

char *leer_de_archivo(char *nombre_archivo, int tamanio_a_leer, int puntero_archivo);

/*******/

void realizar_un_fs_write(void);

void escribir_en_archivo(char *nombre_archivo, int puntero_archivo, char *cadena);

/*******/

#endif