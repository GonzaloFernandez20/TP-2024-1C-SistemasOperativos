#ifndef IO_PROTOCOLO_KERNEL_H_
#define IO_PROTOCOLO_KERNEL_H_

#include <protocolos/protocoloMemoria.h>
#include <include/estructuras.h>
#include <utils/serializacion/serializacion.h>
#include <utils/hilos/hilos.h>
#include <readline/readline.h>
#include <readline/history.h>

void atender_peticiones_kernel(void);

void realizar_un_sleep(void);

void mandar_aviso_kernel(int PID);

void realizar_un_stdin_read();

void realizar_un_stdout_write();
#endif