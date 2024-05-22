#ifndef CICLO_H_INCLUDED
#define CICLO_H_INCLUDED

#include <protocolos/protocoloInterrupt.h>
#include <commons/collections/dictionary.h>

typedef struct {
    uint32_t PC;    // Program Counter, indica la próxima instrucción a ejecutar
    uint8_t  AX;     // Registro Numérico de propósito general
    uint8_t  BX;     // idem
    uint8_t  CX;     // idem 
    uint8_t  DX;     // idem
    uint32_t EAX;   // idem
    uint32_t EBX;   // idem
    uint32_t ECX;   // idem
    uint32_t EDX;   // idem 
    uint32_t SI;    // Contiene la dirección lógica de memoria de origen desde donde se va a copiar un string.
    uint32_t DI;    // Contiene la dirección lógica de memoria de destino a donde se va a copiar un string.
} struct_registros;

typedef struct {
    int PID;
    int Quantum;
    struct_registros registros; 
} struct_PCB;


typedef struct{
    char* operacion_str;
    char* arg1;
    char* arg2;
    char* arg3;
    char* arg4;
    char* arg5;    
}t_instruccion;

typedef enum{
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
    EXIT_OS //EXIT pelado en C es una palabra reservada
} opCode_instrucciones; //19

//Declaraciones de todas las operaciones
void set(char* registro, char* valor);
void move_in(char* registroDatos, char* registroDireccion);
void move_out(char* registroDireccion, char* registroDatos);
void sum(char* registroDestino, char* registroOrigen);
void sub(char* registroDestino, char* registroOrigen);
void jnz(char* registro, char* numDeInstruccion);
void io_gen_sleep(char* interfaz, char* unidadesDeTrabajo);
void rezize(char* tamanio);
void copy_string(char* tamanio);
void wait(char* recurso);
void signal(char* recurso);
void io_stdin_read(char* interfaz,char* registroDireccion,char* registroTamanio);
void io_stdout_write(char* interfaz,char* registroDireccion,char* registroTamanio);
void io_fs_create(char* interfaz, char* nombreDelArchivo);
void io_fs_delete(char* interfaz, char* nombreDelArchivo);
void io_fs_truncate(char* interfaz,char* nombreDelArchivo,char* registroTamanio);
void io_fs_write(char* interfaz,char* nombreDelArchivo,char* registroDireccion,char* registroTamanio,char* registroPunteroArchivo);
void io_fs_read(instruccion[1],instruccion[2],instruccion[3],instruccion[4],instruccion[5]);
void exit();

//Declaraciones de funciones auxiliares para hacer el decode() y execute()

int tamanioDeRegistro(char* registro);
int contiene(char *unArrayDeStrings[],int sizeOfArray,char* unElemento);
void* direccionDelRegistro(char* registro);

//Funciones auxiliares para checkInterrupt()
void hayInterrupcion();
void atenderInterrupcion(void);
void acomodarRegistrosDeCPU(struct_registros registros);
void exportarPCB(void);
void acomodarRegistrosDePCB(struct_registros* registros);

//Funciones del ciclo per se
void arranque();
void ciclo();
void fetch();
void decode();
void execute();
void checkInterrupt();


//Variables globales
struct_PCB PCB;
struct_registros registrosCPU;   
t_instruccion instruccion;       // La utilizamos durante el fetch
int op_code;                     // Código de operación a ejecutar
bool hayInterrupt;                  // Flag de interrupción. CPU recibe, desde Kernel y 
                                 //a través de la conexión Interrupt, PIDs que se quieran interrumpir.
                                 //Si la PID del proceso en ejecución coincide con el PID solicitado a interrumpir,
                                 //entonces seteamos el flag en 1. Luego de procesar la solicitud,
                                 //se debe setear nuevamente el flag en 0 (estado default).

// Me traigo variable global desde protocoloInterrupt.h
extern char* PID_a_interrumpir; 


#endif