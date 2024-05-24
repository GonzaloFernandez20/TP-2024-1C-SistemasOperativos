#include <include/main.h>

int main(int argc, char* argv[]) {
    
    path_config = chequear_parametros(argc, argv);

    init(); 
    iniciar_servidor_memoria();
    gestionar_conexiones_clientes();
 
    return 0;
}

