#ifndef OPERACIONES_H
#define OPERACIONES_H

#include <string.h>
#include <include/ciclo.h>

// Function declarations
void set();
void sum();//Declaraciones de todas las operaciones
void set();
void mov_in(char* registroDatos, char* registroDireccion);
void mov_out(char* registroDireccion, char* registroDatos);
void sum();
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
void sub();
void jnz();
void io_gen_sleep();

void* direccionDelRegistro(char* registro);
int tamanioDeRegistro(char* registro);
int identificarInterfaz(char* interfaz);
void enviarleAkernel(int interfaz, int unidades); // falta implementar


// Me traigo variables globales
extern t_instruccion instruccion;


#endif // OPERACIONES_H
