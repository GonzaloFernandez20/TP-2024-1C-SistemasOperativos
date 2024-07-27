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
    if (list_size(tabla_tlb) == MAX_ENTRADAS) { // si alcanzó el Máximo de Entradas , entonces hay que sacrificar a alguna de las entradas (independientemente del PID) para dar lugar a la nueva entrada.
        list_remove_and_destroy_element(tabla_tlb, 0, free);
        // SI EL ALGORITMO ES FIFO: TOMO EL PRIMERO DE LA LISTA, QUE FUE EL QUE PRIMERO PUSE.
        // SI EL ALGORITMO ES LRU, TOMO EL PRIMERO DE LA LISTA PORQUE ANTE CADA REFERENCIA, SE REMUEVE EL ELEMENTO Y SE LO INSERTA AL FINAL, POR ENDE EL ULTIMO ELEMENTO DE LA LISTA SIEMPRE ES EL MAS RECIENTEMENTE REFERENCIADO Y EL PRIMERO ES EL MAS LEJANO, PORQUE SIGNIFICA QUE NO SE SACO HACE MUCHO DE LA LISTA, POR ENDE HACE MUCHO NO SE REFERENCIA. 
        // LIST_ADD AGREGA SIEMPRE AL FINAL DE LA LISTA, POR ESO. 
    }
    _agregar_nueva_entrada(nro_pagina, nro_marco);
}

// imposible que le llegue una entrada que ya existía en tabla, xq debía pasar por la busqueda en tlb antes de llegar a este punto, que es después de haber buscado en la TP en Memoria.
void _agregar_nueva_entrada(int nro_pagina, int nro_marco) {

    t_entrada_tlb* entrada_nueva = malloc(sizeof(t_entrada_tlb));
    entrada_nueva->pid = PID;
    entrada_nueva->nro_pagina = nro_pagina;
    entrada_nueva->nro_marco = nro_marco;

    list_add(tabla_tlb, entrada_nueva);
}

respuesta_busqueda _buscar_entrada_tlb(int *marco, int nro_pagina) {
    int cant_entradas = list_size(tabla_tlb);    
    
    // SI LA ENCUENTRA, IMPLICA QUE LA REFERENCIA, SI EL ALGORITMO ES LRU, ENTONCES LA REFERENCIA QUEDA COMO CABEZA DE LISTA.
    for(int i = 0; i < cant_entradas; i++) {                                // ITERAR SOBRE TODAS LAS ENTRADAS...
        
        t_entrada_tlb* entrada = list_get(tabla_tlb, i);                    // OBTENEMOS LA ENTRADA 
        if(entrada->pid == PID && entrada->nro_pagina == nro_pagina) {      // SI SU PID COINCIDE CON LA DEL PROCESO EN EJECUCIÓN Y SU NRO_PAGINA COINCIDE CON LA BUSCADA
            *marco = entrada->nro_marco;                                    // GUARDAMOS EL MARCO CORRESPONDIENTE AL NRO_PAGINA BUSCADO
            if (algoritmo_es_LRU()){                                        // SI SE USA LRU, ENTONCES ACTUALIZAMOS LA TABLA_TLB PORQUE HUBO UNA REFERENCIA
                list_remove(tabla_tlb, i);                                  
                list_add(tabla_tlb, entrada);                               // PONGO LA ENTRADA AL FINAL DE LA LISTA
            }
            return SEARCH_OK;
        }
    }
    return SEARCH_ERROR;                                                    // ITERÓ SOBRE TODAS LAS ENTRADAS PERO NO ENCONTRÓ.
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
    // SI LA ENCUENTRA, IMPLICA QUE LA REFERENCIA, SI EL ALGORITMO ES LRU, ENTONCES LA REFERENCIA QUEDA COMO CABEZA DE LISTA.
    if(respuesta == SEARCH_OK) {
       log_info(cpu_log, "PID: %d - TLB HIT - Pagina: %d", PID, nro_pagina);
       return HIT;
    }
    // SI NO LA ENCUENTRA
    log_info(cpu_log, "PID: %d - TLB MISS - Pagina: %d", PID, nro_pagina);
    return MISS;
}


int algoritmo_es_LRU(void){
    return string_equals_ignore_case(ALGORITMO_TLB, LRU);
}


