#ifndef TLB_H_INCLUDED
#define TLB_H_INCLUDED

#define FIFO "FIFO"  // las defino acá por si en algun momento cambia el nombre en config para referirnos a FIFO
#define LRU "LRU"    // lo mismo

#include <include/estructuras.h>
#include <MMU/MMU.h>



typedef enum RESULTADO_BUSQUEDA_EN_TABLA_TLB {
    SEARCH_ERROR,
    SEARCH_OK,
}res_busqueda;


typedef struct ENTRADA_DE_TLB {
    int pid;
    uint32_t nro_pagina;
    uint32_t nro_marco;
    // size_t tiempo_de_vida;
}t_entrada_tlb;


void inicializar_TLB(void);
void agregar_a_TLB(uint32_t nro_pagina, uint32_t nro_marco);
void _eliminar_una_entrada_por_algoritmo(void);
int _elegir_entrada_victima(void);
int elegir_con_algoritmo_LRU();
void _agregar_nueva_entrada(int pid, uint32_t nro_pagina, uint32_t nro_marco);

res_busqueda buscar_entrada_tlb(t_entrada_tlb *entrada, int *indice, int pid, uint32_t nro_pagina);
tlb_res consultar_marco_en_TLB(uint32_t nro_pagina, uint32_t* nro_marco);

#endif