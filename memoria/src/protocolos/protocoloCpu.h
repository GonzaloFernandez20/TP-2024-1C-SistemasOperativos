#ifndef MEMORIA_PROTOCOLO_CPU_H_
#define MEMORIA_PROTOCOLO_CPU_H_

#include <include/estructuras.h>
#include <esquemaEstructuras/memoriaInstrucciones.h>
#include <esquemaEstructuras/paginacion.h>
#include <protocolos/accesoEspacioUsuario.h>

#include <utils/serializacion/serializacion.h>
#include <utils/hilos/hilos.h>

void *procesar_operacion_cpu(void *fd_cpu_casteado);

void devolver_instruccion(void);

void enviar_instruccion_serializada(char* instruccion);

void ajustar_tamanio_proceso(void);

void ampliar_proceso(t_list* tabla, int tamanio_a_ampliar);

void reducir_proceso(t_list* tabla, int tamanio_a_reducir);

void acceso_a_tabla_paginas(void);

void enviar_marco(int marco);

#endif