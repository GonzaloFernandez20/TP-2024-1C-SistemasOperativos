#ifndef CICLO_H_INCLUDED
#define CICLO_H_INCLUDED

#include <commons/collections/dictionary.h>
#include <commons/string.h>
#include <protocolos/protocoloInterrupt.h>
// #include <include/operaciones.h>


#define SET "SET"
#define MOV_IN "MOV_IN"
#define MOV_OUT "MOV_OUT"
#define SUM "SUM"
#define SUB "SUB"
#define JNZ "JNZ"
#define RESIZE "RESIZE"
#define COPY_STRING "COPY_STRING"
#define WAIT "WAIT"
#define SIGNAL "SIGNAL"
#define IO_GEN_SLEEP "IO_GEN_SLEEP"
#define IO_STDIN_READ "IO_STDIN_READ"
#define IO_STDOUT_WRITE "IO_STDOUT_WRITE"
#define IO_FS_CREATE "IO_FS_CREATE"
#define IO_FS_DELETE "IO_FS_DELETE"
#define IO_FS_TRUNCATE "IO_FS_TRUNCATE"
#define IO_FS_WRITE "IO_FS_WRITE"
#define IO_FS_READ "IO_FS_READ"
#define EXIT_OS "EXIT_OS"


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


// Function declarations
void set();
void sum();//Declaraciones de todas las operaciones
void set();
void mov_in(char* registroDatos, char* registroDireccion);
void mov_out(char* registroDireccion, char* registroDatos);
void sub();
void jnz();
void io_gen_sleep();
void resize(char* tamanio);
void copy_string(char* tamanio);
void wait(char* recurso);
void signal_kernel(char* recurso); // signal a secas es una función de la biblioteca estándar signal.h
void io_stdin_read(char* interfaz,char* registroDireccion,char* registroTamanio);
void io_stdout_write(char* interfaz,char* registroDireccion,char* registroTamanio);
void io_fs_create(char* interfaz, char* nombreDelArchivo);
void io_fs_delete(char* interfaz, char* nombreDelArchivo);
void io_fs_truncate(char* interfaz,char* nombreDelArchivo,char* registroTamanio);
void io_fs_write(char* interfaz,char* nombreDelArchivo,char* registroDireccion,char* registroTamanio,char* registroPunteroArchivo);
void io_fs_read();
void exit_os();


void* direccionDelRegistro(char* nombre_registro);
int tamanioDeRegistro(char* registro);
int identificarInterfaz(char* interfaz);
void enviarleAkernel(int interfaz, int unidades); // falta implementar
// bool contiene(char *unArrayDeStrings[],int sizeOfArray,char* unElemento);


void init_opCodes_dictionary(void);

//Funciones auxiliares para checkInterrupt()
void desalojarProcesoActual(void);
void acomodarRegistrosDeCPU(struct_registros registros);
void exportarPCB(void);
struct_PCB recibirPCB(void);
void acomodarRegistrosDePCB(struct_registros* registros);

//Funciones del ciclo per se
void arranque();
void ciclo();
void fetch();
void decode_and_execute();
void checkInterrupt();
void desalojarProcesoActual();

//Variables globales
struct_PCB PCB;
struct_registros registrosCPU;   
t_instruccion instruccion;
t_dictionary* opCodes_diccionario;
t_dictionary* registros_diccionario;

// Me traigo variable global desde protocoloInterrupt.h
extern u_int32_t PID_a_interrumpir; // CPU recibe, desde Kernel y 
                                    //a través de la conexión Interrupt, PIDs que se quieran interrumpir.
                                    //Si la PID del proceso en ejecución coincide con el PID solicitado a interrumpir,
                                    //entonces seteamos el flag en 1. Luego de procesar la solicitud,
                                    //se debe setear nuevamente el flag en 0 (estado default).

#endif