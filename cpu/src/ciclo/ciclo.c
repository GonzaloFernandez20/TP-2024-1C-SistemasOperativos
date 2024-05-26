#include <ciclo/ciclo.h>

void ciclo(void){
    salir_ciclo = 0;
    while(1){
        fetch();              // busca la siguiente instrucción.
        decode_and_execute(); 
        checkInterrupt();     // se fija si hay interrupciones.
        if(salir_ciclo){break;}
    }
}

void fetch(void){
    solicitar_instruccion_a_memoria(PCB.PC);
    log_info(cpu_log, "PID: < %d > - FETCH - Program Counter: < %d >", PCB.PID, PCB.PC);

    registrosCPU.PC += 1;                                         // Al finalizar el ciclo, este último deberá ser actualizado (sumarle 1) si corresponde.
    char* instruccion = recibir_instruccion();           // recibimos una instruccion.
    instruccion_cpu = string_split(instruccion, " ");    // dividimos las partes de la instruccion en un array. 
    free(instruccion);
}


void decode_and_execute(void) {
    // log “PID: <PID> - Ejecutando: <INSTRUCCION> - <PARAMETROS>”.
    void (*funcion_operacion)(void) = dictionary_get(opCodes_diccionario, instruccion_cpu[0]); // obtenemos la función a partir del string del nombre de la instrucción
    funcion_operacion(); // ejecutamos la funcion y para los argumentos se pueden acceder a ellos desde el array global instruccion_cpu.
}


void checkInterrupt(void){;
    if(hay_interrupcion){
        devolver_contexto_ejecucion();
        hay_interrupcion = 0;
        salir_ciclo = 1;
    }
}

