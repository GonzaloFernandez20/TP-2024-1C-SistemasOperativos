#ifndef UTILS_INICIALIZAR_H_
#define UTILS_INICIALIZAR_H_

#include <stdlib.h>
#include <stdio.h>
#include<commons/log.h>
#include<commons/config.h>


/****************** Crea una intancia de logger chequeando el valor de retorno **************/
t_log* iniciar_logger(char* archivo_Log, char *modulo, bool consola_activa, t_log_level level);

/****************** Crea una intancia de config chequeando el valor de retorno **************/
t_config* iniciar_config(char* path_config);


#endif
