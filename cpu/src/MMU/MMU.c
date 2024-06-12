#include <MMU/MMU.h>

/**
 * @brief Traduce una dirección lógica a dirección física.
 * 
 * número_página = floor(dirección_lógica / tamaño_página)
 * desplazamiento = dirección_lógica - número_página * tamaño_página
 * 
 * df = número_página
*/
uint32_t dl_a_df(uint32_t direccion_logica) {
    size_t tamanio_pagina = config_memoria.TAM_PAGINA;

    uint32_t nro_pagina = direccion_logica / tamanio_pagina;       // en C, una división entre Enteros resulta en un Entero, asi que no es necesario usar floor().
    uint32_t desplazamiento = direccion_logica % tamanio_pagina;   // el resto de la division 
    
    // Debo ver qué marco le corresponde al nro_pagina.
    uint32_t nro_marco = obtener_marco_de(nro_pagina);

    uint32_t direccion_fisica = nro_marco * tamanio_pagina + desplazamiento  // el tamaño del marco es igual al tamaño de página, y el desplazamiento se mantiene igual

    return direccion_fisica;
}

/**
 * @brief 
 * Para obtener el marco correspondiente al nro de página dado,
 * se consulta a la TLB, si no está entonces se va a buscar a
 * la Tabla de Páginas del proceso actual.
 * 
 * */ 
uint32_t obtener_marco_de(uint32_t nro_pagina) {
    log_info(logger, "PID: %d - OBTENER MARCO - Página: %d - Marco: %d", PID, nro_pagina, nro_marco);
    
    tlb_res res;
    
    uint32_t nro_marco = consultar_marco_en_TLB(nro_pagina, &res);

    if (res == MISS) { // si no se encuentra en la TLB, entonces busco en la TP del PID actual en Memoria
        uint32_t nro_marco = consultar_marco_en_TP(nro_pagina);
        agregar_a_TLB(nro_pagina, nro_marco);
    }

    return nro_marco;
}