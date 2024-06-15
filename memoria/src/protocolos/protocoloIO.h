#ifndef MEMORIA_PROTOCOLO_IO_H_
#define MEMORIA_PROTOCOLO_IO_H_

#include <include/estructuras.h>
#include <protocolos/accesoEspacioUsuario.h>

#include <utils/serializacion/serializacion.h>
#include <utils/hilos/hilos.h>


void* procesar_operacion_entradaSalida(void* fd_IO_casteado);
void atender_peticion_de_lectura();
void atender_peticion_de_escritura();
void enviar_a_interfaz(char* info, int resultado_de_operacion);
int guardar_en_direccion_fisica(uint32_t direccionFisica,char* datosParaDirFisica);//implementar con MELI
char* leer_direccion_fisica(uint32_t direccionFisica);

#endif
