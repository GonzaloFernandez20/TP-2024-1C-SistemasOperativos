#include <fileSystem/estructurasFS.h>

void levantar_filesystem(void){

    path_archivo_bloques = concatenar_rutas(config_IO.PATH_BASE_DIALFS, "bloques.dat");
    archivo_bloques = abrir_archivo(config_IO.BLOCK_COUNT * config_IO.BLOCK_SIZE, path_archivo_bloques);
    fclose(archivo_bloques);

    path_archivo_bitmap = concatenar_rutas(config_IO.PATH_BASE_DIALFS, "bitmap.dat");
    tamanio_bitmap = (config_IO.BLOCK_COUNT + 7) / 8;
    archivo_bitmap = abrir_archivo(tamanio_bitmap, path_archivo_bitmap);
    mapear_bitmap_en_memoria();
}

FILE* abrir_archivo(int bytes_tamanio, char* path){
    FILE* archivo = fopen(path, "r+b");//abrir el archivo en modo lectura/escritura binario
    
    // Si el archivo no existe, crearlo
    if (archivo == NULL) {archivo = fopen(path, "w+b");}
    
    ftruncate(fileno(archivo), bytes_tamanio);

    free(path);
    return archivo;
}

char* concatenar_rutas(char* directorio, char* archivo){
    int length = strlen(directorio) + strlen(archivo) + 1; // +1 para el terminador nulo

    char* path = malloc(length);

    strcpy(path, directorio);
    strcat(path, archivo);

    return path;
}

void mapear_bitmap_en_memoria(void){
    memoria_mapeada = mmap(NULL, tamanio_bitmap, PROT_READ | PROT_WRITE, MAP_SHARED, fileno(archivo_bitmap), 0);
    bitarray = bitarray_create_with_mode (memoria_mapeada, tamanio_bitmap, LSB_FIRST);
}