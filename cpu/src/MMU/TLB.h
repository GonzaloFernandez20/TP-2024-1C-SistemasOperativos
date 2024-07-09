#ifndef TLB_H_INCLUDED
#define TLB_H_INCLUDED

#define FIFO "FIFO"  // las defino acá por si en algun momento cambia el nombre en config para referirnos a FIFO
#define LRU "LRU"    // lo mismo

#include <include/estructuras.h>
#include <MMU/MMU.h>
#include <commons/string.h>



typedef enum RESULTADO_BUSQUEDA_EN_TABLA_TLB {
    SEARCH_ERROR,
    SEARCH_OK,
}respuesta_busqueda;


typedef struct ENTRADA_DE_TLB {
    int pid;
    int nro_pagina;
    int nro_marco;
}t_entrada_tlb;

void inicializar_TLB(void);
void agregar_a_TLB(int nro_pagina, int nro_marco);
void _eliminar_una_entrada_con_algoritmo(void);
int algoritmo_es_LRU(void);
int _elegir_entrada_victima(void);
int elegir_con_algoritmo_LRU();
void _agregar_nueva_entrada(int nro_pagina, int nro_marco);

respuesta_busqueda _buscar_entrada_tlb(int *marco, int nro_pagina);
tlb_respuesta consultar_marco_en_TLB(int nro_pagina, int* nro_marco);
#endif