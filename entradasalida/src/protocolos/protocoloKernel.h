#ifndef IO_PROTOCOLO_KERNEL_H_
#define IO_PROTOCOLO_KERNEL_H_

#include <protocolos/protocoloMemoria.h>
#include <fileSystem/operaciones.h>
#include <include/estructuras.h>
#include <utils/serializacion/serializacion.h>
#include <utils/hilos/hilos.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <commons/collections/list.h>


void atender_peticiones_kernel(void);

void realizar_un_sleep(void);

void mandar_aviso_kernel(int PID);

void realizar_un_stdin_read();

void realizar_un_stdout_write();

char* truncar_cadena(char *cadena, int tamanio);

void* leer_de_memoria(int PID, int direccion_fisica, int bytes);

void peticion_lectura_memoria(int PID, int direccion_fisica, int bytes);

char *generar_cadena(void* particion, int bytes);

void enviar_cadena_memoria(int PID, char* cadena_leida, void *stream);

char *traer_cadena_memoria(int PID, int tamanio_a_leer, void *stream);
#endif