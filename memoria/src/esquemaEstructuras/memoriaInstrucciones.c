#include <esquemaEstructuras/memoriaInstrucciones.h>

int buscar_PID_memoria_instrucciones(int PID_buscado){

    int indice_proceso;

    pthread_mutex_lock(&mutex_memoria_instrucciones);
    int size = list_size(memoria_de_instrucciones);
    
    for (int i = 0; i < size; i++) {
        t_proceso* proceso = list_get(memoria_de_instrucciones, i);
        if (proceso-> PID == PID_buscado)
        {
            indice_proceso = i;
            free(proceso);
            break;
        }
    }
    pthread_mutex_unlock(&mutex_memoria_instrucciones);

    return indice_proceso;
}