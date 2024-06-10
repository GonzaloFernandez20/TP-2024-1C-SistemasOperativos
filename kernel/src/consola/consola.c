#include <consola/consola.h>

int planificacion_pausada = false; // ESTA ME SIRVE PARA SABER EL ESTADO DE LA PLANIFICACION

void *iniciar_Consola(){

    pthread_mutex_lock(&mutex_log_debug);
        log_info(kernel_log_debugg, "Iniciando consola...");
    pthread_mutex_unlock(&mutex_log_debug);
    
    //int planificacion_iniciada = 1; // ESTA ME SIRVE PARA SABER EL ESTADO DE LA PLANIFICACION
    char* comando_ingresado;

	do{ 
		comando_ingresado = readline("> ");
        char **array_del_comando = _interpretar(comando_ingresado);

        t_comandos_consola opCode_Comando_Ingresado = _obtener_enum_comando(array_del_comando[0]);

        switch (opCode_Comando_Ingresado)
        {
            case EJECUTAR_SCRIPT: // EJECUTAR_SCRIPT [PATH]
                printf("Ejecutando comando: EJECUTAR_SCRIPT\n");
                // DEBERIA LEER UN ARCHIV0 DE COMANDOS, GUARDARLAS EN UN ARRAY E IR EJECUTANDOLAS 1 A 1 
                break;

            case INICIAR_PROCESO: // INICIAR_PROCESO [PATH]

                char * path_proceso = array_del_comando[1];
                crear_proceso(path_proceso);
                free(path_proceso);
                break;

            case FINALIZAR_PROCESO: // FINALIZAR_PROCESO [PID]

                int pid_proceso = atoi(array_del_comando[1]);
                extraer_proceso(pid_proceso);
                
                break;

            case DETENER_PLANIFICACION:
                //pausar_planificacion();
                break;

            case INICIAR_PLANIFICACION:
                if (planificacion_pausada)
                {
                    //retomar_planificacion();
                }            
                break;

            case MULTIPROGRAMACION: // MULTIPROGRAMACION [VALOR]
                /* EJECUCION CORRESPONDIENTE, LOS PARAMETROS LOS SACO DEL ARRAY DEL COMANDO */
                break;

            case PROCESO_ESTADO:
                _imprimir_estados_procesos();

                break;
            default:
                printf("El comando que ingresaste no es valido\n");
                break;
            }

		free(comando_ingresado);
        free(array_del_comando);
	} while(1);
}


// ------------------------ FUNCIONES AUXILIARES

t_comandos_consola _obtener_enum_comando(char *comando_ingresado){

    if(string_equals_ignore_case(comando_ingresado, "EJECUTAR_SCRIPT"))       /* --> */   { return EJECUTAR_SCRIPT; }
    if(string_equals_ignore_case(comando_ingresado, "INICIAR_PROCESO"))       /* --> */   { return INICIAR_PROCESO; }
    if(string_equals_ignore_case(comando_ingresado, "FINALIZAR_PROCESO"))     /* --> */   { return FINALIZAR_PROCESO; }
    if(string_equals_ignore_case(comando_ingresado, "DETENER_PLANIFICACION")) /* --> */   { return DETENER_PLANIFICACION; }
    if(string_equals_ignore_case(comando_ingresado, "INICIAR_PLANIFICACION")) /* --> */   { return INICIAR_PLANIFICACION; }
    if(string_equals_ignore_case(comando_ingresado, "MULTIPROGRAMACION"))     /* --> */   { return MULTIPROGRAMACION; }
    if(string_equals_ignore_case(comando_ingresado, "PROCESO_ESTADO"))        /* --> */   { return PROCESO_ESTADO; }
 
    return COMANDO_NO_RECONOCIDO;
}

char **_interpretar(char *comando){
    return string_split(comando, " ");
}
