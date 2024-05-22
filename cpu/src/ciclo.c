#include <include/ciclo.h>

/*
// De memoria se recibe siempre éste struct, si no se usan ciertos argumentos se envían vacíos 
// y el switch del execute se asegure de que cada operacion use los argumentos correspondientes
*/

t_dictionary* opCodes; 

void init_opCodes_dictionary() {
    opCodes = dictionary_create();
    dictionary_put(opCodes, "SET", SET);
    dictionary_put(opCodes, "MOV_IN", MOV_IN);
    dictionary_put(opCodes, "MOV_OUT", MOV_OUT);
    dictionary_put(opCodes, "SUM", SUM);
    dictionary_put(opCodes, "SUB", SUB);
    dictionary_put(opCodes, "JNZ", JNZ);
    dictionary_put(opCodes, "RESIZE", RESIZE);
    dictionary_put(opCodes, "COPY_STRING", COPY_STRING);
    dictionary_put(opCodes, "WAIT", WAIT);
    dictionary_put(opCodes, "SIGNAL", SIGNAL);
    dictionary_put(opCodes, "IO_GEN_SLEEP", IO_GEN_SLEEP);
    dictionary_put(opCodes, "IO_STDIN_READ", IO_STDIN_READ);
    dictionary_put(opCodes, "IO_STDOUT_WRITE", IO_STDOUT_WRITE);
    dictionary_put(opCodes, "IO_FS_CREATE", IO_FS_CREATE);
    dictionary_put(opCodes, "IO_FS_DELETE", IO_FS_DELETE);
    dictionary_put(opCodes, "IO_FS_TRUNCATE", IO_FS_TRUNCATE);
    dictionary_put(opCodes, "IO_FS_WRITE", IO_FS_WRITE);
    dictionary_put(opCodes, "IO_FS_READ", IO_FS_READ);
    dictionary_put(opCodes, "EXIT_OS", EXIT_OS);
}

void arranque(){
    init_opCodes_dictionary();

    PCB = recibirPCB();
    acomodarRegistrosDeCPU(PCB.registros);
    ciclo();
}
void ciclo() {
    while(1) {
        fetch();              // busca la siguiente instrucción.
        decode();             // interpreta la operación de la instrucción.
        execute();            // ejecuta la operación correspondiente al op_code dadp, junto a los parámetros dados por la instrucción.
        checkInterrupt();     // se fija si hay interrupciones.
    }
    // cuando salga del loop signfica que por alguna razón cpu paró
    dictionary_destroy(opCodes);    // liberamos memoria asignada al diccionario.
}
void fetch(){
    instruccion = pedirAMemoria(PCB.registros.PC + 1);
    PCB.registros.PC += 1;
}

/* 
Interpretamos la instrucción a ejecutar. 
Asignamos el opCode correspondiente a la variable global "op_code".
*/
void decode() {
    char* operacion_str = instruccion.operacion_str;
    op_code = dictionary_get(opCodes, operacion_str);
}

void checkInterrupt() {
    // Si la PID del proceso en ejecución coincide con el PID solicitado a interrumpir, entonces se realiza lo solicitado.
    if(PID_a_interrumpir == PCB.PID) {
        atenderInterrupcion(PID);
    }
    else {
        log_error(cpu_log_debug, "El proceso pedido (PID=%d) no coincide con el actual en ejecución (PID=%d), no se interrumpirá.", PID_recibido, PCB.PID);
    }
    PID_a_interrumpir = 0; // una vez checkeado, reiniciamos esta variable.
}

void atenderInterrupcion(int PID){
    log_info(cpu_log_debug, "Interrumpiendo proceso PID=%s en ejecución", PID_recibido);
    exportarPCB();
    PCB = recibirPCB();   // Actualizar PCB
    acomodarRegistrosDeCPU(PCB.registros); 
}

/*
Ejecutamos la operación junto con los argumentos que extrajimos de la instrucción
*/
void execute() {
    switch (op_code){
        case SET:
            set(instruccion.arg1,instruccion.arg2);
        case MOV_IN:
            move_in(instruccion.arg1,instruccion.arg2);
        case MOV_OUT:
            move_out(instruccion.arg1,instruccion.arg2); 
        case SUM:
            sum(instruccion.arg1,instruccion.arg2);
        case SUB:
            sub(instruccion.arg1,instruccion.arg2);
        case JNZ:
            jnz(instruccion.arg1,instruccion.arg2);
        case RESIZE:
            rezize(instruccion.arg1);
        case COPY_STRING:
            copy_string(instruccion.arg1);
        case WAIT:
            wait(instruccion.arg1);
        case SIGNAL:
            signal(instruccion.arg1);
        case IO_GEN_SLEEP:
            io_gen_sleep(instruccion.arg1,instruccion.arg2);
        case IO_STDIN_READ:
            io_stdin_read(instruccion.arg1,instruccion.arg2,instruccion.arg3);
        case IO_STDOUT_WRITE:
            io_stdout_write(instruccion.arg1,instruccion.arg2,instruccion.arg3);
        case IO_FS_CREATE:
            io_fs_create(instruccion.arg1,instruccion.arg2);
        case IO_FS_DELETE:
            io_fs_delete(instruccion.arg1,instruccion.arg2);
        case IO_FS_TRUNCATE: 
            io_fs_truncate(instruccion.arg1,instruccion.arg2,instruccion.arg3);
        case IO_FS_WRITE:
            io_fs_write(instruccion.arg1,instruccion.arg2,instruccion.arg3,instruccion.arg4,instruccion.arg5);
        case IO_FS_READ:
            io_fs_read(instruccion.arg1,instruccion.arg2,instruccion.arg3,instruccion.arg4,instruccion.arg5);
        case EXIT_OS:
            exit();
    }
}

//Definiciones de las operaciones necesarias para el checkpoint 2
void set(char* registro, char* valor){//asigna al registro seleccionado el valor pasado como argumento
    void* ptr_registroCPU;

    ptr_registroCPU = direccionDelRegistro(registro);

    int valorFinal = atoi(valor);
    //necesito hacer el cast a uint8_t o uint32_t por el void* que no se puede desreferenciar
    if(tamanioDeRegistro(registro) == sizeof(uint8_t)) {
        *(uint8_t*)ptr_registroCPU = valorFinal;
    }
    else {
        *(uint32_t*)ptr_registroCPU = valorFinal;
    }

    free(ptr_registroCPU);
}
void sum(char* registroDestino, char* registroOrigen){//Vale sumar un registro de 32bits a uno de 8bits
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

    free(ptr_registroDestino);
    free(ptr_registroOrigen);

}
void sub(char* registroDestino, char* registroOrigen){
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

    free(ptr_registroDestino);
    free(ptr_registroOrigen);
}
void jnz(char* registro, char* numDeInstruccion){
    void* ptr_registroCPU;
    
    int instruccion = atoi(numDeInstruccion);
    if(instruccion != 0){registrosCPU.PC += *(uint32_t*)ptr_registroOrigen;}//acá no hay if para castear porque el PC es siempre de tipo uint32_t
    else{
        //tirar un error
        }
    free(ptr_registroCPU);
}
void io_gen_sleep(char* interfaz, char* unidadesDeTrabajo){
    int interfaz_para_kernel = identificarInterfaz(interfaz);
    int unidadesDeSleep = atoi(unidadesDeTrabajo);
    enviarleAkernel(interfaz_para_kernel, unidadesDeSleep);//hablar con gonza por protocolo de comunicacion
}

//Funciones auxiliares para checkInterrupt()
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

//Funciones auxiliares para hacer el decode() y execute()

int tamanioDeRegistro(char* registro){
    char* regs_uint8[4] = {"AX","BX","CX","DX"};
    if(contiene(regs_uint8,4,registro)){
        return sizeof(uint8_t);
    }
    else{
        return sizeof(uint32_t);
    }
}

int contiene(char *unArrayDeStrings[],int sizeOfArray,char* unElemento){
for(int i = 0; i < sizeOfArray; ++i)
{
    if(!strcmp(unArrayDeStrings[i], unElemento))
    {
        return 1;
    }
}
}

void* direccionDelRegistro(char* registro){
    int tamanioDelRegistro = tamanioDeRegistro(registro);
    void* ptr_a_registro = malloc(sizeof(tamanioDelRegistro));
    if(!strcmp(registro, "AX")) {  return ptr_a_registro = &registrosCPU.AX;}
    if(!strcmp(registro, "BX")) {  return ptr_a_registro = &registrosCPU.BX;}
    if(!strcmp(registro, "CX")) {  return ptr_a_registro = &registrosCPU.CX;}
    if(!strcmp(registro, "DX")) {  return ptr_a_registro = &registrosCPU.DX;}
    if(!strcmp(registro, "EAX")){  return ptr_a_registro = &registrosCPU.EAX;}
    if(!strcmp(registro, "EBX")){  return ptr_a_registro = &registrosCPU.EBX;}
    if(!strcmp(registro, "ECX")){  return ptr_a_registro = &registrosCPU.ECX;}
    if(!strcmp(registro, "EDX")){  return ptr_a_registro = &registrosCPU.EDX;}
    if(!strcmp(registro, "SI")) {  return ptr_a_registro = &registrosCPU.SI;}
    if(!strcmp(registro, "DI")) {  return ptr_a_registro = &registrosCPU.DI;}
}
