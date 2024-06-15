#include <esquemaEstructuras/paginacion.h>

int buscar_PID_tablas_paginas(int PID_buscado){

    int indice_proceso;

    pthread_mutex_lock(&mutex_tablas_de_paginas);
    int size = list_size(tablas_de_paginas);
    
    for (int i = 0; i < size; i++) {
        t_proceso* proceso = list_get(tablas_de_paginas, i);
        if (proceso-> PID == PID_buscado)
        {
            indice_proceso = i;
            break;
        }
    }
    pthread_mutex_unlock(&mutex_tablas_de_paginas);

    return indice_proceso;
}

int marcos_libres(void){
    int cantidad = 0;

    pthread_mutex_lock(&mutex_bitarray);
    for(int i = 0; i < cantidad_marcos; i++){
        if(!bitarray_test_bit(bitarray, i)){cantidad ++;}
    }
    pthread_mutex_unlock(&mutex_bitarray);

    return cantidad;
}

int buscar_marco_libre(void){
    int marco_libre;

    pthread_mutex_lock(&mutex_bitarray);
    for(int i = 0; i < cantidad_marcos; i++){
        if(!bitarray_test_bit(bitarray, i)){
          marco_libre = i;
          break;
        }
    }
    pthread_mutex_unlock(&mutex_bitarray);

    return marco_libre;
}

void liberar_marcos(int paginas_eliminar, t_list* tabla){
    int cantidad_paginas = list_size(tabla);
    
    for(int i = 0; i < paginas_eliminar; i++){

		t_pagina* pagina = list_remove(tabla, cantidad_paginas-i-1);

		pthread_mutex_lock(&mutex_bitarray);
			bitarray_clean_bit(bitarray, pagina->marco);
		pthread_mutex_unlock(&mutex_bitarray);

		free(pagina);
	}
}