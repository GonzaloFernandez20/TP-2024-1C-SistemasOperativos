#ifndef ESTRUCTURAS_FS_H_
#define ESTRUCTURAS_FS_H_

#include <include/estructuras.h>


void levantar_filesystem(void);

FILE* abrir_archivo(int bytes_tamanio, char* path);

char* concatenar_rutas(char* directorio, char* archivo);

void mapear_bitmap_en_memoria(void);

#endif