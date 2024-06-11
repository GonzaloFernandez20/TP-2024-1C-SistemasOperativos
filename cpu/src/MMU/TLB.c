#include <MMU/TLB.h>



/** 
 * @brief Inicializamos un dictionary de la commons para guardar las estructuras [ pid | página | marco ].
 * Obtendremos un marco según una llave con el formato "<pid> <nro_pagina>"
*/
void inicializar_TLB(void) {
    entradas_TLB_diccionario = dictionary_create();
    entradas_TLB_diccionario.table_max_size = MAX_ENTRADAS;
    return;
}

/**
 * @brief Guardamos número de página, número de marco y PID en el diccionario de la TLB.
*/
void agregar_a_TLB(uint32_t nro_pagina, uint32_t nro_marco) {
    if (dictionary_size(entradas_TLB_diccionario) == MAX_ENTRADAS) { // si alcanzó el Máximo de Entradas , entonces hay que sustituir alguna de las páginas

    }
    


}

void _agregar_nueva_entrada(int pid, uint32_t nro_pagina, uint32_t nro_marco) {
    dictionary_put(entradas_TLB_diccionario, key(pid, nro_pagina), nro_marco);
}
char* _obtener_entrada() {
    
}
void _eliminar_entrada(int pid, uint32_t nro_pagina) {
    dictionary_remove(entradas_TLB_diccionario, key(pid, nro_pagina));
}

// Para generar el string que usaremos como llave para acceder al diccionario de la TLB
char* key(int pid, int nro_pagina) {
    char* key = string_from_format("%d %d", pid, nro_pagina); // las llaves del diccionario TLB van a tener el formato "<pid> <nro_pagina>".
    char* llave;          
    strcpy(llave, key);  // hago esta cosa rara para despreocuparme de liberar la memoria fuera de esta función. En teoría strcpy() retorna un string literal (?)
    free(key);
    return llave;
}

/**
 * @brief Se consulta la TLB para ver si la página tiene un marco asignado.
 * 
 * @param nro_pagina El número de página del cual se quiere saber su marco.
 * @param *res Espera la direccion de una variable tlb_res en donde se almacenará la respuesta (HIT o MISS) de la consulta a la TLB.
 * 
 * @return El número de marco en el cual está la página pasada por parámetro.
*/
uint32_t consultar_marco_en_TLB(uint32_t nro_pagina, tlb_res *res) {
    dictionary_get(entradas_TLB_diccionario, key(PID, nro_pagina));

    return ;
}

