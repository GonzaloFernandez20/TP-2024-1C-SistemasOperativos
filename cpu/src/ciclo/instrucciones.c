#include <ciclo/instrucciones.h>

//SET////////////////////////////////////////////////////////////////////////////////////
//asigna al registro seleccionado el valor pasado como argumento
void set(void){
    char* registro =  (instruccion_ejecutando[1]);
    int valor = atoi(instruccion_ejecutando[2]);  // string to int

    void* ptr_registro = direccion_del_registro(registro);

    //necesito hacer el cast a uint8_t o uint32_t por el void* que no se puede desreferenciar
    if(tamanio_de_registro(registro) == sizeof(uint8_t)) {
        *(uint8_t*)ptr_registro = valor;  // convierte el puntero void* a tipo uint8_t* y lo desreferencia para poder cambiar el valor del espacio de memoria al que apunta.
    }
    else {
        *(uint32_t*)ptr_registro = valor; // idem pero para uint32_t
    }
}
//SUM////////////////////////////////////////////////////////////////////////////////////
//Suma al Registro Destino el Registro Origen y deja el resultado en el Registro Destino.
    //Vale sumar un registro de 32bits a uno de 8bits   
void sum(void){
    char* registro_destino =  (instruccion_ejecutando[1]);
    char* registro_origen =  (instruccion_ejecutando[2]);

    void* ptr_registro_destino = direccion_del_registro(registro_destino);
    void* ptr_registro_origen = direccion_del_registro(registro_origen);

    if(tamanio_de_registro(registro_destino) == sizeof(uint8_t)){
        *(uint8_t*)ptr_registro_destino += *(uint8_t*)ptr_registro_origen;
    }
    else{
        *(uint32_t*)ptr_registro_destino += *(uint32_t*)ptr_registro_origen;
    }
}
//SUB////////////////////////////////////////////////////////////////////////////////////
//Resta al Registro Destino el Registro Origen y deja el resultado en el Registro Destino.
void sub(void){
    char* registro_destino =  (instruccion_ejecutando[1]);
    char* registro_origen =  (instruccion_ejecutando[2]);

    void* ptr_registro_destino = direccion_del_registro(registro_destino);
    void* ptr_registro_origen = direccion_del_registro(registro_origen);

    if(tamanio_de_registro(registro_destino) == sizeof(uint8_t)){
        *(uint8_t*)ptr_registro_destino -= *(uint8_t*)ptr_registro_origen;
    }
    else{
        *(uint32_t*)ptr_registro_destino -= *(uint32_t*)ptr_registro_origen;
    }
}
//JNZ//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Si el valor del registro es distinto de cero, actualiza el program counter al número de instrucción pasada por parámetro.
void jnz(void){
    char* registro = instruccion_ejecutando[1];
    int PC_de_instruccion = atoi(instruccion_ejecutando[2]);  // se espera un program counter

    uint32_t valor_del_registro; // puede almacenar uint8_t también

    void* ptr_registro = direccion_del_registro(registro);

    if(tamanio_de_registro(registro) == sizeof(uint8_t)) {
        valor_del_registro = *(uint8_t*)ptr_registro;    
    }
    else {
        valor_del_registro = *(uint32_t*)ptr_registro;
    } 
    
    if(valor_del_registro != 0){ // si es distinto de 0
        registros.PC = PC_de_instruccion; // salta al valor de PC indicado
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
    char* registro_datos = strdup(instruccion_ejecutando[1]);
    char* registro_direccion = strdup(instruccion_ejecutando[2]);    
    
    void* ptr_registro_direccion = direccion_del_registro(registro_direccion);     // punteros a tipo de dato genérico
    void* ptr_registro_datos = direccion_del_registro(registro_datos);

    free(registro_datos);
    free(registro_direccion);

    // ya sabemos que todo registro dirección es de 32 bits.
    uint32_t direccion_logica = *(uint32_t*)ptr_registro_direccion; // casteo el void* a uint32_t* y luego lo desreferencio 
    
    char* dato_leido = leer_de_memoria(direccion_logica);

    *(char*)ptr_registro_datos = dato_leido;
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
    char* registro_datos = strdup(instruccion_ejecutando[2]);
    char* registro_direccion = strdup(instruccion_ejecutando[1]);    
    
    void* ptr_registro_direccion = direccion_del_registro(registro_direccion); 
    void* ptr_registro_datos = direccion_del_registro(registro_datos);

    free(registro_datos);
    free(registro_direccion);

    // ya sabemos que todo registro dirección es de 32 bits.
    uint32_t direccion_logica = *(uint32_t*)ptr_registro_direccion; // casteo el void* a uint32_t* y luego lo desreferencio 
    char* datos_a_escribir = *(char*)ptr_registro_datos;    // REVISAR ESTO, MEDIO RARO.

    escribir_en_memoria(direccion_logica, datos_a_escribir);    
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

    int ok = solicitar_ajustar_tamanio_de_proceso_a_memoria(tamanio);
    
    if(!ok) { // El error Out of Memory será representada con un 0. Si not ok entonces devolvemos contexto de ejecución con el motivo 
        int motivo = OUT_OF_MEMORY;
        devolver_contexto_ejecucion(motivo);
    }

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

    uint32_t dl_origen = registros.SI;   // dirección lógica orígen
    uint32_t dl_destino = registros.DI;  // dirección lógica destino

    for(size_t offset=0; offset<cantidad_de_bytes; offset++) {
        char* string_leido = leer_de_memoria(dl_origen + offset);
        escribir_en_memoria(dl_destino + offset, string_leido);
    }

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

    // traduccion de direcciones
    devolver_contexto_ejecucion_IO_STDIN_READ(interfaz, registro_tamanio);

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
    char* interfaz = strdup(instruccion_ejecutando[1]);
    void* registro_direccion = direccion_del_registro(instruccion_ejecutando[2]);
    void* registro_tamanio = direccion_del_registro(instruccion_ejecutando[3]);

    

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
//WAIT////////////////////////////////////////////////////////////////////////////////////////////////
void wait_kernel(void){
    char* recurso = strdup(instruccion_ejecutando[1]);
    devolver_contexto_ejecucion_RECURSO(recurso, WAIT);
    int cod_op = recibir_operacion(fd_dispatch);
    if (cod_op != CONTEXTO_EJECUCION) { perror("Rompiste todo");}
    recibir_contexto_ejecucion();
}
//SIGNAL////////////////////////////////////////////////////////////////////////////////////////////////
void signal_kernel(void){
    char* recurso = strdup(instruccion_ejecutando[1]);
    devolver_contexto_ejecucion_RECURSO(recurso, SIGNAL);
    int cod_op = recibir_operacion(fd_dispatch);
    if (cod_op != CONTEXTO_EJECUCION) { perror("Rompiste todo");}
    recibir_contexto_ejecucion();
}







//Auxiliares

// Retorna la direccion casteado a void* del registro dado  
void* direccion_del_registro(char* nombre_registro){
    return dictionary_get(registros_diccionario, nombre_registro); // retorna tipo void*. Se debe definir fuera de la función si se interpreta como uint8_t* o como uint32_t*.
}

int tamanio_de_registro(char* registro){

    if(string_starts_with(registro, "E") || string_ends_with(registro, "I")) {
        return sizeof(uint32_t);  
    }
    else{
        return sizeof(uint8_t);
    }
}