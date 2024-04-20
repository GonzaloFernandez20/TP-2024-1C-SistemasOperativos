#include <include/main.h>

int main(int argc, char* argv[]) {
    path_config = chequear_parametros(argc, argv);

    init(); 
    iniciar_servidor_memoria();
    gestionar_conexiones_clientes();

    return 0;
}


int _deshacer_casting(void *fd_cpu_casteado){ //! Esta funcion deberia ir en utils, porque es generica, si es que se va a usar en otro lado

    int *dato_casteado = (int *)fd_cpu_casteado; //? Convierto el puntero a void en un puntero a int con un nuevo casting
    int dato_sin_casting = *dato_casteado; //? Desreferencio para obtener el valor original

    return dato_sin_casting; //? Devuelvo dicho valor original 
}

