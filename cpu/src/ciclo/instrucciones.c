#include <ciclo/instrucciones.h>

//SET////////////////////////////////////////////////////////////////////////////////////
//asigna al registro seleccionado el valor pasado como argumento
void set(void){
    char* registro =  (instruccion_ejecutando[1]);
    int valor = atoi(instruccion_ejecutando[2]);  // string to int

    void* ptr_registro = direccion_del_registro(registro);

    //necesito hacer el cast a uint8_t o uint32_t por el void* que no se puede desreferenciar
    if(tamanio_de_registro(registro) == sizeof(uint8_t)) {
        *(uint8_t*)ptr_registro = (uint8_t)valor; 
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
//Lee el valor de memoria correspondiente a la DL que se encuentra en el Registro Dirección y lo almacena en el Registro Datos. 
void mov_in(void) {
    void* ptr_registro_datos = obtener_direcciones_fisicas(2,1);
    
    int cantidad_direcciones = list_size(direcciones);
    int offset = 0;
    
    for(int i = 0; i < cantidad_direcciones; i++){
        t_datos_acceso* datos = list_remove(direcciones, 0);
        int bytes = datos->bytes;
        int DF = datos->direccion_fisica;
        free(datos);
    
        void* particion = leer_de_memoria(DF,bytes);
        memcpy(ptr_registro_datos + offset, particion, bytes);
        offset += bytes;

        pthread_mutex_lock(&mutex_log);
        if(bytes == 1){
            log_info(cpu_log,"PID: < %d > - Acción: < LEER > - Dirección Física: < %d > - Valor: < %d >", PID, DF, *(uint8_t*)particion);
        }
        else if (bytes == 4)
        {
            log_info(cpu_log,"PID: < %d > - Acción: < LEER > - Dirección Física: < %d > - Valor: < %d >", PID, DF, *(uint32_t*)particion);
        }
        else{
             log_info(cpu_log,"PID: < %d > - Acción: < LEER > - Dirección Física: < %d > - Valor: < %d >", PID, DF, *(uint16_t*)particion);
        }
        
        pthread_mutex_unlock(&mutex_log);

        free(particion);
    }
}


//MOV_OUT///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Lee el valor del Registro Datos y lo escribe en la dirección física de memoria obtenida a partir de la DL almacenada en el Registro Dirección. 
void mov_out(void) {
    void* ptr_registro_datos = obtener_direcciones_fisicas(1,2);

    int cantidad_direcciones = list_size(direcciones);
    int offset = 0;
    
    for(int i = 0; i < cantidad_direcciones; i++){
        t_datos_acceso* datos = list_remove(direcciones, 0);
        int bytes = datos->bytes;
        int DF = datos->direccion_fisica;
        free(datos);

        void* particion = malloc(bytes);
        memcpy(particion, ptr_registro_datos + offset, bytes);
        offset += bytes;
        
        escribir_en_memoria(particion, bytes, DF);

        pthread_mutex_lock(&mutex_log);
        if(bytes == 1){
            log_info(cpu_log,"PID: < %d > - Acción: < ESCRIBIR > - Dirección Física: < %d > - Valor: < %d >", PID, DF, *(uint8_t*)particion);
        }
        else if (bytes == 4)
        {
            log_info(cpu_log,"PID: < %d > - Acción: < ESCRIBIR > - Dirección Física: < %d > - Valor: < %d >", PID, DF, *(uint32_t*)particion);
        }
        else{
             log_info(cpu_log,"PID: < %d > - Acción: < ESCRIBIR > - Dirección Física: < %d > - Valor: < %d >", PID, DF, *(uint16_t*)particion);
        }
        pthread_mutex_unlock(&mutex_log);

        free(particion);
    }
}

//RESIZE/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Solicitará a la Memoria ajustar el tamaño del proceso al tamaño pasado por parámetro. En caso de que la respuesta de la memoria sea Out of Memory, 
se deberá devolver el contexto de ejecución al Kernel informando de esta situación.*/ 
void resize(void) {
    int tamanio = atoi(instruccion_ejecutando[1]);

    int mensaje_ok = solicitar_ajustar_tamanio(tamanio);
    
    if(!mensaje_ok) { // El error Out of Memory será representada con un 0. Si not ok entonces devolvemos contexto de ejecución con el motivo 
        devolver_contexto_ejecucion(OUT_OF_MEMORY);
        se_devolvio_contexto = 1;
        hay_interrupcion = 0;
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
    uint32_t DL_origen = registros.SI;   // dirección lógica orígen
    uint32_t DL_destino = registros.DI;  // dirección lógica destino
    
    traducir_direcciones(cantidad_de_bytes, DL_origen);
    void* string_leido = leer_string(cantidad_de_bytes);

    traducir_direcciones(cantidad_de_bytes, DL_destino);
    escribir_string(string_leido);
}

void* leer_string(int cantidad_de_bytes){
    void* string_leido = malloc(cantidad_de_bytes);

    int cantidad_direcciones = list_size(direcciones);
    int offset = 0;
    
    for(int i = 0; i < cantidad_direcciones; i++){
        t_datos_acceso* datos = list_remove(direcciones, 0);
        int bytes = datos->bytes;
        int DF = datos->direccion_fisica;
        free(datos);

        void* particion = leer_de_memoria(DF,bytes);
        memcpy(string_leido + offset, particion, bytes);
        offset += bytes;

        char* cadena = generar_cadena(particion, bytes);

        pthread_mutex_lock(&mutex_log);
            log_info(cpu_log,"PID: < %d > - Acción: < LEER > - Dirección Física: < %d > - Valor: < %s >", PID, DF, cadena);
        pthread_mutex_unlock(&mutex_log);

        free(particion);
        free(cadena);
    }

    return string_leido;
}

void escribir_string(void* string_leido){
    int cantidad_direcciones = list_size(direcciones);
    int offset = 0;
    
    for(int i = 0; i < cantidad_direcciones; i++){
        t_datos_acceso* datos =list_remove(direcciones, 0);
        int bytes = datos->bytes;
        int DF = datos->direccion_fisica;
        free(datos);

        void* particion = malloc(bytes);
        memcpy(particion, string_leido + offset, bytes);
        offset += bytes;
        
        escribir_en_memoria(particion, bytes, DF);

        char* cadena = generar_cadena(particion, bytes);

        pthread_mutex_lock(&mutex_log);
            log_info(cpu_log,"PID: < %d > - Acción: < ESCRIBIR > - Dirección Física: < %d > - Valor: < %s >", PID, DF, cadena);
        pthread_mutex_unlock(&mutex_log);

        free(particion);
        free(cadena);
    }
}

//IO_STDIN_READ//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** 
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

    int tamanio_a_leer = *(int*)registro_tamanio;
    uint32_t direccion_logica;

    if(tamanio_de_registro(registro_direccion) == sizeof(uint8_t)) {
        direccion_logica = *(uint8_t*)registro_direccion;    
    }
    else {
        direccion_logica  = *(uint32_t*)registro_direccion;
    } 
    traducir_direcciones(tamanio_a_leer, direccion_logica);

    devolver_contexto_ejecucion_IO_STDIN_READ(interfaz, tamanio_a_leer);
    se_devolvio_contexto = 1;
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
    char* interfaz = instruccion_ejecutando[1];
    
    void* registro_direccion = direccion_del_registro(instruccion_ejecutando[2]);
    void* registro_tamanio = direccion_del_registro(instruccion_ejecutando[3]);

    int tamanio_a_leer = *(int*)registro_tamanio;
    uint32_t direccion_logica;

    if(tamanio_de_registro(registro_direccion) == sizeof(uint8_t)) {
        direccion_logica = *(uint8_t*)registro_direccion;    
    }
    else {
        direccion_logica  = *(uint32_t*)registro_direccion;
    } 

    traducir_direcciones(tamanio_a_leer, direccion_logica);

    devolver_contexto_ejecucion_IO_STDOUT_WRITE(interfaz, tamanio_a_leer);
    se_devolvio_contexto = 1;
}


//IO_GEN_SLEEP////////////////////////////////////////////////////////////////////////////////////////////////
void io_gen_sleep(void){
    char* interfaz =instruccion_ejecutando[1];
    int unidades_de_trabajo = atoi(instruccion_ejecutando[2]);
    devolver_contexto_ejecucion_IO_GEN_SLEEP(interfaz,unidades_de_trabajo);
    se_devolvio_contexto = 1;
    hay_interrupcion = 0;
}

//EXIT////////////////////////////////////////////////////////////////////////////////////////////////
void exit_os(void){
    devolver_contexto_ejecucion(EXIT);
    se_devolvio_contexto = 1;
    hay_interrupcion = 0;
}
//WAIT////////////////////////////////////////////////////////////////////////////////////////////////
void wait_kernel(void){
    char* recurso = instruccion_ejecutando[1];
    devolver_contexto_ejecucion_RECURSO(recurso, WAIT);
    int cod_op = recibir_operacion(fd_dispatch);
    if (cod_op != CONTEXTO_EJECUCION) { perror("Rompiste todo");}
    recibir_contexto_ejecucion();
}
//SIGNAL////////////////////////////////////////////////////////////////////////////////////////////////
void signal_kernel(void){
    char* recurso = instruccion_ejecutando[1];
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

void* obtener_direcciones_fisicas(int indice1, int indice2){
    char* registro_direccion = instruccion_ejecutando[indice1];    
    char* registro_datos = instruccion_ejecutando[indice2];
    
    void* ptr_registro_direccion = direccion_del_registro(registro_direccion);     // punteros a tipo de dato genérico
    void* ptr_registro_datos = direccion_del_registro(registro_datos);

    uint32_t direccion_logica;

    if(tamanio_de_registro(ptr_registro_direccion) == sizeof(uint8_t)) {
        direccion_logica = *(uint8_t*)ptr_registro_direccion;    
    }
    else {
        direccion_logica  = *(uint32_t*)ptr_registro_direccion;
    }
    int tamanio;

    if(tamanio_de_registro(registro_datos) == sizeof(uint8_t)) {tamanio = sizeof(uint8_t); }// tamaño a leer/escribir es de 1 byte 
    else {tamanio = sizeof(uint32_t);} // tamaño a leer/escribir es de 4 byte 
     
    traducir_direcciones(tamanio, direccion_logica); // genera la lista global con los datos para el acceso a cada pagina

    return ptr_registro_datos;
}  

char *generar_cadena(void* particion, int bytes){
    char* nueva_cadena = malloc(bytes + 1);
    int i;

    for (i = 0; i < bytes; i++){
        nueva_cadena[i] = *(char *)(particion + i);
    }

    nueva_cadena[i]='\0';

    return nueva_cadena;
}
