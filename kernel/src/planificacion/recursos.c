#include <planificacion/recursos.h>

void inicializar_recursos(){
    int cant_recursos = string_array_size(config_kernel.RECURSOS);
    recursos_disponibles = dictionary_create();

    for (int i = 0; i < cant_recursos; i++)
    {   
        t_recurso *nuevo_recurso = malloc(sizeof(t_recurso));
        nuevo_recurso->nombre_recurso = strdup(config_kernel.RECURSOS[i]);
        nuevo_recurso->instancias_recursos = atoi(config_kernel.INSTANCIAS_RECURSOS[i]);

        // COLA DE BLOQUEADOS DEL RECURSO:
        t_estado* bloqueados_recurso = malloc(sizeof(t_estado));
        bloqueados_recurso->cola = list_create();
        pthread_mutex_init(&(bloqueados_recurso->mutex_cola),NULL); 
        bloqueados_recurso->nombre = strdup("BLOCKED");

        nuevo_recurso->cola_recurso = bloqueados_recurso;

        // AGREGAMOS EL RECURSO AL DICCIONARIO CON SU NOMBRE COMO KEY:
        pthread_mutex_lock(&diccionario_recursos);
            dictionary_put(recursos_disponibles, nuevo_recurso->nombre_recurso, nuevo_recurso);
        pthread_mutex_unlock(&diccionario_recursos);
    }   

    //INICIALIZO LISTA PARA PROCESOS QUE TIENEN RECURSOS EN USO:
    recursos_usados = list_create();
}

int existe_recurso(char* nombre_recurso){
    return dictionary_has_key(recursos_disponibles, nombre_recurso);
}

void agregar_instancia_recurso(int PID, t_recurso *recurso){
    t_recurso_usado *nuevo_recurso = malloc(sizeof(t_recurso_usado));
    nuevo_recurso->PID = PID;
    nuevo_recurso->nombre_recurso = strdup(recurso->nombre_recurso);
    list_add(recursos_usados, nuevo_recurso);
}

void eliminar_instancia_recurso(int PID, t_recurso *recurso){
    pthread_mutex_lock(&cola_recursos_usados);
    int cant_instancias = list_size(recursos_usados);
    int encontro_instancia = 0; // SI NO ENCUENTRA INSTANCIA EN LA LISTA QUIERE DECIR QUE NO HUBO WAIT PREVIO

    for (int i = 0; i < cant_instancias; i++){ 
        t_recurso_usado *recurso_usado = list_get(recursos_usados, i);
    
        if (PID == recurso_usado->PID && string_equals_ignore_case(recurso->nombre_recurso, recurso_usado->nombre_recurso)){   
            t_recurso_usado *instancia_recurso = list_remove(recursos_usados, i);
            recurso->instancias_recursos++;
            free(instancia_recurso->nombre_recurso);
            free(instancia_recurso);
            encontro_instancia = 1;
            break;
        }

    }   
    pthread_mutex_unlock(&cola_recursos_usados);
    if (!encontro_instancia){ recurso->instancias_recursos++; } // GENERO UNA INSTANCIA DEL AIRE
}

void liberar_recursos(int PID){
    pthread_mutex_lock(&cola_recursos_usados);
    int cant_instancias = list_size(recursos_usados);
    for (int i = 0; i < cant_instancias;) {
        t_recurso_usado *recurso_usado = list_get(recursos_usados, i);

        if (PID == recurso_usado->PID) {
            t_recurso_usado *instancia_recurso = list_remove(recursos_usados, i);
            t_recurso *recurso = dictionary_get(recursos_disponibles, instancia_recurso->nombre_recurso);
            free(instancia_recurso->nombre_recurso);
            free(instancia_recurso);
            
            if (!list_is_empty(recurso->cola_recurso->cola)) // Hay al menos 1 elemento bloqueado en la cola del recurso
            {
                pthread_mutex_lock(&recurso->cola_recurso->mutex_cola);
                t_pcb *pcb_liberado = list_get(recurso->cola_recurso->cola, 0);
                pthread_mutex_unlock(&recurso->cola_recurso->mutex_cola);

                agregar_instancia_recurso(pcb_liberado->pid, recurso);
                trasladar(pcb_liberado->pid, recurso->cola_recurso, ready);
            }
            else
            {
                recurso->instancias_recursos++;
            }
              
        } else {
            i++; // AVANZA AL SIGUIENTE SOLO SI NO ELIMINO.
        }
    }
    pthread_mutex_unlock(&cola_recursos_usados);
}

int _coincide_pid(t_recurso_usado* recurso, int PID){
    return recurso->PID == PID;
}

void _destruir(t_recurso_usado *recurso){
    free(recurso->nombre_recurso);
    free(recurso);
}

/* void imprimir_lista(t_list *lista) {
    if (!list_is_empty(lista)) {
        t_link_element *elemento = lista->head;
        while (elemento != NULL) {
        t_recurso_usado *recurso = elemento->data;
        printf("\t\tPID: %d, Recurso: %s\n", recurso->PID, recurso->nombre_recurso);
        elemento = elemento->next;
        }
    }
} */

void imprimir_bloqueados_por_recursos(void){
    int cant_recursos = string_array_size(config_kernel.RECURSOS);
    for (int i = 0; i < cant_recursos; i++)
    {
        t_recurso *recurso = dictionary_get(recursos_disponibles, config_kernel.RECURSOS[i]);
        int cant_recursos_bloqueados = list_size(recurso->cola_recurso->cola);
        for (int i = 0; i < cant_recursos_bloqueados; i++) // POR CADA RECURSO, ACCEDEMOS A SU COLA DE BLOQUEADOS E IMPRIMIMOS.
        {   
            t_pcb *pcb_bloqueado = list_get(recurso->cola_recurso->cola, i);
            printf("\t%s: PID: %d\n", recurso->nombre_recurso, pcb_bloqueado->pid);
        }
    }
}