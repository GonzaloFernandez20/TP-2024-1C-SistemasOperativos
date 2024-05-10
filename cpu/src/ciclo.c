#include <include/ciclo.h>

/*
// De memoria se recibe siempre éste struct, si no se usan ciertos argumentos se envían vacíos 
// y el switch del execute se asegure de que cada operacion use los argumentos correspondientes
*/
typedef struct{
    char* operacion_str;
    char* arg1;
    char* arg2;
    char* arg3;
    char* arg4;
    char* arg5;    
}t_instruccion;

// variables globales
t_instruccion instruccion;  // la utilizamos durante el fetch
int op_code;                // código de operación a ejecutar



void arranque(){
    PCB = recibirPCB();
    acomodarRegistrosDeCPU(PCB.registros);
    ciclo();
}
void ciclo() {
    while(1) {
        fetch();        // busca la siguiente instrucción.
        decode();          // interpreta la operación de la instrucción.
        execute();                    // ejecuta la operación correspondiente al op_code dadp, junto a los parámetros dados por la instrucción.
        checkInterrupt();             // se fija si hay interrupciones.
    }
}
t_instruccion fetch(){
    instruccion = pedirAMemoria(PCB.registros.PC + 1);
    PCB.registros.PC += 1;
}

/* 
Interpretamos la instrucción a ejecutar. 
Asignamos el opCode correspondiente a la variable global "op_code".
*/
void decode() {
    char* operacion_str = instruccion.operacion_str;

    if (strcmp(operacion_str, "SET") == 0) {
        op_code = SET;
    } else if (strcmp(operacion_str, "MOV_IN") == 0) {
        op_code = MOV_IN;
    } else if (strcmp(operacion_str, "MOV_OUT") == 0) {
        op_code = MOV_OUT;
    } else if (strcmp(operacion_str, "SUM") == 0) {
        op_code = SUM;
    } else if (strcmp(operacion_str, "SUB") == 0) {
        op_code = SUB;
    } else if (strcmp(operacion_str, "JNZ") == 0) {
        op_code = JNZ;
    } else if (strcmp(operacion_str, "RESIZE") == 0) {
        op_code = RESIZE;
    } else if (strcmp(operacion_str, "COPY_STRING") == 0) {
        op_code = COPY_STRING;
    } else if (strcmp(operacion_str, "WAIT") == 0) {
        op_code = WAIT;
    } else if (strcmp(operacion_str, "SIGNAL") == 0) {
        op_code = SIGNAL;
    } else if (strcmp(operacion_str, "IO_GEN_SLEEP") == 0) {
        op_code = IO_GEN_SLEEP;
    } else if (strcmp(operacion_str, "IO_STDIN_READ") == 0) {
        op_code = IO_STDIN_READ;
    } else if (strcmp(operacion_str, "IO_STDOUT_WRITE") == 0) {
        op_code = IO_STDOUT_WRITE;
    } else if (strcmp(operacion_str, "IO_FS_CREATE") == 0) {
        op_code = IO_FS_CREATE;
    } else if (strcmp(operacion_str, "IO_FS_DELETE") == 0) {
        op_code = IO_FS_DELETE;
    } else if (strcmp(operacion_str, "IO_FS_TRUNCATE") == 0) {
        op_code = IO_FS_TRUNCATE;
    } else if (strcmp(operacion_str, "IO_FS_WRITE") == 0) {
        op_code = IO_FS_WRITE;
    } else if (strcmp(operacion_str, "IO_FS_READ") == 0) {
        op_code = IO_FS_READ;
    } else if (strcmp(operacion_str, "EXIT_OS") == 0) {
        op_code = EXIT_OS;
    } else {
        // Si la cadena no coincide con ninguna operación válida
        op_code = -1;
    }
}


void checkInterrupt() {
    if(hayInterrupcion()){
        atenderInterrupcion();
    }
}
void hayInterrupcion(){
    //codear más tarde xq trabaja con la conexion interrupt
}
void atenderInterrupcion(void){
    // ATENDER INTERRUPCIÓN.
    exportarPCB();
    PCB = recibirPCB();
    acomodarRegistrosDeCPU(PCB.registros); // Actualizar PCB
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
    if(tamanioDeRegistro(registro) == sizeof(uint8_t)){*(uint8_t*)ptr_registroCPU = valorFinal;}
    else{*(uint32_t*)ptr_registroCPU = valorFinal;}

    free(ptr_registroCPU);
}
void sum(char* registroDestino, char* registroOrigen){//Vale sumar un registro de 32bits a uno de 8bits
    void* ptr_registroDestino;
    void* ptr_registroOrigen;

    ptr_registroDestino = direccionDelRegistro(registroDestino);
    ptr_registroOrigen = direccionDelRegistro(registroOrigen);

//  *ptr_registroDestino += *ptr_registroOrigen; no se puede hacer por que son void*
//  entonces tengo que hacer el if else para castearlos
    if(tamanioDeRegistro(registroDestino) == sizeof(uint8_t)){*(uint8_t*)ptr_registroDestino += *(uint8_t*)ptr_registroOrigen;}
    else{*(uint32_t*)ptr_registroDestino += *(uint32_t*)ptr_registroOrigen;}

    free(ptr_registroDestino);
    free(ptr_registroOrigen);

}
void sub(char* registroDestino, char* registroOrigen){
    void* ptr_registroDestino;
    void* ptr_registroOrigen;

    ptr_registroDestino = direccionDelRegistro(registroDestino);
    ptr_registroOrigen = direccionDelRegistro(registroOrigen);

    if(tamanioDeRegistro(registroDestino) == sizeof(uint8_t)){*(uint8_t*)ptr_registroDestino -= *(uint8_t*)ptr_registroOrigen;}
    else{*(uint32_t*)ptr_registroDestino -= *(uint32_t*)ptr_registroOrigen;}

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
