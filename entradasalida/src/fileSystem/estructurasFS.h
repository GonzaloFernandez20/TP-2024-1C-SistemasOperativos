#ifndef ESTRUCTURAS_FS_H_
#define ESTRUCTURAS_FS_H_

#include <include/estructuras.h>
#include <commons/string.h>
#include <dirent.h>


void levantar_filesystem(void);

FILE* abrir_archivo(int bytes_tamanio, char* path);

char* concatenar_rutas(char* directorio, char* archivo);

void mapear_bitmap_en_memoria(void);

void cargar_FCBs_archivos_metadata(void);

int es_un_metadata(char* archivo);

bool comparar_por_bloque_inicial(void *FCB_1, void *FCB_2);

int asignar_primer_bloque_libre(void);

int obtener_primer_bloque_libre(void);

void liberar_bloques(int bloque_inicial, int bloque_final_mas_uno);

void ocupar_bloques(int bloque_inicial, int bloque_final_mas_uno);

int hay_suficientes_bloques_contiguos(int primer_bloque, int ultimo_bloque_mas_uno);

int buscar_FCB_en_archivos_metadata(char* archivo_buscado);

#endif