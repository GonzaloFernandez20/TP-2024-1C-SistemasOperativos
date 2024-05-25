#ifndef KERNEL_CONEXIONES_H_
#define KERNEL_CONEXIONES_H_

#include <include/estructuras.h>
#include <inicializar/inicializar.h> 
#include <protocolos/protocoloES.h>

#include <utils/hilos/hilos.h>
#include <utils/conexiones/conexiones.h>
#include <utils/serializacion/serializacion.h>


void iniciar_servidor_kernel(void);

void establecer_conexiones(void);

void conectar_memoria(void);

void conectar_dispatch(void);

void conectar_interrupt(void);

void atender_entradasalida(void);

// funciones auxiliares para atender las interfaces

void cargar_diccionario_instrucciones(void);

void atender_interfaz(void (*procesar_conexion)(void*));

void asignar_hilo_interfaz(char *nombre_interfaz,  void (*procesar_conexion)(void*));

char* recibir_handshake_IO(int fd_cliente);

char* recibir_presentacion_IO(int fd_cliente);

void registrar_interfaz_conectada(char* nombre_interfaz, char* tipo_interfaz, int fd);

void* gestionar_peticiones(void* interfaz);

#endif