#ifndef MEMORIA_PROTOCOLO_IO_H_
#define MEMORIA_PROTOCOLO_IO_H_

#include <include/estructuras.h>

#include <utils/serializacion/serializacion.h>
#include <utils/hilos/hilos.h>


void *procesar_operacion_entradaSalida(void *fd_ent_sal_casteado);

#endif
