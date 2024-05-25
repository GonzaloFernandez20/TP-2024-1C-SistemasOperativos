#include <include/main.h>

int main(int argc, char *argv[]){

    path_config = chequear_parametros(argc, argv);

    init(); 
    iniciar_servidor_kernel();
    establecer_conexiones();
    esperar_dispositivos_IO();
    iniciar_planificacion();

    iniciar_Consola();

    return 0;
}


