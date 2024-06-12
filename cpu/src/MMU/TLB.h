#ifndef TLB_H_INCLUDED
#define TLB_H_INCLUDED

#include <include/estructuras.h>
#include <MMU/MMU.h>


typedef struct ENTRADA_DE_TLB {
    int pid;
    uint32_t nro_pagina;
    uint32_t nro_marco;
}t_entrada_tlb;


// constantes globales
size_t MAX_ENTRADAS;
char* ALGORITMO_TLB;
t_list* tabla_tlb;  //guardará un array de t_entrada_tlb 


tlb_res consultar_marco_en_TLB(uint32_t nro_pagina, tlb_res *res);

#endif