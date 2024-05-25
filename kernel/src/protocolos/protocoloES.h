#ifndef KERNEL_PROTOCOLO_H_
#define KERNEL_PROTOCOLO_H_

#include <include/estructuras.h>
#include <planificacion/pcb.h>
#include <planificacion/colasEstados.h>

#include <utils/hilos/hilos.h>
#include <utils/serializacion/serializacion.h>

void *procesar_conexion_es(void *nombre_interfaz);

void sacar_interfaz_de_diccionario(char* nombre_interfaz);

void mandar_procesos_a_exit(t_estado* bloqueados);

void solicitar_operacion_IO_GEN_SLEEP(int PID, int fd, int parametro);

void recibir_aviso(char* nombre, int fd);


#endif