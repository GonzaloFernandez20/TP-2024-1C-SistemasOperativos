#include <inicializar/inicializar.h>

void init(){
    cpu_log = iniciar_logger("cpu.log", "CPU", 1, LOG_LEVEL_INFO);
    cpu_log_debug = iniciar_logger("debugg.log", "CPU", 1, LOG_LEVEL_DEBUG);
    _leer_configuracion(path_config);
    inicializar_semaforos();
    init_opCodes_dictionary();
    init_registros_dictionary();
}

void _leer_configuracion(char *path_config){
	t_config* nuevo_config = iniciar_config(path_config);

    config_cpu.IP_CPU = strdup(config_get_string_value(nuevo_config,"IP_CPU"));
	config_cpu.IP_MEMORIA = strdup(config_get_string_value(nuevo_config,"IP_MEMORIA"));
    config_cpu.PUERTO_MEMORIA = strdup(config_get_string_value(nuevo_config,"PUERTO_MEMORIA"));
    config_cpu.PUERTO_ESCUCHA_DISPATCH = strdup(config_get_string_value(nuevo_config,"PUERTO_ESCUCHA_DISPATCH"));
    config_cpu.PUERTO_ESCUCHA_INTERRUPT = strdup(config_get_string_value(nuevo_config,"PUERTO_ESCUCHA_INTERRUPT"));
    config_cpu.CANTIDAD_ENTRADAS_TLB = config_get_int_value(nuevo_config,"CANTIDAD_ENTRADAS_TLB");
    config_cpu.ALGORITMO_TLB = strdup(config_get_string_value(nuevo_config,"ALGORITMO_TLB"));

	config_destroy(nuevo_config);
}

/* Cada operacion_str está asociado a un puntero a la función que realiza la operación.*/
void init_opCodes_dictionary(void){
    opCodes_diccionario = dictionary_create();
    
    dictionary_put(opCodes_diccionario, SET,            (void*)set);
    // dictionary_put(opCodes_diccionario, MOV_IN,         (void*)mov_in);
    // dictionary_put(opCodes_diccionario, MOV_OUT,        (void*)mov_out);
    dictionary_put(opCodes_diccionario, SUM,            (void*)sum);
    dictionary_put(opCodes_diccionario, SUB,            (void*)sub);
    dictionary_put(opCodes_diccionario, JNZ,            (void*)jnz);
    // dictionary_put(opCodes_diccionario, RESIZE,         (void*)resize);
    // dictionary_put(opCodes_diccionario, COPY_STRING,    (void*)copy_string);
    // dictionary_put(opCodes_diccionario, WAIT,           (void*)wait);
    // dictionary_put(opCodes_diccionario, SIGNAL,         (void*)signal_kernel);
    dictionary_put(opCodes_diccionario, IO_GEN_SLEEP,   (void*)io_gen_sleep);
    // dictionary_put(opCodes_diccionario, IO_STDIN_READ,  (void*)io_stdin_read);
    // dictionary_put(opCodes_diccionario, IO_STDOUT_WRITE,(void*)io_stdout_write);
    // dictionary_put(opCodes_diccionario, IO_FS_CREATE,   (void*)io_fs_create);
    // dictionary_put(opCodes_diccionario, IO_FS_DELETE,   (void*)io_fs_delete);
    // dictionary_put(opCodes_diccionario, IO_FS_TRUNCATE, (void*)io_fs_truncate);
    // dictionary_put(opCodes_diccionario, IO_FS_WRITE,    (void*)io_fs_write);
    // dictionary_put(opCodes_diccionario, IO_FS_READ,     (void*)io_fs_read);
    // dictionary_put(opCodes_diccionario, EXIT_OS,        (void*)exit_os);
}
/* Cada nombre de registro tiene asociado la dirección de memoria*/
void init_registros_dictionary(void){
    registros_diccionario = dictionary_create();

    dictionary_put(registros_diccionario, "AX",  (void*)(&registrosCPU.AX));
    dictionary_put(registros_diccionario, "BX",  (void*)(&registrosCPU.BX));
    dictionary_put(registros_diccionario, "CX",  (void*)(&registrosCPU.CX));
    dictionary_put(registros_diccionario, "DX",  (void*)(&registrosCPU.DX));
    dictionary_put(registros_diccionario, "EAX", (void*)(&registrosCPU.EAX));
    dictionary_put(registros_diccionario, "EBX", (void*)(&registrosCPU.EBX));
    dictionary_put(registros_diccionario, "ECX", (void*)(&registrosCPU.ECX));
    dictionary_put(registros_diccionario, "EDX", (void*)(&registrosCPU.EDX));
    dictionary_put(registros_diccionario, "SI",  (void*)(&registrosCPU.SI));
    dictionary_put(registros_diccionario, "DI",  (void*)(&registrosCPU.DI));
}
