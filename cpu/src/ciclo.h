#include <include/estructuras.h>
#include <pthread.h>

void arranque();
void ciclo();
void fetch();
void decode();
void execute();
void checkInterrupt();

void sync_PCB_registrosCPU();
void atenderInterrupcion();
int  hayInterrupcion();

void ocupar_mutex(pthread_mutex_t*);
void desocupar_mutex(pthread_mutex_t*);
void esperarInterrupcionInicial();

void dispatchAuxiliar();//desaparece despues de los tests
