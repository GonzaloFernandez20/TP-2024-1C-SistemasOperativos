#include <include/main.h>

int main(int argc, char* argv[]) {
    
    chequear_parametros_inicio_interfaz(argc, argv);
    init();
    establecer_conexiones();
    atender_peticiones_kernel();

    return 0;
}

