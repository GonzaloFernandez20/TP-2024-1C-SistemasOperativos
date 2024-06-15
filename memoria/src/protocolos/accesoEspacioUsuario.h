#ifndef ACCESO_ESPACIO_USUARIO_H_
#define ACCESO_ESPACIO_USUARIO_H_

#include <include/estructuras.h>
#include <utils/serializacion/serializacion.h>

void realizar_lectura(int fd_cliente);

void devolver_valor_leido(void* valor, int tamanio_bytes, int fd_cliente);

void realizar_escritura(int fd_cliente);

#endif