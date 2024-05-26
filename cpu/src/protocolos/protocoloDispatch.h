#ifndef CPU_PROTOCOLO_DISPATCH_H_
#define CPU_PROTOCOLO_DISPATCH_H_

#include <include/estructuras.h>
#include <ciclo/ciclo.h>
#include <utils/serializacion/serializacion.h>
#include <utils/hilos/hilos.h>

void procesar_operacion_dispatch(void);
void actualizarPCB(t_pcb* PCB_recibido);
void actualizarRegistrosDeCPU(t_pcb* un_pcb);
void sync_PCB_registrosCPU(void);

// ------------------- 
void recibir_contexto_ejecucion();
void devolver_contexto_ejecucion();
void devolver_contexto_ejecucion_IO_GEN_SLEEP(char* nombre_interfaz, int unidades_trabajo);
#endif 