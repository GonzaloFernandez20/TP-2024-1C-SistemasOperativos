#include <include/ciclo.h>

void arranque(){
    PCB = recibirPCB();
    acomodarRegistrosDeCPU(PCB.registros);
    ciclo();
}
void ciclo() {
    while(1) {                 //el primer elemento del array es la opercion en sí, los otros 5 son argumentos
        char* instruccion[6] ;//la mayor cantidad de argumentos posibles para una operacion es 5
                             //si hubiera una con más se aumenta el tamaño del array y listo
        int operacion;
        instruccion = fetch(); // busca la instrucción y la guarda
        operacion = decode(instruccion[0]); 
        execute(operacion, instruccion);
        checkInterrupt();
    }
}
void fetch(){
    pedirAMemoria(PCB.registros.PC + 1); // actualiza variable global instruccion
    PCB.registros.PC += 1;
}

void decode(char* operacion_str) { 
    /* 
        Interpretamos el nombre de la operación y 
        asignamos el opCode correspondiente a la 
        variable global "operacion".
    */

    // El Switch-case no funciona con tipo char*
    if (strcmp(operacion_str, "SET") == 0) {
        operacion = SET;
    } else if (strcmp(operacion_str, "SUM") == 0) {
        operacion = SUM;
    } else if (strcmp(operacion_str, "SUB") == 0) {
        operacion = SUB;
    } else if (strcmp(operacion_str, "JNZ") == 0) {
        operacion = JNZ;
    } else if (strcmp(operacion_str, "IO_GEN_SLEEP") == 0) {
        operacion = IO_GEN_SLEEP;
    }else {
        // Si la cadena no coincide con ninguna operación válida, retorna -1
        return -1;
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
void execute(int operacion, char* instruccion[6]){
    //ignorar el elemento instruccion[0] pues es la operacion sin decodificar
    switch (operacion){
        case SET:
            set(instruccion[1],instruccion[2]);
        case MOV_IN:
            move_in(instruccion[1],instruccion[2]);
        case MOV_OUT:
            move_out(instruccion[1],instruccion[2]); 
        case SUM:
            sum(instruccion[1],instruccion[2]);
        case SUB:
            sub(instruccion[1],instruccion[2]);
        case JNZ:
            jnz(instruccion[1],instruccion[2]);
        case RESIZE:
            rezize(instruccion[1]);
        case COPY_STRING:
            copy_string(instruccion[1]);
        case WAIT:
            wait(instruccion[1]);
        case SIGNAL:
            signal(instruccion[1]);
        case IO_GEN_SLEEP:
            io_gen_sleep(instruccion[1],instruccion[2]);
        case IO_STDIN_READ:
            io_stdin_read(instruccion[1],instruccion[2],instruccion[3]);
        case IO_STDOUT_WRITE:
            io_stdout_write(instruccion[1],instruccion[2],instruccion[3]);
        case IO_FS_CREATE:
            io_fs_create(instruccion[1],instruccion[2]);
        case IO_FS_DELETE:
            io_fs_delete(instruccion[1],instruccion[2]);
        case IO_FS_TRUNCATE: 
            io_fs_truncate(instruccion[1],instruccion[2],instruccion[3]);
        case IO_FS_WRITE:
            io_fs_write(instruccion[1],instruccion[2],instruccion[3],instruccion[4],instruccion[5]);
        case IO_FS_READ:
            io_fs_read(instruccion[1],instruccion[2],instruccion[3],instruccion[4],instruccion[5]);
        case EXIT_SSOO:
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

//Funciones auxiliares para checkInterrupo()
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
