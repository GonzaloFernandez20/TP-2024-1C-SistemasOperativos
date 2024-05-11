#include <planificacion/planificador.h>

int pid_nuevo_proceso = 0; 
t_list *new;
t_list *ready;
t_list *exec;
t_list *estado_exit;
t_list *blocked;

void* crear_proceso(char *path_proceso){
    // LOG MINIMO: Creación de Proceso: "Se crea el proceso <PID> en NEW"

    //Iniciar proceso: Se encargará de iniciar un nuevo proceso cuyas instrucciones a ejecutar se encontrarán en el archivo <path> dentro del file system de la máquina donde corra la Memoria. Este mensaje se encargará de la creación del proceso (PCB) en estado NEW.
    int pid_asignado = _asignar_PID();
    t_pcb *nuevo_pcb = crear_pcb(pid_asignado);

    //Envio del path a memoria para que haga lo suyo


/*  pthread_mutex_lock(log);
    log_info(kernelLogger, "Se crea el proceso <%d> en NEW", nuevo_pcb->pid);
    pthread_mutex_unlock(log); */

    //mutex
    //lo agrego a la lista de procesos en NEW
    list_add(new, (void *)nuevo_pcb);
    //mutex


    return NULL;
} 

int _asignar_PID(void){
    return pid_nuevo_proceso++;
}//Lo uso y lo incremento para que otro PCB tenga un pid distinto.

void iniciar_colas_planificacion(void){
    new = list_create();
    ready = list_create();
    exec = list_create();
    estado_exit = list_create();
    blocked = list_create();
    // list_create hace malloc, guarda con eso
}

void _mostrar_pcbs(void *pcbDeLista) {
    t_pcb *pcb = (t_pcb *)pcbDeLista;
    printf("\t\tPID: %d\n", pcb->pid);
}
/*  DUDA:
Deberian ser colas? 

La cola (queue) es también una estructura lineal restrictiva en la que solo podremos poner y sacar elementos respetando la siguiente restricción: el primer elemento en llegar a la cola será también el primero en salir (FIFO, First In First Out).

Pareceria venir al pelo, pero si llegan dos procesos en el mismo instante, uno viene de Fin de Quantumm y otro viene de NEW, y por esas cosas de la vida queda primero el que llega de NEW, yo deberia sacar el que llego de quantum, pero como esta segundo no puedo. 

De todas formas no se si se nos va a dar esta situacion jajaj pero me surgio la duda.
*/