#include <include/ciclo.h>

/*
// De memoria se recibe siempre éste struct, si no se usan ciertos argumentos se envían vacíos 
// y el switch del execute se asegure de que cada operacion use los argumentos correspondientes
*/

/* Cada operacion_str está asociado a un puntero a la función que realiza la operación.*/
void init_opCodes_dictionary() {
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
void init_registros_dictionary() {
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

void arranque(){
    init_opCodes_dictionary();
    init_registros_dictionary();
    pthread_mutex_init(&mutexInterrupt, NULL);

    PCB = recibirPCB();
    acomodarRegistrosDeCPU(PCB.registros);
    ciclo();

    fin_de_ciclo();             // cuando salga del loop signfica que por alguna razón cpu paró (algun error etc)
}
void ciclo() {
    while(1) {
        fetch();                // busca la siguiente instrucción.
        decode_and_execute();   // interpreta y ejecuta la operación que corresponde
        checkInterrupt();       // se fija si hay interrupciones.
    }   
}

void fin_de_ciclo() {
    dictionary_destroy(opCodes_diccionario);    // liberamos memoria asignada al diccionario.
    dictionary_destroy(registros_diccionario);
    string_array_destroy(instruccion_cpu);      // liberamos este array de strings (commons/string.h)
    pthread_mutex_destroy(&mutexInterrupt);
}

void fetch(){
    PCB.registros.PC += 1;                                   // apuntamos a la siguiente instrucción.
    solicitar_instruccion_a_memoria(PCB.registros.PC);       // se la pedimos a Memoria.
    char* instruccion_str = recibir_instruccion();           // recibimos un puntero.
    instruccion_cpu = string_split(instruccion_str, " ");    // recibimos desde Memoria la instrucción como string y dividimos las partes en un array. OJO hay que liberarla.
    free(instruccion_str);
}
void decode_and_execute() {
    void (*funcion_operacion)(void) = dictionary_get(opCodes_diccionario, instruccion_cpu[0]); // obtenemos la función a partir del string del nombre de la instrucción
    funcion_operacion(); // ejecutamos la funcion y para los argumentos se pueden acceder a ellos desde el array global instruccion_cpu.
}

void checkInterrupt() {
    // Si la PID del proceso en ejecución coincide con el PID solicitado a interrumpir, entonces se realiza lo solicitado.
    bool seAceptaInterrupcion;

    pthread_mutex_lock(&mutexInterrupt);
    seAceptaInterrupcion = (PID_a_interrumpir == PCB.PID);
    pthread_mutex_unlock(&mutexInterrupt);
    
    if(seAceptaInterrupcion) {
        log_info(cpu_log_debug, "Interrumpiendo proceso PID=%d en ejecución", PCB.PID);
        desbloquearDispatch();
    }
    else {
        log_error(cpu_log_debug, "El proceso pedido (PID=%d) no coincide con el actual en ejecución (PID=%d), no se interrumpirá.", PID_a_interrumpir, PCB.PID);
    }
    PID_a_interrumpir = 0; // una vez checkeado, reiniciamos esta variable.
}

void desalojarProcesoActual(){
    // Procede a realizar un cambio de contexto
    exportarPCB();        // Mandarle PCB a Kernel.
    PCB = recibirPCB();   // Recibir PCB de la interrupción mandada desde Kernel.
    acomodarRegistrosDeCPU(PCB.registros);  // actualizar registros de la cpu con los de la PCB.
}

void solicitar_instruccion_a_memoria(uint32_t PC){
	t_paquete* paquete = crear_paquete(OBTENER_INSTRUCCION);

	int buffer_size = 2*sizeof(int); 
	crear_buffer(paquete, buffer_size);

	buffer_add_int(paquete->buffer, PCB.PID);   // PID del proceso en ejecución
	buffer_add_int(paquete->buffer, PC);        // PC de la instrucción que queremos del proceso

	enviar_paquete(paquete, fd_conexion_memoria);
	eliminar_paquete(paquete);
}

char* recibir_instruccion(void){
	int codigo_operacion = recibir_operacion(fd_conexion_memoria);  // por convención nos comunicamos usando paquetes, por eso debemos recibir la operación primero.

	t_buffer *buffer = recibir_buffer(fd_conexion_memoria);
	void* stream = buffer->stream;

	int length = buffer_read_int(&stream);

	char* ptr_string = buffer_read_string(&stream, length); // puntero a un espacio de memoria de chars. Hay que liberarlo.

	log_info(cpu_log_debug, "Instruccion recibida: %s\n", ptr_string);
	
	eliminar_buffer(buffer);

	return ptr_string;                
}




struct_PCB recibirPCB() {
    return PCB; // CAMBIAR DESPUÉS
} 


// void enviarAKernel() {

// }

//Funciones auxiliares para checkInterrupt()

/* Actualiza registros de CPU con los pasados por parametro */
void acomodarRegistrosDeCPU(struct_registros registros){
    registrosCPU.PC  = registros.PC;
    registrosCPU.AX  = registros.AX;
    registrosCPU.BX  = registros.BX;
    registrosCPU.CX  = registros.CX;
    registrosCPU.DX  = registros.DX;
    registrosCPU.EAX = registros.EAX;
    registrosCPU.EBX = registros.EBX;
    registrosCPU.ECX = registros.ECX;
    registrosCPU.EDX = registros.EDX; 
    registrosCPU.SI  = registros.SI;
    registrosCPU.DI  = registros.DI;
}

void exportarPCB(void){
    struct_registros* ptr_registros = malloc(sizeof(struct_registros));
    ptr_registros = &(PCB.registros);
    acomodarRegistrosDePCB(ptr_registros);
    //enviarAKernel(); hablac con gonza por el protocolo
}

/* Actualiza el PCB del proceso en ejecución*/
void acomodarRegistrosDePCB(struct_registros* registros){
    registros->PC  = registrosCPU.PC;
    registros->AX  = registrosCPU.AX;
    registros->BX  = registrosCPU.BX;
    registros->CX  = registrosCPU.CX;
    registros->DX  = registrosCPU.DX;
    registros->EAX = registrosCPU.EAX;
    registros->EBX = registrosCPU.EBX;
    registros->ECX = registrosCPU.ECX;
    registros->EDX = registrosCPU.EDX; 
    registros->SI  = registrosCPU.SI;
    registros->DI  = registrosCPU.DI;
}

// FUNCIONES AUXILIARES

void* direccionDelRegistro(char* nombre_registro){
    return dictionary_get(registros_diccionario, nombre_registro); // retorna tipo void*. Se debe definir fuera de la función si se interpreta como uint8_t o como uint32_t.
    // void* ptr_a_registro = malloc(tamanioDeRegistro(registro));
    // if(!strcmp(registro, "AX")) {  return ptr_a_registro = &registrosCPU.AX;}
    // if(!strcmp(registro, "BX")) {  return ptr_a_registro = &registrosCPU.BX;}
    // if(!strcmp(registro, "CX")) {  return ptr_a_registro = &registrosCPU.CX;}
    // if(!strcmp(registro, "DX")) {  return ptr_a_registro = &registrosCPU.DX;}
    // if(!strcmp(registro, "EAX")){  return ptr_a_registro = &registrosCPU.EAX;}
    // if(!strcmp(registro, "EBX")){  return ptr_a_registro = &registrosCPU.EBX;}
    // if(!strcmp(registro, "ECX")){  return ptr_a_registro = &registrosCPU.ECX;}
    // if(!strcmp(registro, "EDX")){  return ptr_a_registro = &registrosCPU.EDX;}
    // if(!strcmp(registro, "SI")) {  return ptr_a_registro = &registrosCPU.SI;}
    // if(!strcmp(registro, "DI")) {  return ptr_a_registro = &registrosCPU.DI;}
}

/**
 * Si el registro empieza con E, entonces es un Extended. Si termina con I entonces es SI o DI. Todos estos son de 32 bits.
 * El resto son de 8 bits, sin contar el registro PC.
*/
int tamanioDeRegistro(char* registro){
    // char* regs_uint8[4] = {"AX","BX","CX","DX"};
    // if(contiene(regs_uint8,4,registro)){
    if(string_starts_with(registro, "E") || string_ends_with(registro, "I")) {
        return sizeof(uint32_t);  
    }
    else{
        return sizeof(uint8_t);
    }
}

// bool contiene(char *unArrayDeStrings[],int sizeOfArray,char* unElemento){
//     for(int i = 0; i < sizeOfArray; ++i)
//     {
//         if(!strcmp(unArrayDeStrings[i], unElemento))
//         {
//             return 1;
//         }
//     }
//     return 0; // si no lo encuentra, entoences devuelve 0.
// }



// LAS FUNCIONES OPERACIÓN

void set(){//asigna al registro seleccionado el valor pasado como argumento
    char* registro = instruccion_cpu[1];
    uint32_t valor = atoi(instruccion_cpu[2]);  // string to int

    void* ptr_registroCPU = direccionDelRegistro(registro);

    //necesito hacer el cast a uint8_t o uint32_t por el void* que no se puede desreferenciar
    if(tamanioDeRegistro(registro) == sizeof(uint8_t)) {
        *(uint8_t*)ptr_registroCPU = valor;  // convierte el puntero void* a tipo uint8_t* y lo desreferencia para poder cambiar el valor del espacio de memoria al que apunta.
    }
    else {
        *(uint32_t*)ptr_registroCPU = valor; // idem pero para uint32_t
    }

    // free(ptr_registroCPU);
}
void sum(){//Vale sumar un registro de 32bits a uno de 8bits
    char* registroDestino = instruccion_cpu[1];
    char* registroOrigen = instruccion_cpu[2];
    
    void* ptr_registroDestino;
    void* ptr_registroOrigen;

    ptr_registroDestino = direccionDelRegistro(registroDestino);
    ptr_registroOrigen = direccionDelRegistro(registroOrigen);

//  *ptr_registroDestino += *ptr_registroOrigen; no se puede hacer por que son void*
//  entonces tengo que hacer el if else para castearlos
    if(tamanioDeRegistro(registroDestino) == sizeof(uint8_t)){
        *(uint8_t*)ptr_registroDestino += *(uint8_t*)ptr_registroOrigen;
    }
    else{
        *(uint32_t*)ptr_registroDestino += *(uint32_t*)ptr_registroOrigen;
    }

    // free(ptr_registroDestino);
    // free(ptr_registroOrigen);

}
void sub(){
    char* registroDestino = instruccion_cpu[1];
    char* registroOrigen = instruccion_cpu[2];

    void* ptr_registroDestino;
    void* ptr_registroOrigen;

    ptr_registroDestino = direccionDelRegistro(registroDestino);
    ptr_registroOrigen = direccionDelRegistro(registroOrigen);

    if(tamanioDeRegistro(registroDestino) == sizeof(uint8_t)){
        *(uint8_t*)ptr_registroDestino -= *(uint8_t*)ptr_registroOrigen;
    }
    else{
        *(uint32_t*)ptr_registroDestino -= *(uint32_t*)ptr_registroOrigen;
    }

    // free(ptr_registroDestino);
    // free(ptr_registroOrigen);
}
void jnz(){
    char* registro = instruccion_cpu[1];
    int instruccion_PC = atoi(instruccion_cpu[2]);  // se espera un program counter

    void* ptr_registroCPU = direccionDelRegistro(registro);

    uint32_t valor_del_registro = *(uint32_t*)ptr_registroCPU;
    if(valor_del_registro != 0){
        registrosCPU.PC = instruccion_PC; 
    }
    else{
        //tirar un error
    }
}
void io_gen_sleep(){
    char* interfaz = instruccion_cpu[1];
    char* unidadesDeTrabajo = instruccion_cpu[2];

    // int interfaz_para_kernel = identificarInterfaz(interfaz);
    int unidadesDeSleep = atoi(unidadesDeTrabajo);
    // enviarleAkernel(interfaz_para_kernel, unidadesDeSleep);//hablar con gonza por protocolo de comunicacion
}





