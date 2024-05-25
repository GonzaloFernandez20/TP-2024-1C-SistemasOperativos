#include <ciclo.h>

void arranque(){
  pthread_mutex_init(&mutex_ciclo, NULL);

  esperarInterrupcionInicial();

  ciclo();
  pthread_mutex_destroy(&mutex_ciclo);
  pthread_mutex_destroy(&mutex_dispatch);

}
void ciclo(){
    while(sistema_encendido){
        ocupar_mutex(&mutex_ciclo);
        fetch();              // busca la siguiente instrucción.
        decode();             // interpreta la operación de la instrucción.
        execute();           // ejecuta la operación correspondiente al op_code_instruccion_recibida dadp, junto a los parámetros dados por la instrucción.
        sync_PCB_registrosCPU();
        checkInterrupt();     // se fija si hay interrupciones.
    }
}

void fetch(){}
void decode(){}
void execute(){}

void sync_PCB_registrosCPU(){}


void desocupar_mutex(pthread_mutex_t* mutex){
  printf("Desocupar mutex\n");
  if (pthread_mutex_unlock(mutex) != 0) {                                          
    perror("mutex_unlock");                                                     
    //loggear y devolver error                                                               
  } 
}
void ocupar_mutex(pthread_mutex_t* mutex){
  printf("Ocupar mutex\n");
  if (pthread_mutex_lock(mutex) != 0) {                                          
    perror("mutex_lock");                                                     
    //loggear y devolver error                                                               
  } 
}

void checkInterrupt() {
    /**
     * Verificar si el flag interrupt está en 1. 
     * Si es así entonces atenderInterrupcion().
     * 
     * Si la PID del proceso en ejecución coincide con 
     * el PID solicitado a interrumpir, entonces se realiza lo solicitado.
     * 
     */

    if(hayInterrupcion()){
        atenderInterrupcion();
    }
   else{
    desocupar_mutex(&mutex_ciclo);//detiene al ciclo antes de que vuelva a entrar a fetch()
   }
}
int hayInterrupcion(){
  int resultado;
    printf("SI HAY INTERRUPCION ESCRIBI 1 SINO 0:");
    scanf("%d",&resultado);
    printf("\n");
    return resultado;
    //codear más tarde xq trabaja con la conexion interrupt
}
void esperarInterrupcionInicial(){
    char* resultado;
    printf("MANDAR UN PCB DESDE KERNEL Y DESPUES DAR ENTER \n");
    printf("SI NO MANDAS EL PCB HAY SEGFAULT");

    scanf("%s",resultado);
    printf("\n");
    desocupar_mutex(&mutex_dispatch);
    return resultado;
    //codear más tarde xq trabaja con la conexion interrupt
    //el problema de la segfault se resuelve esperando a la primera interrupción y desbloqueando el mutes de dispatch
}
void atenderInterrupcion(void){
    // ATENDER INTERRUPCIÓN.
    //exportarPCB();
    //PCB = recibirPCB();                     // Actualizar PCB
    //actualizarRegistrosDeCPU(PCB.registros);
    //Estas tres líneas de código se ejecutan en el hilo bloqueado por mutex_dispatch
    desocupar_mutex(&mutex_dispatch);
    //desaparece la llamada a dispatchAuxiliar() porque después hacemos la solución con hilos
    dispatchAuxiliar();
}
void dispatchAuxiliar(){}