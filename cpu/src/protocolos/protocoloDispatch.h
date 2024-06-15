#ifndef CPU_PROTOCOLO_DISPATCH_H_
#define CPU_PROTOCOLO_DISPATCH_H_

#include <include/estructuras.h>
#include <ciclo/ciclo.h>
#include <utils/serializacion/serializacion.h>
#include <utils/hilos/hilos.h>

void procesar_operacion_dispatch(void);

// ------------------- 
void recibir_contexto_ejecucion();

void devolver_contexto_ejecucion(int motivo);
void devolver_contexto_ejecucion_IO_GEN_SLEEP(char* nombre_interfaz, int unidades_trabajo);
void devolver_contexto_ejecucion_RECURSO(char *recurso, int codigo_recurso);
void devolver_contexto_ejecucion_IO_STDIN_READ(char* nombre_interfaz, char* registro_tamanio);

#endif 