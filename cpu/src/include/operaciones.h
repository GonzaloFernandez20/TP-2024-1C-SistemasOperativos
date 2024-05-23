#ifndef OPERACIONES_H
#define OPERACIONES_H

// Function declarations
void set();
void sum();
void sub();
void jnz();
void io_gen_sleep();

void* direccionDelRegistro(char* registro);
size_t tamanioDeRegistro(char* registro);
int identificarInterfaz(char* interfaz);
void enviarleAkernel(int interfaz, int unidades); // falta implementar

#endif // OPERACIONES_H
