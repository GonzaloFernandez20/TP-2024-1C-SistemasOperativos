#include <stdlib.h>
#include <stdio.h>
#include <utils/hello.h>
#include <utils/conexionClientes.c>

//CREACIÓN DEL CLIENTE (dispatcher)

int main(){
    int conexionDispatcherConCPU = conectar_con_CPU();
    int conexionInterruptConCPU  = conectar_con_CPU();

    int conexionMemoria = conectar_con_MEMORIA();
	// Armamos y enviamos el paquete
	// ! paquete(conexionDispatcherConCPU); 
    
    // Liberamos recursos
	//iberar_recursos(conexionDispatcher, logger, configDispatcher);
    return 0;
}


// prueba