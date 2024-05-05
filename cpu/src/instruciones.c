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
    char operacion,
    char* argumentos[5]
}t_instruccion;

// variables globales
t_instruccion* instruccion; // recibirá un puntero a un struct instruccion
int operacion; // guardará el código de la operación a ejecutar

void ciclo() {
    while(1) {                
        fetch(); // busca la instrucción y recibe un puntero a un struct
        operacion = decode(instruccion->operacion);
        argumentos = 
        execute(operacion, &instruccion);
        checkInterrupt();
    }
}


void fetch(){
    pedirAMemoria(PCB.registros.PC); // actualiza variable global instruccion
    PCB.registros.PC += 1;
}

void decode() {
    int operacion = 
}

void execute() {

}