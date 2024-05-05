
//fetch->decode->execute->checkInterrupt
//|----------------------|
//no se puede interrumpir
//Process Control Block --> Tiene una foto de la CPU en el momento en el que se quita el proceso de running
//la foto contiene : PID, ProgramCounter, Quantum, Contenido de los registros


typedef struct {
    uint32_t PC,    // Program Counter, indica la próxima instrucción a ejecutar
    uint8_t AX,     // Registro Numérico de propósito general
    uint8_t BX,     // idem
    uint8_t CX,     // idem 
    uint8_t DX,     // idem
    uint32_t EAX,   // idem
    uint32_t EBX,   // idem
    uint32_t ECX,   // idem
    uint32_t EDX,   // idem 
    uint32_t SI,    // Contiene la dirección lógica de memoria de origen desde donde se va a copiar un string.
    uint32_t DI     // Contiene la dirección lógica de memoria de destino a donde se va a copiar un string.
} struct_registros;

struct_registros registrosCPU;

typedef struct {
    int PID,
    int Quantum,
    struct_registros registros 
} struct_PCB;

typedef enum{
    //estas son las que pide el checkpoint 2
    SET,
    MOV_IN,
    MOV_OUT, 
    SUM,
    SUB,
    JNZ,
    RESIZE,
    COPY_STRING,
    WAIT,
    SIGNAL,
    IO_GEN_SLEEP,
    IO_STDIN_READ,
    IO_STDOUT_WRITE,
    IO_FS_CREATE,
    IO_FS_DELETE,
    IO_FS_TRUNCATE, 
    IO_FS_WRITE,
    IO_FS_READ,
    EXIT
} opCode_instrucciones; //19



struct_PCB PCB;


void arranque(){
    PCB = recibirPCB();
    ciclo();
}



void ciclo() {
    while(1) {
        fetch();
        decode();
        execute();
        checkInterrupt();
    }
}


void fetch(){
    pedirAMemoria(PCB.registros.PC + 1); // actualiza variable global instruccion
    PCB.registros.PC += 1;
}

void decode() {
    
}

void execute() {
    
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
    PCB = recibirPCB(); // Actualizar PCB
}


/*
Dir     Binario  Humano
0x0001  0010011  SET
0x0002  0000111  AX 
0x0003  0000001  1  

Registro CPU 16 bits

EAX 10100011 10101110 10100011 10101110 32bits => ocupa 4 posiciones de memoria

Puede que los cuatro bytes de EAX no esten almacenados en la misma página
por ejemplo:

página 1:
0xfffe 10100011
0xffff 10101110 

página 2:
0x0001 10100011
0x0002 10101110


Almacenamos de a 8 bits por byte AX en la memoria:
0x0001 10100011 
0x0002 10101110

*/


/*
Fetch
La primera etapa del ciclo consiste en buscar la próxima instrucción a ejecutar. En este trabajo
práctico cada instrucción deberá ser pedida al módulo Memoria utilizando el Program Counter
(también llamado Instruction Pointer) que representa el número de instrucción a buscar relativo al
proceso en ejecución. Al finalizar el ciclo, este último deberá ser actualizado (sumarle 1) si
corresponde.
*/

/*
-Problema: las instrucciones toman distintos tipos y cantidades de argumentos

SET           AX 1
RESIZE        8 
IO_STDIN_READ Interfaz Registro Tamanio

-Solución Posible 1:

Recibimos de memoria:
Operacion               --> "SET"
Struct con los argumentos --> struct.Registro = "AX"  struct.valor = "1"

Esto tiene que ser así porque las instrucciones toman distinta cantidad y tipos de argumentos
el switch del execute() se va a encargar de trabajar con la instruccion y el struct correspondiente
que tendríamos que recibir de memoria con los argumentos

-Solucion Posible 2:

Recibimos de memoria:
Operacion   --> arr[0] = "SET"
Argumentos  --> arr[1] = "AX", arr[2] = "1", arr[3] = vacio, arr[4] = vacio, arr[5] = vacio
Instruccion --> arr[6] = {"SET","AX","1","","",""}

La idea sería recibir un array de strings con lo que se mande memoria donde el primer elemento del array
es la operacion en sí y el resto los operandos. Jamás vas a acceder a un elemento del array que no necesite
la operación que recibiste porque el switch en el execute() sólo va a pedir los elementos requeridos como argumentos
de la operacion

*/

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
void execute(int operacion, char* instruccion[6]){
    //ignorar el elemento instruccion[0] pues es la instruccion sin decodificar
}

void execute(int operacion, char* instruccion[6]){
//ignorar el elemento instruccion[0] pues es la instruccion sin decodificar
switch (operacion){
    case SET:
        set(instruccion[1],instruccion[2]);
    case MOV_IN:

    case MOV_OUT: 
    case SUM:
    case SUB:
    case JNZ:
    case RESIZE:
    case COPY_STRING:
    case WAIT:
    case SIGNAL:
    case IO_GEN_SLEEP:
    case IO_STDIN_READ:
    case IO_STDOUT_WRITE:
    case IO_FS_CREATE:
    case IO_FS_DELETE:
    case IO_FS_TRUNCATE: 
    case IO_FS_WRITE:
    case IO_FS_READ:
    case EXIT:
}

}

void set(char* registro, char* valor){//asigna al registro seleccionado el valor pasado
    void* ptr_registroCPU;

    ptr_registroCPU = direccionDelRegistro(registro);

    int valorFinal = atoi(valor);

    *ptr_registroCPU = valorFinal;
    free(ptr_registroCPU);
}

int tamanioDeRegistro(char* registro){
    char* regs_uint8[4] = {"AX","BX","CX","DX"}
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
        return 1
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







