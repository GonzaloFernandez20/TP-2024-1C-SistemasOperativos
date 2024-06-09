#include <include/main.h>

//MUCHO CUIDADO CON bitarray_test_bit(), si está libre devuelve 0 == false y si está ocupado devuelve 1 == true

/*
int main(int argc, char* argv[]) {
    
    path_config = chequear_parametros(argc, argv);

    init(); 
    iniciar_servidor_memoria();
    gestionar_conexiones_clientes();
 
    return 0;
}

*/
#define TAMANIO_HARCODEADO 4
//DECLARACIONES
void* destructor(void* elemento);
void* base_de_marco(int numeroDeMarco);
void mostrar(char* proceso, t_list* tablaDePaginas);
void llenar_espacio_de_usuario();
void init_espacio_de_usuario();
t_list* init_tabla_de_paginas(int cantidadDePaginas);
t_bitarray* init_bitarray(void* espacioBitArray);
void ocupar_algunos_marcos();

//VARIABLES GLOBALES
void* espacioDeUsuario;
int tamanioDeMarco = 4;
int cantidadDeMarcos = 32;
t_bitarray* bitarray;


int main(){
    
    init_espacio_de_usuario();

    void* espacioBitArray = malloc(TAMANIO_HARCODEADO);//1 byte representa a 8 bits de disponibilidad => 8 marcos
    bitarray = init_bitarray(espacioBitArray);//está en formato LSB, leer las commons

    ocupar_algunos_marcos();

    printf("Tamanio del bitarray: %d\n",bitarray_get_max_bit(bitarray));
    if(bitarray_test_bit(bitarray,0)){
            printf("ARAMIDA\n");
        }
    llenar_espacio_de_usuario();

//Tablas de Páginas
//el índice de la lista marca el número de página y el dato en la lista es un puntero al marco
//el marco es el puntero espacioDeUsuario + offset de n páginas

    t_list* tablaDePaginasProcesoA = init_tabla_de_paginas(10);
    t_list* tablaDePaginasProcesoB = init_tabla_de_paginas(4);

    printf("\nProceso A\n");
    vincular_paginas_a_marcos(tablaDePaginasProcesoA);

    printf("\nProceso B\n");
    vincular_paginas_a_marcos(tablaDePaginasProcesoB);

    free(espacioDeUsuario);
    list_destroy(tablaDePaginasProcesoA);
    list_destroy(tablaDePaginasProcesoB);
    bitarray_destroy(bitarray);
    free(espacioBitArray);
    return 0;
}
void ocupar_algunos_marcos(){//ocupo marcos pares
    for(int i = 0;2*i<8*TAMANIO_HARCODEADO;i++){
        bitarray_set_bit(bitarray,2*i);
    } 
}
t_list* init_tabla_de_paginas(int cantidadDePaginas){
    t_list* tablaDePaginas = list_create();
    for(int i = 0;i<cantidadDePaginas;i++){
        list_add(tablaDePaginas, NULL);//todavía no se asignaron marcos a las páginas
    }
    return tablaDePaginas;
}

void vincular_paginas_a_marcos(t_list* tablaDePags){
    for(int i=0;i<list_size(tablaDePags);i++){
        int marcoLibre = buscar_marco_libre();
        bitarray_set_bit(bitarray,marcoLibre);
//        printf("Marco Libre %d\n", marcoLibre);
        list_replace(tablaDePags, i, base_de_marco(marcoLibre));
        printf("Pag %d Marco %d Dato %d DirFis:%p \n", i, marcoLibre,*(int*)list_get(tablaDePags,i), list_get(tablaDePags,i));
    }
}
int buscar_marco_libre(){
    for(int i=0;i<bitarray_get_max_bit(bitarray);++i){
        if(!bitarray_test_bit(bitarray,i)){
            return i;
        }   
    }
}
t_bitarray* init_bitarray(void* espacioBitArray){
    t_bitarray* unBitarray = bitarray_create_with_mode(espacioBitArray, TAMANIO_HARCODEADO, LSB_FIRST);
    for(int i = 0;i<8*TAMANIO_HARCODEADO;i++){
        bitarray_clean_bit(unBitarray,i);
    }
    return unBitarray;
}
void llenar_espacio_de_usuario(){
//Cargo un dato a cada marco, cada marco lleva un entero que se corresponde con su número
    for(int i = 0 ; i<cantidadDeMarcos; i++){
        int* num;
        num = (int*)base_de_marco(i);
        *num = i;
    }
//reviso que se haya cargado todo bien
    for(int i = 0 ; i<cantidadDeMarcos; i++){
        int* num;
        num = (int*)base_de_marco(i);
        printf("Marco %d contiene el numero: %d\n",i,*num);
    }
    
}

void *destructor(void* elemento){
    free(elemento);
    return NULL;
}
void* base_de_marco(int numeroDeMarco){
    if(numeroDeMarco < cantidadDeMarcos){
        return espacioDeUsuario+(numeroDeMarco*tamanioDeMarco);
    } 
}
void init_espacio_de_usuario(){
    printf("Tamanio de las paginas y marcos: 4");
//    scanf("%d",&tamanioDeMarco);
    printf("\nCantidad de marcos: 32");
//    scanf("%d",&cantidadDeMarcos);
    printf("\nBytes en memoria a reservar: %d\n",(tamanioDeMarco*cantidadDeMarcos));
    espacioDeUsuario = malloc(tamanioDeMarco*cantidadDeMarcos); 
}