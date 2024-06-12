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
    if (list_size(entradas_TLB_diccionario) == MAX_ENTRADAS) { // si alcanzó el Máximo de Entradas , entonces hay que sustituir alguna de las páginas
        return;
    }
    
    return;
}

void _agregar_nueva_entrada(int pid, uint32_t nro_pagina, uint32_t nro_marco) {
    t_entrada_tlb entrada;
    entrada.pid = PID; // le asignamos el PID actual
    entrada.nro_pagina = nro_pagina;
    entrada.nro_marco = nro_marco;

    list_add(TLB, (void*)entrada);
}


// t_entrada_tlb _obtener_entrada_donde(int pid, uint32_t nro_pagina) {
    
// }
void _eliminar_entrada_donde(int pid, uint32_t nro_pagina) {
    
    
    return;
}

bool buscar_entrada_tlb(t_entrada_tlb *entrada, size_t *indice, int pid, uint32_t nro_pagina) {
    for(; i<MAX_ENTRADAS; i++) {
        t_entrada_tlb elemento = list_get(tabla_tlb, i);
        if(elemento.pid == pid && elemento.nro_pagina == nro_pagina) {
            entrada = elemento;
            return 1; // 1 significa que todo bien
        }
    }
    return 0; // 0 significa ERROR
}

/**
 * @brief Se consulta la TLB para ver si la página tiene un marco asignado.
 * 
 * @param nro_pagina El número de página del cual se quiere saber su marco.
 * @param El número de marco en el cual está la página pasada por parámetro.
 * @return *res Espera la direccion de una variable tlb_res en donde se almacenará la respuesta (HIT o MISS) de la consulta a la TLB.
 * 
*/
tlb_res consultar_marco_en_TLB(uint32_t nro_pagina, uint32_t* nro_marco) {
    size_t indice = 0;
    t_entrada_tlb entrada;
    int ok = buscar_entrada_tlb(&entrada, &indice, PID, 1);

    // SI LA ENCUENTRA
    if(ok) {
       nro_marco = entrada.nro_marco;
       return HIT;
    }

    // SI NO LA ENCUENTRA
    
    return MISS;
}

