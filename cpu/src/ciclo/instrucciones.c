#include <ciclo/instrucciones.h>


//SET////////////////////////////////////////////////////////////////////////////////////
//asigna al registro seleccionado el valor pasado como argumento
void set(void){
    char* registro = strdup(instruccion_ejecutando[1]);
    int valor = atoi(instruccion_ejecutando[2]);  // string to int

    void* registro_CPU = direccion_del_registro(registro);

    //necesito hacer el cast a uint8_t o uint32_t por el void* que no se puede desreferenciar
    if(tamanio_de_registro(registro) == sizeof(uint8_t)) {
        *(uint8_t*)registro_CPU = valor;  // convierte el puntero void* a tipo uint8_t* y lo desreferencia para poder cambiar el valor del espacio de memoria al que apunta.
    }
    else {
        *(uint32_t*)registro_CPU = valor; // idem pero para uint32_t
    }
    free(registro);
}
//SUM////////////////////////////////////////////////////////////////////////////////////
//Suma al Registro Destino el Registro Origen y deja el resultado en el Registro Destino.
    //Vale sumar un registro de 32bits a uno de 8bits   
void sum(void){
    char* parametro_registro_destino = strdup(instruccion_ejecutando[1]);
    char* parametro_registro_origen = strdup(instruccion_ejecutando[2]);
    
    void* registro_destino = direccion_del_registro(parametro_registro_destino);
    void* registro_origen = direccion_del_registro(parametro_registro_origen);

    free(parametro_registro_destino);
    free(parametro_registro_origen);

    if(tamanio_de_registro(registro_destino) == sizeof(uint8_t)){
        *(uint8_t*)registro_destino += *(uint8_t*)registro_origen;
    }
    else{
        *(uint32_t*)registro_destino += *(uint32_t*)registro_origen;
    }
}
//SUB////////////////////////////////////////////////////////////////////////////////////
//Resta al Registro Destino el Registro Origen y deja el resultado en el Registro Destino.
void sub(void){
    char* parametro_registro_destino = strdup(instruccion_ejecutando[1]);
    char* parametro_registro_origen = strdup(instruccion_ejecutando[2]);

    void* registro_destino = direccion_del_registro(parametro_registro_destino);
    void* registro_origen = direccion_del_registro(parametro_registro_origen);

    free(parametro_registro_destino);
    free(parametro_registro_origen);

    if(tamanio_de_registro(registro_destino) == sizeof(uint8_t)){
        *(uint8_t*)registro_destino -= *(uint8_t*)registro_origen;
    }
    else{
        *(uint32_t*)registro_destino -= *(uint32_t*)registro_origen;
    }
}
//JNZ//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Si el valor del registro es distinto de cero, actualiza el program counter al número de instrucción pasada por parámetro.
void jnz(void){
    char* registro = strdup(instruccion_ejecutando[1]);
    int instruccion_PC = atoi(instruccion_ejecutando[2]);  // se espera un program counter

    void* registro_CPU = direccion_del_registro(registro);
    free(registro);

    uint32_t valor_del_registro = *(uint32_t*)registro_CPU;
    if(valor_del_registro != 0){
        registros.PC = instruccion_PC; 
    }
}

//IO_GEN_SLEEP////////////////////////////////////////////////////////////////////////////////////////////////
void io_gen_sleep(void){
    char* interfaz = strdup(instruccion_ejecutando[1]);
    int unidades_de_trabajo = atoi(instruccion_ejecutando[2]);
    devolver_contexto_ejecucion_IO_GEN_SLEEP(interfaz,unidades_de_trabajo);
    se_devolvio_contexto = 1;
}

//EXIT////////////////////////////////////////////////////////////////////////////////////////////////
void exit_os(void){
    devolver_contexto_ejecucion(EXIT);
    se_devolvio_contexto = 1;
    hay_interrupcion = 0;
}

void wait_kernel(void){
    char* recurso = strdup(instruccion_ejecutando[1]);
    devolver_contexto_ejecucion_RECURSO(recurso, WAIT);
    int cod_op = recibir_operacion(fd_dispatch);
    if (cod_op != CONTEXTO_EJECUCION) { perror("Rompiste todo");}
    recibir_contexto_ejecucion();
    int respuesta_exitosa;
    recv(fd_dispatch, &respuesta_exitosa, sizeof(int), 0);

    if (!respuesta_exitosa){   
        hay_interrupcion = 1;
    }
}

void signal_kernel(void){
    char* recurso = strdup(instruccion_ejecutando[1]);
    devolver_contexto_ejecucion_RECURSO(recurso, SIGNAL);
    int cod_op = recibir_operacion(fd_dispatch);
    if (cod_op != CONTEXTO_EJECUCION) { perror("Rompiste todo");}
    recibir_contexto_ejecucion();
}

//Auxiliares
void* direccion_del_registro(char* nombre_registro){
    return dictionary_get(registros_diccionario, nombre_registro); // retorna tipo void*. Se debe definir fuera de la función si se interpreta como uint8_t o como uint32_t.
}

int tamanio_de_registro(char* registro){

    if(string_starts_with(registro, "E") || string_ends_with(registro, "I")) {
        return sizeof(uint32_t);  
    }
    else{
        return sizeof(uint8_t);
    }
}