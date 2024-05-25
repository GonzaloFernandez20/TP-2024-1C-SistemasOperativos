#ifndef IO_PROTOCOLO_KERNEL_H_
#define IO_PROTOCOLO_KERNEL_H_

#include <include/estructuras.h>
#include <utils/serializacion/serializacion.h>
#include <utils/hilos/hilos.h>

void atender_peticiones_kernel(void);

void realizar_un_sleep(void);

void mandar_aviso_kernel(int PID);

#endif