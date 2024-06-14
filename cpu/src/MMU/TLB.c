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
void agregar_a_TLB(uint32_t nro_pagina, uint32_t nro_marco) {
    if (list_size(entradas_TLB_diccionario) == MAX_ENTRADAS) { // si alcanzó el Máximo de Entradas , entonces hay que sustituir alguna de las entradas (independientemente del PID).
        _reemplazar_una_entrada_por(PID, nro_pagina, nro_marco);
    }
    _agregar_nueva_entrada(PID, nro_pagina, nro_marco);
}

void _reemplazar_una_entrada_por(int pid, uint32_t nro_pagina, uint32_t nro_marco) {
    size_t indice = _elegir_entrada_victima();
    list_remove(tabla_tlb, indice);
}

size_t _elegir_entrada_victima(void) {



    return 1;
}

void _agregar_nueva_entrada(int pid, uint32_t nro_pagina, uint32_t nro_marco) {
    t_entrada_tlb entrada;
    entrada.pid = PID; // le asignamos el PID actual
    entrada.nro_pagina = nro_pagina;
    entrada.nro_marco = nro_marco;

    list_add(tabla_tlb, (void*)entrada);
}


// t_entrada_tlb _obtener_entrada_donde(int pid, uint32_t nro_pagina) {
    
// }
void _eliminar_entrada_donde(int pid, uint32_t nro_pagina) {
    size_t i = 0;
    t_entrada_tlb entrada;
    res_busqueda res = buscar_entrada_tlb(&entrada, &indice, pid, nro_pagina) {
    
    list_remove(tabla_tlb, indice);
    return;
}

res_busqueda buscar_entrada_tlb(t_entrada_tlb *entrada, size_t *indice, int pid, uint32_t nro_pagina) {
    for(; indice<MAX_ENTRADAS; indice++) {
        t_entrada_tlb elemento = list_get(tabla_tlb, i);
        if(elemento.pid == pid && elemento.nro_pagina == nro_pagina) {
            entrada = elemento;
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
tlb_res consultar_marco_en_TLB(uint32_t nro_pagina, uint32_t* nro_marco) {
    size_t indice = 0; // empezar desde el indice 0 a buscar
    t_entrada_tlb entrada;
    res_busqueda res = buscar_entrada_tlb(&entrada, &indice, PID, nro_pagina);

    // SI LA ENCUENTRA
    if(res == SEARCH_OK) {
       *nro_marco = entrada.nro_marco;
       log_info(logger, "PID: %d - TLB HIT - Pagina: %d", PID, nro_pagina);
       return HIT;
    }

    // SI NO LA ENCUENTRA
    log_info(logger, "PID: %d - TLB MISS - Pagina: %d", PID, nro_pagina);
    return MISS;
}

