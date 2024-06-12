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
    
    // Debo ver qué marco le corresponde al nro_pagina. Hago una consulta a la Tabla de Páginas (TP).
    uint32_t marco = obtener_marco(nro_pagina);



    return 0;
}

/**
 * @brief 
 * Primero se consulta a la TLB, si no está entonces se va a buscar a
 * la Tabla de Páginas del proceso actual (apuntado por la PTBR) qué marco se encuentra el nro_pagina pasado.
 * 
 * 
 * 
 * */ 
uint32_t obtener_marco(uint32_t nro_pagina) {
    tlb_res resultado_tlb;    
    uint32_t nro_marco;
    
    resultado_tlb = consultar_marco_en_TLB(uint32_t nro_pagina, &nro_marco);
    if (resultado_tlb == MISS) {
        // se deberá informar el TLB Miss y se deberá consultar a la memoria para obtener el frame correspondiente a la página buscada.
        uint32_t nro_marco = consultar_marco_en_TP(nro_pagina);
    }

    return nro_marco;
}

uint32_t consultar_marco_en_TP() {
    // pedirle a Memoria que me traiga el marco en el cual está la página.
    uint32_t nro_marco = pedir_marco_a_Memoria(nro_pagina); // FALTA IMPLEMENTARR

    


    return 1;
}