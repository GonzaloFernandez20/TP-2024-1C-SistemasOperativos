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
    int PC_de_instruccion = atoi(instruccion_ejecutando[2]);  // se espera un program counter

    void* registro_CPU = direccion_del_registro(registro);
    free(registro);

    uint32_t valor_del_registro = *(uint32_t*)registro_CPU;
    if(valor_del_registro != 0){
        registros.PC = PC_de_instruccion; 
    }
}


//MOV_IN/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * MOV_IN (Registro Datos, Registro Dirección): 
 * Lee el valor de memoria correspondiente a la Dirección Lógica 
 * que se encuentra en el Registro Dirección y lo almacena en el Registro Datos.
 * 
 * Ej: MOV_IN EDX ECX
 * */ 
void mov_in(void) {
    void* registro_datos = direccion_del_registro(instruccion_ejecutando[1]);   
    void* registro_direccion = direccion_del_registro(instruccion_ejecutando[2]);



    return;
}


//MOV_OUT////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * MOV_OUT (Registro Dirección, Registro Datos): 
 * Lee el valor del Registro Datos y lo escribe en 
 * la dirección física de memoria obtenida a partir de 
 * la Dirección Lógica almacenada en el Registro Dirección.
 * 
 * Ej: MOV_OUT EDX ECX
 * */ 
void mov_out(void) {
    void* registro_direccion = direccion_del_registro(instruccion_ejecutando[1]);    
    void* registro_datos = direccion_del_registro(instruccion_ejecutando[2]);        



    return;
}

//RESIZE/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * RESIZE (Tamaño): 
 * Solicitará a la Memoria ajustar el tamaño del proceso al tamaño pasado por parámetro. 
 * En caso de que la respuesta de la memoria sea Out of Memory, 
 * se deberá devolver el contexto de ejecución al Kernel informando de esta situación.
 * 
 * Ej: RESIZE 128
 * */ 
void resize(void) {
    int tamanio = atoi(instruccion_ejecutando[1]);

    int ok = solicitar_ajustar_tamanio_de_proceso_a_memoria(tamanio); // FALTA IMPLEMENTAR
    
    if(!ok) { // El error Out of Memory será representada con un 0. Si not ok entonces devolvemos contexto de ejecución con el motivo 
        int motivo = OUT_OF_MEMORY;
        devolver_contexto_ejecucion(motivo);
    }

    return;
}


//COPY_STRING////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * COPY_STRING (Tamaño): 
 * Toma del string apuntado por el registro SI y copia la cantidad de bytes indicadas en 
 * el parámetro tamaño a la posición de memoria apuntada por el registro DI. 
 * 
 * Ej: COPY_STRING 8
 * */
void copy_string(void) {
    int cantidad_de_bytes = atoi(instruccion_ejecutando[1]);

    copiar_string_desde_memoria(registros.SI, registros.DI, cantidad_de_bytes);  // FALTA IMPLEMENTAR 

    return;
}


//IO_STDIN_READ//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** ,
 * IO_STDIN_READ (Interfaz, Registro Dirección, Registro Tamaño): 
 * Esta instrucción solicita al Kernel que mediante la interfaz ingresada 
 * se lea desde el STDIN (Teclado) un valor cuyo tamaño está delimitado 
 * por el valor del Registro Tamaño y el mismo se guarde a partir 
 * de la Dirección Lógica almacenada en el Registro Dirección.
 *
 * Ej: IO_STDIN_READ Int2 EAX AX
 * */ 
void io_stdin_read(void) {
    char* interfaz = instruccion_ejecutando[1];
    void* registro_direccion = direccion_del_registro(instruccion_ejecutando[2]);
    void* registro_tamanio = direccion_del_registro(instruccion_ejecutando[3]);



    return;
}


//IO_STDOUT_WRITE////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * IO_STDOUT_WRITE (Interfaz, Registro Dirección, Registro Tamaño): 
 * Esta instrucción solicita al Kernel que mediante la interfaz seleccionada, 
 * se lea desde la posición de memoria indicada por la Dirección Lógica 
 * almacenada en el Registro Dirección, un tamaño indicado por 
 * el Registro Tamaño y se imprima por pantalla.
 * 
 * Ej: IO_STDOUT_WRITE Int3 BX EAX
 * */ 
void io_stdout_write(void) {
    return;
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

// Toma direcciones lógicas de origen y destino y la cantidad de bytes que se quieren copiar de un lado para el otro.
void copiar_string_desde_memoria(uint32_t dl_origen, uint32_t dl_destino, uint32_t cantidad_de_bytes) {

    

    
    return;
}