#ifndef MMU_H_INCLUDED
#define MMU_H_INCLUDED

#include <include/estructuras.h>
#include <MMU/TLB.h>

uint32_t traducir_DL_a_DF(uint32_t direccion_logica);
uint32_t obtener_marco(uint32_t nro_pagina);
void traducir_direcciones(uint32_t tamanio, uint32_t direccion_logica);

#endif