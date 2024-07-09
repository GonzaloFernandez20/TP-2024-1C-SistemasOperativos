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
void devolver_contexto_ejecucion_IO_STDIN_READ(char* nombre_interfaz, int tamanio);
void devolver_contexto_ejecucion_IO_STDOUT_WRITE(char* nombre_interfaz, int tamanio);
void devolver_contexto_ejecucion_IO_FS(char* nombre_interfaz, char* nombre_archivo, int operacion);
void devolver_contexto_ejecucion_IO_FS_TRUNCATE(char* nombre_interfaz, char* nombre_archivo, int tamanio_truncado);
void devolver_contexto_ejecucion_IO_FS_WRITE_READ(char* nombre_interfaz, char* nombre_archivo, int registro_tamanio, int reg_puntero, int operacion);

void _cargar_pcb(t_paquete* paquete, int motivo);
#endif 