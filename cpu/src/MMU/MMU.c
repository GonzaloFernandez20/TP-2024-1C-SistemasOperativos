#include <MMU/MMU.h>

/**
 * @brief Traduce una dirección lógica a dirección física.
 * 
 * número_página = floor(dirección_lógica / tamaño_página)
 * desplazamiento = dirección_lógica - número_página * tamaño_página
 * 
*/
int traducir_DL_a_DF(uint32_t direccion_logica) {
    size_t tamanio_pagina = TAM_PAGINA;  

    int nro_pagina = direccion_logica / tamanio_pagina;       // en C, una división entre Enteros resulta en un Entero, asi que no es necesario usar floor().
    int desplazamiento = direccion_logica % tamanio_pagina;   // el resto de la division 
    
    // Debo ver qué marco le corresponde al nro_pagina.
    int nro_marco = obtener_marco(nro_pagina);

    int direccion_fisica = nro_marco * tamanio_pagina + desplazamiento;  // el tamaño del marco es igual al tamaño de página, y el desplazamiento se mantiene igual

    return direccion_fisica;
}

/**
 * @brief 
 * Para obtener el marco correspondiente al nro de página dado,
 * se consulta a la TLB, si no está entonces se va a buscar a
 * la Tabla de Páginas del proceso actual.
 * 
 * */ 
int obtener_marco(int nro_pagina) {
    int nro_marco;

    tlb_res res = consultar_marco_en_TLB(nro_pagina, &nro_marco);

    if (res == MISS) { // si no se encuentra en la TLB, entonces busco en la TP del PID actual en Memoria
        nro_marco = consultar_marco_en_TP(nro_pagina);
        agregar_a_TLB(nro_pagina, nro_marco);
    }

    log_info(cpu_log, "PID: %d - OBTENER MARCO - Página: %d - Marco: %d", PID, nro_pagina, nro_marco);

    return nro_marco;
}
// ARMA UNA LISTA DE LAS DIRECCIONES FISICAS NECESARIAS CON LA CANTIDAD DE BYTES QUE VA EN CADA UNA PARA LA OPERACION QUE SOLICITE
void traducir_direcciones(int tamanio, uint32_t direccion_logica){ 
    int bytes_restantes = tamanio; // INICIALMENTE TODOS
    int cant_paginas = 0;
    int bytes_consumidos = 0;

    int direccion_fisica = traducir_DL_a_DF(direccion_logica);  // 1RA DIRECCION FISICA

    if (direccion_fisica % TAM_PAGINA != 0){ // SI HAY DESPLAZAMIENTO EN LA 1RA PAGINA A LEER O ESCRIBIR
        cant_paginas++;
        int bytes_disponibles = TAM_PAGINA - (direccion_fisica % TAM_PAGINA);
        if (tamanio >= bytes_disponibles){
            bytes_consumidos = bytes_disponibles;
        }else{
            bytes_consumidos = tamanio;
        }
        bytes_restantes -= bytes_consumidos; // LE SACO LOS BYTES QUE ESCRIBO EN LA PRIMER PAGINA
        agregar_direccion(direccion_fisica, bytes_consumidos);
    }

    while (bytes_restantes > 0){

        direccion_fisica = traducir_DL_a_DF((uint32_t)cant_paginas * TAM_PAGINA);

        if ((bytes_restantes - TAM_PAGINA) >= 0){
            agregar_direccion(TAM_PAGINA, direccion_fisica);
            cant_paginas++;
            bytes_restantes -= TAM_PAGINA;
        }else{
            agregar_direccion(bytes_restantes, direccion_fisica);
            cant_paginas++;
            bytes_restantes -= bytes_restantes;
        }   
    }
}


void agregar_direccion(int bytes, int direccion){
    t_datos_acceso *nueva_direccion = malloc(sizeof(t_datos_acceso));
    nueva_direccion->bytes = bytes;
    nueva_direccion->direccion_fisica = direccion;

    list_add(direcciones, nueva_direccion);
}