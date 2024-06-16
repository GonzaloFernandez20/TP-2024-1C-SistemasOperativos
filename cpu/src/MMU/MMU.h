#ifndef MMU_H_INCLUDED
#define MMU_H_INCLUDED

#include <include/estructuras.h>
#include <MMU/TLB.h>
#include <protocolos/protocoloMemoria.h>

int traducir_DL_a_DF(uint32_t direccion_logica);

int obtener_marco(int nro_pagina);

void traducir_direcciones(int tamanio, uint32_t direccion_logica);

void agregar_direccion(int bytes, int direccion);

#endif