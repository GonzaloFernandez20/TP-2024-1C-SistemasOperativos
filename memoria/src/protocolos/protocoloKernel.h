#ifndef MEMORIA_PROTOCOLO_KERNEL_H_
#define MEMORIA_PROTOCOLO_KERNEL_H_

#include <stdlib.h>

#include <include/estructuras.h>
#include <esquemaEstructuras/memoriaInstrucciones.h>

#include <utils/serializacion/serializacion.h>
#include <utils/hilos/hilos.h>

void *procesar_operacion_kernel(void *fd_kernel_casteado);

void* crear_proceso(void);

void crear_estructuras_administrativas(int PID, char* path_archivo);

void cargar_instrucciones(int PID, char* path_archivo);

FILE* abrir_archivo(char* archivo);

char* concatenar_rutas(char* directorio, char* archivo);

int contar_lineas(FILE* archivo_seudocodigo);

void leer_almacenar_instrucciones(char** CODE_segmento, FILE* seudocodigo);

void* eliminar_proceso(void);

void liberar_estructuras_asociadas(int PID);

void liberar_segmento_codigo(t_proceso* proceso);



#endif