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

    registros.PC += 1;                                         // Al finalizar el ciclo, este último deberá ser actualizado (sumarle 1) si corresponde.
    char* instruccion = recibir_instruccion();           // recibimos una instruccion.
    instruccion_ejecutando = string_split(instruccion, " ");    // dividimos las partes de la instruccion en un array. 
    free(instruccion);
}


void decode_and_execute(void) {
    
    char* parametros = _armado_parametros(instruccion_ejecutando);

    log_info(cpu_log,"PID: < %d > - Ejecutando: < %s > - < %s >", PID ,instruccion_ejecutando[0], parametros);
    free(parametros);
    
    void (*funcion_operacion)(void) = dictionary_get(opCodes_diccionario, instruccion_ejecutando[0]); // obtenemos la función a partir del string del nombre de la instrucción
    funcion_operacion(); // ejecutamos la funcion y para los argumentos se pueden acceder a ellos desde el array global instruccion_cpu.   
} 

void checkInterrupt(void){
    if(hay_interrupcion){
        char* interrupcion_string = _enum_interrupcion_string();

        log_info(cpu_log_debug, "Se detecto una interrupcion de tipo < %s >", interrupcion_string);
        free(interrupcion_string);

        devolver_contexto_ejecucion(tipo_interrupcion);

        hay_interrupcion = 0;
        se_devolvio_contexto = 1;
    }else{
        log_info(cpu_log_debug, "No hay interrupciones");
    }
}

char* _armado_parametros(char** instrucciones){
    char* nueva_cadena = string_duplicate("");
    int i = 1;

    while (instrucciones[i] != NULL)
    {
        string_append(&nueva_cadena, instrucciones[i]);
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