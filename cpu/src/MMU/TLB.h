#ifndef TLB_H_INCLUDED
#define TLB_H_INCLUDED

#include <include/estructuras.h>
#include <MMU/MMU.h>


typedef struct 


// constantes globales
const size_t MAX_ENTRADAS = config_cpu.CANTIDAD_ENTRADAS_TLB;
const char* ALGORITMO_TLB = config_cpu.ALGORITMO_TLB;


tlb_res consultar_marco_en_TLB(uint32_t nro_pagina, tlb_res *res);

#endif