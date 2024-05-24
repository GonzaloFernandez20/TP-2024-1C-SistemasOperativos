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
            /* EJECUCION CORRESPONDIENTE, LOS PARAMETROS LOS SACO DEL ARRAY DEL COMANDO */
            break;

        case INICIAR_PROCESO: // INICIAR_PROCESO [PATH]

            char * path_proceso = array_del_comando[1];
            
            crear_proceso(path_proceso);

            free(path_proceso);
            break;

        case FINALIZAR_PROCESO: // FINALIZAR_PROCESO [PID]
            printf("Ejecutando comando: FINALIZAR_PROCESO\n");
            
            int pid_proceso = atoi(array_del_comando[1]);

            //pthread_mutex_lock(SEMAFORO DE PROCESOS);
	        //log_info(kernel_log, "Finaliza el proceso <%d> - Motivo: <INTERRUPTED_BY_USER>", pid_proceso);
	        //pthread_mutex_unlock(SEMAFORO DE PROCESOS);

            extraer_proceso(pid_proceso);

            break;

        case DETENER_PLANIFICACION:
            printf("Ejecutando comando: DETENER_PLANIFICACION\n");
            //pausar_planificacion();
            break;

        case INICIAR_PLANIFICACION:
            printf("Ejecutando comando: INICIAR_PLANIFICACION\n");
            if (planificacion_pausada)
            {
                //retomar_planificacion();
            }            
            break;

        case MULTIPROGRAMACION: // MULTIPROGRAMACION [VALOR]
            printf("Ejecutando comando: MULTIPROGRAMACION\n");
            /* EJECUCION CORRESPONDIENTE, LOS PARAMETROS LOS SACO DEL ARRAY DEL COMANDO */
            break;

        case PROCESO_ESTADO:
            printf("Ejecutando comando: PROCESO_ESTADO\n");
 
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
/**
 * @brief Esta funcion recibe un comando por teclado y lo interpreta.
 *
 * La utilizamos para interpretar los comando que se ingresen, obteniendo su enum para luego utilizarlo en el 
 * switch principal de la consola, que sabe luego que operaciones tiene que ejecutar para cada comando.
 *
 * @param comando_ingresado Comando ingresado por teclado.
 * @return Enum correspondiente a dicho comando.
 */
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



/*
ENUNCIADO: 

Funciones por consola
? El Kernel dispondrá de una consola donde se permitirá la ejecución de los diferentes comandos que se especifican a continuación:

Ejecutar Script de Comandos: Se encargará de abrir un archivo de comandos que se encontrará en la máquina donde corra el Kernel, el mismo contendrá una secuencia de comandos y se los deberá ejecutar uno a uno hasta finalizar el archivo.
*Nomenclatura: EJECUTAR_SCRIPT [PATH]


Iniciar proceso: Se encargará de iniciar un nuevo proceso cuyas instrucciones a ejecutar se encontrarán en el archivo <path> dentro del file system de la máquina donde corra la Memoria. Este mensaje se encargará de la creación del proceso (PCB) en estado NEW.
*Nomenclatura: INICIAR_PROCESO [PATH]


Finalizar proceso: Se encargará de finalizar un proceso que se encuentre dentro del sistema. Este mensaje se encargará de realizar las mismas operaciones como si el proceso llegara a EXIT por sus caminos habituales (deberá liberar recursos, archivos y memoria).
*Nomenclatura: FINALIZAR_PROCESO [PID]


Detener planificación: Este mensaje se encargará de pausar la planificación de corto y largo plazo. El proceso que se encuentra en ejecución NO es desalojado, pero una vez que salga de EXEC se va a pausar el manejo de su motivo de desalojo. De la misma forma, los procesos bloqueados van a pausar su transición a la cola de Ready.
*Nomenclatura: DETENER_PLANIFICACION


Iniciar planificación: Este mensaje se encargará de retomar (en caso que se encuentre pausada) la planificación de corto y largo plazo. En caso que la planificación no se encuentre pausada, se debe ignorar el mensaje.
*Nomenclatura: INICIAR_PLANIFICACION


Modificar el grado de multiprogramación del módulo: Se cambiará el grado de multiprogramación del sistema reemplazandolo por el valor indicado.
*Nomenclatura: MULTIPROGRAMACION [VALOR]

Listar procesos por estado: Se encargará de mostrar por consola el listado de los estados con los procesos que se encuentran dentro de cada uno de ellos.
*Nomenclatura: PROCESO_ESTADO

!Se debe tener en cuenta que frente a un fallo en la escritura de un comando en consola el sistema debe permanecer estable sin reacción alguna.
En caso de que se tengan más procesos ejecutando que lo que permite el grado de multiprogramación, no se tomarán acciones sobre los mismos y se esperará su finalización normal.

*/