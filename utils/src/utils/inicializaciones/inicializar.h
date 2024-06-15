#ifndef UTILS_INICIALIZAR_H_
#define UTILS_INICIALIZAR_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>



t_log* iniciar_logger(char* archivo_Log, char *modulo, bool consola_activa, t_log_level level);

t_config* iniciar_config(char* path_config);

char* chequear_parametros(int argc, char* argv[]);

void inicializar_tail_logs(char* nombre_archivo_log, char* nombre_archivo_log_debug);

#endif
