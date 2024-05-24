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
            break;
        }
    }
    pthread_mutex_unlock(&mutex_memoria_instrucciones);

    return indice_proceso;
}

void mostrar_procesos_cargados(void){
    pthread_mutex_lock(&mutex_memoria_instrucciones);
    char* lista_de_pids = string_duplicate(armar_lista_pids());
    pthread_mutex_unlock(&mutex_memoria_instrucciones);

    pthread_mutex_lock(&mutex_log_debug);
        log_info(memoria_log_debugg, "Memoria de instrucciones: [%s]\n", lista_de_pids);
    pthread_mutex_unlock(&mutex_log_debug);

    free(lista_de_pids);
}

char* armar_lista_pids(void){
    int size = list_size(memoria_de_instrucciones);
    char* lista = string_duplicate(""); // El máximo tamaño de un número de 2 dígitos + coma + carácter nulo 
    
    if (size < 1)
    {
        char *vacia = string_duplicate("");
        return vacia;
    }

    for (int i = 0; i < size; i++) {
        char* cadena = malloc(sizeof(char) * 3); // Tamaño suficiente para un número de hasta 2 dígitos + carácter nulo

        t_proceso* proceso = list_get(memoria_de_instrucciones, i);
        sprintf(cadena, " %d ", proceso->PID);
        string_append(&lista, cadena);

        free(cadena);
    }
    return lista;
}
