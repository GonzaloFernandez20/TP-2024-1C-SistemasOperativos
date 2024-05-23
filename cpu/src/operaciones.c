// LAS FUNCIONES OPERACIÓN

void set(){//asigna al registro seleccionado el valor pasado como argumento
    char* registro = instruccion.arg1;
    char* valor = instruccion.arg2;
    
    void* ptr_registroCPU;

    ptr_registroCPU = direccionDelRegistro(registro);

    int valorFinal = atoi(valor);
    //necesito hacer el cast a uint8_t o uint32_t por el void* que no se puede desreferenciar
    if(tamanioDeRegistro(registro) == sizeof(uint8_t)) {
        *(uint8_t*)ptr_registroCPU = valorFinal;
    }
    else {
        *(uint32_t*)ptr_registroCPU = valorFinal;
    }

    free(ptr_registroCPU);
}
void sum(){//Vale sumar un registro de 32bits a uno de 8bits
    char* registroDestino = instruccion.arg1;
    char* registroOrigen = instruccion.arg2;
    
    void* ptr_registroDestino;
    void* ptr_registroOrigen;

    ptr_registroDestino = direccionDelRegistro(registroDestino);
    ptr_registroOrigen = direccionDelRegistro(registroOrigen);

//  *ptr_registroDestino += *ptr_registroOrigen; no se puede hacer por que son void*
//  entonces tengo que hacer el if else para castearlos
    if(tamanioDeRegistro(registroDestino) == sizeof(uint8_t)){
        *(uint8_t*)ptr_registroDestino += *(uint8_t*)ptr_registroOrigen;
    }
    else{
        *(uint32_t*)ptr_registroDestino += *(uint32_t*)ptr_registroOrigen;
    }

    free(ptr_registroDestino);
    free(ptr_registroOrigen);

}
void sub(){
    char* registroDestino = instruccion.arg1;
    char* registroOrigen = instruccion.arg2;

    void* ptr_registroDestino;
    void* ptr_registroOrigen;

    ptr_registroDestino = direccionDelRegistro(registroDestino);
    ptr_registroOrigen = direccionDelRegistro(registroOrigen);

    if(tamanioDeRegistro(registroDestino) == sizeof(uint8_t)){
        *(uint8_t*)ptr_registroDestino -= *(uint8_t*)ptr_registroOrigen;
    }
    else{
        *(uint32_t*)ptr_registroDestino -= *(uint32_t*)ptr_registroOrigen;
    }

    free(ptr_registroDestino);
    free(ptr_registroOrigen);
}
void jnz(){
    char* registro = instruccion.arg1;
    char* numDeInstruccion = instruccion.arg2;

    void* ptr_registroCPU;
    
    int instruccion = atoi(numDeInstruccion);
    if(instruccion != 0){registrosCPU.PC += *(uint32_t*)ptr_registroOrigen;}//acá no hay if para castear porque el PC es siempre de tipo uint32_t
    else{
        //tirar un error
        }
    free(ptr_registroCPU);
}
void io_gen_sleep(){
    char* interfaz = instruccion.arg1;
    char* unidadesDeTrabajo = instruccion.arg2;

    int interfaz_para_kernel = identificarInterfaz(interfaz);
    int unidadesDeSleep = atoi(unidadesDeTrabajo);
    enviarleAkernel(interfaz_para_kernel, unidadesDeSleep);//hablar con gonza por protocolo de comunicacion
}





// FUNCIONES AUXILIARES

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