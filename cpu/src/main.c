#include <include/main.h>

int main(int argc,char* argv[]) {

    path_config = chequear_parametros(argc, argv);

	init();
    iniciar_servidores();
    conectar_memoria();
    gestionar_conexiones_kernel(); 
    
	return 0;
}

