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
}

int existe_recurso(char* nombre_recurso){
    return dictionary_has_key(recursos_disponibles, nombre_recurso);
}

/*
retomar_ejecucion(pcb_execute){
    t_algoritmo algoritmo_planificacion = _chequear_algoritmo();
    
    switch (algoritmo_planificacion){
        case FIFO:
            enviar_contexto_ejecucion(pcb_execute);
            recibir_contexto_ejecucion(pcb_execute);
            break;
        default:
            enviar_contexto_ejecucion(pcb_execute);
                int *pidsito = malloc(sizeof(int));
                *pidsito = pcb_execute->pid;

                pthread_create(&manejo_quantum, NULL, (void *)iniciar_quantum, (void *)pidsito);
                pthread_detach(manejo_quantum);

                recibir_contexto_ejecucion(pcb_execute);
            break;
    }
}
*/
