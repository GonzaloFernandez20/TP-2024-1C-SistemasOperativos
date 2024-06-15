#include <utils/inicializaciones/inicializar.h>


t_log* iniciar_logger(char* archivo_Log, char *modulo, bool consola_activa, t_log_level level){
	
    t_log* nuevo_logger = log_create(archivo_Log, modulo, consola_activa, level);	

	if(nuevo_logger == NULL){
		fprintf(stderr,"Error: No se pudo crear el logger de %s\n",modulo);
		exit(EXIT_FAILURE);
	}

	return nuevo_logger;
}

char* chequear_parametros(int argc, char* argv[]){ 
    if(argc < 2) {
        printf("Faltan parametros de entrada\n");
    }
    char* path_config = strdup(argv[1]);    // recordar liberar el espacio de memoria
    
    return path_config;
}

t_config* iniciar_config(char* path_config){
	t_config* nuevo_config = config_create(path_config);
	
	if(nuevo_config == NULL){
		perror("Error: No se pudo crear el config\n");
		exit(EXIT_FAILURE);
	}

	return nuevo_config;
}

void inicializar_tail_logs(char* nombre_archivo_log, char* nombre_archivo_log_debug) {
	
    // char* cmd_touch = string_from_format("touch %s ; touch %s", nombre_archivo_log, nombre_archivo_log_debug);
	char* cmd_limpiar_logs_pasados = string_from_format("cat /dev/null > %s ; cat /dev/null > %s", nombre_archivo_log, nombre_archivo_log_debug);
    char* cmd_tail_logs_in_new_terminal = string_from_format("terminator -x tail -f %s %s & > /dev/null", nombre_archivo_log, nombre_archivo_log_debug);

    system(cmd_limpiar_logs_pasados);
    system(cmd_tail_logs_in_new_terminal);
}