#include <MMU/TLB.h>

/** 
 * @brief Inicializamos un dictionary de la commons para guardar las estructuras [ pid | página | marco ].
 * Obtendremos un marco según una llave con el formato "<pid> <nro_pagina>"
*/
void inicializar_TLB(void) {
    MAX_ENTRADAS = config_cpu.CANTIDAD_ENTRADAS_TLB;
    ALGORITMO_TLB = config_cpu.ALGORITMO_TLB;

    tabla_tlb = list_create();
}

/**
 * @brief Guardamos número de página, número de marco y PID en el diccionario de la TLB.
*/
void agregar_a_TLB(int nro_pagina, int nro_marco) {
    if (list_size(tabla_tlb) == MAX_ENTRADAS) { // si alcanzó el Máximo de Entradas , entonces hay que sustituir alguna de las entradas (independientemente del PID).
        _eliminar_una_entrada_con_algoritmo();
    }
    _agregar_nueva_entrada(nro_pagina, nro_marco);
}

void _eliminar_una_entrada_con_algoritmo(void){

    if(string_equals_ignore_case(ALGORITMO_TLB, FIFO))  // saco el primero que se metió a la lista, osea el que está en indice 0
    {
        t_entrada_tlb* entrada_remover = list_remove(tabla_tlb, 0);
        free(entrada_remover);
    }
    else if(string_equals_ignore_case(ALGORITMO_TLB, LRU))  // saco la entrada menos consultada.
    {
        t_entrada_tlb* entrada_menos_consultada = list_get_minimum(tabla_tlb, menos_consultado);
        int indice = entrada_menos_consultada->index_in_table;
        list_remove_and_destroy_element(tabla_tlb, indice, free);
    }
    else {
        log_error(cpu_log_debug, "Algoritmo de sustitución de TLB inválido.");
    }
}

void* menos_consultado(void* entrada1, void* entrada2) {

    size_t usos1 = ((t_entrada_tlb*)entrada1)->usos;
    size_t usos2 = ((t_entrada_tlb*)entrada2)->usos;

    if (usos1 <= usos2) {
        return entrada1;  
    } 
    return entrada2;
}


// imposible que le llegue una entrada que ya existía en tabla, xq debía pasar por la busqueda en tlb antes de llegar a este punto, que es después de haber buscado en la TP en Memoria.
void _agregar_nueva_entrada(int nro_pagina, int nro_marco) {

    t_entrada_tlb* entrada_nueva = malloc(sizeof(t_entrada_tlb));
    entrada_nueva->pid = PID;
    entrada_nueva->nro_pagina = nro_pagina;
    entrada_nueva->nro_marco = nro_marco;
    entrada_nueva->usos = 1;
    entrada_nueva->index_in_table = list_size(tabla_tlb);

    list_add(tabla_tlb, entrada_nueva);
}

respuesta_busqueda _buscar_entrada_tlb(int *marco, int nro_pagina) {
    int cant_entradas = list_size(tabla_tlb);    

    for(int i = 0; i < cant_entradas; i++) {
        t_entrada_tlb* entrada = list_get(tabla_tlb, i);
        if(entrada->pid == PID && entrada->nro_pagina == nro_pagina) {
            *marco = entrada->nro_marco;
            return SEARCH_OK;
        }
    }
    return SEARCH_ERROR;
}

/**
 * @brief Se consulta la TLB para ver si la página dada del proceso actual tiene un marco asignado.
 * 
 * @param nro_pagina El número de página del cual se quiere saber su marco.
 * @param nro_marco Un puntero a una variable en donde se guardará el nro de marco encontrado.
 * 
*/
tlb_respuesta consultar_marco_en_TLB(int nro_pagina, int *nro_marco) {
    respuesta_busqueda respuesta = _buscar_entrada_tlb(nro_marco, nro_pagina);
    // SI LA ENCUENTRA
    if(respuesta == SEARCH_OK) {
       log_info(cpu_log, "PID: %d - TLB HIT - Pagina: %d", PID, nro_pagina);
       return HIT;
    }
    // SI NO LA ENCUENTRA
    log_info(cpu_log, "PID: %d - TLB MISS - Pagina: %d", PID, nro_pagina);
    return MISS;
}


