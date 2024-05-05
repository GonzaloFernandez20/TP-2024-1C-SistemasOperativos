void  set(){}
void  mov_in(){}
void  mov_out(){}MOV_OUT 
void  sum(){}
void  sub(){}
void  jnz(){}
void  rezize(){}
void  copy_string(){}
void  wait(){}
void  signal(){}
void  io_gen_slee
void  io_STDIN_READ
void  io_STDOUT_WRITE
void  io_fs_CREATE
void  io_fs_DELETE
void  io_fs_TRUNCATE 
void  io_fs_WRITE
void  io_fs_READ
void  EXIT


typedef struct {
    char* operacion_str,
    char* argumentos[5] 
}t_instruccion;

// variables globales
t_instruccion* instruccion; // recibirá un puntero a un struct t_instruccion
int operacion; // guardará el código de la operación a ejecutar

void ciclo() {
    while(1) {
        fetch(); // busca la instrucción y recibe un puntero a un struct
        operacion = decode(instruccion->operacion_str);
        argumentos = 
        execute(operacion, &instruccion);
        checkInterrupt();
    }
}


void fetch(){
    pedirAMemoria(PCB.registros.PC); // actualiza variable global instruccion
    PCB.registros.PC += 1;
}

int decode(char* operacion_str) {
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

void execute() {

}