#include <stdlib.h>
#include <stdio.h>
#include <utils/conexiones/clientes/clientes.h>


int conectar_con_CPU(void) {
    char* nombreArchivoConexionConCPU = "cpu.config"; 
    int conexion = conectar_con_modulo(nombreArchivoConexionConCPU);
    return conexion;
}
int conectar_con_MEMORIA(void) {
    char* nombreArchivoConexionConMEMORIA = "memoria.config"; 
    int conexion = conectar_con_modulo(nombreArchivoConexionConMEMORIA); 
    return conexion;
}
int conectar_con_KERNEL(void) {
    char* nombreArchivoConexionConKERNEL = "kernel.config"; 
    int conexion = conectar_con_modulo(nombreArchivoConexionConKERNEL);
    return conexion;
}


// Esta funcion la utilizamos para conectarnos (como clientes) a todos los demás módulos.
int conectar_con_modulo(char* nombreArchivoConfigConexion){  
    //Iniciar config
    t_config* config = iniciar_config(nombreArchivoConfigConexion);
    //Obteniendo los datos del config
    char* ip     = config_get_string_value(config,"IP");
    char* puerto = config_get_string_value(config, "PUERTO");
    char* clave  = config_get_string_value(config, "CLAVE");
    
    // Creamos conexion con el dispatcher
    int conexion = crear_conexion(ip,puerto);
    
    // Enviamos al servidor el valor de CLAVE como mensaje (handshake)
	enviar_mensaje(clave, conexion);
    
    // Armamos y enviamos el paquete
	// ! paquete(conexionDispatcher); 

    config_destroy(config);
}

// DATA DEL TP 0 va todo a otro archivo para no llenar el main de cosas

void escribir_log(t_log* log, t_log_level level, char* mensaje){
	switch (level)
	{
	case LOG_LEVEL_INFO:
		log_info(log, mensaje);
		break;
	case LOG_LEVEL_DEBUG:
		log_debug(log, mensaje);
		break;
	case LOG_LEVEL_ERROR:
		log_error(log, mensaje);
		break;
	case LOG_LEVEL_TRACE:
		log_trace(log, mensaje);
		break;
	case LOG_LEVEL_WARNING:
		log_warning(log, mensaje);
		break;
	default:
		printf("Pusiste un tipo de log incorrecto!");
		break;
	}
}

t_log* iniciar_logger(char* nombreDelArchivo, char* nombreDelProceso, bool estaLaConsolaActiva, t_log_level nivel)
{
	t_log* nuevo_logger = log_create(nombreDelArchivo,nombreDelProceso,estaLaConsolaActiva,nivel);

	return nuevo_logger;
}

t_config* iniciar_config(char *direccion)
{
	t_config* nuevo_config = config_create(direccion);

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	while (true) {
		leido = readline("> ");
		if (!strcmp(leido, "")) {
			break; // salimos del loop
		}
		log_info(logger, leido);
	}

	// ¡No te olvides de liberar las lineas antes de regresar!
	free(leido);
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
	while (true) {
		leido = readline("> ");
		if (!strcmp(leido, "")) {
			break; // salimos del loop
		}
		agregar_a_paquete(paquete, leido, strlen(leido) + 1);
	}

	// Enviamos el paquete
	enviar_paquete(paquete, conexion);

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	free(leido);
	eliminar_paquete(paquete);
}

void liberar_recursos(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	liberar_conexion(conexion);
	log_destroy(logger);
	config_destroy(config);
}
