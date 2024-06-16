#ifndef MMU_H_INCLUDED
#define MMU_H_INCLUDED

#include <include/estructuras.h>
#include <MMU/TLB.h>
#include <protocolos/protocoloMemoria.h>

uint32_t dl_a_df(uint32_t direccion_logica);
uint32_t obtener_marco_de(uint32_t nro_pagina);



#endif