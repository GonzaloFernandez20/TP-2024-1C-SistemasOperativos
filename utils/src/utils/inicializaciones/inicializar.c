#include <utils/inicializaciones/inicializar.h>


t_log* iniciar_logger(char* archivo_Log, char *modulo, bool consola_activa, t_log_level level){
	
    t_log* nuevo_logger = log_create(archivo_Log, modulo, consola_activa, level);	

	if(nuevo_logger == NULL){
		fprintf(stderr,"Error: No se pudo crear el logger de %s\n",modulo);
		exit(EXIT_FAILURE);
	}

	return nuevo_logger;
}

t_config* iniciar_config(char* path_config){
	t_config* nuevo_config = config_create(path_config);
	
	if(nuevo_config == NULL){
		perror("Error: No se pudo crear el config\n");
		exit(EXIT_FAILURE);
	}

	return nuevo_config;
}