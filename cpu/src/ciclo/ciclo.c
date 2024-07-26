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
    char* instruccion = solicitar_instruccion_a_memoria();
    log_info(cpu_log, "PID: < %d > - FETCH - Program Counter: < %d >", PID, registros.PC);

    registros.PC += 1;                                          // PC debe ser actualizado (sumarle 1) después de pedir una instrucción.
    instruccion_ejecutando = string_split(instruccion, " ");    // Dividimos las partes de la instruccion en un array. 
    free(instruccion);                                          // liberamos el string creado con malloc()
}


void decode_and_execute(void) {
    
    char* parametros_string = _armado_parametros(instruccion_ejecutando);
    log_info(cpu_log,"PID: < %d > - Ejecutando: < %s > - < %s >", PID ,instruccion_ejecutando[0], parametros_string);
    free(parametros_string);
    
    void (*funcion_operacion)(void) = dictionary_get(opCodes_diccionario, instruccion_ejecutando[0]); // obtenemos la función a partir del string del nombre de la instrucción
    funcion_operacion(); // ejecutamos la funcion y para los argumentos se pueden acceder a ellos desde el array global instruccion_cpu.   
    
    free(instruccion_ejecutando); // Debemos liberar el string_array anterior, porque cuando le reasignemos un valor a nuestro puntero "instruccion_ejecutando", el puntero al string_array anterior se perderá y generará memory leaks. 
} 

void checkInterrupt(void){
    if(hay_interrupcion){
        
        void *ptr_tipo_interrupcion = list_get_minimum(lista_interrupciones, elemento_minimo);
        int tipo_interrupcion = *(int *)ptr_tipo_interrupcion;

        char* interrupcion_string = _enum_interrupcion_string(tipo_interrupcion);

        log_info(cpu_log_debug, "Se detecto una interrupcion de tipo < %s >", interrupcion_string);

        devolver_contexto_ejecucion(tipo_interrupcion);
        pthread_mutex_lock(&mutex_lista_interrupciones);
            list_clean_and_destroy_elements(lista_interrupciones, free);
        pthread_mutex_unlock(&mutex_lista_interrupciones);
        pthread_mutex_lock(&mutex_hay_interrupcion);
                hay_interrupcion = 0; 
        pthread_mutex_unlock(&mutex_hay_interrupcion);
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

char* _enum_interrupcion_string(int tipo_interrupcion){

    if (tipo_interrupcion == FIN_DE_QUANTUM){ return "FIN DE QUANTUM"; }
    if (tipo_interrupcion == FIN_DE_PROCESO){ return "FIN DE PROCESO"; }
    if (tipo_interrupcion == RECURSO_INVALIDO){ return "RECURSO INVALIDO"; }
    if (tipo_interrupcion == PETICION_RECURSO){ return "PETICION RECURSO"; }
    
    return "DESCONOCIDO";
}

void *elemento_minimo(void *elem1, void *elem2){
    return (*(int *)elem1 < *(int *)elem2) ? elem1 : elem2;
}