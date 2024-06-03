#include <ciclo/ciclo.h>

void ciclo(void){
    se_devolvio_contexto = 0;
    while(1){
        fetch();              // busca la siguiente instrucción.
        decode_and_execute(); 
        checkInterrupt();     // se fija si hay interrupciones.
        if(se_devolvio_contexto){ break;}
    }
}

void fetch(void){
    solicitar_instruccion_a_memoria();

    log_info(cpu_log, "PID: < %d > - FETCH - Program Counter: < %d >", PID, registros.PC);

    registros.PC += 1;                                          // Al finalizar el ciclo, este último deberá ser actualizado (sumarle 1) si corresponde.
    char* instruccion = recibir_instruccion();                  // Recibimos una instruccion.
    string_array_destroy(instruccion_ejecutando);               // Debemos liberar el string_array anterior, porque cuando le reasignemos un valor a nuestro puntero "instruccion_ejecutando", el puntero al string_array anterior se perderá y generará memory leaks. 
    instruccion_ejecutando = string_split(instruccion, " ");    // Dividimos las partes de la instruccion en un array. 
    free(instruccion);
}


void decode_and_execute(void) {
    
    char* parametros_string = _armado_parametros(instruccion_ejecutando);

    log_info(cpu_log,"PID: < %d > - Ejecutando: < %s > - < %s >", PID ,instruccion_ejecutando[0], parametros_string);
    free(parametros_string);
    
    void (*funcion_operacion)(void) = dictionary_get(opCodes_diccionario, instruccion_ejecutando[0]); // obtenemos la función a partir del string del nombre de la instrucción
    funcion_operacion(); // ejecutamos la funcion y para los argumentos se pueden acceder a ellos desde el array global instruccion_cpu.   
} 

void checkInterrupt(void){
    if(hay_interrupcion){
        char* interrupcion_string = _enum_interrupcion_string();

        log_info(cpu_log_debug, "Se detecto una interrupcion de tipo < %s >", interrupcion_string);

        devolver_contexto_ejecucion(tipo_interrupcion);

        hay_interrupcion = 0;
        se_devolvio_contexto = 1;
    }else{
        log_info(cpu_log_debug, "No hay interrupciones");
    }
}

// función auxiliar para crear un string con los parámetros separados por un espacio blanco.
char* _armado_parametros(char** instruccion_array){
    char *nueva_cadena = string_new();
    int i = 1; // empieza en 1 para ignorar el opcode.

    while (instruccion_array[i] != NULL) // iteramos sobre cada elemento del instruccion_array
    {
        string_append(&nueva_cadena, instruccion_array[i]);
        string_append(&nueva_cadena, " ");
        i++;
    }
    return nueva_cadena;
}

char* _enum_interrupcion_string(){

    if (tipo_interrupcion == FIN_DE_QUANTUM){ return "FIN DE QUANTUM"; }
    if (tipo_interrupcion == INTERRUPCION){ return "FIN DE PROCESO"; }
    
    return "DESCONOCIDO";
}