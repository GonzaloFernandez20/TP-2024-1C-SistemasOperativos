#include <fileSystem/estructurasFS.h>

void levantar_filesystem(void){
    path_archivo_bloques = concatenar_rutas(config_IO.PATH_BASE_DIALFS, "bloques.dat");
    archivo_bloques = abrir_archivo(config_IO.BLOCK_COUNT * config_IO.BLOCK_SIZE, path_archivo_bloques);
    fclose(archivo_bloques);

    path_archivo_bitmap = concatenar_rutas(config_IO.PATH_BASE_DIALFS, "bitmap.dat");
    tamanio_bitmap = (config_IO.BLOCK_COUNT + 7) / 8;
    archivo_bitmap = abrir_archivo(tamanio_bitmap, path_archivo_bitmap);
    mapear_bitmap_en_memoria();
    cargar_FCBs_archivos_metadata();
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

void cargar_FCBs_archivos_metadata(void){

    archivos_metadata = list_create();

    DIR* directorio_dialfs;
    struct dirent* entrada;
    
    directorio_dialfs = opendir(config_IO.PATH_BASE_DIALFS);
    
    while ((entrada = readdir(directorio_dialfs)) != NULL) {
        if(es_un_metadata(entrada->d_name)){
            t_FCB* metadata = malloc(sizeof(t_FCB));
            metadata->archivo = strdup(entrada->d_name);
            metadata->path = strdup(concatenar_rutas(config_IO.PATH_BASE_DIALFS, entrada->d_name));

            t_config* config = iniciar_config(metadata->path);
            metadata->bloque_inicial = config_get_int_value(config, "BLOQUE_INICIAL");
            metadata->tamanio_archivo = config_get_int_value(config, "TAMANIO_ARCHIVO");
            config_destroy(config);

            list_add_sorted(archivos_metadata, metadata, comparar_por_bloque_inicial);
        }
    }

    closedir(directorio_dialfs);
}

int es_un_metadata(char* archivo){

    return  !string_equals_ignore_case(archivo, "bloques.dat") &&
            !string_equals_ignore_case(archivo, "bitmap.dat") &&
            !string_equals_ignore_case(archivo, ".") &&
            !string_equals_ignore_case(archivo, "..");
}

bool comparar_por_bloque_inicial(void *FCB_1, void *FCB_2){
    t_FCB *FCB_a = (t_FCB *)FCB_1;
    t_FCB *FCB_b = (t_FCB *)FCB_2;

    return FCB_a->bloque_inicial < FCB_b->bloque_inicial;
}

int asignar_primer_bloque_libre(void){
    int bloque_libre = obtener_primer_bloque_libre();
    
    bitarray_set_bit(bitarray, bloque_libre);
    msync(memoria_mapeada, tamanio_bitmap, MS_SYNC);

    return bloque_libre;
}
int obtener_primer_bloque_libre(void){
    int bloque_libre;

    for(int i = 0; i < config_IO.BLOCK_COUNT; i++){
        if(!bitarray_test_bit(bitarray, i)){
          bloque_libre = i;
          break;
        }
    }
    return bloque_libre;
}

void liberar_bloques(int bloque_inicial, int bloque_final_mas_uno){
    //si el archivo ocupa los bloques 5, 6, 7, el bloque final mas uno es 8
    for(int i = bloque_inicial; i < bloque_final_mas_uno; i++){
        bitarray_clean_bit(bitarray, i);
    }
     msync(memoria_mapeada, tamanio_bitmap, MS_SYNC);
}

void ocupar_bloques(int bloque_inicial, int bloque_final_mas_uno){
    for(int i = bloque_inicial; i < bloque_final_mas_uno; i++){
        bitarray_set_bit(bitarray, i);
    }
     msync(memoria_mapeada, tamanio_bitmap, MS_SYNC);
}

int hay_suficientes_bloques_contiguos(int primer_bloque, int ultimo_bloque_mas_uno){
    for(int i = primer_bloque; i < ultimo_bloque_mas_uno; i++){
       if(bitarray_test_bit(bitarray, i)){return 0;}
    }
    return 1;
}

int buscar_FCB_en_archivos_metadata(char* archivo_buscado){

    int indice;
    int size = list_size(archivos_metadata);
    
    for (int i = 0; i < size; i++) {
        t_FCB* archivo = list_get(archivos_metadata, i);
        if (string_equals_ignore_case(archivo->archivo, archivo_buscado))
        {
            indice = i;
            break;
        }
    }

    return indice;
}

