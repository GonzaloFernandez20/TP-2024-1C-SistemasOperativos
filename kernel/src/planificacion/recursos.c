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
        bloqueados_recurso->nombre = strdup("BLOCKED: ");
        string_append(&(bloqueados_recurso->nombre), nuevo_recurso->nombre_recurso);

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

    pthread_mutex_lock(&cola_recursos_usados);
        list_add(recursos_usados, nuevo_recurso);
    pthread_mutex_unlock(&cola_recursos_usados);

    imprimir_lista(recursos_usados);
}

void eliminar_instancia_recurso(int PID, t_recurso *recurso){
    pthread_mutex_lock(&cola_recursos_usados);
    int cant_instancias = list_size(recursos_usados);
    int encontro_instancia = 0; // SI NO ENCUENTRA INSTANCIA EN LA LISTA QUIERE DECIR QUE NO HUBO WAIT PREVIO

    for (int i = 0; i < cant_instancias; i++){ 
        t_recurso_usado *recurso_usado = list_get(recursos_usados, i);
    
        if (PID == recurso_usado->PID && string_equals_ignore_case(recurso->nombre_recurso, recurso_usado->nombre_recurso)){   
            t_recurso_usado *instancia_recurso = list_remove(recursos_usados, i);
            printf("Eliminada instancia del recurso: %s de PID: %d \n", recurso_usado->nombre_recurso, recurso_usado->PID);
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

            printf("Eliminada instancia del recurso: %s de PID: %d\n", recurso_usado->nombre_recurso, recurso_usado->PID);
            
            free(instancia_recurso->nombre_recurso);
            free(instancia_recurso);
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

void imprimir_lista(t_list *lista) {
    t_link_element *elemento = lista->head;
    while (elemento != NULL) {
        t_recurso_usado *recurso = elemento->data;
        printf("PID: %d, Recurso: %s\n", recurso->PID, recurso->nombre_recurso);
        elemento = elemento->next;
    }
}