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

    printf("Agregada instancia del recurso: %s a PID: %d \n", nuevo_recurso->nombre_recurso, nuevo_recurso->PID);
}

void eliminar_instancia_recurso(int PID, t_recurso *recurso){
    pthread_mutex_lock(&cola_recursos_usados);
    int cant_instancias = list_size(recursos_usados);

    for (int i = 0; i < cant_instancias; i++){
        t_recurso_usado *recurso_usado = list_get(recursos_usados, i);
    
        if (PID == recurso_usado->PID){   
            list_remove(recursos_usados, i);
            pthread_mutex_unlock(&cola_recursos_usados);
            printf("Eliminado el recurso: %s de PID: %d \n", recurso_usado->nombre_recurso, recurso_usado->PID);
            break;
        }
    }   
}

void liberar_recursos(int PID){
    pthread_mutex_lock(&cola_recursos_usados);
    int cant_instancias = list_size(recursos_usados);

    for (int i = 0; i < cant_instancias; i++){
        t_recurso_usado *recurso_usado = list_get(recursos_usados, i);
    
        if (PID == recurso_usado->PID){   
            list_remove(recursos_usados, i);
            t_recurso *recurso = dictionary_get(recursos_disponibles, recurso_usado->nombre_recurso);
            recurso->instancias_recursos++;            
        }
    }  
    pthread_mutex_unlock(&cola_recursos_usados);
}


/* void retomar_ejecucion(pcb_execute){
    t_algoritmo algoritmo_planificacion = _chequear_algoritmo();
    
    switch (algoritmo_planificacion){
        case FIFO:
            recibir_contexto_ejecucion(pcb_execute);
            break;
        default:
            // EL QUANTUM NO SE RESETEA -> SI MANDA LA INTERRUPCION LA MANDA Y EL PROCESO DESALOJA.
            recibir_contexto_ejecucion(pcb_execute);
        break;
    }
} */

