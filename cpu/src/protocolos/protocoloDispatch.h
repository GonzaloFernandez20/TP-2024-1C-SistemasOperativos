#ifndef CPU_PROTOCOLO_DISPATCH_H_
#define CPU_PROTOCOLO_DISPATCH_H_

#include <include/estructuras.h>
#include <ciclo.h>
#include <utils/serializacion/serializacion.h>
#include <utils/hilos/hilos.h>

void *procesar_operacion_dispatch(void *fd_dispatch_casteado);
void  serializar_PCB(t_buffer_PCB* buffer_PCB,t_pcb* PCB);
void crear_buffer_PCB(t_paquete_PCB* paquete_PCB);
t_paquete_PCB* crear_paquete_PCB(void);
void write_buffer_PCB_uint32_t(t_buffer_PCB* buffer_PCB,uint32_t* dato);
void write_buffer_PCB_uint8_t(t_buffer_PCB* buffer_PCB,uint8_t* dato);
void write_buffer_PCB_int(t_buffer_PCB* buffer_PCB,int* dato);
uint32_t read_buffer_PCB_uint32_t(t_buffer_PCB* buffer_PCB_llegada);
uint8_t read_buffer_PCB_uint8_t(t_buffer_PCB* buffer_PCB_llegada);
int read_buffer_PCB_int(t_buffer_PCB* buffer_PCB_llegada);
t_pcb* deserializar_PCB(t_buffer_PCB* buffer_PCB_llegada);
void actualizarRegistrosDeCPU(t_pcb* un_pcb);
void actualizarPCB(t_pcb* PCB_recibido);
t_pcb* recibirPCB();
void liberar_paquete(t_paquete_PCB* paquete);
int recibir_motivo_de_dealojo(int socket_cliente);
void recibir_buffer_PCB(int fd, t_paquete_PCB* paquete);
t_paquete_PCB*  recibir_paquete_PCB(int fd);
void inicializar_PCB();

#endif 