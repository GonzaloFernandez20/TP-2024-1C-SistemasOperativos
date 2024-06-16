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
    if (list_size(tabla_tlb) == MAX_ENTRADAS) { // si alcanzó el Máximo de Entradas , entonces hay que sustituir alguna de las entradas (independientemente del PID).
        _eliminar_una_entrada_con_algoritmo();
    }
    
    _agregar_nueva_entrada(PID, nro_pagina, nro_marco);
}

// void liberar_entrada(t_entrada_tlb* entrada) {

//     free(entrada->pid);
//     free(entrada->nro_pagina);
//     free(entrada->nro_marco);
//     free(entrada->usos);
//     free(entrada);
// }

void _eliminar_una_entrada_con_algoritmo(void){

    if(!strcmp(ALGORITMO_TLB, FIFO))  // saco el primero que se metió a la lista, osea el que está en indice 0
    {
        int indice = 0;
        list_remove(tabla_tlb, indice);
    }
    else if(!strcmp(ALGORITMO_TLB, LRU))  // saco la entrada menos consultada.
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

// Malloc'ea t_entrada_tlb y le asigna valores iniciales a sus miembros
t_entrada_tlb* crear_entrada(int pid, uint32_t nro_pagina, uint32_t nro_marco){

    t_entrada_tlb* entrada = malloc(sizeof(t_entrada_tlb));
    entrada->pid = pid;
    entrada->nro_pagina = nro_pagina;
    entrada->nro_marco = nro_marco;
    entrada->usos = 0;
    return entrada;
}


// imposible que le llegue una entrada que ya existía en tabla, xq debía pasar por la busqueda en tlb antes de llegar a este punto, que es después de haber buscado en la TP en Memoria.
void _agregar_nueva_entrada(int pid, uint32_t nro_pagina, uint32_t nro_marco) {
    int indice = 0;
    t_entrada_tlb entrada;

    switch (_buscar_entrada_tlb(&entrada, &indice, PID, nro_pagina)) // buscamos si la [pid, nro_pagina] ya tiene un marco asignado.
    {
    case SEARCH_OK:
        // actualizo los valores de nro_marco
        entrada.nro_marco = nro_marco;
        return;

    case SEARCH_ERROR:
        t_entrada_tlb* entrada_nueva = crear_entrada(PID, nro_pagina, nro_marco);
        entrada_nueva->usos = 1;
        entrada_nueva->index_in_table = list_size(tabla_tlb);
        list_add(tabla_tlb, (void*)entrada_nueva);
        return;
    }
}


// res_tabla_tlb _eliminar_entrada_donde(int pid, uint32_t nro_pagina) {
//     size_t i = 0;
//     t_entrada_tlb entrada;
//     res_tabla_tlb res = buscar_entrada_tlb(&entrada, &indice, pid, nro_pagina) {
    
//     if (res == SEARCH_OK) {
//         list_remove(tabla_tlb, indice);
//         return REMOVE_OK;
//     } 
    
//     return REMOVE_ERROR;
// }

res_busqueda _buscar_entrada_tlb(t_entrada_tlb *ptr_entrada, int *ptr_indice, int pid, uint32_t nro_pagina) {
    for(; *ptr_indice<MAX_ENTRADAS; (*ptr_indice)++) {
        t_entrada_tlb* ptr_elemento = list_get(tabla_tlb, *ptr_indice);
        if(ptr_elemento->pid == pid && ptr_elemento->nro_pagina == nro_pagina) {
            ptr_entrada = ptr_elemento;   // el puntero "entrada" ahora apunta a lo mismo que el puntero "elemento". OJO CON MEMORY LEAKS
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
tlb_res consultar_marco_en_TLB(uint32_t nro_pagina, int *nro_marco) {

    int indice = 0; // empezar desde el indice 0 a buscar
    t_entrada_tlb entrada;
    res_busqueda res = _buscar_entrada_tlb(&entrada, &indice, PID, nro_pagina);
    // SI LA ENCUENTRA
    if(res == SEARCH_OK) {
       *nro_marco = entrada.nro_marco;
       log_info(cpu_log, "PID: %d - TLB HIT - Pagina: %d", PID, nro_pagina);
       return HIT;
    }
    // SI NO LA ENCUENTRA
    log_info(cpu_log, "PID: %d - TLB MISS - Pagina: %d", PID, nro_pagina);
    return MISS;
}


