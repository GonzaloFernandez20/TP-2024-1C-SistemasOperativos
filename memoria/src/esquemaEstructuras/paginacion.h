#ifndef PAGINACION_H_
#define PAGINACION_H_

#include <include/estructuras.h>

int buscar_PID_tablas_paginas(int PID_buscado);

int marcos_libres(void);

int buscar_marco_libre(void);

void liberar_marcos(int paginas_eliminar, t_list* tabla);

#endif