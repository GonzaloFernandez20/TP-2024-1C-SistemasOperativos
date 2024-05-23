#include <include/ciclo.h>

/*
// De memoria se recibe siempre éste struct, si no se usan ciertos argumentos se envían vacíos 
// y el switch del execute se asegure de que cada operacion use los argumentos correspondientes
*/
 
/* Cada operacion_str está asociado a un puntero a la función que realiza la operación.*/
void init_opCodes_dictionary() {
    opCodes = dictionary_create();
    
    dictionary_put(opCodes, SET,            (void*)set);
    dictionary_put(opCodes, MOV_IN,         (void*)mov_in);
    dictionary_put(opCodes, MOV_OUT,        (void*)mov_out);
    dictionary_put(opCodes, SUM,            (void*)sum);
    dictionary_put(opCodes, SUB,            (void*)sub);
    dictionary_put(opCodes, JNZ,            (void*)jnz);
    dictionary_put(opCodes, RESIZE,         (void*)resize);
    dictionary_put(opCodes, COPY_STRING,    (void*)copy_string);
    dictionary_put(opCodes, WAIT,           (void*)wait);
    dictionary_put(opCodes, SIGNAL,         (void*)signal_kernel);
    dictionary_put(opCodes, IO_GEN_SLEEP,   (void*)io_gen_sleep);
    dictionary_put(opCodes, IO_STDIN_READ,  (void*)io_stdin_read);
    dictionary_put(opCodes, IO_STDOUT_WRITE,(void*)io_stdout_write);
    dictionary_put(opCodes, IO_FS_CREATE,   (void*)io_fs_create);
    dictionary_put(opCodes, IO_FS_DELETE,   (void*)io_fs_delete);
    dictionary_put(opCodes, IO_FS_TRUNCATE, (void*)io_fs_truncate);
    dictionary_put(opCodes, IO_FS_WRITE,    (void*)io_fs_write);
    dictionary_put(opCodes, IO_FS_READ,     (void*)io_fs_read);
    dictionary_put(opCodes, EXIT_OS,        (void*)exit_os);
}

void arranque(){
    init_opCodes_dictionary();

    PCB = recibirPCB();
    acomodarRegistrosDeCPU(PCB.registros);
    ciclo();
}
void ciclo() {
    while(1) {
        fetch();                // busca la siguiente instrucción.
        decode_and_execute();   // interpreta y ejecuta la operación que corresponde
        checkInterrupt();       // se fija si hay interrupciones.
    }
    // cuando salga del loop signfica que por alguna razón cpu paró
    dictionary_destroy(opCodes);    // liberamos memoria asignada al diccionario.
}
void fetch(){
    PCB.registros.PC += 1;      // buscamos la siguiente instrucción
    // pedirAMemoria(PCB.registros.PC); 
    
}
void decode_and_execute() {
    void (*funcion_operacion)(void) = dictionary_get(opCodes, instruccion.operacion_str); // obtenemos la función
    funcion_operacion(); // ejecutamos la funcion y para los argumentos se pueden acceder a ellos desde el struct global instruccion.
}

void checkInterrupt() {
    // Si la PID del proceso en ejecución coincide con el PID solicitado a interrumpir, entonces se realiza lo solicitado.
    if(PID_a_interrumpir == PCB.PID) {
        log_info(cpu_log_debug, "Interrumpiendo proceso PID=%d en ejecución", PCB.PID);
        desalojarProcesoActual();
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


struct_PCB recibirPCB() {
    return PCB; // CAMBIAR DESPUÉS
} 

// Función auxiliar para fetch()
// void pedirAMemoria(PCB.registros.PC) {
//     return
// }

// void enviarAKernel() {

// }

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