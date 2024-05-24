#ifndef MEMORIA_INSTRUCCIONES_H_
#define MEMORIA_INSTRUCCIONES_H_

#include <include/estructuras.h>
#include <commons/string.h>

int buscar_PID_memoria_instrucciones(int PID_buscado);

void mostrar_procesos_cargados(void);

char* armar_lista_pids(void);

#endif